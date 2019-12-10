#include "Pacman.h"
#include <sstream>
#include <time.h>
#include <iostream>

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv)
{
	srand(time(NULL));
	
	// Initialise member variables
	_pacman = new Player();
	_menu = new Menu();
	_pop = new SoundEffect();
	_pauseMusic = new SoundEffect();
	_backgroundMusic = new SoundEffect();
	_deathSound = new SoundEffect();
	_victorySound = new SoundEffect();
	//_player2 = new Player();

	_pacman->speedMultiplier = 1.0f; // Initialise pacman's speed multiplier

	// Efficient method
	//_munchies = new Enemy[munchieCount];

	// Initialise munchie variables
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchies[i] = new Enemy();
		_munchies[i]->currentFrameTime = 0;
		_munchies[i]->frame = rand() % 1;
		_munchies[i]->frameTime = rand() % 500 + 50;
	}

	// Load Ghost Character Data
	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		_ghosts[i] = new MovingEnemy();
		_ghosts[i]->direction = 0;
		_ghosts[i]->speed = 0.2f;
	}

	// Load Wall Sprite Data
	for (int i = 0; i < WALLCOUNT; i++)
	{
		_wall[i] = new Wall();
	}

	// Initialise cherry variables
	_cherry = new Enemy();
	_cherry->currentFrameTime = 0;
	_cherry->frame = rand() % 1;
	_cherry->frameTime = rand() % 500 + 50;

	//Initialise important Game aspects
	Audio::Initialise();
	Graphics::Initialise(argc, argv, this, 1000, 760, false, 25, 25, "Pac-Man", 60);
	Input::Initialise();

	Graphics::StartGameLoop(); // Start the Game Loop - This calls Update and Draw in game loop
}

Pacman::~Pacman()
{	
	// Clean up pointers within the Player structure
	delete _pacman->position;
	delete _pacman->sourceRect;
	delete _pacman->texture;

	delete _pacman; // Clean up the Player structure pointer

	// Clean up pointers within the Enemy structure
	delete _munchies[0]->texture;
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		delete _munchies[i]->sourceRect;
		delete _munchies[i]->rect;
	}
	delete[] _munchies; // Clean up the Enemy structure pointer for munchies

	delete _cherry; // Clean up Enemy stucture pointer for cherry

	// Clean up the pointers within the MovingEnemy struct
	delete _ghosts[0]->texture;
	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		delete _ghosts[i]->position;
		delete _ghosts[i]->sourceRect;
	}
	delete[] _ghosts; // Clean up the MovingEnemy struct pointer for the ghosts

	// Clean up the pointers within the Wall struct
	delete _wall[0]->texture;
	for (int i = 0; i < WALLCOUNT; i++)
	{
		delete _wall[i]->position;
		delete _wall[i]->sourceRect;
	}
	delete[] _wall;

	// Clean up Audio pointers
	delete _pop;
	delete _pauseMusic;
	delete _backgroundMusic;
	delete _deathSound;
	delete _victorySound;
}

void Pacman::LoadContent()
{
	// Load Pacman
	_pacman->texture = new Texture2D();
	_pacman->texture->Load("Textures/r2d2.png", false);
	_pacman->position = new Vector2(Graphics::GetViewportWidth() / 2.05, Graphics::GetViewportHeight() / 2.1);
	_pacman->sourceRect = new Rect(0.0f, 0.0f, 28, 27);
	_pacman->direction = 0;
	_pacman->frameTime = 0;
	_pacman->frame = 0;
	_pacman->speed = 0.1f;
	_pacman->frameTime = 50;
	_pacman->dead = false;

	// Load walls
	Texture2D* wallTex = new Texture2D();
	wallTex->Load("Textures/WallTexture.png", true);
	for (int i = 0; i < WALLCOUNT; i++)
	{
		_wall[i]->texture = wallTex;
		_wall[i]->sourceRect = new Rect(0.0f, 0.0f, 20, 20);
		LoadWalls(_wall[i], i);
	}

	// Load Player 2
	/*_player2->texture = new Texture2D();
	_player2->texture->Load("Textures/Pacman.tga", false);
	_player2->position = new Vector2(350.0f, 250.0f);
	_player2->sourceRect = new Rect(0.0f, 0.0f, 32, 32);
	_player2->direction = 0;
	_player2->frameTime = 0;
	_player2->frame = 0;
	_player2->speed = 0.1f;
	_player2->frameTime = 250;
	_player2->dead = false;*/

	// Load Munchie Data
	Texture2D* munchieTex = new Texture2D();
	munchieTex->Load("Textures/MunchieSpriteSheet.png", true);
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchies[i]->texture = munchieTex;
		_munchies[i]->currentFrameTime = 500;
		_munchies[i]->sourceRect = new Rect(150.0f, 300.0f, 12, 12);
		_munchies[i]->frame = 0;
		LoadMunchies(_munchies[i], i);
	}

	// Load Ghost Character Data
	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		_ghosts[i]->texture = new Texture2D();
		_ghosts[i]->position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
		_ghosts[i]->sourceRect = new Rect(0.0f, 0.0f, 20, 20);
		_ghosts[i]->direction = 0;
	}
	_ghosts[0]->position = new Vector2(20.0f, 40.0f);
	_ghosts[1]->position = new Vector2(880.0f, 40.0f);
	_ghosts[1]->direction = 1;
	_ghosts[2]->position = new Vector2(600.0f, 140.0f);
	_ghosts[3]->position = new Vector2(400.0f, 220.0f);
	_ghosts[3]->direction = 1;
	_ghosts[4]->position = new Vector2(600.0f, 220.0f);
	_ghosts[5]->position = new Vector2(600.0f, 520.0f);
	_ghosts[5]->direction = 1;
	_ghosts[6]->position = new Vector2(400.0f, 520.0f);
	_ghosts[7]->position = new Vector2(300.0f, 600.0f);
	_ghosts[7]->direction = 1;
	_ghosts[8]->position = new Vector2(20.0f, 700.0f);
	_ghosts[9]->position = new Vector2(880.0f, 700.0f);
	_ghosts[9]->direction = 1;
	_ghosts[0]->texture->Load("Textures/BlueGhostSpriteSheet.png", false);
	_ghosts[1]->texture->Load("Textures/OrangeGhostSpriteSheet.png", false);
	_ghosts[2]->texture->Load("Textures/RedGhostSpriteSheet.png", false);
	_ghosts[3]->texture->Load("Textures/PinkGhostSpriteSheet.png", false);
	_ghosts[4]->texture->Load("Textures/BlueGhostSpriteSheet.png", false);
	_ghosts[5]->texture->Load("Textures/OrangeGhostSpriteSheet.png", false);
	_ghosts[6]->texture->Load("Textures/RedGhostSpriteSheet.png", false);
	_ghosts[7]->texture->Load("Textures/PinkGhostSpriteSheet.png", false);
	_ghosts[8]->texture->Load("Textures/BlueGhostSpriteSheet.png", false);
	_ghosts[9]->texture->Load("Textures/OrangeGhostSpriteSheet.png", false);
	
	// Load Cherry Data
	Texture2D* cherryTex = new Texture2D();
	cherryTex->Load("Textures/CherrySpriteSheetSmall.png", true);
	_cherry->texture = cherryTex;
	_cherry->currentFrameTime = 500;
	_cherry->position = new Vector2(490.0f, 100.0f);
	_cherry->sourceRect = new Rect(150.0f, 300.0f, 12, 12);
	_cherry->frame = 0;

	// Load Menu
	_menu->background = new Texture2D();
	_menu->background->Load("Textures/Transparency.png", false);
	_menu->rectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_menu->paused = false;
	_menu->pKeyDown = false;

	// Set string position
	_stringPosition = new Vector2(10.0f, 25.0f);
	_menu->stringPosition = new Vector2(Graphics::GetViewportWidth() / 2.2f, Graphics::GetViewportHeight() / 2.0f);
	_menu->scoreStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.1f, 25.0f);
	_menu->helpStringPosition = new Vector2(Graphics::GetViewportWidth() - 250, 25.0f);
	_menu->helpListStringPosition = new Vector2(Graphics::GetViewportWidth() - 250, 25.0f);

	_menu->restartStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.5f, Graphics::GetViewportHeight() / 1.9f);
	_menu->winStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.3f, Graphics::GetViewportHeight() / 2.0f);

	_menu->highScoreStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.27f, Graphics::GetViewportHeight() / 1.9f);
	_menu->playAgainStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.5f, Graphics::GetViewportHeight() / 1.8f);

	// Load Game Audio
	_pop->Load("Sounds/munch.wav");
	_pauseMusic->Load("Sounds/pause-music.wav");
	_backgroundMusic->Load("Sounds/background-music.wav");
	_deathSound->Load("Sounds/death-sound.wav");
	_victorySound->Load("Sounds/victory.wav");
	Audio::Play(_backgroundMusic);
}

void Pacman::Update(int elapsedTime)
{	
	// Stop certain game audio from looping
	_backgroundMusic->SetLooping(true);
	_pauseMusic->SetLooping(true);
	_pop->SetLooping(false);
	_victorySound->SetLooping(false);
	_deathSound->SetLooping(false);

	
	// Get keyboard and mouse state
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();
	Input::MouseState* mouseState = Input::Mouse::GetState();

	CheckPaused(keyboardState, Input::Keys::P); // Checks to see if the P key is pressed
	
	if (!_menu->paused) // Check to see if the game is paused
	{
		_backgroundMusic->SetPitch(1.5f);
		
		// Pac-Man
		Input(elapsedTime, keyboardState); // Get input from the player to move Pacman
		UpdatePacman(elapsedTime); // Animate Pacman
		CheckViewportCollision(); // Manage wall wrapping for Pacman
		
		for (int i = 0; i < GHOSTCOUNT; i++)
		{
			CheckWallCollisions(_ghosts[i], elapsedTime); // Check for ghost and pacman collisions with the walls
		}

		// Munchies
		for (int i = 0; i < MUNCHIECOUNT; i++)
		{
			UpdateMunchie(_munchies[i], elapsedTime); // Animate Munchies
		}
		UpdateCherry(_cherry, elapsedTime, keyboardState, mouseState); // Move and Animate the Cherry
		CheckMunchieCollisions(); // Checks for Pacman collision with munchie
		
		// Ghosts
		for (int i = 0; i < GHOSTCOUNT; i++)
		{
			UpdateGhost(_ghosts[i], elapsedTime); // Move ghosts around the screen
		}
		TrackGhost(_ghosts[9], elapsedTime, mouseState);
		//PatrolGhost(_ghosts[3], elapsedTime); // Ghost patrols around the map
		CheckGhostCollisions(); // Checks for ghost collision with Pacman

		CloseGame(keyboardState);
	}
	else
	{
		_backgroundMusic->SetPitch(1.25f); // Lower music pitch when game is paused
	}
}

