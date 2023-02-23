#include "State.h"
#include "StateManager.h"
#include <iostream>

void State::Render()
{
	SDL_RenderPresent(Engine::Instance().GetRenderer());
}

TitleState::TitleState(){}


void TitleState::Enter()
{
	std::cout << "Entering TitleState!" << std::endl;

	Mix_VolumeMusic(36);

	Mix_PlayMusic(Engine::backgroundMusic["titleMusic"], -1);
}

void TitleState::Update()
{
	if (Engine::Instance().KeyDown(SDL_SCANCODE_N))
	{
		std::cout << "Changing to GameState!" << std::endl;
		STMA::ChangeState(new GameState()); // Transitioning to new GameState
	}
}

void TitleState::Render()
{
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 0, 0, 255, 255);
	SDL_RenderClear(Engine::Instance().GetRenderer());

	// Any unique rendering in TitleState goes here ...
}

void TitleState::Exit()
{
	std::cout << "exiting TitleState" << std::endl;
	// Make sure to invoke mix_freemusic
}


GameState::GameState():m_ship(nullptr), speed(200),
m_enemyCtr(0), m_enemyMax(600) {} // m_enemyMax: 2 seconds if our framerate is 60

void GameState::Enter() // Initializing everything
{
	SDL_Renderer* engine_renderer = Engine::Instance().GetRenderer(); // Making it easier instead of multiple callbacks to Engine
	std::cout << "Entering GameState!" << std::endl;
	plrCanShoot = true;

	// Putting the textures into the map
	textures.emplace("player", IMG_LoadTexture(engine_renderer, "../Assets/img/seal.png"));
	textures.emplace("background", IMG_LoadTexture(engine_renderer, "../Assets/img/background.png"));
	textures.emplace("playerBullet", IMG_LoadTexture(engine_renderer, "../Assets/img/ice1.png"));
	textures.emplace("enemy", IMG_LoadTexture(engine_renderer, "../Assets/img/enemy.png"));

	// Error checking the textures
	for (std::pair<string, SDL_Texture*> texture : textures)
	{
		if (texture.second == nullptr)
		{
			std::cout << SDL_GetError() << std::endl;
		}
	}

	// Putting the sounds into the map
	m_sfx.emplace("playerShoot", Mix_LoadWAV("../Assets/mus/plrShoot.wav"));
	m_sfx.emplace("enemyShoot", Mix_LoadWAV("../Assets/mus/enemyShoot.wav"));
	m_sfx.emplace("collision", Mix_LoadWAV("../Assets/mus/collisionSound.wav"));

	// Error checking sounds...... (maybe no errors?)
	for (std::pair<string, Mix_Chunk*> soundFX : m_sfx) 
	{
		if (soundFX.second == nullptr)
		{
			cout << Mix_GetError() << endl;
		}
	}

	m_ship = new Player({ WIDTH / 2.0f, HEIGHT / 2.0f }); // Invokes constructors again 
	m_bg1 = { 0.0f, 0.0f, WIDTH, HEIGHT }; // BG on screen/in-window
	m_bg2 = { WIDTH, 0.0f, WIDTH, HEIGHT }; // One full BG above. Just above window.

	m_scrollSpeed = 200.0f;

	Mix_VolumeMusic(16);

	Mix_PlayMusic(Engine::backgroundMusic["playMusic"], -1);
}

