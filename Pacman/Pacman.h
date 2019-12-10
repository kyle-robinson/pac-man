#pragma once

// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr
#ifdef WIN32 
	#ifndef _DEBUG
		#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	#endif
#endif

// Definitions
#define MUNCHIECOUNT 294
#define	GHOSTCOUNT 10
#define WALLCOUNT 376

// Include main header file
#include "S2D/S2D.h"
//#include "S2D/GL/freeglut_std.h"
//#include "S2D/GL/freeglut_ext.h"

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

// Pacman Structure Definition
struct Player
{
	Vector2* position;
	Rect* sourceRect;
	Texture2D* texture;
	float speed;
	float speedMultiplier;
	int direction;
	int frame;
	int currentFrameTime;
	int frameTime;
	int score;
	bool dead;
};

// Wall Structure Definition
struct Wall
{
	Vector2* position;
	Rect* sourceRect;
	Texture2D* texture;
};

// Enemy Structure Definition
struct Enemy
{
	Vector2* position;
	Rect* sourceRect;
	Rect* rect;
	Texture2D* texture;
	int frame;
	int currentFrameTime;
	int frameTime;
	bool rKeyDown = false;
	bool eaten = false;
};

// Moving Enemy Structure Definition
struct MovingEnemy
{
	Vector2* position;
	Texture2D* texture;
	Rect* sourceRect;
	int direction;
	float speed;
};

// Menu Structure Definition
struct Menu
{
	Vector2* stringPosition;
	Vector2* scoreStringPosition;
	Vector2* restartStringPosition;
	Vector2* winStringPosition;
	Vector2* highScoreStringPosition;
	Vector2* playAgainStringPosition;
	Vector2* helpStringPosition;
	Vector2* helpListStringPosition;
	Rect* rectangle;
	Texture2D* background;
	bool paused;
	bool helpOpen;
	bool pKeyDown;
	bool hKeyDown;
};

// Declares the Pacman class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.
class Pacman : public Game
{
private:
	/// <summary> Allows the user to pause the game. </summary>
	void CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey);

	/// <summary> Gets input from the user to move Pac-Man. </summary>
	void Input(int elapsedTime, Input::KeyboardState* state);

	/// <summary> Animate Pac-Man as he moves around the screen. </summary>
	void UpdatePacman(int elapsedTime);

	/// <summary> Wrap Pac-Man across the screen when he moves out of bounds. </summary>
	void CheckViewportCollision();

	/// <summary> Set positions of the wall tiles in the game. </summary>
	void LoadWalls(Wall*, int wallNumber);

	/// <summary> Set positions of the munchies in the game. </summary>
	void LoadMunchies(Enemy*, int munchieNumber);

	/// <summary> Add collisions to the wall tiles. </summary>
	void CheckWallCollisions(MovingEnemy* ghost, int elapsedTime);

	/// <summary> Animate the munchies on screen. </summary>
	void UpdateMunchie(Enemy* munchie, int elapsedTime);

	/// <summary> Animate the cherry, add collisions, and add mouse functionality. </summary>
	void UpdateCherry(Enemy* cherry, int elapsedTime, Input::KeyboardState* state, Input::MouseState* mouseState);

	/// <summary> Add collisions to the munchies. </summary>
	void CheckMunchieCollisions();

	/// <summary> Animate the ghosts when they touch the edge of the viewport. </summary>
	void UpdateGhost(MovingEnemy*, int elapsedTime);

	/// <summary> Make one of the ghosts move to pacman's position. </summary>
	void TrackGhost(MovingEnemy*, int elapsedTime, Input::MouseState* mouseState);

	/// <summary> Add collisions for when the ghost touches Pac-Man. </summary>
	void CheckGhostCollisions();

	/// <summary> Close game when ESCAPE key is pressed. </summary>
	void CloseGame(Input::KeyboardState* state);

	/// <summary> Define co-ordinates for ghost to patrol around the screen. </summary>
	//void PatrolGhost(MovingEnemy*, int elapsedTime);

	/// <summary> Efficent method for updating the munchies in the game. </summary>
	//void UpdateMunchie(Enemy& munchie, int elapsedTime);

	// Structure Pointers
	Player* _pacman;
	Enemy* _munchies[MUNCHIECOUNT];
	Enemy* _cherry;
	MovingEnemy* _ghosts[GHOSTCOUNT];
	Menu* _menu;
	Wall* _wall[WALLCOUNT];
	//Player* _player2;
	//Enemy** _munchies;

	// Position for String
	Vector2* _stringPosition;

	// Audio Pointers
	SoundEffect* _pop;
	SoundEffect* _pauseMusic;
	SoundEffect* _backgroundMusic;
	SoundEffect* _deathSound;
	SoundEffect* _victorySound;

	// Variable for dynamic munchie count
	//int munchieCount;

public:
	/// <summary> Constructs the Pacman class. </summary>
	Pacman(int argc, char* argv[]);

	/// <summary> Destroys any data associated with Pacman class. </summary>
	virtual ~Pacman();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);
};