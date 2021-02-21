#include "States.h"
#include "StateManager.h"
#include "Engine.h"
#include<iostream>
#include "Engine.cpp"

using namespace std;

void State::Render()
{
	SDL_RenderPresent(Engine::Instance().GetRenderer());
}

void State::Resume(){}

// Title Class Begins
TitleState::TitleState(){}

void TitleState::Enter()
{
	cout << "Entering TitleState..." << endl;
}

void TitleState::Update()
{
	if (Engine::Instance().KeyDown(SDL_SCANCODE_N))
		STMA::ChangeState(new GameState());
	// Parse N Key and Change to teh new GameState
}

void TitleState::Render()
{
	cout << "Rendering TitleState..." << endl;
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 0,200,100,255);
	SDL_RenderClear(Engine::Instance().GetRenderer());
	State::Render();
}

void TitleState::Exit()
{
	cout << "Exiting TitleState..." << endl;
}
//TitleState End

// GameState Begins
GameState::GameState(){}
void GameState::Enter()
{	
	cout << "Entering GameState..." << endl;
}
void GameState::Update()
{
	if (Engine::Instance().KeyDown(SDL_SCANCODE_X))
		STMA::ChangeState(new TitleState());
	// Parse X Key and Change to teh new TitleState

		//Scroll the backgrounds. Check if they need to snap back.
	for (int i = 0; i < 2; i++)
		Engine::Instance().bgArray[i].GetDstP()->x -= BGSCROLL;
	if (Engine::Instance().bgArray[1].GetDstP()->x <= 0)
	{
		Engine::Instance().bgArray[0].GetDstP()->x = 0;
		Engine::Instance().bgArray[1].GetDstP()->x = 1024;
	}
	// Player animation/movement.
	Engine::Instance().m_player->Animate(); // Oh! We're telling the player to animate itself. This is good! Hint hint.
	if (Engine::Instance().KeyDown(SDL_SCANCODE_A) && Engine::Instance().m_player->GetDstP()->x > Engine::Instance().m_player->GetDstP()->h)
		Engine::Instance().m_player->GetDstP()->x -= PSPEED;
	else if (Engine::Instance().KeyDown(SDL_SCANCODE_D) && Engine::Instance().m_player->GetDstP()->x < WIDTH / 2)
		Engine::Instance().m_player->GetDstP()->x += PSPEED;
	if (Engine::Instance().KeyDown(SDL_SCANCODE_W) && Engine::Instance().m_player->GetDstP()->y > 0)
		Engine::Instance().m_player->GetDstP()->y -= PSPEED;
	else if (Engine::Instance().KeyDown(SDL_SCANCODE_S) && Engine::Instance().m_player->GetDstP()->y < HEIGHT - Engine::Instance().m_player->GetDstP()->w)
		Engine::Instance().m_player->GetDstP()->y += PSPEED;

	if (Engine::Instance().KeyDown(SDL_SCANCODE_SPACE) && Engine::Instance().m_bCanShoot)
	{
		Engine::Instance().m_bCanShoot = false;
		Engine::Instance().m_vPBullets.push_back(new Bullet({ 376,0,10,100 }, { Engine::Instance().m_player->GetDstP()->x + 85,Engine::Instance().m_player->GetDstP()->y + 42,10,100 }, 30));
		Mix_PlayChannel(-1, Engine::Instance().m_vSounds[1], 0);
	}
	// Enemy animation/movement.
	for (int i = 0; i < (int)Engine::Instance().m_vEnemies.size(); i++)
	{
		Engine::Instance().m_vEnemies[i]->Update(); // Oh, again! We're telling the enemies to update themselves. Good good!
		if (Engine::Instance().m_vEnemies[i]->GetDstP()->x < -56)
		{
			delete Engine::Instance().m_vEnemies[i];
			Engine::Instance().m_vEnemies[i] = nullptr;
			Engine::Instance().m_bENull = true;
		}
	}
	if (Engine::Instance().m_bENull) CleanVector<Enemy*>(Engine::Instance().m_vEnemies, Engine::Instance().m_bENull); // Better to have a logic check (if) than a function call all the time!
	// Update enemy spawns.
	if (Engine::Instance().m_iESpawn++ == Engine::Instance().m_iESpawnMax)
	{
		Engine::Instance().m_vEnemies.push_back(new Enemy({ 0,100,40,56 }, { WIDTH,56 + rand() % (HEIGHT - 114),40,56 }, &Engine::Instance().m_vEBullets, Engine::Instance().m_vSounds[0],
			30 + rand() % 91)); // Randomizing enemy bullet spawn to every 30-120 frames.
		Engine::Instance().m_iESpawn = 0;
	}
	// Update the bullets. Player's first.
	for (int i = 0; i < (int)Engine::Instance().m_vPBullets.size(); i++)
	{
		Engine::Instance().m_vPBullets[i]->Update();
		if (Engine::Instance().m_vPBullets[i]->GetDstP()->x > WIDTH)
		{
			delete Engine::Instance().m_vPBullets[i];
			Engine::Instance().m_vPBullets[i] = nullptr;
			Engine::Instance().m_bPBNull = true;
		}
	}
	if (Engine::Instance().m_bPBNull) CleanVector<Bullet*>(Engine::Instance().m_vPBullets, Engine::Instance().m_bPBNull);
	// Now enemy bullets. Is Update() getting a little long?
	for (int i = 0; i < (int)Engine::Instance().m_vEBullets.size(); i++)
	{
		Engine::Instance().m_vEBullets[i]->Update();
		if (Engine::Instance().m_vEBullets[i]->GetDstP()->x < -10)
		{
			delete Engine::Instance().m_vEBullets[i];
			Engine::Instance().m_vEBullets[i] = nullptr;
			Engine::Instance().m_bEBNull = true;
		}
	}
	if (Engine::Instance().m_bEBNull) CleanVector<Bullet*>(Engine::Instance().m_vEBullets, Engine::Instance().m_bEBNull);
	Engine::Instance().CheckCollision();
}
void GameState::Render()
{
	cout << "Rendering GameState..." << endl;
	//SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 50, 100, 200, 255);
	//SDL_RenderClear(Engine::Instance().GetRenderer());
	//State::Render();

		SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 0, 0, 0, 255);
	SDL_RenderClear(Engine::Instance().GetRenderer()); // Clear the screen with the draw color.
	// Render stuff. Background first.
	for (int i = 0; i < 2; i++)
		SDL_RenderCopy(Engine::Instance().GetRenderer(), Engine::Instance().m_pBGText, Engine::Instance().bgArray[i].GetSrcP(), Engine::Instance().bgArray[i].GetDstP());
	// Player.
	SDL_RenderCopyEx(Engine::Instance().GetRenderer(), Engine::Instance().m_pSprText, Engine::Instance().m_player->GetSrcP(), Engine::Instance().m_player->GetDstP(), Engine::Instance().m_player->GetAngle(), &Engine::Instance().m_pivot, SDL_FLIP_NONE);
	/*SDL_SetRenderDrawBlendMode(m_pRenderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 255, 128);
	SDL_RenderFillRect(m_pRenderer, m_player->GetDstP());*/
	// Player bullets.	
	for (int i = 0; i < (int)Engine::Instance().m_vPBullets.size(); i++)
	{
		SDL_RenderCopyEx(Engine::Instance().GetRenderer(), Engine::Instance().m_pSprText, Engine::Instance().m_vPBullets[i]->GetSrcP(), Engine::Instance().m_vPBullets[i]->GetDstP(), 90, &Engine::Instance().m_pivot, SDL_FLIP_NONE);
		/*SDL_SetRenderDrawColor(m_pRenderer, 255, 255, 0, 128);
		SDL_RenderFillRect(m_pRenderer, m_vPBullets[i]->GetDstP());*/
	}
	// Enemies.
	for (int i = 0; i < (int)Engine::Instance().m_vEnemies.size(); i++)
	{
		SDL_RenderCopyEx(Engine::Instance().GetRenderer(), Engine::Instance().m_pSprText, Engine::Instance().m_vEnemies[i]->GetSrcP(), Engine::Instance().m_vEnemies[i]->GetDstP(), -90, &Engine::Instance().m_pivot, SDL_FLIP_NONE);
		/*SDL_SetRenderDrawColor(m_pRenderer, 255, 0, 0, 128);
		SDL_RenderFillRect(m_pRenderer, m_vEnemies[i]->GetDstP());*/
	}
	// Enemy bullets.
	for (int i = 0; i < (int)Engine::Instance().m_vEBullets.size(); i++)
		SDL_RenderCopy(Engine::Instance().GetRenderer(), Engine::Instance().m_pSprText, Engine::Instance().m_vEBullets[i]->GetSrcP(), Engine::Instance().m_vEBullets[i]->GetDstP());
	SDL_RenderPresent(Engine::Instance().GetRenderer());
}
void GameState::Exit()
{
	cout << "Exiting GameState..." << endl;
}
void GameState::Resume()
{
	cout << "Resuming GameState..." << endl;
}
//GameState End