void Pacman::Draw(int elapsedTime)
{
	SpriteBatch::BeginDraw(); // Starts Drawing
	
	// Draw Pacman
	if (!_pacman->dead) // If pacman is alive
	{
		SpriteBatch::Draw(_pacman->texture, _pacman->position, _pacman->sourceRect); // Draw Pacman to the screen
	}

	// Draw Walls
	for (int i = 0; i < WALLCOUNT; i++)
	{
		SpriteBatch::Draw(_wall[i]->texture, _wall[i]->position, _wall[i]->sourceRect); // Draw Walls to the screen
	}

	// Draw Munchies
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		SpriteBatch::Draw(_munchies[i]->texture, _munchies[i]->position, _munchies[i]->sourceRect); // Draw Munchies to the screen
	}
	SpriteBatch::Draw(_cherry->texture, _cherry->position, _cherry->sourceRect); // Draw Cherry to the screen
	
	// Draw Ghosts
	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		SpriteBatch::Draw(_ghosts[i]->texture, _ghosts[i]->position, _ghosts[i]->sourceRect); // Draw Ghosts to the screen
	}
	
	// Draw User Interface
	std::stringstream stream;
	std::stringstream scoreStream;
	std::stringstream helpStream;
	std::stringstream helpListStream;

	stream << "Pacman X: " << _pacman->position->X << " Y: " << _pacman->position->Y;
	scoreStream << "Score: " << _pacman->score;
	helpStream << "Press [H] for help";
	helpListStream << "W A S D = Move Pac-Man" << endl << "LSHIFT = Move Faster" << endl << "R = Move Cherry" << endl << "LMOUSE = Move Cherry" << endl << "RMOUSE = Move Ghost" << endl << "P = Pause Game" << endl << "ESC = Exit Game" << endl << "H = Close Options";

	SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green); // Draws String updating Pacman's coordinates
	SpriteBatch::DrawString(scoreStream.str().c_str(), _menu->scoreStringPosition, Color::Cyan); // Draws String updating Pacman's coordinates
	if (_menu->helpOpen)
	{
		SpriteBatch::DrawString(helpListStream.str().c_str(), _menu->helpListStringPosition, Color::Yellow); // Draw String with list of tips on how to play the game
	}
	else
	{
		SpriteBatch::DrawString(helpStream.str().c_str(), _menu->helpStringPosition, Color::Yellow);
	}

	// Draw Menu
	if (_menu->paused)
	{
		std::stringstream menuStream;
		menuStream << "PAUSED!";

		SpriteBatch::Draw(_menu->background, _menu->rectangle, nullptr); // Transparency background
		SpriteBatch::DrawString(menuStream.str().c_str(), _menu->stringPosition, Color::Red); // Draw red text
	}

	//Draw Game Over screen
	if (_pacman->dead)
	{
		std::stringstream gameOverStream;
		std::stringstream restartStream;
		gameOverStream << "GAME OVER!";
		restartStream << "Press ESC to exit the game.";

		SpriteBatch::Draw(_menu->background, _menu->rectangle, nullptr); // Transparency background
		SpriteBatch::DrawString(gameOverStream.str().c_str(), _menu->stringPosition, Color::Red); // Game over text
		SpriteBatch::DrawString(restartStream.str().c_str(), _menu->restartStringPosition, Color::White); // Ask the user to try again

		// Draw buttons to restart or exit game
	}

	// Draw winning game screen
	if (_pacman->score >= 3000)
	{
		std::stringstream winStream;
		std::stringstream highScoreStream;
		std::stringstream restartStream;
		winStream << "A WINNER IS YOU!";
		highScoreStream << "HIGH SCORE: " << _pacman->score;
		restartStream << "Press ESC to exit the game.";

		SpriteBatch::Draw(_menu->background, _menu->rectangle, nullptr); // Transparency background
		SpriteBatch::DrawString(winStream.str().c_str(), _menu->winStringPosition, Color::Green); // Display winning message
		SpriteBatch::DrawString(highScoreStream.str().c_str(), _menu->highScoreStringPosition, Color::Cyan); // Display high score
		SpriteBatch::DrawString(restartStream.str().c_str(), _menu->playAgainStringPosition, Color::White); // Ask the user to play again
	}

	SpriteBatch::EndDraw(); // Ends Drawing
}

void Pacman::CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey)
{
	// Check for paused game
	if (state->IsKeyDown(Input::Keys::P) && !_menu->pKeyDown) // Checks if P key is pressed
	{
		_menu->pKeyDown = true;
		_menu->paused = !_menu->paused;
	}

	if (state->IsKeyUp(Input::Keys::P))
	{
		_menu->pKeyDown = false;
	}

	// Check for list of game options
	if (state->IsKeyDown(Input::Keys::H) && !_menu->hKeyDown) // Checks if H key is pressed
	{
		_menu->hKeyDown = true;
		_menu->helpOpen = !_menu->helpOpen;
	}

	if (state->IsKeyUp(Input::Keys::H))
	{
		_menu->hKeyDown = false;
	}
}

void Pacman::Input(int elapsedTime, Input::KeyboardState *state)
{	
	/* Pacman Speed Multiplier */
	float pacmanSpeed = _pacman->speed * elapsedTime * _pacman->speedMultiplier; // Effcient speed variable implementation

	if (state->IsKeyDown(Input::Keys::LEFTSHIFT)) // Checks if LEFT SHIFT is pressed
	{
		_pacman->speedMultiplier = 2.0f; // Double Pacman's movement speed
	}
	else
	{
		_pacman->speedMultiplier = 1.0f; // Reset Pacman's movement speed
	}
	
	/* Pacman Movement */
	if (state->IsKeyDown(Input::Keys::W)) // Checks if W key is pressed
	{
		_pacman->position->Y -= pacmanSpeed; //Moves Pacman up along the Y axis
		_pacman->direction = 3; // Make Pacman face up
	}
	else if (state->IsKeyDown(Input::Keys::A)) // Checks if A key is pressed
	{
		_pacman->position->X -= pacmanSpeed; //Moves Pacman left along the X axis
		_pacman->direction = 2; // Make Pacman face right
	}
	else if (state->IsKeyDown(Input::Keys::S)) // Checks if S key is pressed
	{
		_pacman->position->Y += pacmanSpeed; //Moves Pacman down along the Y axis
		_pacman->direction = 1; // Make Pacman face down
	}
	else if (state->IsKeyDown(Input::Keys::D)) // Checks if D key is pressed
	{
		_pacman->position->X += pacmanSpeed; //Moves Pacman right along the X axis
		_pacman->direction = 0; // Make Pacman face left
	}
}

void Pacman::UpdatePacman(int elapsedTime)
{
	// Handle Pac-Man animation
	_pacman->currentFrameTime += elapsedTime;
	if (_pacman->currentFrameTime > _pacman->frameTime)
	{
		_pacman->frame++;

		if (_pacman->frame >= 4)
			_pacman->frame = 0;

		_pacman->currentFrameTime = 0;
	}
	_pacman->sourceRect->X = _pacman->sourceRect->Width * _pacman->frame; // Animate Pacman by changing sprite within the spritesheet
	_pacman->sourceRect->Y = _pacman->sourceRect->Height * _pacman->direction; // Change the direction Pacman is facing

	// Move Pac-Man off screen when he dies
	if (_pacman->dead)
	{
		_pacman->position->X = -1000;
		_pacman->position->Y = -1000;
	}
}

void Pacman::CheckViewportCollision()
{
	if (_pacman->position->X >= Graphics::GetViewportWidth() - _pacman->sourceRect->Width) // right wall collsion
	{
		_pacman->position->X = 0; // wrap Pacman back to the left wall
	}

	if (_pacman->position->X < 0) // left wall collision
	{
		_pacman->position->X = Graphics::GetViewportWidth() - _pacman->sourceRect->Width; // wrap Pacman back to the right wall
	}

	if (_pacman->position->Y >= Graphics::GetViewportHeight() - _pacman->sourceRect->Height) // bottom wall collision
	{
		_pacman->position->Y = 0; // wrap Pacman to the top of the viewport
	}

	if (_pacman->position->Y < 0) // top wall collision
	{
		_pacman->position->Y = Graphics::GetViewportHeight() - _pacman->sourceRect->Height; // wrap Pacman to the bottom of the viewport
	}
}

