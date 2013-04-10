#pragma once
#include "Game.h"
#include "Field.h"
#include "Circle.h"
#include <windows.h>

const float Game::BALL_CIRCLE_RAY		= 0.4f;
const float Game::SPECTATOR_CIRCLE_RAY = 0.5f;
const float Game::PLAYER_CIRCLE_RAY		= 1.0f;
const float Game::ROTATION_STEP			= 0.1f;
const float Game::SPEED					= 12.0f;
const float Game::GOAL_OFFSET			= 2.0f;
const int Game::FRAMES_TO_CHEER_LIMIT	= 10;

Game::Game(CoordinateSystem2d* system): 
	m_system(system), 
	m_restart(false),
	frames_to_cheer(0),
	cheer_start(0),
	m_last_time(0), 
	m_dt(0) {
	ResetScore();
	srand((unsigned int)time(NULL));
};

Game::~Game(){
};

/* The game is Initialized here: the players and the field. */ 
void Game::Init(){
	float xball, yball;
	ball = new Circle(10, BALL_CIRCLE_RAY);
	// Positions of player1
	player1_pos.push_back(Point2d(0.0f, 15.0f));
	player1_pos.push_back(Point2d(7.8f, 13.0f)); 
	player1_pos.push_back(Point2d(-5.7f, 3.5f)); 
	player1_pos.push_back(Point2d(-0.3f, 1.3f)); 
	player1_pos.push_back(Point2d(-7.0f, -8.0f));
	player1_pos.push_back(Point2d(7.5f, -11.0f));
	std::vector<Point2d>::iterator it;
	for(it = player1_pos.begin(); it != player1_pos.end(); ++it){
		// We have to keep every circle we create in a vector
		// to be able to make transformations with them later
		Circle circle(10, PLAYER_CIRCLE_RAY);
		circle.SetColor(RGBcolor(0.06f, 0.21f, 1.0f));
		circle.SetArmsColor(RGBcolor(0.2f, 0.40f, 1.0f));
		circle.DrawArms(m_system, *it);
		circle.DrawCircle(m_system, *it); 
		player1.push_back(circle);
	}

	// Positions of player2
	player2_pos.push_back(Point2d(0.0f, -14.5f)); 
	player2_pos.push_back(Point2d(-7.8f, -13.0f));
	player2_pos.push_back(Point2d(4.5f, -3.5f));
	player2_pos.push_back(Point2d(-0.3f, -1.3f));
	player2_pos.push_back(Point2d(7.0f, 8.0f));
	player2_pos.push_back(Point2d(-7.8f, 11.0f));
	for(it = player2_pos.begin(); it != player2_pos.end(); ++it){
		// Same as earlier on player1
		Circle circle(10, PLAYER_CIRCLE_RAY);
		circle.SetColor(RGBcolor(0.86f, 1.0f, 0.31f));
		circle.SetArmsColor(RGBcolor(0.89f, 0.54f, 0.05f));
		circle.DrawArms(m_system, *it);
		circle.DrawCircle(m_system, *it);
		
		player2.push_back(circle);
	}
	
	// Initialize the ball
	ball->SetColor(RGBcolor(0.0f, 0.0f, 0.0f));
	// Pick randomly the team and the player which holds the ball
	srand((unsigned int)time(NULL));
	m_team = rand() % 2;
	m_player = rand() % 6;
	if (m_team == 0){
		xball = player1_pos[m_player].x;
		yball = player1_pos[m_player].y - PLAYER_CIRCLE_RAY - BALL_CIRCLE_RAY;
	}
	else if (m_team == 1){
		xball = player2_pos[m_player].x; 
		yball = player2_pos[m_player].y + PLAYER_CIRCLE_RAY + BALL_CIRCLE_RAY; 
	}
	ball_position.set(xball, yball);
	ball->DrawCircle(m_system, ball_position);
	DrawSpectators();

	// Draw also the field
	field = new Field(m_system);
	field->DrawField();
	state = HOLD;

}



/* Draw the spectators for a team. The parameters taken 
 * are the right upper corner of the spectator tribunes 
 * and the team which the spectators like */ 