Engine::Engine() :m_iESpawn(0), m_iESpawnMax(60), m_pivot({ 0,0 })
{	// Again showing a mix of either initializers or in-body initialization. Initializers happen first.
	m_bRunning = m_bEBNull = m_bENull = m_bPBNull = false; // Setting all to false.
	m_bCanShoot = true;
	cout << "Engine class constructed!" << endl;
}
Engine::~Engine() {}

bool Engine::Init(const char* title, int xpos, int ypos, int width, int height, int flags)
{
	cout << "Initializing game." << endl;
	srand((unsigned)time(NULL));
	// Attempt to initialize SDL.
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		// Create the window.
		m_pWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (m_pWindow != nullptr) // Window init success.
		{
			m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);
			if (m_pRenderer != nullptr) // Renderer init success.
			{
				m_pBGText = IMG_LoadTexture(m_pRenderer, "Img/background.png");
				m_pSprText = IMG_LoadTexture(m_pRenderer, "Img/sprites.png");
				if (Mix_Init(MIX_INIT_MP3) != 0) // Mixer init success.
				{
					Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 2048); // Good for most games.
					Mix_AllocateChannels(16);
					m_pMusic = Mix_LoadMUS("Aud/game.mp3"); // Load the music track.
					// Load the chunks into the Mix_Chunk vector.
					m_vSounds.reserve(3); // Optional but good practice.
					m_vSounds.push_back(Mix_LoadWAV("Aud/enemy.wav"));
					m_vSounds.push_back(Mix_LoadWAV("Aud/laser.wav"));
					m_vSounds.push_back(Mix_LoadWAV("Aud/explode.wav"));
					/* By the way, you should check to see if any of these loads are failing and
					   you can use Mix_GetError() to print out the error message. Wavs can be finicky.*/
				}
				else return false;
			}
			else return false; // Renderer init fail.
		}
		else return false; // Window init fail.
	}
	else return false; // SDL init fail.
	m_fps = (Uint32)round((1 / (double)FPS) * 1000); // Sets FPS in milliseconds and rounds.
	m_iKeystates = SDL_GetKeyboardState(nullptr);
	STMA::PushState(new TitleState());
	bgArray[0] = { {0,0,1024,768}, {0, 0, 1024, 768} };
	bgArray[1] = { {0,0,1024,768}, {1024, 0, 1024, 768} };
	m_player = new Player({ 0,0,94,100 }, { 256,384 - 50,94,100 });
	Mix_PlayMusic(m_pMusic, -1); // Play. -1 = looping.
	Mix_VolumeMusic(16); // 0-MIX_MAX_VOLUME (128).
	m_bRunning = true; // Everything is okay, start the engine.
	cout << "Success!" << endl;
	return true;
}