void Pacman::LoadWalls(Wall* _wall, int wallNumber)
{
	// Get array index of wall and set individual position for each tile on the map
	switch (wallNumber)
	{
	// Top Row Walls
	case 0:
		_wall->position = new Vector2(0.0f, 0.0f);
		break;
	case 1:
		_wall->position = new Vector2(20.0f, 0.0f);
		break;
	case 2:
		_wall->position = new Vector2(40.0f, 0.0f);
		break;
	case 3:
		_wall->position = new Vector2(60.0f, 0.0f);
		break;
	case 4:
		_wall->position = new Vector2(80.0f, 0.0f);
		break;
	case 5:
		_wall->position = new Vector2(100.0f, 0.0f);
		break;
	case 6:
		_wall->position = new Vector2(120.0f, 0.0f);
		break;
	case 7:
		_wall->position = new Vector2(140.0f, 0.0f);
		break;
	case 8:
		_wall->position = new Vector2(160.0f, 0.0f);
		break;
	case 9:
		_wall->position = new Vector2(180.0f, 0.0f);
		break;
	case 10:
		_wall->position = new Vector2(200.0f, 0.0f);
		break;
	case 11:
		_wall->position = new Vector2(220.0f, 0.0f);
		break;
	case 12:
		_wall->position = new Vector2(240.0f, 0.0f);
		break;
	case 13:
		_wall->position = new Vector2(260.0f, 0.0f);
		break;
	case 14:
		_wall->position = new Vector2(280.0f, 0.0f);
		break;
	case 15:
		_wall->position = new Vector2(300.0f, 0.0f);
		break;
	case 16:
		_wall->position = new Vector2(320.0f, 0.0f);
		break;
	case 17:
		_wall->position = new Vector2(340.0f, 0.0f);
		break;
	case 18:
		_wall->position = new Vector2(360.0f, 0.0f);
		break;
	case 19:
		_wall->position = new Vector2(380.0f, 0.0f);
		break;
	case 20:
		_wall->position = new Vector2(400.0f, 0.0f);
		break;
	case 21:
		_wall->position = new Vector2(420.0f, 0.0f);
		break;
		// Remove Wall Start
	case 22:
		_wall->position = new Vector2(420.0f, 20.0f);
		break;
	case 23:
		_wall->position = new Vector2(420.0f, 40.0f);
		break;
	case 24:
		_wall->position = new Vector2(-480.0f, 0.0f);
		break;
	case 25:
		_wall->position = new Vector2(540.0f, 20.0f);
		break;
	case 26:
		_wall->position = new Vector2(540.0f, 40.0f);
		break;
		// Remove Wall End
	case 27:
		_wall->position = new Vector2(540.0f, 0.0f);
		break;
	case 28:
		_wall->position = new Vector2(560.0f, 0.0f);
		break;
	case 29:
		_wall->position = new Vector2(580.0f, 0.0f);
		break;
	case 30:
		_wall->position = new Vector2(600.0f, 0.0f);
		break;
	case 31:
		_wall->position = new Vector2(620.0f, 0.0f);
		break;
	case 32:
		_wall->position = new Vector2(640.0f, 0.0f);
		break;
	case 33:
		_wall->position = new Vector2(660.0f, 0.0f);
		break;
	case 34:
		_wall->position = new Vector2(680.0f, 0.0f);
		break;
	case 35:
		_wall->position = new Vector2(700.0f, 0.0f);
		break;
	case 36:
		_wall->position = new Vector2(720.0f, 0.0f);
		break;
	case 37:
		_wall->position = new Vector2(740.0f, 0.0f);
		break;
	case 38:
		_wall->position = new Vector2(760.0f, 0.0f);
		break;
	case 39:
		_wall->position = new Vector2(780.0f, 0.0f);
		break;
	case 40:
		_wall->position = new Vector2(800.0f, 0.0f);
		break;
	case 41:
		_wall->position = new Vector2(820.0f, 0.0f);
		break;
	case 42:
		_wall->position = new Vector2(840.0f, 0.0f);
		break;
	case 43:
		_wall->position = new Vector2(860.0f, 0.0f);
		break;
	case 44:
		_wall->position = new Vector2(880.0f, 0.0f);
		break;
	case 45:
		_wall->position = new Vector2(900.0f, 0.0f);
		break;
	case 46:
		_wall->position = new Vector2(920.0f, 0.0f);
		break;
	case 47:
		_wall->position = new Vector2(940.0f, 0.0f);
		break;
	case 48:
		_wall->position = new Vector2(960.0f, 0.0f);
		break;
	case 49:
		_wall->position = new Vector2(980.0f, 0.0f);
		break;
	// Left Column Walls
	case 50:
		_wall->position = new Vector2(0.0f, 20.0f);
		break;
	case 51:
		_wall->position = new Vector2(0.0f, 40.0f);
		break;
	case 52:
		_wall->position = new Vector2(0.0f, 60.0f);
		break;
	case 53:
		_wall->position = new Vector2(0.0f, 80.0f);
		break;
	case 54:
		_wall->position = new Vector2(0.0f, 100.0f);
		break;
	case 55:
		_wall->position = new Vector2(0.0f, 120.0f);
		break;
	case 56:
		_wall->position = new Vector2(0.0f, 140.0f);
		break;
	case 57:
		_wall->position = new Vector2(0.0f, 160.0f);
		break;
	case 58:
		_wall->position = new Vector2(0.0f, 180.0f);
		break;
	case 59:
		_wall->position = new Vector2(0.0f, 200.0f);
		break;
	case 60:
		_wall->position = new Vector2(0.0f, 220.0f);
		break;
	case 61:
		_wall->position = new Vector2(0.0f, 240.0f);
		break;
	case 62:
		_wall->position = new Vector2(0.0f, 260.0f);
		break;
	case 63:
		_wall->position = new Vector2(0.0f, 280.0f);
		break;
	case 64:
		_wall->position = new Vector2(0.0f, 300.0f);
		break;
	case 65:
		_wall->position = new Vector2(0.0f, 320.0f);
		break;
		// Remove Wall Start
	case 66:
		_wall->position = new Vector2(20.0f, 320.0f);
		break;
	case 67:
		_wall->position = new Vector2(40.0f, 320.0f);
		break;
	case 68:
		_wall->position = new Vector2(20.0f, 420.0f);
		break;
	case 69:
		_wall->position = new Vector2(40.0f, 420.0f);
		break;
		// Remove Wall End
	case 70:
		_wall->position = new Vector2(0.0f, 420.0f);
		break;
	case 71:
		_wall->position = new Vector2(0.0f, 440.0f);
		break;
	case 72:
		_wall->position = new Vector2(0.0f, 460.0f);
		break;
	case 73:
		_wall->position = new Vector2(0.0f, 480.0f);
		break;
	case 74:
		_wall->position = new Vector2(0.0f, 500.0f);
		break;
	case 75:
		_wall->position = new Vector2(0.0f, 520.0f);
		break;
	case 76:
		_wall->position = new Vector2(0.0f, 540.0f);
		break;
	case 77:
		_wall->position = new Vector2(0.0f, 560.0f);
		break;
	case 78:
		_wall->position = new Vector2(0.0f, 580.0f);
		break;
	case 79:
		_wall->position = new Vector2(0.0f, 600.0f);
		break;
	case 80:
		_wall->position = new Vector2(0.0f, 620.0f);
		break;
	case 81:
		_wall->position = new Vector2(0.0f, 640.0f);
		break;
	case 82:
		_wall->position = new Vector2(0.0f, 660.0f);
		break;
	case 83:
		_wall->position = new Vector2(0.0f, 680.0f);
		break;
	case 84:
		_wall->position = new Vector2(0.0f, 700.0f);
		break;
	case 85:
		_wall->position = new Vector2(0.0f, 720.0f);
		break;
	case 86:
		_wall->position = new Vector2(0.0f, 740.0f);
		break;
	// Right Column Walls
	case 87:
		_wall->position = new Vector2(980.0f, 20.0f);
		break;
	case 88:
		_wall->position = new Vector2(980.0f, 40.0f);
		break;
	case 89:
		_wall->position = new Vector2(980.0f, 60.0f);
		break;
	case 90:
		_wall->position = new Vector2(980.0f, 80.0f);
		break;
	case 91:
		_wall->position = new Vector2(980.0f, 100.0f);
		break;
	case 92:
		_wall->position = new Vector2(980.0f, 120.0f);
		break;
	case 93:
		_wall->position = new Vector2(980.0f, 140.0f);
		break;
	case 94:
		_wall->position = new Vector2(980.0f, 160.0f);
		break;
	case 95:
		_wall->position = new Vector2(980.0f, 180.0f);
		break;
	case 96:
		_wall->position = new Vector2(980.0f, 200.0f);
		break;
	case 97:
		_wall->position = new Vector2(980.0f, 220.0f);
		break;
	case 98:
		_wall->position = new Vector2(980.0f, 240.0f);
		break;
	case 99:
		_wall->position = new Vector2(980.0f, 260.0f);
		break;
	case 100:
		_wall->position = new Vector2(980.0f, 280.0f);
		break;
	case 101:
		_wall->position = new Vector2(980.0f, 300.0f);
		break;
	case 102:
		_wall->position = new Vector2(980.0f, 320.0f);
		break;
		// Remove Wall Start
	case 103:
		_wall->position = new Vector2(960.0f, 320.0f);
		break;
	case 104:
		_wall->position = new Vector2(940.0f, 320.0f);
		break;
	case 105:
		_wall->position = new Vector2(960.0f, 420.0f);
		break;
	case 106:
		_wall->position = new Vector2(940.0f, 420.0f);
		break;
		// Remove Wall End
	case 107:
		_wall->position = new Vector2(980.0f, 420.0f);
		break;
	case 108:
		_wall->position = new Vector2(980.0f, 440.0f);
		break;
	case 109:
		_wall->position = new Vector2(980.0f, 460.0f);
		break;
	case 110:
		_wall->position = new Vector2(980.0f, 480.0f);
		break;
	case 111:
		_wall->position = new Vector2(980.0f, 500.0f);
		break;
	case 112:
		_wall->position = new Vector2(980.0f, 520.0f);
		break;
	case 113:
		_wall->position = new Vector2(980.0f, 540.0f);
		break;
	case 114:
		_wall->position = new Vector2(980.0f, 560.0f);
		break;
	case 115:
		_wall->position = new Vector2(980.0f, 580.0f);
		break;
	case 116:
		_wall->position = new Vector2(980.0f, 600.0f);
		break;
	case 117:
		_wall->position = new Vector2(980.0f, 620.0f);
		break;
	case 118:
		_wall->position = new Vector2(980.0f, 640.0f);
		break;
	case 119:
		_wall->position = new Vector2(980.0f, 660.0f);
		break;
	case 120:
		_wall->position = new Vector2(980.0f, 680.0f);
		break;
	case 121:
		_wall->position = new Vector2(980.0f, 700.0f);
		break;
	case 122:
		_wall->position = new Vector2(980.0f, 720.0f);
		break;
	case 123:
		_wall->position = new Vector2(980.0f, 740.0f);
		break;
	// Bottom Row Walls
	case 124:
		_wall->position = new Vector2(20.0f, 740.0f);
		break;
	case 125:
		_wall->position = new Vector2(40.0f, 740.0f);
		break;
	case 126:
		_wall->position = new Vector2(60.0f, 740.0f);
		break;
	case 127:
		_wall->position = new Vector2(80.0f, 740.0f);
		break;
	case 128:
		_wall->position = new Vector2(100.0f, 740.0f);
		break;
	case 129:
		_wall->position = new Vector2(120.0f, 740.0f);
		break;
	case 130:
		_wall->position = new Vector2(140.0f, 740.0f);
		break;
	case 131:
		_wall->position = new Vector2(160.0f, 740.0f);
		break;
	case 132:
		_wall->position = new Vector2(180.0f, 740.0f);
		break;
	case 133:
		_wall->position = new Vector2(200.0f, 740.0f);
		break;
	case 134:
		_wall->position = new Vector2(220.0f, 740.0f);
		break;
	case 135:
		_wall->position = new Vector2(240.0f, 740.0f);
		break;
	case 136:
		_wall->position = new Vector2(260.0f, 740.0f);
		break;
	case 137:
		_wall->position = new Vector2(280.0f, 740.0f);
		break;
	case 138:
		_wall->position = new Vector2(300.0f, 740.0f);
		break;
	case 139:
		_wall->position = new Vector2(320.0f, 740.0f);
		break;
	case 140:
		_wall->position = new Vector2(340.0f, 740.0f);
		break;
	case 141:
		_wall->position = new Vector2(360.0f, 740.0f);
		break;
	case 142:
		_wall->position = new Vector2(380.0f, 740.0f);
		break;
	case 143:
		_wall->position = new Vector2(400.0f, 740.0f);
		break;
	case 144:
		_wall->position = new Vector2(420.0f, 740.0f);
		break;
	case 145:
		_wall->position = new Vector2(440.0f, 740.0f);
		break;
		// Remove Wall Start
	case 146:
		_wall->position = new Vector2(440.0f, 720.0f);
		break;
	case 147:
		_wall->position = new Vector2(440.0f, 700.0f);
		break;
	case 148:
		_wall->position = new Vector2(540.0f, 720.0f);
		break;
	case 149:
		_wall->position = new Vector2(540.0f, 700.0f);
		break;
		// Remove Wall End
	case 150:
		_wall->position = new Vector2(540.0f, 740.0f);
		break;
	case 151:
		_wall->position = new Vector2(560.0f, 740.0f);
		break;
	case 152:
		_wall->position = new Vector2(580.0f, 740.0f);
		break;
	case 153:
		_wall->position = new Vector2(600.0f, 740.0f);
		break;
	case 154:
		_wall->position = new Vector2(620.0f, 740.0f);
		break;
	case 155:
		_wall->position = new Vector2(640.0f, 740.0f);
		break;
	case 156:
		_wall->position = new Vector2(660.0f, 740.0f);
		break;
	case 157:
		_wall->position = new Vector2(680.0f, 740.0f);
		break;
	case 158:
		_wall->position = new Vector2(700.0f, 740.0f);
		break;
	case 159:
		_wall->position = new Vector2(720.0f, 740.0f);
		break;
	case 160:
		_wall->position = new Vector2(740.0f, 740.0f);
		break;
	case 161:
		_wall->position = new Vector2(760.0f, 740.0f);
		break;
	case 162:
		_wall->position = new Vector2(780.0f, 740.0f);
		break;
	case 163:
		_wall->position = new Vector2(800.0f, 740.0f);
		break;
	case 164:
		_wall->position = new Vector2(820.0f, 740.0f);
		break;
	case 165:
		_wall->position = new Vector2(840.0f, 740.0f);
		break;
	case 166:
		_wall->position = new Vector2(860.0f, 740.0f);
		break;
	case 167:
		_wall->position = new Vector2(880.0f, 740.0f);
		break;
	case 168:
		_wall->position = new Vector2(900.0f, 740.0f);
		break;
	case 169:
		_wall->position = new Vector2(920.0f, 740.0f);
		break;
	case 170:
		_wall->position = new Vector2(940.0f, 740.0f);
		break;
	case 171:
		_wall->position = new Vector2(960.0f, 740.0f);
		break;
	// Top Left Section
		// Rectangle 1
	case 172:
		_wall->position = new Vector2(100.0f, 80.0f);
		break;
	case 173:
		_wall->position = new Vector2(120.0f, 80.0f);
		break;
	case 174:
		_wall->position = new Vector2(140.0f, 80.0f);
		break;
	case 175:
		_wall->position = new Vector2(100.0f, 100.0f);
		break;
	case 176:
		_wall->position = new Vector2(120.0f, 100.0f);
		break;
	case 177:
		_wall->position = new Vector2(140.0f, 100.0f);
		break;
		// Rectangle 2
	case 178:
		_wall->position = new Vector2(100.0f, 180.0f);
		break;
	case 179:
		_wall->position = new Vector2(120.0f, 180.0f);
		break;
	case 180:
		_wall->position = new Vector2(140.0f, 180.0f);
		break;
		// Rectangle 3
	case 181:
		_wall->position = new Vector2(240.0f, 80.0f);
		break;
	case 182:
		_wall->position = new Vector2(260.0f, 80.0f);
		break;
	case 183:
		_wall->position = new Vector2(280.0f, 80.0f);
		break;
	case 184:
		_wall->position = new Vector2(300.0f, 80.0f);
		break;
	case 185:
		_wall->position = new Vector2(240.0f, 100.0f);
		break;
	case 186:
		_wall->position = new Vector2(260.0f, 100.0f);
		break;
	case 187:
		_wall->position = new Vector2(280.0f, 100.0f);
		break;
	case 188:
		_wall->position = new Vector2(300.0f, 100.0f);
		break;
		// T-Shape 1
	case 189:
		_wall->position = new Vector2(240.0f, 180.0f);
		break;
	case 190:
		_wall->position = new Vector2(240.0f, 200.0f);
		break;
	case 191:
		_wall->position = new Vector2(240.0f, 220.0f);
		break;
	case 192:
		_wall->position = new Vector2(240.0f, 240.0f);
		break;
	case 193:
		_wall->position = new Vector2(240.0f, 260.0f);
		break;
	case 194:
		_wall->position = new Vector2(240.0f, 280.0f);
		break;
	case 195:
		_wall->position = new Vector2(240.0f, 300.0f);
		break;
	case 196:
		_wall->position = new Vector2(240.0f, 320.0f);
		break;
	case 197:
		_wall->position = new Vector2(220.0f, 320.0f);
		break;
	case 198:
		_wall->position = new Vector2(200.0f, 320.0f);
		break;
	case 199:
		_wall->position = new Vector2(180.0f, 320.0f);
		break;
		// Added part to the left wall
	case 200:
		_wall->position = new Vector2(60.0f, 320.0f);
		break;
		// Add bottom part to 2nd rectangle
	case 201:
		_wall->position = new Vector2(120.0f, 200.0f);
		break;
	case 202:
		_wall->position = new Vector2(120.0f, 220.0f);
		break;
	case 203:
		_wall->position = new Vector2(120.0f, 240.0f);
		break;
		// Right part of T-Shape
	case 204:
		_wall->position = new Vector2(260.0f, 260.0f);
		break;
	case 205:
		_wall->position = new Vector2(280.0f, 260.0f);
		break;
	case 206:
		_wall->position = new Vector2(300.0f, 260.0f);
		break;
	case 207:
		_wall->position = new Vector2(320.0f, 260.0f);
		break;
	case 208:
		_wall->position = new Vector2(340.0f, 260.0f);
		break;
	case 209:
		_wall->position = new Vector2(360.0f, 260.0f);
		break;
	// Top-Middle T-Shape
	case 210:
		_wall->position = new Vector2(340.0f, 180.0f);
		break;
	case 211:
		_wall->position = new Vector2(360.0f, 180.0f);
		break;
	case 212:
		_wall->position = new Vector2(380.0f, 180.0f);
		break;
	case 213:
		_wall->position = new Vector2(400.0f, 180.0f);
		break;
	case 214:
		_wall->position = new Vector2(420.0f, 180.0f);
		break;
	case 215:
		_wall->position = new Vector2(440.0f, 180.0f);
		break;
	case 216:
		_wall->position = new Vector2(460.0f, 180.0f);
		break;
	case 217:
		_wall->position = new Vector2(480.0f, 180.0f);
		break;
	case 218:
		_wall->position = new Vector2(500.0f, 180.0f);
		break;
	case 219:
		_wall->position = new Vector2(520.0f, 180.0f);
		break;
	case 220:
		_wall->position = new Vector2(540.0f, 180.0f);
		break;
	case 221:
		_wall->position = new Vector2(560.0f, 180.0f);
		break;
	case 222:
		_wall->position = new Vector2(580.0f, 180.0f);
		break;
	case 223:
		_wall->position = new Vector2(600.0f, 180.0f);
		break;
	case 224:
		_wall->position = new Vector2(620.0f, 180.0f);
		break;
	case 225:
		_wall->position = new Vector2(640.0f, 180.0f);
		break;
	case 226:
		_wall->position = new Vector2(480.0f, 200.0f);
		break;
	case 227:
		_wall->position = new Vector2(480.0f, 220.0f);
		break;
	case 228:
		_wall->position = new Vector2(480.0f, 240.0f);
		break;
	case 229:
		_wall->position = new Vector2(480.0f, 260.0f);
		break;
	case 230:
		_wall->position = new Vector2(480.0f, 280.0f);
		break;
	case 231:
		_wall->position = new Vector2(500.0f, 200.0f);
		break;
	case 232:
		_wall->position = new Vector2(500.0f, 220.0f);
		break;
	case 233:
		_wall->position = new Vector2(500.0f, 240.0f);
		break;
	case 234:
		_wall->position = new Vector2(500.0f, 260.0f);
		break;
	case 235:
		_wall->position = new Vector2(500.0f, 280.0f);
		break;
	// Top Right Section
		// Rectangle 1
	case 236:
		_wall->position = new Vector2(880.0f, 80.0f);
		break;
	case 237:
		_wall->position = new Vector2(860.0f, 80.0f);
		break;
	case 238:
		_wall->position = new Vector2(840.0f, 80.0f);
		break;
	case 239:
		_wall->position = new Vector2(880.0f, 100.0f);
		break;
	case 240:
		_wall->position = new Vector2(860.0f, 100.0f);
		break;
	case 241:
		_wall->position = new Vector2(840.0f, 100.0f);
		break;
		// Rectangle 2
	case 242:
		_wall->position = new Vector2(880.0f, 180.0f);
		break;
	case 243:
		_wall->position = new Vector2(860.0f, 180.0f);
		break;
	case 244:
		_wall->position = new Vector2(840.0f, 180.0f);
		break;
		// Rectangle 3
	case 245:
		_wall->position = new Vector2(740.0f, 80.0f);
		break;
	case 246:
		_wall->position = new Vector2(720.0f, 80.0f);
		break;
	case 247:
		_wall->position = new Vector2(700.0f, 80.0f);
		break;
	case 248:
		_wall->position = new Vector2(680.0f, 80.0f);
		break;
	case 249:
		_wall->position = new Vector2(740.0f, 100.0f);
		break;
	case 250:
		_wall->position = new Vector2(720.0f, 100.0f);
		break;
	case 251:
		_wall->position = new Vector2(700.0f, 100.0f);
		break;
	case 252:
		_wall->position = new Vector2(680.0f, 100.0f);
		break;
		// T-Shape 1
	case 253:
		_wall->position = new Vector2(740.0f, 180.0f);
		break;
	case 254:
		_wall->position = new Vector2(740.0f, 200.0f);
		break;
	case 255:
		_wall->position = new Vector2(740.0f, 220.0f);
		break;
	case 256:
		_wall->position = new Vector2(740.0f, 240.0f);
		break;
	case 257:
		_wall->position = new Vector2(740.0f, 260.0f);
		break;
	case 258:
		_wall->position = new Vector2(740.0f, 280.0f);
		break;
	case 259:
		_wall->position = new Vector2(740.0f, 300.0f);
		break;
	case 260:
		_wall->position = new Vector2(740.0f, 320.0f);
		break;
	case 261:
		_wall->position = new Vector2(760.0f, 320.0f);
		break;
	case 262:
		_wall->position = new Vector2(780.0f, 320.0f);
		break;
	case 263:
		_wall->position = new Vector2(800.0f, 320.0f);
		break;
		// Added part to the right wall
	case 264:
		_wall->position = new Vector2(920.0f, 320.0f);
		break;
		// Add bottom part to 2nd rectangle
	case 265:
		_wall->position = new Vector2(860.0f, 200.0f);
		break;
	case 266:
		_wall->position = new Vector2(860.0f, 220.0f);
		break;
	case 267:
		_wall->position = new Vector2(860.0f, 240.0f);
		break;
		// Right part of T-Shape
	case 268:
		_wall->position = new Vector2(720.0f, 260.0f);
		break;
	case 269:
		_wall->position = new Vector2(700.0f, 260.0f);
		break;
	case 270:
		_wall->position = new Vector2(680.0f, 260.0f);
		break;
	case 271:
		_wall->position = new Vector2(660.0f, 260.0f);
		break;
	case 272:
		_wall->position = new Vector2(640.0f, 260.0f);
		break;
	case 273:
		_wall->position = new Vector2(620.0f, 260.0f);
		break;
	// Bottom Left Section
		// Rectangle 1
	case 274:
		_wall->position = new Vector2(100.0f, 660.0f);
		break;
	case 275:
		_wall->position = new Vector2(120.0f, 660.0f);
		break;
	case 276:
		_wall->position = new Vector2(140.0f, 660.0f);
		break;
	case 277:
		_wall->position = new Vector2(100.0f, 640.0f);
		break;
	case 278:
		_wall->position = new Vector2(120.0f, 640.0f);
		break;
	case 279:
		_wall->position = new Vector2(140.0f, 640.0f);
		break;
		// Rectangle 2
	case 280:
		_wall->position = new Vector2(100.0f, 560.0f);
		break;
	case 281:
		_wall->position = new Vector2(120.0f, 560.0f);
		break;
	case 282:
		_wall->position = new Vector2(140.0f, 560.0f);
		break;
		// Rectangle 3
	case 283:
		_wall->position = new Vector2(240.0f, 660.0f);
		break;
	case 284:
		_wall->position = new Vector2(260.0f, 660.0f);
		break;
	case 285:
		_wall->position = new Vector2(280.0f, 660.0f);
		break;
	case 286:
		_wall->position = new Vector2(300.0f, 660.0f);
		break;
	case 287:
		_wall->position = new Vector2(240.0f, 640.0f);
		break;
	case 288:
		_wall->position = new Vector2(260.0f, 640.0f);
		break;
	case 289:
		_wall->position = new Vector2(280.0f, 640.0f);
		break;
	case 290:
		_wall->position = new Vector2(300.0f, 640.0f);
		break;
		// T-Shape 1
	case 291:
		_wall->position = new Vector2(240.0f, 560.0f);
		break;
	case 292:
		_wall->position = new Vector2(240.0f, 540.0f);
		break;
	case 293:
		_wall->position = new Vector2(240.0f, 520.0f);
		break;
	case 294:
		_wall->position = new Vector2(240.0f, 500.0f);
		break;
	case 295:
		_wall->position = new Vector2(240.0f, 480.0f);
		break;
	case 296:
		_wall->position = new Vector2(240.0f, 460.0f);
		break;
	case 297:
		_wall->position = new Vector2(240.0f, 440.0f);
		break;
	case 298:
		_wall->position = new Vector2(240.0f, 420.0f);
		break;
	case 299:
		_wall->position = new Vector2(220.0f, 420.0f);
		break;
	case 300:
		_wall->position = new Vector2(200.0f, 420.0f);
		break;
	case 301:
		_wall->position = new Vector2(180.0f, 420.0f);
		break;
		// Added part to the left wall
	case 302:
		_wall->position = new Vector2(60.0f, 420.0f);
		break;
		// Add bottom part to 2nd rectangle
	case 303:
		_wall->position = new Vector2(120.0f, 540.0f);
		break;
	case 304:
		_wall->position = new Vector2(120.0f, 520.0f);
		break;
	case 305:
		_wall->position = new Vector2(120.0f, 500.0f);
		break;
		// Right part of T-Shape
	case 306:
		_wall->position = new Vector2(260.0f, 480.0f);
		break;
	case 307:
		_wall->position = new Vector2(280.0f, 480.0f);
		break;
	case 308:
		_wall->position = new Vector2(300.0f, 480.0f);
		break;
	case 309:
		_wall->position = new Vector2(320.0f, 480.0f);
		break;
	case 310:
		_wall->position = new Vector2(340.0f, 480.0f);
		break;
	case 311:
		_wall->position = new Vector2(360.0f, 480.0f);
		break;
	// Bottom Right Section
		// Rectangle 1
	case 312:
		_wall->position = new Vector2(880.0f, 660.0f);
		break;
	case 313:
		_wall->position = new Vector2(860.0f, 660.0f);
		break;
	case 314:
		_wall->position = new Vector2(840.0f, 660.0f);
		break;
	case 315:
		_wall->position = new Vector2(880.0f, 640.0f);
		break;
	case 316:
		_wall->position = new Vector2(860.0f, 640.0f);
		break;
	case 317:
		_wall->position = new Vector2(840.0f, 640.0f);
		break;
		// Rectangle 2
	case 318:
		_wall->position = new Vector2(880.0f, 560.0f);
		break;
	case 319:
		_wall->position = new Vector2(860.0f, 560.0f);
		break;
	case 320:
		_wall->position = new Vector2(840.0f, 560.0f);
		break;
		// Rectangle 3
	case 321:
		_wall->position = new Vector2(740.0f, 660.0f);
		break;
	case 322:
		_wall->position = new Vector2(720.0f, 660.0f);
		break;
	case 323:
		_wall->position = new Vector2(700.0f, 660.0f);
		break;
	case 324:
		_wall->position = new Vector2(680.0f, 660.0f);
		break;
	case 325:
		_wall->position = new Vector2(740.0f, 640.0f);
		break;
	case 326:
		_wall->position = new Vector2(720.0f, 640.0f);
		break;
	case 327:
		_wall->position = new Vector2(700.0f, 640.0f);
		break;
	case 328:
		_wall->position = new Vector2(680.0f, 640.0f);
		break;
		// T-Shape 1
	case 329:
		_wall->position = new Vector2(740.0f, 560.0f);
		break;
	case 330:
		_wall->position = new Vector2(740.0f, 540.0f);
		break;
	case 331:
		_wall->position = new Vector2(740.0f, 520.0f);
		break;
	case 332:
		_wall->position = new Vector2(740.0f, 500.0f);
		break;
	case 333:
		_wall->position = new Vector2(740.0f, 480.0f);
		break;
	case 334:
		_wall->position = new Vector2(740.0f, 460.0f);
		break;
	case 335:
		_wall->position = new Vector2(740.0f, 440.0f);
		break;
	case 336:
		_wall->position = new Vector2(740.0f, 420.0f);
		break;
	case 337:
		_wall->position = new Vector2(760.0f, 420.0f);
		break;
	case 338:
		_wall->position = new Vector2(780.0f, 420.0f);
		break;
	case 339:
		_wall->position = new Vector2(800.0f, 420.0f);
		break;
		// Added part to the left wall
	case 340:
		_wall->position = new Vector2(920.0f, 420.0f);
		break;
		// Add bottom part to 2nd rectangle
	case 341:
		_wall->position = new Vector2(860.0f, 540.0f);
		break;
	case 342:
		_wall->position = new Vector2(860.0f, 520.0f);
		break;
	case 343:
		_wall->position = new Vector2(860.0f, 500.0f);
		break;
		// Right part of T-Shape
	case 344:
		_wall->position = new Vector2(720.0f, 480.0f);
		break;
	case 345:
		_wall->position = new Vector2(700.0f, 480.0f);
		break;
	case 346:
		_wall->position = new Vector2(680.0f, 480.0f);
		break;
	case 347:
		_wall->position = new Vector2(660.0f, 480.0f);
		break;
	case 348:
		_wall->position = new Vector2(640.0f, 480.0f);
		break;
	case 349:
		_wall->position = new Vector2(620.0f, 480.0f);
		break;
	// Bottom-Middle T-Shape
	case 350:
		_wall->position = new Vector2(340.0f, 560.0f);
		break;
	case 351:
		_wall->position = new Vector2(360.0f, 560.0f);
		break;
	case 352:
		_wall->position = new Vector2(380.0f, 560.0f);
		break;
	case 353:
		_wall->position = new Vector2(400.0f, 560.0f);
		break;
	case 354:
		_wall->position = new Vector2(420.0f, 560.0f);
		break;
	case 355:
		_wall->position = new Vector2(440.0f, 560.0f);
		break;
	case 356:
		_wall->position = new Vector2(460.0f, 560.0f);
		break;
	case 357:
		_wall->position = new Vector2(480.0f, 560.0f);
		break;
	case 358:
		_wall->position = new Vector2(500.0f, 560.0f);
		break;
	case 359:
		_wall->position = new Vector2(520.0f, 560.0f);
		break;
	case 360:
		_wall->position = new Vector2(540.0f, 560.0f);
		break;
	case 361:
		_wall->position = new Vector2(560.0f, 560.0f);
		break;
	case 362:
		_wall->position = new Vector2(580.0f, 560.0f);
		break;
	case 363:
		_wall->position = new Vector2(600.0f, 560.0f);
		break;
	case 364:
		_wall->position = new Vector2(620.0f, 560.0f);
		break;
	case 365:
		_wall->position = new Vector2(640.0f, 560.0f);
		break;
	case 366:
		_wall->position = new Vector2(480.0f, 540.0f);
		break;
	case 367:
		_wall->position = new Vector2(480.0f, 520.0f);
		break;
	case 368:
		_wall->position = new Vector2(480.0f, 500.0f);
		break;
	case 369:
		_wall->position = new Vector2(480.0f, 480.0f);
		break;
	case 370:
		_wall->position = new Vector2(480.0f, 460.0f);
		break;
	case 371:
		_wall->position = new Vector2(500.0f, 540.0f);
		break;
	case 372:
		_wall->position = new Vector2(500.0f, 520.0f);
		break;
	case 373:
		_wall->position = new Vector2(500.0f, 500.0f);
		break;
	case 374:
		_wall->position = new Vector2(500.0f, 480.0f);
		break;
	case 375:
		_wall->position = new Vector2(500.0f, 460.0f);
		break;
	default:
		cout << "Walls don't work! Plz fix dem xx" << endl;
		break;
	}
}

