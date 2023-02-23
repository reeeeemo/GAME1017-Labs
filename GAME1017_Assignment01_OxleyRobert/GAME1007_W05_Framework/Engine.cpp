#include "Engine.h"
#include <iostream>
#include <string>
#include <sstream>
#include "StateManager.h"

using namespace std;

Engine::Engine():m_pWindow(nullptr), m_pRenderer(nullptr), m_isRunning(false)
{
}

int Engine::Run()
{
	if (m_isRunning)
	{
		return 1; // 1 arbitrarily means that engine is already running.
	}
	if (Init("GAME1017_L1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, NULL))
	{
		return 2; // 2 arbitrarily means that something went wrong in init.
	}
	while (m_isRunning) // Main game loop. Run while isRunning = true.
	{
		Wake();
		HandleEvents(); // Input
		Update();       // Processing
		Render();       // Output
		if (m_isRunning == true)
			Sleep();
	}
	Clean(); // Deinitialize SDL and free up memory.
	return 0;
}

int Engine::Init(const char* title, const int xPos, const int yPos, 
	const int width, const int height, const int flags)
{
	std::cout << "Initializing framework..." << std::endl;
	srand((unsigned)time(nullptr)); // Seed random sequence. Only once.
	SDL_Init(SDL_INIT_EVERYTHING);
	m_pWindow = SDL_CreateWindow(title,	xPos, yPos, width, height, flags);
	if (m_pWindow == nullptr) // Or NULL is okay too
	{
		std::cout << "Error during window creation!" << std::endl;
		return 1;
	}
	m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);
	if (m_pRenderer == nullptr) // Or NULL is okay too
	{
		std::cout << "Error during renderer creation!" << std::endl;
		return 1;
	}
	if (IMG_Init( IMG_INIT_PNG | IMG_INIT_JPG ) == 0)
	{
		std::cout << SDL_GetError() << std::endl; // Prints last SDL error msg.
		return 1;
	}
	if (Mix_Init(MIX_INIT_MP3) != 0) {
		// Configure mixer
		Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 2048);
		Mix_AllocateChannels(16);
		// Load background music
		backgroundMusic.emplace("titleMusic", Mix_LoadMUS("../Assets/mus/titleMusic.mp3"));
		backgroundMusic.emplace("playMusic", Mix_LoadMUS("../Assets/mus/bgMusic.mp3"));

		// Ensuring the background music has no errors
		for (pair<string, Mix_Music*> bgMusic : backgroundMusic)
		{
			if (bgMusic.second == nullptr)
			{
				std::cout << SDL_GetError << std::endl;
			}
		}
	}
	else return false;


	std::cout << SDL_GetError() << std::endl;


	m_fps = 1.0 / (double)FPS; // Converts FPS into a fraction of seconds.
	m_pKeystates = SDL_GetKeyboardState(nullptr);
	lastFrameTime = chrono::high_resolution_clock::now();
	m_isRunning = true; // Start your engine.

	STMA::ChangeState(new TitleState());
	return 0;
}

void Engine::HandleEvents()
{
	//cout << "Handling events..." << endl;
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT: // Pressing 'X' icon in SDL window.
			m_isRunning = false; // Tell Run() we're done.
			break;
		case SDL_KEYDOWN:
			// Keeping this here in case needed to revert
			// if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
			//
			break;
		}
	}
}

void Engine::Wake()
{
	thisFrameTime = chrono::high_resolution_clock::now(); // New snapshot of number of seconds.
	lastFrameDuration = thisFrameTime - lastFrameTime;
	deltaTime = lastFrameDuration.count(); // Now we have our deltaTime multiplier.
	lastFrameTime = thisFrameTime;

	m_start = thisFrameTime; // Comment this out to just use deltaTime.
}

bool Engine::KeyDown(SDL_Scancode c)
{
	if (m_pKeystates != nullptr)
	{
		if (m_pKeystates[c] == 1)
			return true;
	}
	return false;
}

SDL_Renderer* Engine::GetRenderer()
{
	return m_pRenderer;
}

Engine& Engine::Instance()
{
	static Engine instance; // Creating Engine Object
	return instance;
}



void Engine::Update()
{
	//cout << "Updating frame..." << endl;
	string tickLabel = "DT: " + to_string(deltaTime);
	SDL_SetWindowTitle(m_pWindow, tickLabel.c_str()); // c_str just returns the char array (char *)
	STMA::Update(); // Updating current state
}

void Engine::Sleep() 
{
	// Note: Not really better, but you can decide to not limit frameRate and just use deltaTime.
	// Comment all this out to just use deltaTime.
	m_end = chrono::high_resolution_clock::now();
	m_diff = m_end - m_start; // Similar to before, but now chrono and double.
	if (m_diff.count() < m_fps)
		SDL_Delay((Uint32)((m_fps - m_diff.count()) * 1000.0)); // Sleep for number of ms.
}

void Engine::Render()
{
	//cout << "Rendering changes..." << endl;
	STMA::Render();
	
	SDL_RenderPresent(m_pRenderer); // Flips the buffers.
}

double Engine::GetDeltaTime()
{
	return deltaTime;
}


void Engine::Clean()
{
	std::cout << "Cleaning up..." << std::endl;

	// Deletion of renderer and window
	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);

	// Deallocating all background music >:)
	for (pair<string, Mix_Music*> music : backgroundMusic)
	{
		Mix_FreeMusic(music.second);
	}

	backgroundMusic.clear(); // Finally, clearing out the math
	Mix_CloseAudio();
	Mix_Quit();
	STMA::Quit();
	IMG_Quit();
	SDL_Quit();
}

vector<Bullet*> Engine::m_enemyBulletVec;
map<std::string, Mix_Music*> Engine::backgroundMusic; // Stores all the music!