void Engine::Wake()
{
	m_start = SDL_GetTicks();
}

void Engine::Sleep()
{
	m_end = SDL_GetTicks();
	m_delta = m_end - m_start;
	if (m_delta < m_fps) // Engine has to sleep.
		SDL_Delay(m_fps - m_delta);
}

void Engine::HandleEvents()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT: // User pressed window's 'x' button.
			m_bRunning = false;
			break;
		case SDL_KEYDOWN: // Try SDL_KEYUP instead.
			if (event.key.keysym.sym == SDLK_ESCAPE)
				m_bRunning = false;
			break;
		case SDL_KEYUP:
			if (event.key.keysym.sym == SDLK_SPACE)
				m_bCanShoot = true;
			break;
		}

	}
}

// Keyboard utility function.
bool Engine::KeyDown(SDL_Scancode c)
{
	if (m_iKeystates != nullptr)
	{
		if (m_iKeystates[c] == 1)
			return true;
		else
			return false;
	}
	return false;
}

void Engine::CheckCollision()
{
	// Player vs. Enemy.
	SDL_Rect p = { m_player->GetDstP()->x - 100, m_player->GetDstP()->y, 100, 94 };
	for (int i = 0; i < (int)m_vEnemies.size(); i++)
	{
		SDL_Rect e = { m_vEnemies[i]->GetDstP()->x, m_vEnemies[i]->GetDstP()->y - 40, 56, 40 };
		if (SDL_HasIntersection(&p, &e))
		{
			// Game over!
			cout << "Player goes boom!" << endl;
			Mix_PlayChannel(-1, m_vSounds[2], 0);
			break;
		}
	}
	// Player bullets vs. Enemies.
	for (int i = 0; i < (int)m_vPBullets.size(); i++)
	{
		SDL_Rect b = { m_vPBullets[i]->GetDstP()->x - 100, m_vPBullets[i]->GetDstP()->y, 100, 10 };
		for (int j = 0; j < (int)m_vEnemies.size(); j++)
		{
			if (m_vEnemies[j] == nullptr) continue;
			SDL_Rect e = { m_vEnemies[j]->GetDstP()->x, m_vEnemies[j]->GetDstP()->y - 40, 56, 40 };
			if (SDL_HasIntersection(&b, &e))
			{
				Mix_PlayChannel(-1, m_vSounds[2], 0);
				delete m_vEnemies[j];
				m_vEnemies[j] = nullptr;
				delete m_vPBullets[i];
				m_vPBullets[i] = nullptr;
				m_bENull = true;
				m_bPBNull = true;
				break;
			}
		}
	}
	if (m_bENull) CleanVector<Enemy*>(m_vEnemies, m_bENull);
	if (m_bPBNull) CleanVector<Bullet*>(m_vPBullets, m_bPBNull);
	// Enemy bullets vs. player.
	for (int i = 0; i < (int)m_vEBullets.size(); i++)
	{
		if (SDL_HasIntersection(&p, m_vEBullets[i]->GetDstP()))
		{
			// Game over!
			cout << "Player goes boom!" << endl;
			Mix_PlayChannel(-1, m_vSounds[2], 0);
			delete m_vEBullets[i];
			m_vEBullets[i] = nullptr;
			CleanVector<Bullet*>(m_vEBullets, m_bEBNull);
			break;
		}
	}
}

