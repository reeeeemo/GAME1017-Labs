#include <iostream>
#include "States.h"
#include "StateManager.h"
#include "CollisionManager.h"
#include "EventManager.h"
#include "RenderManager.h"
#include "TextureManager.h"
using namespace std;

void State::Render()
{
	SDL_RenderPresent(REMA::Instance().GetRenderer());
}

TitleState::TitleState(){}

void TitleState::Enter()
{
	
}

void TitleState::Update()
{
	if (EVMA::KeyPressed(SDL_SCANCODE_N))
	{
		STMA::ChangeState( new GameState() );
	}
}

void TitleState::Render()
{
	SDL_SetRenderDrawColor(REMA::Instance().GetRenderer(), 0, 0, 255, 255);
	SDL_RenderClear(REMA::Instance().GetRenderer());
	State::Render();
}

void TitleState::Exit()
{
	
}

void GameState::ClearTurrets()
{
	for (unsigned i = 0; i < m_turrets.size(); i++)
	{
		delete m_turrets[i];
		m_turrets[i] = nullptr;
	}
	m_turrets.clear();
	m_turrets.shrink_to_fit();
	// You can assimilate some parts of this code for deleting bullets and enemies.
}

GameState::GameState():m_spawnCtr(1) {}

void GameState::Enter()
{
	TEMA::Load("../Assets/img/Turret.png", "turret");
	TEMA::Load("../Assets/img/Enemies.png", "enemy");
	s_enemies.push_back(new Enemy({ 80,0,40,57 }, { 512.0f, -57.0f, 40.0f, 57.0f }));
	// Create the DOM and load the XML file.

	// Iterate through the Turret elements in the file and push_back new Turrets into the m_turrets vector.
		// Keep the width and height as 100.
	// Look at the last two XML examples from Week 3
}

void GameState::Update()
{
	// Parse T and C events.
	if (EVMA::KeyPressed(SDL_SCANCODE_T))
	{
		m_turrets.push_back(new Turret({ 0, 0, 100, 100 }, { 50.0f, 618.0f, 100.0f, 100.0f } ));
	}
	if (EVMA::KeyPressed(SDL_SCANCODE_C))
	{
		ClearTurrets();
	}
	// Update all GameObjects individually. Spawn enemies. Update turrets. Update enemies. Update bullets.
	if (m_spawnCtr++ % 180 == 0)
	{
		s_enemies.push_back(new Enemy( {80,0,40,57}, {(float)(rand() % (1024-40)), -57.0f, 40.0f, 57.0f, } ));
	}
	for (unsigned i = 0; i < m_turrets.size(); i++)
	{
		m_turrets[i]->Update();
	}
	for (unsigned i = 0; i < s_enemies.size(); i++)
	{
		s_enemies[i]->Update();

		if (s_enemies[i]->deleteMe == true)
		{
			delete s_enemies[i];
			s_enemies[i] = nullptr;
			s_enemies.erase(s_enemies.begin() + i);
			s_enemies.shrink_to_fit();
		}
	}
	for (unsigned i = 0; i < s_bullets.size(); i++)
	{
		s_bullets[i]->Update();

		// Cleanup bullets if they are to be deleted.
		if (s_bullets[i]->deleteMe == true)
		{
			delete s_bullets[i];
			s_bullets[i] = nullptr;
			s_bullets.erase(s_bullets.begin() + i);
			s_bullets.shrink_to_fit();
		}
	}
	
		// for all enemies, similar to above

	// Check for collisions with bullets and enemies.
	for (unsigned int i = 0; i < s_bullets.size(); i++)
	{
		for (unsigned int j = 0; j < s_enemies.size(); j++)
		{
			if (COMA::AABBCheck(s_bullets[i]->GetDst(),s_enemies[j]->GetDst()))
			{
				delete s_bullets[i];
				s_bullets[i] = nullptr;
				s_bullets.erase(s_bullets.begin() + i);
				s_bullets.shrink_to_fit();

				delete s_enemies[j];
				s_enemies[j] = nullptr;
				s_enemies.erase(s_enemies.begin() + j);
				s_enemies.shrink_to_fit();
			}
		}
	}
}

void GameState::Render()
{
	SDL_SetRenderDrawColor(REMA::Instance().GetRenderer(), 0, 0, 0, 255);
	SDL_RenderClear(REMA::Instance().GetRenderer());

	for (unsigned i = 0; i < m_turrets.size(); i++)
		m_turrets[i]->Render();
	for (unsigned i = 0; i < s_enemies.size(); i++)
		s_enemies[i]->Render();
	for (unsigned i = 0; i < s_bullets.size(); i++)
		s_bullets[i]->Render();

	SDL_Rect spawnBox = { 50, 618, 100, 100 };
	SDL_SetRenderDrawColor(REMA::Instance().GetRenderer(), 255, 255, 255, 255);
	SDL_RenderDrawRect(REMA::Instance().GetRenderer(), &spawnBox);

	// This code below prevents SDL_RenderPresent from running twice in one frame.
	if ( dynamic_cast<GameState*>( STMA::GetStates().back() ) ) // If current state is GameState.
		State::Render();
}

void GameState::Exit()
{
	// You can clear all children of the root node by calling .DeleteChildren(); and this will essentially clear the DOM.

	// Iterate through all the turrets and save their positions as child elements of the root node in the DOM.

	// Make sure to save to the XML file.
		// xmlDoc.SaveFile("Turrets.xml");
	ClearTurrets(); // Deallocate all turrets, then all other objects.
	for (unsigned i = 0; i < s_enemies.size(); i++)
	{
		delete s_enemies[i];
		s_enemies[i] = nullptr;
	}
	s_enemies.clear();
	s_enemies.shrink_to_fit();
	for (unsigned i = 0; i < s_bullets.size(); i++)
	{
		delete s_bullets[i];
		s_bullets[i] = nullptr;
	}
	s_bullets.clear();
	s_bullets.shrink_to_fit();
}

void GameState::Resume()
{
	
}

// This is how static properties are allocated.
std::vector<Bullet*> GameState::s_bullets;
std::vector<Enemy*> GameState::s_enemies;