void Pacman::LoadMunchies(Enemy* _munchie, int munchieNumber)
{
	// Get array index of the munchie and set individual position throughout the map
	switch (munchieNumber)
	{
	// Top-Left Corner munchies
		// 1st Row
	case 0:
		_munchie->position = new Vector2(30.0f, 30.0f);
		break;
	case 1:
		_munchie->position = new Vector2(70.0f, 30.0f);
		break;
	case 2:
		_munchie->position = new Vector2(110.0f, 30.0f);
		break;
	case 3:
		_munchie->position = new Vector2(150.0f, 30.0f);
		break;
	case 4:
		_munchie->position = new Vector2(190.0f, 30.0f);
		break;
	case 5:
		_munchie->position = new Vector2(230.0f, 30.0f);
		break;
	case 6:
		_munchie->position = new Vector2(270.0f, 30.0f);
		break;
	case 7:
		_munchie->position = new Vector2(310.0f, 30.0f);
		break;
	case 8:
		_munchie->position = new Vector2(350.0f, 30.0f);
		break;
	case 9:
		_munchie->position = new Vector2(390.0f, 30.0f);
		break;
		// 2nd Row
	case 10:
		_munchie->position = new Vector2(30.0f, 60.0f);
		break;
	case 11:
		_munchie->position = new Vector2(70.0f, 60.0f);
		break;
	case 12:
		_munchie->position = new Vector2(110.0f, 60.0f);
		break;
	case 13:
		_munchie->position = new Vector2(150.0f, 60.0f);
		break;
	case 14:
		_munchie->position = new Vector2(190.0f, 60.0f);
		break;
	case 15:
		_munchie->position = new Vector2(230.0f, 60.0f);
		break;
	case 16:
		_munchie->position = new Vector2(270.0f, 60.0f);
		break;
	case 17:
		_munchie->position = new Vector2(310.0f, 60.0f);
		break;
	case 18:
		_munchie->position = new Vector2(350.0f, 60.0f);
		break;
	case 19:
		_munchie->position = new Vector2(390.0f, 60.0f);
		break;
		// 1st Column
	case 20:
		_munchie->position = new Vector2(30.0f, 90.0f);
		break;
	case 21:
		_munchie->position = new Vector2(70.0f, 90.0f);
		break;
	case 22:
		_munchie->position = new Vector2(30.0f, 120.0f);
		break;
	case 23:
		_munchie->position = new Vector2(70.0f, 120.0f);
		break;
	case 24:
		_munchie->position = new Vector2(30.0f, 150.0f);
		break;
	case 25:
		_munchie->position = new Vector2(70.0f, 150.0f);
		break;
	case 26:
		_munchie->position = new Vector2(30.0f, 180.0f);
		break;
	case 27:
		_munchie->position = new Vector2(70.0f, 180.0f);
		break;
	case 28:
		_munchie->position = new Vector2(30.0f, 210.0f);
		break;
	case 29:
		_munchie->position = new Vector2(70.0f, 210.0f);
		break;
	case 30:
		_munchie->position = new Vector2(30.0f, 240.0f);
		break;
	case 31:
		_munchie->position = new Vector2(70.0f, 240.0f);
		break;
	case 32:
		_munchie->position = new Vector2(30.0f, 270.0f);
		break;
	case 33:
		_munchie->position = new Vector2(70.0f, 270.0f);
		break;
	case 34:
		_munchie->position = new Vector2(30.0f, 300.0f);
		break;
	case 35:
		_munchie->position = new Vector2(70.0f, 300.0f);
		break;
		// 2nd Column
	case 36:
		_munchie->position = new Vector2(190.0f, 90.0f);
		break;
	case 37:
		_munchie->position = new Vector2(190.0f, 120.0f);
		break;
	case 38:
		_munchie->position = new Vector2(190.0f, 150.0f);
		break;
	case 39:
		_munchie->position = new Vector2(190.0f, 180.0f);
		break;
	case 40:
		_munchie->position = new Vector2(190.0f, 210.0f);
		break;
	case 41:
		_munchie->position = new Vector2(190.0f, 240.0f);
		break;
	case 42:
		_munchie->position = new Vector2(190.0f, 270.0f);
		break;
	case 43:
		_munchie->position = new Vector2(190.0f, 300.0f);
		break;
		// Top Row across screen
	case 44:
		_munchie->position = new Vector2(110.0f, 150.0f);
		break;
	case 45:
		_munchie->position = new Vector2(150.0f, 150.0f);
		break;
	case 46:
		_munchie->position = new Vector2(230.0f, 150.0f);
		break;
	case 47:
		_munchie->position = new Vector2(270.0f, 150.0f);
		break;
	case 48:
		_munchie->position = new Vector2(310.0f, 150.0f);
		break;
	case 49:
		_munchie->position = new Vector2(350.0f, 150.0f);
		break;
	case 50:
		_munchie->position = new Vector2(390.0f, 150.0f);
		break;
	case 51:
		_munchie->position = new Vector2(430.0f, 150.0f);
		break;
	case 52:
		_munchie->position = new Vector2(470.0f, 150.0f);
		break;
	case 53:
		_munchie->position = new Vector2(510.0f, 150.0f);
		break;
	case 54:
		_munchie->position = new Vector2(550.0f, 150.0f);
		break;
	case 55:
		_munchie->position = new Vector2(590.0f, 150.0f);
		break;
	case 56:
		_munchie->position = new Vector2(630.0f, 150.0f);
		break;
	case 57:
		_munchie->position = new Vector2(670.0f, 150.0f);
		break;
	case 58:
		_munchie->position = new Vector2(710.0f, 150.0f);
		break;
	case 59:
		_munchie->position = new Vector2(750.0f, 150.0f);
		break;
	case 60:
		_munchie->position = new Vector2(790.0f, 150.0f);
		break;
	case 61:
		_munchie->position = new Vector2(830.0f, 150.0f);
		break;
	case 62:
		_munchie->position = new Vector2(870.0f, 150.0f);
		break;
	case 63:
		_munchie->position = new Vector2(910.0f, 150.0f);
		break;
	case 64:
		_munchie->position = new Vector2(950.0f, 150.0f);
		break;
	case 65:
		_munchie->position = new Vector2(-990.0f, -150.0f); // forget this one
		break;
		// 3rd Column
	case 66:
		_munchie->position = new Vector2(350.0f, 90.0f);
		break;
	case 67:
		_munchie->position = new Vector2(390.0f, 90.0f);
		break;
	case 68:
		_munchie->position = new Vector2(350.0f, 120.0f);
		break;
	case 69:
		_munchie->position = new Vector2(390.0f, 120.0f);
		break;
		// 4th Row
	case 70:
		_munchie->position = new Vector2(110.0f, 270.0f);
		break;
	case 71:
		_munchie->position = new Vector2(150.0f, 270.0f);
		break;
	case 72:
		_munchie->position = new Vector2(110.0f, 300.0f);
		break;
	case 73:
		_munchie->position = new Vector2(150.0f, 300.0f);
		break;
		// 4th Column
	case 74:
		_munchie->position = new Vector2(270.0f, 180.0f);
		break;
	case 75:
		_munchie->position = new Vector2(310.0f, 180.0f);
		break;
	case 76:
		_munchie->position = new Vector2(270.0f, 210.0f);
		break;
	case 77:
		_munchie->position = new Vector2(310.0f, 210.0f);
		break;
	case 78:
		_munchie->position = new Vector2(270.0f, 240.0f);
		break;
	case 79:
		_munchie->position = new Vector2(310.0f, 240.0f);
		break;
		// 5th Row
	case 80:
		_munchie->position = new Vector2(350.0f, 210.0f);
		break;
	case 81:
		_munchie->position = new Vector2(390.0f, 210.0f);
		break;
	case 82:
		_munchie->position = new Vector2(430.0f, 210.0f);
		break;
	case 83:
		_munchie->position = new Vector2(350.0f, 240.0f);
		break;
	case 84:
		_munchie->position = new Vector2(390.0f, 240.0f);
		break;
	case 85:
		_munchie->position = new Vector2(430.0f, 240.0f);
		break;
	// Top-Right Corner munchies
		// 1st Row
	case 86:
		_munchie->position = new Vector2(950.0f, 30.0f);
		break;
	case 87:
		_munchie->position = new Vector2(910.0f, 30.0f);
		break;
	case 88:
		_munchie->position = new Vector2(870.0f, 30.0f);
		break;
	case 89:
		_munchie->position = new Vector2(830.0f, 30.0f);
		break;
	case 90:
		_munchie->position = new Vector2(790.0f, 30.0f);
		break;
	case 91:
		_munchie->position = new Vector2(750.0f, 30.0f);
		break;
	case 92:
		_munchie->position = new Vector2(710.0f, 30.0f);
		break;
	case 93:
		_munchie->position = new Vector2(670.0f, 30.0f);
		break;
	case 94:
		_munchie->position = new Vector2(630.0f, 30.0f);
		break;
	case 95:
		_munchie->position = new Vector2(590.0f, 30.0f);
		break;
		// 2nd Row
	case 96:
		_munchie->position = new Vector2(950.0f, 60.0f);
		break;
	case 97:
		_munchie->position = new Vector2(910.0f, 60.0f);
		break;
	case 98:
		_munchie->position = new Vector2(870.0f, 60.0f);
		break;
	case 99:
		_munchie->position = new Vector2(830.0f, 60.0f);
		break;
	case 100:
		_munchie->position = new Vector2(790.0f, 60.0f);
		break;
	case 101:
		_munchie->position = new Vector2(750.0f, 60.0f);
		break;
	case 102:
		_munchie->position = new Vector2(710.0f, 60.0f);
		break;
	case 103:
		_munchie->position = new Vector2(670.0f, 60.0f);
		break;
	case 104:
		_munchie->position = new Vector2(630.0f, 60.0f);
		break;
	case 105:
		_munchie->position = new Vector2(590.0f, 60.0f);
		break;
		// 1st Column
	case 106:
		_munchie->position = new Vector2(950.0f, 90.0f);
		break;
	case 107:
		_munchie->position = new Vector2(910.0f, 90.0f);
		break;
	case 108:
		_munchie->position = new Vector2(950.0f, 120.0f);
		break;
	case 109:
		_munchie->position = new Vector2(910.0f, 120.0f);
		break;
		// miss one here
	case 110:
		_munchie->position = new Vector2(950.0f, 180.0f);
		break;
	case 111:
		_munchie->position = new Vector2(910.0f, 180.0f);
		break;
	case 112:
		_munchie->position = new Vector2(950.0f, 210.0f);
		break;
	case 113:
		_munchie->position = new Vector2(910.0f, 210.0f);
		break;
	case 114:
		_munchie->position = new Vector2(950.0f, 240.0f);
		break;
	case 115:
		_munchie->position = new Vector2(910.0f, 240.0f);
		break;
	case 116:
		_munchie->position = new Vector2(950.0f, 270.0f);
		break;
	case 117:
		_munchie->position = new Vector2(910.0f, 270.0f);
		break;
	case 118:
		_munchie->position = new Vector2(950.0f, 300.0f);
		break;
	case 119:
		_munchie->position = new Vector2(910.0f, 300.0f);
		break;
		// 2nd Column
	case 120:
		_munchie->position = new Vector2(790.0f, 90.0f);
		break;
	case 121:
		_munchie->position = new Vector2(790.0f, 120.0f);
		break;
	case 122:
		_munchie->position = new Vector2(790.0f, 180.0f);
		break;
	case 123:
		_munchie->position = new Vector2(790.0f, 210.0f);
		break;
	case 124:
		_munchie->position = new Vector2(790.0f, 240.0f);
		break;
	case 125:
		_munchie->position = new Vector2(790.0f, 270.0f);
		break;
	case 126:
		_munchie->position = new Vector2(790.0f, 300.0f);
		break;
		// 3rd Column
	case 127:
		_munchie->position = new Vector2(590.0f, 90.0f);
		break;
	case 128:
		_munchie->position = new Vector2(630.0f, 90.0f);
		break;
	case 129:
		_munchie->position = new Vector2(590.0f, 120.0f);
		break;
	case 130:
		_munchie->position = new Vector2(630.0f, 120.0f);
		break;
		// 4th Row
	case 131:
		_munchie->position = new Vector2(830.0f, 270.0f);
		break;
	case 132:
		_munchie->position = new Vector2(870.0f, 270.0f);
		break;
	case 133:
		_munchie->position = new Vector2(830.0f, 300.0f);
		break;
	case 134:
		_munchie->position = new Vector2(870.0f, 300.0f);
		break;
		// 4th Column
	case 135:
		_munchie->position = new Vector2(670.0f, 180.0f);
		break;
	case 136:
		_munchie->position = new Vector2(710.0f, 180.0f);
		break;
	case 137:
		_munchie->position = new Vector2(670.0f, 210.0f);
		break;
	case 138:
		_munchie->position = new Vector2(710.0f, 210.0f);
		break;
	case 139:
		_munchie->position = new Vector2(670.0f, 240.0f);
		break;
	case 140:
		_munchie->position = new Vector2(710.0f, 240.0f);
		break;
		// 5th Row
	case 141:
		_munchie->position = new Vector2(550.0f, 210.0f);
		break;
	case 142:
		_munchie->position = new Vector2(590.0f, 210.0f);
		break;
	case 143:
		_munchie->position = new Vector2(630.0f, 210.0f);
		break;
	case 144:
		_munchie->position = new Vector2(550.0f, 240.0f);
		break;
	case 145:
		_munchie->position = new Vector2(590.0f, 240.0f);
		break;
	case 146:
		_munchie->position = new Vector2(630.0f, 240.0f);
		break;
	// Bottom-Left Corner munchies
		// 1st Row
	case 147:
		_munchie->position = new Vector2(30.0f, 720.0f);
		break;
	case 148:
		_munchie->position = new Vector2(70.0f, 720.0f);
		break;
	case 149:
		_munchie->position = new Vector2(110.0f, 720.0f);
		break;
	case 150:
		_munchie->position = new Vector2(150.0f, 720.0f);
		break;
	case 151:
		_munchie->position = new Vector2(190.0f, 720.0f);
		break;
	case 152:
		_munchie->position = new Vector2(230.0f, 720.0f);
		break;
	case 153:
		_munchie->position = new Vector2(270.0f, 720.0f);
		break;
	case 154:
		_munchie->position = new Vector2(310.0f, 720.0f);
		break;
	case 155:
		_munchie->position = new Vector2(350.0f, 720.0f);
		break;
	case 156:
		_munchie->position = new Vector2(390.0f, 720.0f);
		break;
		// 2nd Row
	case 157:
		_munchie->position = new Vector2(30.0f, 690.0f);
		break;
	case 158:
		_munchie->position = new Vector2(70.0f, 690.0f);
		break;
	case 159:
		_munchie->position = new Vector2(110.0f, 690.0f);
		break;
	case 160:
		_munchie->position = new Vector2(150.0f, 690.0f);
		break;
	case 161:
		_munchie->position = new Vector2(190.0f, 690.0f);
		break;
	case 162:
		_munchie->position = new Vector2(230.0f, 690.0f);
		break;
	case 163:
		_munchie->position = new Vector2(270.0f, 690.0f);
		break;
	case 164:
		_munchie->position = new Vector2(310.0f, 690.0f);
		break;
	case 165:
		_munchie->position = new Vector2(350.0f, 690.0f);
		break;
	case 166:
		_munchie->position = new Vector2(390.0f, 690.0f);
		break;
		// 1st Column
	case 167:
		_munchie->position = new Vector2(30.0f, 660.0f);
		break;
	case 168:
		_munchie->position = new Vector2(70.0f, 660.0f);
		break;
	case 169:
		_munchie->position = new Vector2(30.0f, 630.0f);
		break;
	case 170:
		_munchie->position = new Vector2(70.0f, 630.0f);
		break;
	case 171:
		_munchie->position = new Vector2(30.0f, 600.0f);
		break;
	case 172:
		_munchie->position = new Vector2(70.0f, 600.0f);
		break;
	case 173:
		_munchie->position = new Vector2(30.0f, 570.0f);
		break;
	case 174:
		_munchie->position = new Vector2(70.0f, 570.0f);
		break;
	case 175:
		_munchie->position = new Vector2(30.0f, 540.0f);
		break;
	case 176:
		_munchie->position = new Vector2(70.0f, 540.0f);
		break;
	case 177:
		_munchie->position = new Vector2(30.0f, 510.0f);
		break;
	case 178:
		_munchie->position = new Vector2(70.0f, 510.0f);
		break;
	case 179:
		_munchie->position = new Vector2(30.0f, 480.0f);
		break;
	case 180:
		_munchie->position = new Vector2(70.0f, 480.0f);
		break;
	case 181:
		_munchie->position = new Vector2(30.0f, 450.0f);
		break;
	case 182:
		_munchie->position = new Vector2(70.0f, 450.0f);
		break;
		// 2nd Column
	case 183:
		_munchie->position = new Vector2(190.0f, 660.0f);
		break;
	case 184:
		_munchie->position = new Vector2(190.0f, 630.0f);
		break;
	case 185:
		_munchie->position = new Vector2(190.0f, 600.0f);
		break;
	case 186:
		_munchie->position = new Vector2(190.0f, 570.0f);
		break;
	case 187:
		_munchie->position = new Vector2(190.0f, 540.0f);
		break;
	case 188:
		_munchie->position = new Vector2(190.0f, 510.0f);
		break;
	case 189:
		_munchie->position = new Vector2(190.0f, 480.0f);
		break;
	case 190:
		_munchie->position = new Vector2(190.0f, 450.0f);
		break;
		// Bottom Row across screen
	case 191:
		_munchie->position = new Vector2(110.0f, 600.0f);
		break;
	case 192:
		_munchie->position = new Vector2(150.0f, 600.0f);
		break;
	case 193:
		_munchie->position = new Vector2(230.0f, 600.0f);
		break;
	case 194:
		_munchie->position = new Vector2(270.0f, 600.0f);
		break;
	case 195:
		_munchie->position = new Vector2(310.0f, 600.0f);
		break;
	case 196:
		_munchie->position = new Vector2(350.0f, 600.0f);
		break;
	case 197:
		_munchie->position = new Vector2(390.0f, 600.0f);
		break;
	case 198:
		_munchie->position = new Vector2(430.0f, 600.0f);
		break;
	case 199:
		_munchie->position = new Vector2(470.0f, 600.0f);
		break;
	case 200:
		_munchie->position = new Vector2(510.0f, 600.0f);
		break;
	case 201:
		_munchie->position = new Vector2(550.0f, 600.0f);
		break;
	case 202:
		_munchie->position = new Vector2(590.0f, 600.0f);
		break;
	case 203:
		_munchie->position = new Vector2(630.0f, 600.0f);
		break;
	case 204:
		_munchie->position = new Vector2(670.0f, 600.0f);
		break;
	case 205:
		_munchie->position = new Vector2(710.0f, 600.0f);
		break;
	case 206:
		_munchie->position = new Vector2(750.0f, 600.0f);
		break;
	case 207:
		_munchie->position = new Vector2(790.0f, 600.0f);
		break;
	case 208:
		_munchie->position = new Vector2(830.0f, 600.0f);
		break;
	case 209:
		_munchie->position = new Vector2(870.0f, 600.0f);
		break;
	case 210:
		_munchie->position = new Vector2(910.0f, 600.0f);
		break;
	case 211:
		_munchie->position = new Vector2(950.0f, 600.0f);
		break;
	case 212:
		_munchie->position = new Vector2(-990.0f, -600.0f); // forget this one
		break;
		// 3rd Column
	case 213:
		_munchie->position = new Vector2(350.0f, 660.0f);
		break;
	case 214:
		_munchie->position = new Vector2(390.0f, 660.0f);
		break;
	case 215:
		_munchie->position = new Vector2(350.0f, 630.0f);
		break;
	case 216:
		_munchie->position = new Vector2(390.0f, 630.0f);
		break;
		// 4th Row
	case 217:
		_munchie->position = new Vector2(110.0f, 480.0f);
		break;
	case 218:
		_munchie->position = new Vector2(150.0f, 480.0f);
		break;
	case 219:
		_munchie->position = new Vector2(110.0f, 450.0f);
		break;
	case 220:
		_munchie->position = new Vector2(150.0f, 450.0f);
		break;
		// 4th Column
	case 221:
		_munchie->position = new Vector2(270.0f, 570.0f);
		break;
	case 222:
		_munchie->position = new Vector2(310.0f, 570.0f);
		break;
	case 223:
		_munchie->position = new Vector2(270.0f, 540.0f);
		break;
	case 224:
		_munchie->position = new Vector2(310.0f, 540.0f);
		break;
	case 225:
		_munchie->position = new Vector2(270.0f, 510.0f);
		break;
	case 226:
		_munchie->position = new Vector2(310.0f, 510.0f);
		break;
		// 5th Row
	case 227:
		_munchie->position = new Vector2(350.0f, 540.0f);
		break;
	case 228:
		_munchie->position = new Vector2(390.0f, 540.0f);
		break;
	case 229:
		_munchie->position = new Vector2(430.0f, 540.0f);
		break;
	case 230:
		_munchie->position = new Vector2(350.0f, 510.0f);
		break;
	case 231:
		_munchie->position = new Vector2(390.0f, 510.0f);
		break;
	case 232:
		_munchie->position = new Vector2(430.0f, 510.0f);
		break;
	// Top-Right Corner munchies
		// 1st Row
	case 233:
		_munchie->position = new Vector2(950.0f, 720.0f);
		break;
	case 234:
		_munchie->position = new Vector2(910.0f, 720.0f);
		break;
	case 235:
		_munchie->position = new Vector2(870.0f, 720.0f);
		break;
	case 236:
		_munchie->position = new Vector2(830.0f, 720.0f);
		break;
	case 237:
		_munchie->position = new Vector2(790.0f, 720.0f);
		break;
	case 238:
		_munchie->position = new Vector2(750.0f, 720.0f);
		break;
	case 239:
		_munchie->position = new Vector2(710.0f, 720.0f);
		break;
	case 240:
		_munchie->position = new Vector2(670.0f, 720.0f);
		break;
	case 241:
		_munchie->position = new Vector2(630.0f, 720.0f);
		break;
	case 242:
		_munchie->position = new Vector2(590.0f, 720.0f);
		break;
		// 2nd Row
	case 243:
		_munchie->position = new Vector2(950.0f, 690.0f);
		break;
	case 244:
		_munchie->position = new Vector2(910.0f, 690.0f);
		break;
	case 245:
		_munchie->position = new Vector2(870.0f, 690.0f);
		break;
	case 246:
		_munchie->position = new Vector2(830.0f, 690.0f);
		break;
	case 247:
		_munchie->position = new Vector2(790.0f, 690.0f);
		break;
	case 248:
		_munchie->position = new Vector2(750.0f, 690.0f);
		break;
	case 249:
		_munchie->position = new Vector2(710.0f, 690.0f);
		break;
	case 250:
		_munchie->position = new Vector2(670.0f, 690.0f);
		break;
	case 251:
		_munchie->position = new Vector2(630.0f, 690.0f);
		break;
	case 252:
		_munchie->position = new Vector2(590.0f, 690.0f);
		break;
		// 1st Column
	case 253:
		_munchie->position = new Vector2(950.0f, 660.0f);
		break;
	case 254:
		_munchie->position = new Vector2(910.0f, 660.0f);
		break;
	case 255:
		_munchie->position = new Vector2(950.0f, 630.0f);
		break;
	case 256:
		_munchie->position = new Vector2(910.0f, 630.0f);
		break;
		// miss one here
	case 257:
		_munchie->position = new Vector2(950.0f, 570.0f);
		break;
	case 258:
		_munchie->position = new Vector2(910.0f, 570.0f);
		break;
	case 259:
		_munchie->position = new Vector2(950.0f, 540.0f);
		break;
	case 260:
		_munchie->position = new Vector2(910.0f, 540.0f);
		break;
	case 261:
		_munchie->position = new Vector2(950.0f, 510.0f);
		break;
	case 262:
		_munchie->position = new Vector2(910.0f, 510.0f);
		break;
	case 263:
		_munchie->position = new Vector2(950.0f, 480.0f);
		break;
	case 264:
		_munchie->position = new Vector2(910.0f, 480.0f);
		break;
	case 265:
		_munchie->position = new Vector2(950.0f, 450.0f);
		break;
	case 266:
		_munchie->position = new Vector2(910.0f, 450.0f);
		break;
		// 2nd Column
	case 267:
		_munchie->position = new Vector2(790.0f, 660.0f);
		break;
	case 268:
		_munchie->position = new Vector2(790.0f, 630.0f);
		break;
	case 269:
		_munchie->position = new Vector2(790.0f, 570.0f);
		break;
	case 270:
		_munchie->position = new Vector2(790.0f, 540.0f);
		break;
	case 271:
		_munchie->position = new Vector2(790.0f, 510.0f);
		break;
	case 272:
		_munchie->position = new Vector2(790.0f, 480.0f);
		break;
	case 273:
		_munchie->position = new Vector2(790.0f, 450.0f);
		break;
		// 3rd Column
	case 274:
		_munchie->position = new Vector2(590.0f, 660.0f);
		break;
	case 275:
		_munchie->position = new Vector2(630.0f, 660.0f);
		break;
	case 276:
		_munchie->position = new Vector2(590.0f, 630.0f);
		break;
	case 277:
		_munchie->position = new Vector2(630.0f, 630.0f);
		break;
		// 4th Row
	case 278:
		_munchie->position = new Vector2(830.0f, 480.0f);
		break;
	case 279:
		_munchie->position = new Vector2(870.0f, 480.0f);
		break;
	case 280:
		_munchie->position = new Vector2(830.0f, 450.0f);
		break;
	case 281:
		_munchie->position = new Vector2(870.0f, 450.0f);
		break;
		// 4th Column
	case 282:
		_munchie->position = new Vector2(670.0f, 570.0f);
		break;
	case 283:
		_munchie->position = new Vector2(710.0f, 570.0f);
		break;
	case 284:
		_munchie->position = new Vector2(670.0f, 540.0f);
		break;
	case 285:
		_munchie->position = new Vector2(710.0f, 540.0f);
		break;
	case 286:
		_munchie->position = new Vector2(670.0f, 510.0f);
		break;
	case 287:
		_munchie->position = new Vector2(710.0f, 510.0f);
		break;
		// 5th Row
	case 288:
		_munchie->position = new Vector2(550.0f, 540.0f);
		break;
	case 289:
		_munchie->position = new Vector2(590.0f, 540.0f);
		break;
	case 290:
		_munchie->position = new Vector2(630.0f, 540.0f);
		break;
	case 291:
		_munchie->position = new Vector2(550.0f, 510.0f);
		break;
	case 292:
		_munchie->position = new Vector2(590.0f, 510.0f);
		break;
	case 293:
		_munchie->position = new Vector2(630.0f, 510.0f);
		break;
	default:
		cout << "Munchies don't work." << endl;
		break;
	}
}

