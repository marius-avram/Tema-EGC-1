//-----------------------------------------------------------------------------------------------
//	TEMA 1: Fotbal Static
//	Proiect dezvoltat pe baza celui folosit in cadrul laboratorului 1 de EGC.
//----------------------------------------------------------------------------------------------

#include "WorldDrawer2d.h"
#include "Game.h"
bool WorldDrawer2d::animation=true;


//used global vars
CoordinateSystem2d *cs;
Game *game;

void WorldDrawer2d::init(){
	// Create new coordinate system
	cs = new CoordinateSystem2d();
	cs_used.push_back(cs);
	

	// Initialize game in the coordinate system
	game = new Game(cs);
	game->Init();
	
}
void WorldDrawer2d::onIdle(){	//per frame
	static int iteration=1;
	static bool o1dir=true;
	static bool o2dir=true;
	static bool o3dir=true;
	static bool o3dir2=true;
	if(animation){
		game->CalculateElapsedSeconds();
		game->Run();
		iteration++;
	}
}

void WorldDrawer2d::onKey(unsigned char key){
	switch(key){
		// The control of player 2
		case 13: // Enter key
			game->KickEvent(1);
			break;
		case KEY_UP:
			game->RotateClockwise(1);
			break;
		case KEY_DOWN:
			game->RotateCounterClockwise(1);
			break;
		case GLUT_KEY_LEFT:
			game->TranslateGoalKeeper(1, -1);
			break;
		case KEY_RIGHT:
			game->TranslateGoalKeeper(1, 1);
			break;
		case KEY_SPACE:
			animation=!animation;
			break;
		// The controls of player 1
		case 's':
			game->RotateClockwise(0);
			break;
		case 'x':
			game->RotateCounterClockwise(0);
			break;
		case 'z': 
			game->TranslateGoalKeeper(0, -1);
			break;
		case 'c':
			game->TranslateGoalKeeper(0, 1);
		case 'a':
			game->KickEvent(0);
			break;
		default:
			break;
	}
}


int main(int argc, char** argv){
	WorldDrawer2d wd2d(argc,argv,700,600,200,80,std::string("Tema 1: Fotbal Static"));
	wd2d.init();
	wd2d.run();
	return 0;
}