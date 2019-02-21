#pragma once
#include <iostream>
#include <vector>
#include "../../Commons.h"
#include "b023200hTank.h"
struct ANode
{
	bool Starting, Ending, isClosed;
	int MyNumber, LastNumber, InPath;
	Vector2D position;
	ANode()
	{
		InPath = false;
		Starting = false;
		Ending = false;
		isClosed = false;
		MyNumber = 0;
		LastNumber = 0;
		position = Vector2D(0, 0);
	}
	ANode(ANode& old)
	{
		InPath = old.InPath;
		Starting = old.Starting;
		Ending = old.Ending;
		isClosed = old.isClosed;
		MyNumber = old.MyNumber;
		LastNumber = old.LastNumber;
		position = old.position;
	}
	ANode(int X, int Y, int N)
	{
		InPath = false;
		MyNumber = N;
		LastNumber = 0;
		isClosed = false;
		position = Vector2D(X, Y);
		Starting = false;
		Ending = false;
	}
	ANode(int X, int Y)
	{
		InPath = false;
		MyNumber = 0;
		LastNumber = 0;
		isClosed = false;
		position = Vector2D(X, Y);
		Starting = false;
		Ending = false;
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
protected:
	std::vector<ANode> nodes;//list of all nodes

	std::vector<int> neighbours;//list of neighbours
	class b023200hTank* myself;//refence to my tank
	Vector2D endpos;//TEMP 
	bool pathfound = false;//
	int current;//current node
	int end;//end node
	int start;//starting node
public:
	std::vector<ANode> finalpath;//list of all path nodes
	void MapWorld();
	int NodeLen() { return nodes.size() - 1; }
	ANode getNode(int i) { return nodes.at(i); }
	void FindClosestToSandE();
	void Reset();
	void FindNeighbours();
	void FindBestNeighbour();
	void Tick();

	AStar(b023200hTank* tank,TileType map[kMapWidth][kMapHeight]);
	~AStar();
};