void Pacman::CheckWallCollisions(MovingEnemy* ghost, int elapsedTime)
{
	// Local variables
	int i = 0;
	int bottom1 = _pacman->position->Y + _pacman->sourceRect->Height;
	int bottom2 = 0;
	int bottom3 = ghost->position->Y + ghost->sourceRect->Height;
	int left1 = _pacman->position->X;
	int left2 = 0;
	int left3 = ghost->position->X;
	int right1 = _pacman->position->X + _pacman->sourceRect->Width;
	int right2 = 0;
	int right3 = ghost->position->X + ghost->sourceRect->Width;
	int top1 = _pacman->position->Y;
	int top2 = 0;
	int top3 = ghost->position->Y;

	for (i = 0; i < WALLCOUNT; i++)
	{
		// Provide variables with wall position data
		bottom2 = _wall[i]->position->Y + _wall[i]->sourceRect->Height;
		left2 = _wall[i]->position->X;
		right2 = _wall[i]->position->X + _wall[i]->sourceRect->Width;
		top2 = _wall[i]->position->Y;

		// Pacman wall collision
		if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2) && (left1 < right2))
		{
			if (_pacman->direction == 0) // pacman is moving left
			{
				_pacman->position->X -= 5;
			}
			else if (_pacman->direction == 2) // pacman is moving right
			{
				_pacman->position->X += 5;
			}
			else if (_pacman->direction == 3) // pacman is moving up
			{
				_pacman->position->Y += 5;
			}
			else if (_pacman->direction == 1) // pacman is moving down
			{
				_pacman->position->Y -= 5;
			}
			i = WALLCOUNT;
		}

		// Ghost wall collision
		if ((bottom3 > top2) && (top3 < bottom2) && (right3 > left2) && (left3 < right2))
		{
			if (ghost->direction == 0) // ghost is moving left
			{
				ghost->position->X -= 5;
				ghost->direction = 1;
				ghost->position->X += ghost->speed * elapsedTime;
			}
			else if (ghost->direction == 1) // ghost is moving right
			{
				ghost->position->X += 5;
				ghost->direction = 0;
				ghost->position->X -= ghost->speed * elapsedTime;
			}
			i = WALLCOUNT;
		}
	}
}