void Game::DrawSpectators(Point2d corner, int team){
	const int columns = 5; 
	int rows = 14;
	Circle spectator(10, SPECTATOR_CIRCLE_RAY);
	float randr, randg;
	for(int i=0; i < columns; ++i){
		corner.set(corner.x + SPECTATOR_CIRCLE_RAY*2 + 0.1f,
					corner.y - SPECTATOR_CIRCLE_RAY*2 + 0.43f);
		for(int j=0; j < rows; ++j){
			if (team == 0){
				randr = (float)rand()/(3*RAND_MAX);
				randg = (float)rand()/RAND_MAX;
				spectator.SetColor(RGBcolor(randr, randg, 1.0f));
			}
			else if (team == 1){
				randg = (float)rand()/RAND_MAX; 
				spectator.SetColor(RGBcolor(1.0f, randg, 0.09f));
			}
			spectator.DrawCircle(m_system, Point2d(corner.x, 
					corner.y - (SPECTATOR_CIRCLE_RAY*2 + 0.1f)*j));
			if (team == 0){
				spectators1.push_back(spectator.GetObject());
			}
			else {
				spectators2.push_back(spectator.GetObject());
			}
		}
		rows--;
	}
}

void Game::DrawSpectators(){
	DrawSpectators(Point2d(-21.0f, 16.5f), 0);
	DrawSpectators(Point2d(-21.0f, -1.0f), 1);
}
/* The method removes a spectator team. 
 * We need to remove them in case we want to change 
 * the color. The framework doesn't allow us to 
 * make changes to the color without redrawing everything */ 
void Game::RemoveSpectators(int team){
	std::vector<Object2d*>::iterator it;
	if (team == 0){
		for(it = spectators1.begin(); it != spectators1.end(); ++it)
			m_system->objectRemove(*it);
	}
	else{
		for(it = spectators2.begin(); it != spectators2.end(); ++it)
			m_system->objectRemove(*it);
	}

}

/* Memorates the time when the spectators began to 
 * cheer. */  
void Game::BeginSpectatorsCheers(){
	cheer_start = clock();
}
void Game::SpectatorsCheers(){
	Point2d corner;
	// The cheering will last for 2 seconds
	if (clock() - cheer_start < 2000 && cheer_start != 0){
		frames_to_cheer++;
		if (frames_to_cheer == FRAMES_TO_CHEER_LIMIT){
			// The changes of colors for the spectators will 
			// happen once on FRAMES_TO_CHEER_LIMIT frames, 
			// which in our case is 10
			(m_last_scored == 0)?corner.set(-21.0f, 16.5f):corner.set(-21.0f, -1.0f);
			RemoveSpectators(m_last_scored);
			DrawSpectators(corner, m_last_scored);
			frames_to_cheer = 0;
		}
	}
}

/* Rotates the active player and its attached ball clockwise. */ 
void Game::RotateClockwise(int team){
	Object2d* obj = NULL;
	// In openGL the rotation is counter-clockwise so 
	// we'll need to rotate it with a negative angle
	if (state == HOLD && team == m_team){
		if (team == 0){
			obj = player1[m_player].GetObject();
			player1[m_player].RotateCircle(-ROTATION_STEP);
			ball->GetObject()->rotateRelativeToPoint(obj->axiscenter, -ROTATION_STEP);
		}
		else if (team == 1){
			obj = player2[m_player].GetObject();
			player2[m_player].RotateCircle(-ROTATION_STEP);
			ball->GetObject()->rotateRelativeToPoint(obj->axiscenter, -ROTATION_STEP);
		}
	}
}

/* Rotates the active player and its attached ball 
 * counter-clockwise. */ 
void Game::RotateCounterClockwise(int team){
	Object2d* obj = NULL;

	if (state == HOLD && team == m_team){
		if (team == 0){
			obj = player1[m_player].GetObject();
			player1[m_player].RotateCircle(ROTATION_STEP);
			ball->GetObject()->rotateRelativeToPoint(obj->axiscenter, ROTATION_STEP);
		}
		else if (team == 1){
			obj = player2[m_player].GetObject();
			player2[m_player].RotateCircle(ROTATION_STEP);
			ball->GetObject()->rotateRelativeToPoint(obj->axiscenter, ROTATION_STEP);
		}
	}
}

/* The method is improperly called CalculateSlope.
 * it only calculates the numerator and the
 * denominator from the slope formula. */ 
void Game::CalculateSlope(){
	Circle player = player1[m_player];
	if (m_team == 1)
		player = player2[m_player];
	deltay = - player.GetObject()->axiscenter.y + ball->GetObject()->axiscenter.y;
	deltax = - player.GetObject()->axiscenter.x + ball->GetObject()->axiscenter.x;

}

