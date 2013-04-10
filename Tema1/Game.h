#pragma once 
#include "HeadersAndDefines.h"
#include "Circle.h"
#include <time.h>

class Field;

// A enum which describes the state in which the game is
enum State { HOLD, WALK, GOAL, RESTART, BLANK };

/* Contains dynamic/animated elements of the game 
 * and also the logic of the game. Takes various 
 * decisions depending on the positions and posesion 
 * of the ball on the field */ 
class Game{

	public:
		Game(CoordinateSystem2d* system);
		virtual ~Game();
		void Init();
		void RotateClockwise(int team);
		void RotateCounterClockwise(int team);
		void CalculateSlope();
		void KickEvent(int team);
		float CheckCollision(Point2d &ball_center, Point2d &center);
		int CheckGoal(bool increase);
		bool CheckGoalLimit();
		void WallHitCheck();
		void PickNewRandomPlayer(int team);
		void ResetScore();
		void DrawScoreForTeam(int team);
		void TranslateGoalKeeper(int team, int direction);
		void Run();
		
		void DrawSpectators(Point2d corner, int team);
		void DrawSpectators();
		void RemoveSpectators(int team);
		void BeginSpectatorsCheers();
		void SpectatorsCheers();

		void CalculateElapsedSeconds();

	private:
		CoordinateSystem2d* m_system;
		Field* field;
		// positions of the players on the field
		std::vector<Point2d> player1_pos; 
		std::vector<Point2d> player2_pos;
		// the actual players
		std::vector<Circle>	player1;
		std::vector<Circle> player2;
		// The team and the player who hold the ball
		int m_team, m_player;
		int m_team_last, m_player_last, m_last_scored;
		// The scores of team1 and team2
		int score1, score2;
		Point2d ball_position;
		Circle *ball;
		float deltax, deltay;
		State state;
		bool m_restart;
		// Spectator related attributes
		clock_t cheer_start;
		int frames_to_cheer;
		std::vector<Object2d*>	spectators1, spectators2;

		// The time of the last frame
		float m_last_time;
		float m_dt;
	
		// Some constants
		static const float PLAYER_CIRCLE_RAY, BALL_CIRCLE_RAY,
						   SPECTATOR_CIRCLE_RAY, ROTATION_STEP, 
						   GOAL_OFFSET, SPEED;
		static const int  FRAMES_TO_CHEER_LIMIT;
};
