#include <iostream>
#include "States.h"
#include "StateManager.h"
// GameObject includes.
#include "Button3.h"
#include "Primitives.h"
#include "PlatformPlayer.h"
#include "TiledLevel.h"
// Remaining managers.
#include "CollisionManager.h"
#include "EventManager.h"
#include "RenderManager.h"
#include "SoundManager.h"
#include "TextureManager.h"

#include <iostream>
using namespace std;

void State::Update()
{
	for (auto const& i : m_objects)
	{
		i.second->Update();
		if (STMA::StateChanging()) return;
	}
}

void State::Render()
{
	for (auto const& i : m_objects)
		i.second->Render();
	if (dynamic_cast<GameState*>(this) && dynamic_cast<PauseState*>(STMA::GetStates().back())) 
		return; // If GameState is rendering but PauseState is the current state, return.
	SDL_RenderPresent(REMA::GetRenderer());
}

void State::Exit()
{
	for (auto& i : m_objects)
	{
		delete i.second;
		i.second = nullptr;
	}
	m_objects.clear();
	m_objects.shrink_to_fit();
}

void State::AddChild(std::string key, GameObject* object)
{
	m_objects.push_back(pair<string, GameObject*>(key, object));
}

GameObject* State::GetChild(const std::string& key)
{
	auto it = std::find_if(m_objects.begin(), m_objects.end(),
		[&](const std::pair<std::string, GameObject*>& element)
		{
			return element.first == key;
		});
	if (it != m_objects.end())
		return it->second;
	else return nullptr;
}

void State::RemoveChild(const std::string& key)
{
	auto it = std::find_if(m_objects.begin(), m_objects.end(),
		[&](const std::pair<std::string, GameObject*>& element)
		{
			return element.first == key;
		});
	if (it != m_objects.end())
	{
		delete it->second; // Deallocate GameObject.
		it->second = nullptr; // Wrangle dangle.
		m_objects.erase(it);
		m_objects.shrink_to_fit();
	}
}

// Begin TitleState.
TitleState::TitleState() = default;

void TitleState::Enter()
{
	cout << "Entering TitleState..." << endl;
	TEMA::Load("../Assets/img/button.png", "play");
	SOMA::AllocateChannels(16);
	SOMA::SetMusicVolume(32);

	AddChild("play", new PlayButton({ 0, 0, 400, 100 }, { 412.0f, 384.0f, 200.0f, 50.0f }, "play"));
}

void TitleState::Update()
{
	State::Update();
}

void TitleState::Render()
{
	SDL_SetRenderDrawColor(REMA::GetRenderer(), 0, 0, 0, 255);
	SDL_RenderClear(REMA::GetRenderer());
	State::Render();
}

void TitleState::Exit()
{
	cout << "Exiting TitleState..." << endl;
	TEMA::Unload("play");
	State::Exit();
}
// End TitleState.

// Begin PauseState.
PauseState::PauseState() = default;

void PauseState::Enter()
{
	cout << "Entering PauseState..." << endl;
}

void PauseState::Update()
{
	if (EVMA::KeyPressed(SDL_SCANCODE_R))
	{
		STMA::PopState();
		return;
	}
	State::Update();
}

void PauseState::Render()
{
	// First render the GameState
	STMA::GetStates().front()->Render();
	// Now render rest of PauseState
	SDL_SetRenderDrawColor(REMA::GetRenderer(), 0, 0, 255, 128);
	SDL_Rect rect = { 256, 128, 512, 512 };
	SDL_RenderFillRect(REMA::GetRenderer(), &rect);
	State::Render();
}

void PauseState::Exit()
{
	cout << "Exiting PauseState..." << endl;
	State::Exit();
}
// End PauseState.

// Begin GameState.
GameState::GameState() = default;

void GameState::Enter()
{
	cout << "Entering GameState..." << endl;
	TEMA::Load("../Assets/img/Player.png", "player");
	TEMA::Load("../Assets/img/Tiles.png", "tiles");
	SOMA::Load("../Assets/aud/Jump.wav", "jump", SOUND_SFX);
	SOMA::Load("../Assets/aud/Guile.mp3", "guile", SOUND_MUSIC);
	SOMA::SetSoundVolume(32);
	SOMA::SetMusicVolume(16);
	SOMA::PlayMusic("guile", -1, 2000);

	AddChild("level", new TiledLevel(24,32,32,32,
		"../Assets/dat/Tiledata.txt", "../Assets/dat/Level1.txt", "tiles"));
	//AddChild("platform", new Rectangle({ 0, 700, 1024, 25 }, true));

	AddChild("player", new PlatformPlayer({ 0,0,0,0 }, { 128,576,64,64 }));
}

void GameState::Update()
{
	if (EVMA::KeyPressed(SDL_SCANCODE_X))
	{
		STMA::ChangeState(new TitleState()); // Change to new TitleState
		return;
	}
	else if (EVMA::KeyPressed(SDL_SCANCODE_P))
	{
		SOMA::PauseMusic();
		STMA::PushState(new PauseState()); // Add new PauseState
		return;
	}
	// Update objects first.
	State::Update();
	// Check collision.
	PlatformPlayer* pp = static_cast<PlatformPlayer*>(GetChild("player"));
	SDL_FRect* p = pp->GetDst(); // Copies address of player m_dst.
	vector<Tile*>& obstacles = static_cast<TiledLevel*>(GetChild("level"))->GetObstacles();

	for (int i = 0; i < obstacles.size(); i++)
	{
		SDL_FRect* t = obstacles[i]->GetDst();
		if (COMA::AABBCheck(*p, *t)) // Collision check between player rect and platform rect.
		{
			if ((p->y + p->h) - (float)pp->GetVelY() <= t->y) // If bottom of player < top of platform in "previous frame"
			{ // Colliding with top side of tile. Or collided from top.
				pp->StopY();
				pp->SetY(t->y - p->h);
				pp->SetGrounded(true);
			} else if (p->y - static_cast<float>(pp->GetVelY()) >= t->y + t->h)
			{ // Colliding with bottom side of tile
				pp->StopY();
				pp->SetY(t->y + t->h);
			} else if ((p->x + p->w) - static_cast<float>(pp->GetVelX()) <= t->x)
			{ // Colliding with left side of tile
				pp->StopX();
				pp->SetX(t->x - p->w);
			} else if (p->x - static_cast<float>(pp->GetVelX()) >= (t->x + t->w))
			{ // Colliding with right side of tile
				pp->StopX();
				pp->SetX(t->x + t->w);
			}
		}
		// End collision checks.
	}
	
}

void GameState::Render()
{
	SDL_SetRenderDrawColor(REMA::GetRenderer(), 0, 0, 0, 255);
	SDL_RenderClear(REMA::GetRenderer());
	State::Render();

}

void GameState::Exit()
{
	cout << "Exiting GameState..." << endl;
	TEMA::Unload("player");
	TEMA::Unload("tiles");
	SOMA::StopSound();
	SOMA::StopMusic();
	SOMA::Unload("jump", SOUND_SFX);
	SOMA::Unload("guile", SOUND_MUSIC);
	State::Exit();
}

void GameState::Resume()
{
	cout << "Resuming GameState..." << endl;
	SOMA::ResumeMusic();
}
// End GameState