/* Changes that are made when a player kicks a ball. */ 
void Game::KickEvent(int team){
	if (state == HOLD && team == m_team){
		state = WALK; 
		CalculateSlope();
		m_team_last = m_team;
		m_player_last = m_player;
	}
}

/* Takes as a parameter the future position of the ball and
 * the center of the circle which is posibly colliding 
 * with the ball.
 * Returns the remaining distance to the center of 
 * the circle given as a parameter. If the distance is
 * greater than zero then a collision took place. */
float Game::CheckCollision(Point2d &ball_center, Point2d &center){
	float distance = sqrt(pow(ball_center.x - center.x, 2) + pow(ball_center.y - center.y, 2));
	return PLAYER_CIRCLE_RAY + BALL_CIRCLE_RAY - distance; 
}


/* Returns the index of the team which scored the goal. 
 * In case there is no goal it returns -1.
 * If the 'increase' parameter is set to true the score 
 * will be increased, otherwise it would simply make a check. */ 
int Game::CheckGoal(bool increase){
	Point2d ball_pos = ball->GetObject()->axiscenter;
	float ex_distance = PLAYER_CIRCLE_RAY + BALL_CIRCLE_RAY;

	if (ball_pos.x >= Field::BOTTOM_MIDDLE_LEFT.x + ex_distance &&
		ball_pos.x <= Field::BOTTOM_MIDDLE_RIGHT.x - ex_distance &&
		ball_pos.y - BALL_CIRCLE_RAY - Field::LINE_WIDTH <= Field::LEFTLCORNER.y){
			if (increase){
				score1++;
			}
			return 0;
	}
	else if (ball_pos.x >= Field::UP_MIDDLE_LEFT.x + ex_distance && 
		ball_pos.x <= Field::UP_MIDDLE_RIGHT.x - ex_distance &&
		ball_pos.y + BALL_CIRCLE_RAY >= Field::LEFTUCORNER.y){
			if (increase){
				score2++;
			}
			return 1;
	}
	
	return -1;
}

/* Returns true/false if the distance from the line 
 * of the goal is to big. If it is the ball will need 
 * to be given to a new player from the opponent team */ 
bool Game::CheckGoalLimit(){
	Point2d ball_pos = ball->GetObject()->axiscenter;
	if (ball_pos.y - BALL_CIRCLE_RAY - Field::LINE_WIDTH <= 
		Field::LEFTLCORNER.y - GOAL_OFFSET) {
			return true;
	}
	if (ball_pos.y + BALL_CIRCLE_RAY + Field::LINE_WIDTH >= 
		Field::LEFTUCORNER.y + GOAL_OFFSET){
			return true;
	}
	return false;
}

/* Checks if the ball hit a wall or a goal post. 
 * In case it did the ball will bounce off. 
 * The parameter is the future positions of the
 * center of the ball. It's used to check the collision
 * with the goal posts. */
void Game::WallHitCheck(){

	Point2d ball_pos = ball->GetObject()->axiscenter;
	// Collision with the ball
	if (ball_pos.x - BALL_CIRCLE_RAY <= Field::LEFTUCORNER.x + Field::LINE_WIDTH ||
		ball_pos.x + BALL_CIRCLE_RAY >= Field::RIGHTUCORNER.x){
			deltax *= -1;
			m_team_last = -1;
			m_player_last = -1;
	}
	else if (ball_pos.y + BALL_CIRCLE_RAY >= Field::RIGHTUCORNER.y ||
		ball_pos.y - BALL_CIRCLE_RAY <= Field::RIGHTLCORNER.y + Field::LINE_WIDTH){
			deltay *= -1;
			m_team_last = -1;
			m_player_last = -1;
	}

	Point2d ball_center(ball_pos.x + SPEED * deltax * m_dt, 
						ball_pos.y + SPEED * deltay * m_dt);
	// Collision with one of the goal posts
	if (CheckCollision(ball_center, Field::UP_MIDDLE_LEFT) > 0 || 
		CheckCollision(ball_center, Field::UP_MIDDLE_RIGHT) > 0 ||
		CheckCollision(ball_center, Field::BOTTOM_MIDDLE_LEFT) > 0 ||
		CheckCollision(ball_center, Field::BOTTOM_MIDDLE_RIGHT) > 0) {
			deltay *= -1;
			m_team_last = -1;
			m_player_last = -1;
	}
}