void GameState::Update()
{
	// Making the background scroll!
	for (int i = 0; i < 2; i++) {
		m_bg1.x -= m_scrollSpeed * Engine::Instance().GetDeltaTime();
		m_bg2.x -= m_scrollSpeed * Engine::Instance().GetDeltaTime();
		// Check if bg's need to reset
		if (m_bg1.x <= 0.0f) {
			m_bg1.x = WIDTH;
			m_bg2.x = 0.0f;
		}
	}

	// Moving the player
	switch (m_ship->GetState())
	{
	case PLR_IDLE:
		// Transition to move animation
		if (Engine::Instance().KeyDown(SDL_SCANCODE_W) || Engine::Instance().KeyDown(SDL_SCANCODE_S) || 
			Engine::Instance().KeyDown(SDL_SCANCODE_A) || Engine::Instance().KeyDown(SDL_SCANCODE_D)) {
			m_ship->SetAnimation(PLR_MOVE, 0, 1);
		}
		break;
	case PLR_MOVE:

		// Transition to idle animation
		if (!Engine::Instance().KeyDown(SDL_SCANCODE_W) && !Engine::Instance().KeyDown(SDL_SCANCODE_S) && 
			!Engine::Instance().KeyDown(SDL_SCANCODE_A) && !Engine::Instance().KeyDown(SDL_SCANCODE_D)) {
			m_ship->SetAnimation(PLR_IDLE, 0, 1);
		}
		if (Engine::Instance().KeyDown(SDL_SCANCODE_W) && m_ship->GetDst()->y > 0) { // Moving up with collisions
			m_ship->GetDst()->y -= 2.0f;

		}
		else if (Engine::Instance().KeyDown(SDL_SCANCODE_S) && m_ship->GetDst()->y < (HEIGHT - m_ship->GetDst()->h)) { // Moving down with collisions
			m_ship->GetDst()->y += 2.0f;
		}

		if (Engine::Instance().KeyDown(SDL_SCANCODE_A) && m_ship->GetDst()->x > 0) { // Moving backwards with collisions
			m_ship->GetDst()->x -= 2.0f;
		}
		else if (Engine::Instance().KeyDown(SDL_SCANCODE_D)) { // Moving forward with collisions
			if (m_ship->GetDst()->x < (WIDTH - m_ship->GetDst()->w) / 4.0f) {
				m_ship->GetDst()->x += 2.0f;
			}
		}

		break;
	}

	// Updating bullets
	for (int i = 0; i < m_bulletVec.size(); i++) {
		if ((m_bulletVec[i])->GetXpos() > WIDTH) {
			delete (m_bulletVec[i]);
			(m_bulletVec[i]) = nullptr;
			m_bulletVec.erase(m_bulletVec.begin() + i);
			m_bulletVec.shrink_to_fit();
		}
		else {
			(m_bulletVec[i])->Update(Engine::Instance().GetDeltaTime());
		}
	}

	// For each 2 seconds, spawn a new enemy
	if (m_enemyCtr++ % m_enemyMax == 0) { 
		m_enemyVec.push_back(new Enemy(WIDTH, (float)(rand() % HEIGHT), 200)); // Random!
		cout << "New Enemy Spawned!\n";
	}

	m_ship->Update();

	// Enemy destroyer if too far of screen
	for (int i = 0; i < m_enemyVec.size(); i++) {
		if (m_enemyVec[i]->GetDst()->x < 0) {
			delete m_enemyVec[i];
			m_enemyVec[i] = nullptr;
			m_enemyVec.erase(m_enemyVec.begin() + i);
			m_enemyVec.shrink_to_fit();
		}
		else {
			m_enemyVec[i]->Update(Engine::Instance().GetDeltaTime(), m_sfx["enemyShoot"]);
		}
	}

	// Enemy bullet Handler
	for (int i = 0; i < Engine::m_enemyBulletVec.size(); i++) {
		// If bullets are off screen
		if ((Engine::m_enemyBulletVec[i])->GetXpos() < 0) {
			delete (Engine::m_enemyBulletVec[i]);
			(Engine::m_enemyBulletVec[i]) = nullptr;
			Engine::m_enemyBulletVec.erase(Engine::m_enemyBulletVec.begin() + i);
			Engine::m_enemyBulletVec.shrink_to_fit();
		}
		// If collision with player
		else if (SDL_HasIntersectionF(Engine::m_enemyBulletVec[i]->GetRect(), m_ship->GetDst())) {
			Mix_PlayChannel(-1, m_sfx["collision"], 0);
			delete Engine::m_enemyBulletVec[i];
			Engine::m_enemyBulletVec[i] = nullptr;

			Engine::m_enemyBulletVec.erase(Engine::m_enemyBulletVec.begin() + i);
			Engine::m_enemyBulletVec.shrink_to_fit();

			plrCanShoot = false;
			delete m_ship;
		}
		else {
			(Engine::m_enemyBulletVec[i])->Update(Engine::Instance().GetDeltaTime());
		}
	}
	// Enemy collision handler
	for (int i = 0; i < m_enemyVec.size(); i++) {
		if (SDL_HasIntersectionF(m_enemyVec[i]->GetDst(), m_ship->GetDst())) {
			Mix_PlayChannel(-1, m_sfx["collision"], 0);
			delete m_enemyVec[i];
			m_enemyVec[i] = nullptr;
			m_enemyVec.erase(m_enemyVec.begin() + i);
			m_enemyVec.shrink_to_fit();
			plrCanShoot = false;
			delete m_ship;
		}
	}

	// Player bullet handler
	for (std::vector<Bullet*>::size_type i = 0; i < m_bulletVec.size(); i++) {
		for (std::vector<Bullet*>::size_type j = 0; j < m_enemyVec.size(); j++) {
			if (SDL_HasIntersectionF(m_bulletVec[i]->GetRect(), m_enemyVec[j]->GetDst())) {
				Mix_PlayChannel(-1, m_sfx["collision"], 0);
				delete m_bulletVec[i];
				m_bulletVec[i] = nullptr;
				delete m_enemyVec[j];
				m_enemyVec[j] = nullptr;

				m_enemyVec.erase(m_enemyVec.begin() + j);
				m_bulletVec.erase(m_bulletVec.begin() + i);
				m_enemyVec.shrink_to_fit();
				m_bulletVec.shrink_to_fit();
			}
		}
	}

	// Probably can put this in HandleEvents, will do that later
	if (Engine::Instance().KeyDown(SDL_SCANCODE_P))
	{
		std::cout << "Changing to PauseState" << std::endl;
		Mix_PauseMusic(); // Pauses all sounds, head empty.
		STMA::PushState(new PauseState()); // Pause time!
	}
	// Handles if Spacebar is pressed to shoot
	if (Engine::Instance().KeyDown(SDL_SCANCODE_SPACE))
	{
		if (plrCanShoot) {
			if (!hasPlrShot)
			{
				Mix_PlayChannel(-1, m_sfx["playerShoot"], 0);
				m_bulletVec.push_back(new Bullet(
					{ m_ship->GetDst()->x + m_ship->GetDst()->w, m_ship->GetDst()->y + m_ship->GetDst()->h / 2 },
					400, true));
				hasPlrShot = true;
			}
		}
	} else
	{
		hasPlrShot = false;
	}
	// When X is pressed go to EndState
	if (Engine::Instance().KeyDown(SDL_SCANCODE_X))
	{
		std::cout << "Changing to EndState" << std::endl;
		STMA::ChangeState(new EndState());
	}
}

