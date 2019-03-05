#ifndef W019563hTANK_H
#define W019563hTANK_H

#include "../../BaseTank.h"
#include <SDL.h>
#include "../../Commons.h"

//---------------------------------------------------------------

struct Node
{
	Vector2D Position;
	Vector2D GridPosition;
	float f;
	int thisnode;
	int lastnode;
	bool haschecked;
	int tileID;
};

struct Tank
{
	string TankName;
	Vector2D Position;
	Vector2D Heading;
	Vector2D Side;
	float Strength;
};


class w019563hTank : public BaseTank
{
	//---------------------------------------------------------------
public:
	w019563hTank(SDL_Renderer* renderer, TankSetupDetails details);
	~w019563hTank();

	void ChangeState(BASE_TANK_STATE newState);

	void Update(float deltaTime, SDL_Event e);

	void RotateHeadingByRadian(double radian, int sign);
	void Render();



	//---------------------------------------------------------------
protected:
	void	MoveInHeadingDirection(float deltaTime);
	void MoveToLocation(float deltaTime, Vector2D location);
	bool AtLocation(float deltaTime, Vector2D TankPosition, Vector2D EndPosition);
	void Avoid(float deltaTime, Vector2D ObsticalPosition, Vector2D EndPosition);
	void DoPathFinding(Node start, Node end);
	bool IsBlocked(Vector2D TileLocation);
	bool IsChecked(Vector2D TileLocation);
	void CheckTiles(Node StartNode);
	Node FindBestTile();
	void MakePath(Vector2D EndPosition);
	int TileIDAssigner = 0;
	bool FoundPath = false;
	int NodeToDelete = NULL;
	bool PathLength;
	void reset();
	bool DangerousTile(Vector2D TileLocation);

	void Attack(Tank TankToAttack);
	void FindTanks();
	void SetupNodes(int mx, int my, int xx, int yy);
	TileType mMap[kMapWidth][kMapHeight];
	bool canfire;
	int mx;
	int my;

	int NumberOfSteps;

	Node StartNode;
	Node EndNode;
	Node CurrentNode;
	Node CollisionNode;
	Tank CurrentTank;

	vector<Node> OpenList;
	vector<Node> ClosedList;
	vector<Node> FinalList;
	vector<Node> NodesToSearch;
	vector<Vector2D> EnemyFovTiles;
	vector<Tank> Tanks;

	void SearchForTanks();

	void FindBlocking();

	Vector2D Center(Vector2D position);

	float distance = 0;

private:
};

//---------------------------------------------------------------

#endif //CONTROLLEDTANK_H