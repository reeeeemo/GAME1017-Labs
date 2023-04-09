#ifndef __OBSTACLEROW_H__
#define __OBSTACLEROW_H__
#include "GameObject.h"
#include "Obstacle.h"
#include <vector>

class ObstacleRow : public GameObject
{
public: // Methods.
	ObstacleRow();
	void Update();
	void Render();
	
private: // Properties.
	std::vector<Obstacle*> m_obstacles;
	int m_gapCtr, m_gapMax;
};

#endif