void Engine::Clean()
{
	cout << "Cleaning game." << endl;
	STMA::Quit();
	delete m_player;
	m_player = nullptr;
	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);
	for (int i = 0; i < (int)m_vSounds.size(); i++)
		Mix_FreeChunk(m_vSounds[i]);
	m_vSounds.clear();
	Mix_FreeMusic(m_pMusic);
	Mix_CloseAudio();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

int Engine::Run()
{
	if (m_bRunning == true)
		return 1;
	//start and run engine
	if (Init("GAME1017_A1_HendersonPhilip", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0) == false)
		return 2;
	while (m_bRunning)
	{
		Wake();
		HandleEvents();
		Update();
		Render();
		if (m_bRunning == true)
			Sleep();
	}
	Clean();
	return 0;
}

void Engine::Update()
{
	//StateManager
	STMA::Update();

}

/* In the render I commented out some lines that rendered the original destination rectangles
   for the sprites so I can show you the after-effect of rotating via SDL_RenderCopyEX(). In
   order to do collision, I manually created new rectangles for collision in the CheckCollision method. */
void Engine::Render()
{
	STMA::Render();

}

Engine& Engine::Instance()
{
	static Engine instance; // Magic statics. Creating the object.
	return instance;
}

PauseState::PauseState(){}

void PauseState::Enter()
{
	cout << "Entering PauseState..." << endl;
}

void PauseState::Update()
{
	if (Engine::Instance().KeyDown(SDL_SCANCODE_R))
		STMA::PopState();

}

void PauseState::Render()
{
	cout << "Rendering PauseState..." << endl;
}

void PauseState::Exit()
{
	cout << "Exiting PauseState..." << endl;
}
