#include "AStar.h"
#include <algorithm>

AStar::AStar(b023200hTank* tank,TileType map[kMapWidth][kMapHeight])
{
	myself = tank;
	for (int x = 0; x < kMapWidth; x++)
	{
		for (int y = 0; y < kMapHeight; y++)
		{
			world[x][y] = map[x][y];
		}
	}
	MapWorld();
	
}


AStar::~AStar()
{
}

void AStar::Tick()
{

	Reset();
	FindClosestToSandE();

	int loops = 1;
	for (int is = 0; is < 1000; is++)
	{
		if (pathfound)
			break;
		loops = is;

		FindNeighbours();
		FindBestNeighbour();
		
	}
	
	int next = end;
	int last = end;
	finalpath.push_back(ANode(nodes.at(end)));
	for (int is = 0; is < 100; is++)
	{
		if (next == start)
		{
			finalpath.push_back(ANode(nodes.at(next)));
			break;
		}
		std::cout << nodes.at(next).dir << " : " << nodes.at(last).dir << "\n";
		if(nodes.at(next).dir != nodes.at(last).dir)
		{ 
			finalpath.push_back(ANode(nodes.at(next)));		
			next = nodes.at(next).LastNumber;
			last = next;
			continue;

		}
		next = nodes.at(next).LastNumber;
		
	

	}

}

void AStar::MapWorld()
{
	int loops = 0;
	for (int i = 0; i < kMapWidth; i++)
	{
		for (int j = 0; j < kMapHeight; j++)
		{
			if (world[i][j] == TileType::TILETYPE_EMPTY)
			{
				nodes.push_back(ANode(i * kTileDimensions, j * kTileDimensions, loops));
				loops++;
			}
		}
	}
}

void AStar::FindBestNeighbour()
{
	float bestDist = 99999;
	int bestN = 0;
	int neigh = -1;
	int bestneigh = 0;
	for (int i : neighbours)
	{
		neigh++;
		float extra = 0;
		
		float DistToC = (float)nodes.at(current).position.Distance(nodes.at(i).position);
		float DistToE = (float)nodes.at(end).position.Distance(nodes.at(i).position);
		float DistToS = (float)nodes.at(start).position.Distance(nodes.at(i).position);
		if (nodes.at(i).dir != nodes.at(nodes.at(i).LastNumber).dir && nodes.at(i).LastNumber != start)
			extra = DistToC + 10;
		if (DistToC + DistToE  + extra <= bestDist)
		{
			bestDist = DistToC + DistToE + extra;
			bestN = i;
			bestneigh = neigh;
			if (bestN == end && bestsofar == 0)
				bestsofar = bestDist;
			
		}
	
	}
	if (bestN == end)
	{
		nodes.at(end).LastNumber = current;
		neighbours.erase(neighbours.begin() + bestneigh);
		std::cout << "end found " << " last num = " << nodes.at(end).LastNumber << " \n ";
		if (bestsofar != 0 && bestsofar < bestDist)
		{
			pathfound = true;
			return;
		}
		if (neighbours.size() == 0)
		{
			pathfound = true;
			return;
		}
		return;
	}
	if (bestsofar != 0 && bestsofar < bestDist)
	{
		pathfound = true;
	}
	if (neighbours.size() == 0)
	{
		pathfound = true;
		return;
	}
	nodes.at(bestN).LastNumber = current;
	neighbours.erase(neighbours.begin() + bestneigh);
	current = bestN;

}

void AStar::FindNeighbours()
{
	ANode currentN = nodes.at(current);
	int f = 0;
	if (std::find(nodes.begin(), nodes.end(), ANode(currentN.position.x + 32, currentN.position.y)) != nodes.end())
	{
		f = std::find(nodes.begin(), nodes.end(), ANode(currentN.position.x + 32, currentN.position.y))._Ptr->MyNumber;
		if (!nodes.at(f).isClosed)
		{
			//finalpath.push_back(ANode(nodes.at(f)));
			nodes.at(f).LastNumber = current;
			nodes.at(f).dir = 1;
			nodes.at(f).isClosed = true;
			neighbours.push_back(f);
		}
	}
	if (std::find(nodes.begin(), nodes.end(), ANode(currentN.position.x - 32, currentN.position.y)) != nodes.end())
	{
		f = std::find(nodes.begin(), nodes.end(), ANode(currentN.position.x - 32, currentN.position.y))._Ptr->MyNumber;
		if (!nodes.at(f).isClosed)
		{
			nodes.at(f).LastNumber = current;
			//finalpath.push_back(ANode(nodes.at(f)));
			nodes.at(f).dir = 0;
			nodes.at(f).isClosed = true;
			neighbours.push_back(f);
		}
	}
	if (std::find(nodes.begin(), nodes.end(), ANode(currentN.position.x , currentN.position.y + 32)) != nodes.end())
	{
		f = std::find(nodes.begin(), nodes.end(), ANode(currentN.position.x , currentN.position.y + 32))._Ptr->MyNumber;
		if (!nodes.at(f).isClosed)
		{
			nodes.at(f).LastNumber = current;
		//finalpath.push_back(ANode(nodes.at(f)));
			nodes.at(f).dir = 3;
			nodes.at(f).isClosed = true;
			neighbours.push_back(f);
		}
	}
	if (std::find(nodes.begin(), nodes.end(), ANode(currentN.position.x, currentN.position.y - 32)) != nodes.end())
	{
		f = std::find(nodes.begin(), nodes.end(), ANode(currentN.position.x , currentN.position.y - 32 ))._Ptr->MyNumber;
		if (!nodes.at(f).isClosed)
		{
			nodes.at(f).LastNumber = current;
		//finalpath.push_back(ANode(nodes.at(f)));
			nodes.at(f).dir = 2;
			nodes.at(f).isClosed = true;
			neighbours.push_back(f);
		}
	}

}

void AStar::Reset()
{
	pathfound = false;
	current = 0;
	start = 0;
	end = 0;
	neighbours.empty();
	finalpath.empty();
	for (unsigned int i = 0; i < nodes.size(); i++)
	{
		nodes.at(i).isClosed = false;
		nodes.at(i).Ending = false;
		nodes.at(i).Starting = false;
		nodes.at(i).LastNumber = 0;
	}
}

void AStar::FindClosestToSandE()
{

	int x = 0, y = 0;
	if (myself->GetLowPrior() == SEEK || myself->GetLowPrior() == ARRIVE)
	{
		SDL_GetMouseState(&x, &y);
	}
	else	if (myself->GetLowPrior() == PURSUIT)
	{
	
			x = myself->GetTargetPos().x;
			y = myself->GetTargetPos().y;
	}
	
	Vector2D startpos = myself->GetCentralPosition();


	Vector2D endpos = Vector2D(x, y);
	int FDistance = 99999;

	for (ANode node : nodes)
	{
		if (startpos.Distance(node.position) <= FDistance)
		{
			start = node.MyNumber;
			FDistance = startpos.Distance(node.position);
		}
	}

	int EDistance = 99999;

	for (ANode node : nodes)
	{
		if (endpos.Distance(node.position) <= EDistance)
		{
			end = node.MyNumber;
			EDistance = endpos.Distance(node.position);
		}
	}

	current = start;
	nodes.at(start).Starting = true;
	nodes.at(end).Ending = true;
}
