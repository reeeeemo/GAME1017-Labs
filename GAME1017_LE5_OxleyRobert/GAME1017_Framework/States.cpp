#include <iostream>
#include "States.h"
#include "StateManager.h"
// GameObject includes.
#include "Button3.h"
#include "ObstacleRow.h"
#include "Primitives.h"
// Remaining managers.
#include "CollisionManager.h"
#include "EventManager.h"
#include "FontManager.h"
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
	SDL_SetRenderDrawColor(REMA::GetRenderer(), 32, 0, 0, 255);
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
	FOMA::Load("../Assets/img/ltype.TTF", "ltype24", 24);
	AddChild("pauseText", new Label("ltype24", 325, 175, "Paused - press R to resume"));
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
	SDL_SetRenderDrawColor(REMA::GetRenderer(), 64, 0, 0, 128);
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
	AddChild("obstacles", new ObstacleRow());
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
	State::Exit();
}

void GameState::Resume()
{
	cout << "Resuming GameState..." << endl;
	SOMA::ResumeMusic();
}
// End GameState