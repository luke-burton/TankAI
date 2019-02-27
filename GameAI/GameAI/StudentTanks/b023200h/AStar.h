#pragma once
#include <iostream>
#include <vector>

#include "../../Commons.h"
#include "b023200hTank.h"
struct ANode
{
	bool isClosed;
	int MyNumber, LastNumber;
	float lastdist;
	int dir;//0 = up, 1 = down, 2 = left, 3 = right
	Vector2D position;
	ANode()
	{
		lastdist = 0;
		dir = 0;
		isClosed = false;
		MyNumber = 0;
		LastNumber = 0;
		position = Vector2D(0, 0);
	}
	ANode(ANode& old)
	{
		lastdist = old.lastdist;
		dir = old.dir;
		isClosed = old.isClosed;
		MyNumber = old.MyNumber;
		LastNumber = old.LastNumber;
		position = old.position;
	}
	ANode(int X, int Y, int N)
	{
		lastdist = 0;
		dir = 0;
		MyNumber = N;
		LastNumber = 0;
		isClosed = false;
		position = Vector2D(X, Y);
	}
	ANode(int X, int Y)
	{
		lastdist = 0;
		dir = 0;
		MyNumber = 0;
		LastNumber = 0;
		isClosed = false;
		position = Vector2D(X, Y);
	}

	bool operator== (const ANode& other) const {
		if (position == other.position)
			return true;
		else
			return false;
	}
	bool operator== (const Vector2D& other) const {
		if (position == other)
			return true;
		else
			return false;
	}

	
};
class AStar
{

private:
	TileType world[kMapWidth][kMapHeight];
	void MapWorld();
	void FindClosestToSandE();
	void Reset();
	void FindNeighbours();
	void FindBestNeighbour();
protected:
	std::vector<ANode> nodes;//list of all nodes
	std::vector<int> neighbours;//list of neighbours
	class b023200hTank* myself;//refence to my tank
	Vector2D endpos;
	bool pathfound = false;//
	int bestsofar = 0;
	int current;//current node
	int end;//end node
	int start;//starting node
public:
	std::vector<ANode> finalpath;//list of all path nodes
	int NodeLen() { return nodes.size() - 1; }
	ANode getNode(int i) { return nodes.at(i); }


	void Tick();

	AStar(b023200hTank* tank,TileType map[kMapWidth][kMapHeight]);
	~AStar();
};