/* Picks a radom player from the opponent team when one of them 
 * scored a goal and gives him the ball.*/ 
void Game::PickNewRandomPlayer(int team){
	Point2d player_pos;
	
	m_player = rand() % 6;
	m_team = team;
	
	// We translate the ball to the random picked player
	if (team == 0){
		player_pos = player1_pos[m_player];
		m_system->objectTranslate(ball->GetObject(), player_pos.x - ball->GetObject()->axiscenter.x, 
			player_pos.y - BALL_CIRCLE_RAY - PLAYER_CIRCLE_RAY - ball->GetObject()->axiscenter.y);
	}
	else if (team == 1) {
		player_pos = player2_pos[m_player];
		m_system->objectTranslate(ball->GetObject(), player_pos.x - ball->GetObject()->axiscenter.x,
			player_pos.y + BALL_CIRCLE_RAY + PLAYER_CIRCLE_RAY - ball->GetObject()->axiscenter.y);
	}
}

/* Simple method which resets the score to 0-0 */ 
void Game::ResetScore(){
	score1 = 0; 
	score2 = 0;
}

/* Adds a line to the score of the team given as a parameter */ 
void Game::DrawScoreForTeam(int team){
	const Point2d SCORE1_POSU(15.0f, 2.0f), SCORE1_POSB(15.0f, 7.0f),
				  SCORE2_POSU(15.0f, -7.0f), SCORE2_POSB(15.0f, -2.0f);
	if (team == 0){
		// We draw the lines depicting the scores in 
		// the upper part of the screen
		field->SetObjectColor(RGBcolor(0.06f, 0.21f, 1.0f));
		switch (score1) {
			case (1):
				field->DrawScoreLine(SCORE1_POSU, SCORE1_POSB);	
				break; 
			case (2):
				field->DrawScoreLine(Point2d(SCORE1_POSU.x + 1.0f,
					SCORE1_POSU.y), Point2d(SCORE1_POSB.x + 1.0f,
					SCORE1_POSB.y));
				break; 
			case (3):
				field->DrawScoreLine(Point2d(SCORE1_POSU.x + 2.0f,
					SCORE1_POSU.y), Point2d(SCORE1_POSB.x + 2.0f,
					SCORE1_POSB.y));
				break;
			default:
				break;
		}
	}
	if (team == 1){
		field->SetObjectColor(RGBcolor(0.86f, 1.0f, 0.31f));
		switch (score2) {
			case (1):
				field->DrawScoreLine(SCORE2_POSU, SCORE2_POSB);	
				break; 
			case (2):
				field->DrawScoreLine(Point2d(SCORE2_POSU.x + 1.0f,
					SCORE2_POSU.y), Point2d(SCORE2_POSB.x + 1.0f,
					SCORE2_POSB.y));
				break; 
			case (3):
				field->DrawScoreLine(Point2d(SCORE2_POSU.x + 2.0f,
					SCORE2_POSU.y), Point2d(SCORE2_POSB.x + 2.0f,
					SCORE2_POSB.y));
				break;
			default:
				break;
		}
	}
}

/* It translates the goalkeeper from a team in a certain 
 * direction. The direction should be negative for left 
 * and positive for right */ 
void Game::TranslateGoalKeeper(int team, int direction){
	float const SPEED_X = 0.25f;
	float offset = 2*BALL_CIRCLE_RAY + 1.0f, 
		  & left = Field::UP_MIDDLE_LEFT.x, 
		  & right = Field::UP_MIDDLE_RIGHT.x;
	if (direction > 1){
		direction = 1;
	}
	if (team == 0){
		if ((player1_pos[0].x > left + offset && direction < 0) ||
			(player1_pos[0].x < right - offset && direction > 0)){
			// We translate the goal keeper only if he we still be in the interval
			// of the goal after the translation will be made
			player1[0].TranslateCircle(m_system, direction*SPEED_X, 0);
			player1_pos[0].x += direction*SPEED_X;
			if (m_team == 0 && m_player == 0){
				// In case the goalkeeper holds the ball we also need to
				// translate the ball
				ball->TranslateCircle(m_system, direction*SPEED_X, 0);
			}
		}
	}
	else if (team == 1){
		if ((player2_pos[0].x > left + offset && direction < 0) || 
			(player2_pos[0].x < right - offset && direction > 0)){
			// Same as above
			player2[0].TranslateCircle(m_system, direction*SPEED_X, 0);
			player2_pos[0].x += direction*SPEED_X;
			if (m_team == 1 && m_player == 0){
				ball->TranslateCircle(m_system, direction*SPEED_X, 0);
			}
		}
	}
}