void GameState::Render()
{
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 0, 0, 255, 255);
	SDL_RenderClear(Engine::Instance().GetRenderer());

	SDL_Renderer* engine_renderer = Engine::Instance().GetRenderer(); // Making it easier instead of multiple callbacks to Engine
	// Any unique rendering in TitleState goes here ...
	SDL_SetRenderDrawColor(engine_renderer, 0, 0, 0, 255);
	SDL_RenderClear(engine_renderer);

	// Rendering the background
	SDL_RenderCopyF(engine_renderer, textures["background"], NULL, &m_bg1);
	SDL_RenderCopyF(engine_renderer, textures["background"], NULL, &m_bg2); // All the pixels!
	// Rendering the player bullets
	for (auto bullet : m_bulletVec) {
		SDL_RenderCopyF(engine_renderer, textures["playerBullet"], NULL, bullet->GetRect());
	}
	// Rendering the enemy bullets
	for (auto bullet : Engine::m_enemyBulletVec) {
		SDL_SetRenderDrawColor(engine_renderer, bullet->m_col.r, bullet->m_col.g, bullet->m_col.b, bullet->m_col.a);
		SDL_RenderFillRectF(engine_renderer, bullet->GetRect());
	}

	// Rendering the enemies
	for (auto enemy : m_enemyVec) {
		SDL_RenderCopyExF(engine_renderer, textures["enemy"], enemy->GetSrc(), enemy->GetDst()
			, 0, NULL, SDL_FLIP_NONE);
	}

	// Rendering the player ship
	SDL_RenderCopyExF(engine_renderer, textures["player"], m_ship->GetSrc(), m_ship->GetDst(),
		0, NULL, SDL_FLIP_NONE);

}

void GameState::Exit()
{
	std::cout << "Exiting GameState!" << std::endl;

	for (auto bullet : m_bulletVec) {
		delete bullet; // Deallocate Bullet in heap.
		bullet = nullptr; // Optional because we're quitting
	}
	m_bulletVec.clear(); // Remove all pointers from vector. size = 0
	m_bulletVec.shrink_to_fit(); // Shrink capacity to size.

	// Deallocation of enemy vector
	for (auto enemy : m_enemyVec) {
		delete enemy; 
		enemy = nullptr;
	}
	m_enemyVec.clear();
	m_enemyVec.shrink_to_fit();

	// Deallocation of enemy bullet vector
	for (auto bullet : Engine::m_enemyBulletVec) {
		delete bullet;
		bullet = nullptr;
	}

	Engine::m_enemyBulletVec.clear();
	Engine::m_enemyBulletVec.shrink_to_fit();

	// Deallocating all texture pointers :D
	for (pair<string, SDL_Texture*> texture : textures)
	{
		SDL_DestroyTexture(texture.second);
	}
	textures.clear(); // Finally, clearing out the map.

	// Deallocating all sound pointers :)
	for (pair<string, Mix_Chunk*> soundFX : m_sfx) 
	{
		Mix_FreeChunk(soundFX.second);
	}
	m_sfx.clear(); // Finally, clearing out the map
	Mix_HaltMusic();

}

void GameState::Resume()
{
	std::cout << "resuming gameState" << std::endl;
	Mix_ResumeMusic();
}

PauseState::PauseState() {}

void PauseState::Enter()
{
	std::cout << "Entering PauseState" << std::endl;
}

void PauseState::Update()
{
	if (Engine::Instance().KeyDown(SDL_SCANCODE_R))
	{
		STMA::PopState();
	}
}

void PauseState::Render()
{
	STMA::GetStates().front()->Render(); // Basically, renders the GameState as well that has not been booted out yet of the vector
	SDL_SetRenderDrawBlendMode(Engine::Instance().GetRenderer(), SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 255, 0, 0, 128); // Making window transparent
	SDL_Rect pause_rect = { (WIDTH / 2), (HEIGHT / 2), 512, 512 };
	SDL_RenderFillRect(Engine::Instance().GetRenderer(), &pause_rect);
}

void PauseState::Exit()
{
}

EndState::EndState() {}

void EndState::Enter()
{
}

void EndState::Update()
{
	if (Engine::Instance().KeyDown(SDL_SCANCODE_R))
	{
		STMA::ChangeState(new TitleState);
	}
}

void EndState::Render()
{
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 0, 255, 0, 255);
	SDL_RenderClear(Engine::Instance().GetRenderer());
}

void EndState::Exit()
{
	std::cout << "Now exiting ExitState" << std::endl;
}


