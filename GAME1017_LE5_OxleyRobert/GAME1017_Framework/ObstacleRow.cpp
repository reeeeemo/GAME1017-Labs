#include "ObstacleRow.h"
#include "RenderManager.h"
#include "TextureManager.h"

ObstacleRow::ObstacleRow():m_gapCtr(0), m_gapMax(3)
{
	// Create the vector now.
	for (int i = 0; i < 9; i++)
	{
		m_obstacles.push_back(new Obstacle( { 128.0f * i, 384.0f, 128.0f, 128.0f }, false, nullptr));
	}
	m_obstacles.shrink_to_fit();

	// Load the textures
	TEMA::Load("../Assets/img/obstacles/Introl_Trap.png", "obstacleSheet");
	InitializeTextures();

}

void ObstacleRow::Update()
{
	for (const auto obstacle : m_obstacles)
	{
		// When the first obstacle goes completely offscreen
		if (obstacle != nullptr) // Error checker!
		{
			if (obstacle->GetPos().x <= 0 - 128.0f)
			{
				delete m_obstacles[0];
				m_obstacles[0] = nullptr;
				m_obstacles.erase(m_obstacles.begin());

				if (m_gapCtr++ % m_gapMax == 0)
				{
					m_obstacles.push_back(new Obstacle({ m_obstacles.back()->GetPos().x + 128.0f, 384.0f, 128.0f, 128.0f }, true, new Image({ 160, 480, 128, 128 }, { 0, 0, 128, 128 }, "obstacleSheet")));
				}
				else {
					m_obstacles.push_back(new Obstacle({ m_obstacles.back()->GetPos().x + 128.0f, 384.0f, 128.0f, 128.0f}, false, nullptr));
				}


				m_obstacles.shrink_to_fit();
			}
			else { // Scroll the obstacles.
				obstacle->Update();
			}
		}
	}
}

void ObstacleRow::Render()
{
	// Blue line under obstacles.
	SDL_SetRenderDrawColor(REMA::GetRenderer(), 0, 0, 255, 255);
	SDL_RenderDrawLine(REMA::GetRenderer(), 0, 512, 1024, 512);
	// Render the obstacles.
	for (const auto obstacle : m_obstacles)
	{
		if (obstacle != nullptr) // Error checker!
		{
			obstacle->Render();
		}
	}
}

// Does not work atm, I hate poinjtersedwgfs
void ObstacleRow::InitializeTextures()
{
	m_obstacleTextures[SPIKE] = new Image( { 0, 0, 320, 320 }, { 0, 0, 320, 320 }, "obstacleSheet");
	m_obstacleTextures[SAW] = new Image( { 0, 0, 320, 320 }, { 0, 0, 320, 320 }, "obstacleSheet");
	m_obstacleTextures[LONG_SPIKE] = new Image( { 160, 480, 128, 128 }, { 0, 0, 128, 128 }, "obstacleSheet");

	m_currentObstacleTexture = m_obstacleTextures[LONG_SPIKE];
}