/* Calculates elapsed seconds since the last frame. 
 * It's called in main every frame. Depending on the 
 * elapsed time the ball moves with a distance multiple
 * of m_dt every frame. This way the speed is constant 
 * no matter what the framerate is.

 * Also the method caps the framerate at 200fps.*/ 
void Game::CalculateElapsedSeconds(){
	float current_time = (float)clock();
	m_dt = current_time - m_last_time;
	float last_frame_time = m_last_time;
	// We limit the game at 200 FPS
	while ((float)CLOCKS_PER_SEC/m_dt > 200.0f){
		current_time = (float)clock(); 
		m_dt = current_time - last_frame_time;
	}
	m_last_time = current_time;
	m_dt /= CLOCKS_PER_SEC;
	// Print the framerate in the console 
	//std::cout << "Framerate: " << (float)1/m_dt << std::endl;
}


/* Applies changes(transformations) to objects and 
 * the method is called every frame in main. Therefore
 * contributes to the effect of animation. */ 
void Game::Run(){
	Point2d next_position;
	float distance, nextx, nexty;
	bool reset = false;
	
	SpectatorsCheers();
	switch (state){
		case (HOLD):
			if (score1 == 3 || score2 == 3)
				// We use a bool and not a state because
				// we want the game to be restarted when 
				// the player hits the ball again.
				m_restart = true;
			break;
		case (GOAL):
		case (WALK): {
			// In case m_restart is true we must restart the game
			if (m_restart){
				state = RESTART; 
				m_restart = false;
				break;
			}

			// Move if in a lower section
			// We check if somebody scored a goal
			if (state == WALK){
				m_last_scored = CheckGoal(true);
				if (m_last_scored != -1){
					DrawScoreForTeam(m_last_scored);
					reset = CheckGoalLimit();
					if (reset == true) {
						// Might be that the ball need to be 
						// reseted immediately after it crossed the
						// line of the gate. That can happend if 
						// the speed is to high
						BeginSpectatorsCheers();
						PickNewRandomPlayer(!m_last_scored);
					}
					state = GOAL;
				}
				else {
					// If the ball hits the wall it will bounce off
					WallHitCheck();
				}
			}
			else if (state == GOAL){
				reset = CheckGoalLimit();
				// If the ball reached the limit withing the goal
				// then we give it to a player from the other team
				if (reset == true){
					BeginSpectatorsCheers();
					PickNewRandomPlayer(!m_last_scored);
					state = HOLD;
				}
			}

			// We check for collisions with other players
			nextx = SPEED * deltax * m_dt;
			nexty = SPEED * deltay * m_dt;
			next_position.set(ball->GetObject()->axiscenter.x + nextx, 
						  ball->GetObject()->axiscenter.y + nexty);
			
			std::vector<Point2d>::iterator it;

			// with players from team1
			for (it = player1_pos.begin(); it != player1_pos.end(); ++it){
				distance = CheckCollision(next_position, *it);
				if (distance > 0.0f){ 
					m_team = 0;
					m_player = it - player1_pos.begin();
					if (m_player != m_player_last || m_team != m_team_last){
						state = HOLD;
					}
				}
			}
			
			// with players from team2
			for (it = player2_pos.begin(); it != player2_pos.end(); ++it){
				distance = CheckCollision(next_position, *it);
				if (distance > 0.0f){
					m_team = 1;
					m_player = it - player2_pos.begin();
					if (m_player != m_player_last || m_team != m_team_last){
						state = HOLD;
					}
				}
			}

			// In the case the ball has been thrown then 
			// we translate it on the field
			m_system->objectTranslate(ball->GetObject(), nextx, nexty);

			break;
		}
		case (BLANK):{
			// We are going to wait
			Sleep(3000);
			// And then we are going to start the game again
			Init();
			state = HOLD;
			break;
		}
		case (RESTART):{
			field->RemoveScoreLines();
			PickNewRandomPlayer(rand() % 2);
			// We need to wait for the next frame to see the
			// changes on the screen. Therefore we use another
			// state.
			state = BLANK;
			m_restart = false;
			ResetScore();
			m_system->objectRemoveAll();
			player1_pos.clear();
			player2_pos.clear();
			player1.clear(); 
			player2.clear();
			break;
		}
		
	}

}