void Pacman::UpdateMunchie(Enemy* munchie, int elapsedTime)
{
	munchie->currentFrameTime += elapsedTime;
	if (munchie->currentFrameTime > munchie->frameTime)
	{
		munchie->frame++;
		
		if (munchie->frame >= 2)
			munchie->frame = 0;

		munchie->currentFrameTime = 0;
	}
	munchie->sourceRect->X = munchie->sourceRect->Height * munchie->frame; // Animate Munchies by changing sprite within the spritesheet
	
	// Attempt at efficiency
	/*munchie.currentFrameTime += elapsedTime;
	if (munchie.currentFrameTime > munchie.frameTime)
	{
		munchie.frame++;

		if (munchie.frame >= 2)
			munchie.frame = 0;

		munchie.currentFrameTime = 0;
	}
	munchie.sourceRect->X = munchie.sourceRect->Height * munchie.frame;*/ // Animate Munchies by changing sprite within the spritesheet
}

void Pacman::UpdateCherry(Enemy* cherry, int elapsedTime, Input::KeyboardState* state, Input::MouseState* mouseState)
{
	// Cherry Animation
	cherry->currentFrameTime += elapsedTime;
	if (cherry->currentFrameTime > cherry->frameTime)
	{
		cherry->frame++;

		if (cherry->frame >= 2)
			cherry->frame = 0;

		cherry->currentFrameTime = 0;
	}
	cherry->sourceRect->X = cherry->sourceRect->Height * cherry->frame; // Animate Cherry by changing sprite within the spritesheet

	// Cherry Movement
	mouseState->LeftButton;
	if (mouseState->LeftButton == Input::ButtonState::PRESSED) // Checks for Left Mouse Button input
	{
		_cherry->position->X = mouseState->X; // Move cherry X position to mouse X position
		_cherry->position->Y = mouseState->Y; // Move cherry Y position to mouse Y position
	}

	if (state->IsKeyDown(Input::Keys::R) && !_cherry->rKeyDown) // Checks if R key is pressed
	{
		_cherry->rKeyDown = true;
		_cherry->position = new Vector2(rand() % Graphics::GetViewportWidth(), rand() % Graphics::GetViewportHeight()); // Randomise the position of the cherry withing the game
	}
	if (state->IsKeyUp(Input::Keys::R))
	{
		_cherry->rKeyDown = false;
	}

	// Chery Collisions
	// Local variables
	int i = 0;
	int bottom1 = _pacman->position->Y + _pacman->sourceRect->Height;
	int bottom2 = 0;
	int left1 = _pacman->position->X;
	int left2 = 0;
	int right1 = _pacman->position->X + _pacman->sourceRect->Width;
	int right2 = 0;
	int top1 = _pacman->position->Y;
	int top2 = 0;

	// Provide variables with cherry position data
	bottom2 = cherry->position->Y + cherry->sourceRect->Height;
	left2 = cherry->position->X;
	right2 = cherry->position->X + cherry->sourceRect->Width;
	top2 = cherry->position->Y;

	// Pacman collides with cherry
	if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2) && (left1 < right2) && !cherry->eaten)
	{
		Audio::Play(_pop);
		cherry->eaten = true;
		cherry->position->X = -100.0f;
		cherry->position->Y = -100.0f;
		_pacman->score += 80;
	}
}

