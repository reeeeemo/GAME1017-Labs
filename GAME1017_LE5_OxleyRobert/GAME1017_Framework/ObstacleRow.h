#ifndef __OBSTACLEROW_H__
#define __OBSTACLEROW_H__
#include "GameObject.h"
#include "Obstacle.h"
#include <vector>
#include <map>

enum CurrentObstacle {
	GREEN_TREE,
	BLUE_TREE,
	HANGING_RED_TREE,
	NUM_OBSTACLES
};

class ObstacleRow : public GameObject
{
public: // Methods.
	ObstacleRow();
	void Update();
	void Render();
	
	void PickRandomObstacle();

private: // Properties.
	std::vector<Obstacle*> m_obstacles;
	int m_gapCtr, m_gapMax;
	CurrentObstacle curObstacle;
};

#endif