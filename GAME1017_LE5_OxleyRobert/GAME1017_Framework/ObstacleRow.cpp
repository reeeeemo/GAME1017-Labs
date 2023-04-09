#include "ObstacleRow.h"
#include "RenderManager.h"

ObstacleRow::ObstacleRow():m_gapCtr(0), m_gapMax(3)
{
	// Create the vector now.
	// 
	// Fill in for lab.
}

void ObstacleRow::Update()
{
	// Fill in for lab.
}

void ObstacleRow::Render()
{
	// Blue line under obstacles.
	SDL_SetRenderDrawColor(REMA::GetRenderer(), 0, 0, 255, 255);
	SDL_RenderDrawLine(REMA::GetRenderer(), 0, 512, 1024, 512);
	// Render the obstacles.
	// 
	// Fill in for lab.
}