void Pacman::CheckMunchieCollisions()
{
	// Local variables
	int i = 0;
	int bottom1 = _pacman->position->Y + _pacman->sourceRect->Height;
	int bottom2 = 0;
	int left1 = _pacman->position->X;
	int left2 = 0;
	int right1 = _pacman->position->X + _pacman->sourceRect->Width;
	int right2 = 0;
	int top1 = _pacman->position->Y;
	int top2 = 0;

	for (i = 0; i < MUNCHIECOUNT; i++)
	{
		// Provide variables with munchie position data
		bottom2 = _munchies[i]->position->Y + _munchies[i]->sourceRect->Height;
		left2 = _munchies[i]->position->X;
		right2 = _munchies[i]->position->X + _munchies[i]->sourceRect->Width;
		top2 = _munchies[i]->position->Y;

		// Pacman collides with munchie
		if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2) && (left1 < right2) && !_munchies[i]->eaten)
		{
			Audio::Play(_pop);
			_munchies[i]->eaten = true;
			_munchies[i]->position->X = -100.0f;
			_munchies[i]->position->Y = -100.0f;
			_pacman->score += 10;

			if (_pacman->score == 3000)
			{
				Audio::Stop(_backgroundMusic);
				Audio::Play(_victorySound);
			}
		}
	}
}

