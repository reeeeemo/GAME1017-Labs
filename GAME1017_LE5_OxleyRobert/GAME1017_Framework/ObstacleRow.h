#ifndef __OBSTACLEROW_H__
#define __OBSTACLEROW_H__
#include "GameObject.h"
#include "Obstacle.h"
#include <vector>
#include <map>

// Not great to put in same .h file, but for this exercise it's fine.
enum ObstacleTextures {
	SPIKE,
	SAW,
	LONG_SPIKE,
	NUM_OF_OBSTACLES,
};

class ObstacleRow : public GameObject
{
public: // Methods.
	ObstacleRow();
	void Update();
	void Render();
	
	void InitializeTextures();
private: // Properties.
	std::vector<Obstacle*> m_obstacles;
	int m_gapCtr, m_gapMax;
	std::map<ObstacleTextures, Image*> m_obstacleTextures;
	Image* m_currentObstacleTexture;
};

#endif