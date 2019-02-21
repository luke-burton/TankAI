#include "StudentTanks/b023200h/AStar.h"


AStar::AStar(b023200hTank* tank,SDL_Renderer* renderer,TileType map[kMapWidth][kMapHeight])
{
	myself = tank;
	int loops = 0;
	for (int i = 0; i < kMapWidth; i++)
	{
		for (int j = 0; j < kMapHeight; j++)
		{
			if (map[i][j] == TileType::TILETYPE_EMPTY)
			{
				nodes.push_back(ANode(i * kTileDimensions, j * kTileDimensions,loops));
				loops++;
			}
		}
	}

	FindClosestToSandE();
}

void AStar::FindClosestToSandE()
{
	Vector2D startpos = myself->GetCentralPosition();
	Vector2D endpos = myself->GetCentralPosition() + Vector2D(-50, -50);

	int FDistance = 99999;

	for (ANode node : nodes)
	{
		if (startpos.Distance(node.position) <= FDistance)
		{
			start = node.MyNumber;
			FDistance = startpos.Distance(node.position) ;
		}
	}

	int EDistance = 99999;

	for (ANode node : nodes)
	{
		if (endpos.Distance(node.position) <= EDistance)
		{
			end = node.MyNumber;
			EDistance = endpos.Distance(node.position) ;
		}
	}

	current = start;
	nodes.at(start).Starting = true;
	nodes.at(end).Ending = true;
}

AStar::~AStar()
{
}


void AStar::MapWorld()
{

}