void Pacman::UpdateGhost(MovingEnemy* ghost, int elapsedTime)
{

	if (ghost->direction == 0)
	{
		ghost->position->X += ghost->speed * elapsedTime; // Move the Ghost left
	}
	else if (ghost->direction == 1)
	{
		ghost->position->X -= ghost->speed * elapsedTime; // Move the Ghost right
	}

	// Change ghost direction when they hit the edge of the viewport
	// Don't need anymore since they don't every hit the viewport
	/*if (ghost->position->X + ghost->sourceRect->Width >= Graphics::GetViewportWidth()) // Hits right side of viewport
	{
		ghost->direction = 1; // Change the Ghosts direction
	}
	else if (ghost->position->X <= 0) // Hits left side of viewport
	{
		ghost->direction = 0; // Change the Ghosts direction
	}*/
	
	ghost->sourceRect->Y = ghost->sourceRect->Height * ghost->direction; // Change the direction the ghost is facing
}

void Pacman::TrackGhost(MovingEnemy* ghost, int elapsedTime, Input::MouseState* mouseState)
{
	// Rather than tracking - Move one ghost to position of mouse
	mouseState->RightButton;
	if (mouseState->RightButton == Input::ButtonState::PRESSED) // Checks for Right Mouse Button input
	{
		ghost->position->X = mouseState->X; // Move ghost X to position of mouse
		ghost->position->Y = mouseState->Y; // Move ghost Y to position of mouse
	}
}

/*void Pacman::PatrolGhost(MovingEnemy* ghost, int elapsedTime)
{
	// Attempt the have a ghost patrol around the map
	Vector2* topLeft = new Vector2(0,0);
	Vector2* topRight = new Vector2(Graphics::GetViewportWidth(), 0);
	Vector2* bottomLeft = new Vector2(Graphics::GetViewportWidth(),Graphics::GetViewportHeight());
	Vector2* bottomRight = new Vector2(0,Graphics::GetViewportHeight());

	ghost->position = topLeft;
	ghost->position = topRight;
	ghost->position = bottomLeft;
	ghost->position = bottomRight;
}*/

void Pacman::CheckGhostCollisions()
{
	// Local variables
	int i = 0;
	int bottom1 = _pacman->position->Y + _pacman->sourceRect->Height;
	int bottom2 = 0;
	int left1 = _pacman->position->X;
	int left2 = 0;
	int right1 = _pacman->position->X + _pacman->sourceRect->Width;
	int right2 = 0;
	int top1 = _pacman->position->Y;
	int top2 = 0;

	for (i = 0; i < GHOSTCOUNT; i++)
	{
		// Provide variables with ghost position data
		bottom2 = _ghosts[i]->position->Y + _ghosts[i]->sourceRect->Height;
		left2 = _ghosts[i]->position->X;
		right2 = _ghosts[i]->position->X + _ghosts[i]->sourceRect->Width;
		top2 = _ghosts[i]->position->Y;

		// Ghost collides with Pacman
		if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2) && (left1 < right2))
		{
			Audio::Stop(_backgroundMusic);
			Audio::Play(_deathSound);
			_pacman->dead = true;
			i = GHOSTCOUNT;
		}
	}
}

void Pacman::CloseGame(Input::KeyboardState* state)
{
	if (state->IsKeyDown(Input::Keys::ESCAPE))
	{
		std::exit(42); // Close game
	}
}