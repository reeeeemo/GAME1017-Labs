#include <iostream>
#include "States.h"
#include "StateManager.h"
#include "CollisionManager.h"
#include "EventManager.h"
#include "RenderManager.h"
#include "TextureManager.h"
#include "tinyxml2.h"
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
	// Initializes XML Doc if it exists.
	if (gameDataDoc.LoadFile("GameData.xml") == 0)
	{
		pRoot = gameDataDoc.FirstChild();
		if (pRoot == nullptr) // Error checker in case root is not found
		{
			std::cout << tinyxml2::XML_ERROR_FILE_COULD_NOT_BE_OPENED << std::endl;
		}
		// Gets the turret count so we know how many turrets to look out for.
		pElement = pRoot->FirstChildElement("TurretCount");
		if (pElement == nullptr)
		{
			std::cout << tinyxml2::XML_ERROR_PARSING_ELEMENT << std::endl;
		}
		int turretCount;
		pElement->QueryIntText(&turretCount);

		pElement = pRoot->FirstChildElement("Turret");
		// Gets the turrets and pushes the turrets into the vector
		for (int i = 0; i < turretCount; i++)
		{
			if (pElement == nullptr) // Error checker in case element is not found
			{
				std::cout << tinyxml2::XML_ERROR_PARSING_ELEMENT << std::endl;
			}
			float posX;
			float posY;
			int targetsDestroyed;
			pElement->QueryFloatAttribute("PosX", &posX);
			pElement->QueryFloatAttribute("PosY", &posY);
			pElement->QueryIntAttribute("TargetsDestroyed", &targetsDestroyed);

			Turret* tempTurret = new Turret({ 0, 0, 100, 100 }, { posX, posY, 100.0f, 100.0f });
			tempTurret->targetsDestroyed = targetsDestroyed;
			m_turrets.push_back(tempTurret);

			pElement = pElement->NextSiblingElement("Turret");

		}

	}
	
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

	// Check for collisions with bullets and enemies.
	for (int i = 0; i < s_bullets.size(); i++)
	{
		for (int j = 0; j < s_enemies.size(); j++)
		{
			if (COMA::AABBCheck(s_bullets[i]->GetDst(), s_enemies[j]->GetDst()))
			{
				// Add destroyed counter to turret
				s_bullets[i]->startingTurret->targetsDestroyed++;

				// Destroy bullets and enemies
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

	// Enemy Update
	for (unsigned i = 0; i < s_enemies.size(); i++)
	{
		s_enemies[i]->Update();

		if (s_enemies[i]->deleteMe == true)
		{
			// Destroy enemies
			delete s_enemies[i];
			s_enemies[i] = nullptr;
			s_enemies.erase(s_enemies.begin() + i);
			s_enemies.shrink_to_fit();
		}
	}
	// Bullet Update
	for (int i = 0; i < s_bullets.size(); i++)
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
	// Clearing the DOM.
	gameDataDoc.DeleteChildren();

	// Creating the root.	
	pRoot = gameDataDoc.NewElement("Root");
	gameDataDoc.InsertFirstChild(pRoot);

	pElement = gameDataDoc.NewElement("TurretCount");
	pElement->SetText(m_turrets.size());
	pRoot->InsertEndChild(pElement);

	// Iterate through all the turrets and save their positions as well as kill count.
	for (Turret* turret: m_turrets)
	{
		pElement = gameDataDoc.NewElement("Turret");
		pElement->SetAttribute("PosX", turret->GetDst().x);
		pElement->SetAttribute("PosY", turret->GetDst().y);
		pElement->SetAttribute("DestroyCounter", turret->targetsDestroyed);
		pRoot->InsertEndChild(pElement);
	}

	// Saving file.
	gameDataDoc.SaveFile("GameData.xml");

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