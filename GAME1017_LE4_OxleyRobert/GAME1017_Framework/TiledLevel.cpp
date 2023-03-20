#include "TiledLevel.h"
#include "RenderManager.h"
#include "TextureManager.h"
#include <iostream>

TiledLevel::TiledLevel(const unsigned short rows, const unsigned short cols, const int tileWidth, const int tileHeight, 
	const char* tileData, const char* levelData, const char* tileKey) :m_rows(rows), m_cols(cols), m_tileKey(tileKey)
{
	// First build prototype tiles.
	std::ifstream inFile(tileData);
	if (inFile.is_open())
	{
		char key;
		int x, y;
		bool obs, haz;

		while (!inFile.eof())
		{
			inFile >> key >> x >> y >> obs >> haz;

			m_tiles.emplace(key, new Tile({x * tileWidth, y * tileHeight,tileWidth,tileHeight}, {0.0f,0.0f,(float)tileWidth,(float)tileHeight}, obs, haz));
		}
		inFile.close();
	} else
	{
		std::cout << "Error opening tileData!" << std::endl;
		return;
	}
	// Now construct the level.
	inFile.open(levelData);
	if (!inFile.is_open())
	{
		std::cout << "Error opening levelData!" << std::endl;
		return;
	}
	char key;
	m_level.resize(m_rows); // We need resize in order to use subscripts
	for (unsigned row = 0; row < m_rows; row++)
	{
		m_level[row].resize(m_cols);
		for (unsigned col = 0; col < m_cols; col++)
		{
			inFile >> key;
			m_level[row][col] = m_tiles[key]->Clone();
			m_level[row][col]->SetXY((float)(col * tileWidth), (float)(row * tileHeight));
			if (m_level[row][col]->IsObstacle())
			{
				m_obstacles.push_back(m_level[row][col]);
				m_obstacles.shrink_to_fit();
			}
		}
	}
	inFile.close();
}

TiledLevel::~TiledLevel()
{
	// Clear the tile clones. This also clears the ones in m_obstacles.
	for (unsigned row = 0; row < m_rows; row++)
	{
		for (unsigned col = 0; col < m_cols; col++)
		{
			delete m_level[row][col];
			m_level[row][col] = nullptr;
		}
	}
	
	// Clear the original tiles.
	for (int i = 0; i < m_tiles.size(); i++)
	{
		delete m_tiles[i];
		m_tiles[i] = nullptr;
	}
	m_tiles.clear();
}

void TiledLevel::Render()
{
	for (unsigned row = 0; row < m_rows; row++)
	{
		for (unsigned col = 0; col < m_cols; col++)
		{
			SDL_RenderCopyF(REMA::GetRenderer(), TEMA::GetTexture(m_tileKey), 
				m_level[row][col]->GetSrc(), m_level[row][col]->GetDst());
		}
	}
}

std::vector<Tile*>& TiledLevel::GetObstacles() { return m_obstacles; }