#ifndef b023200hTank_H
#define b023200hTank_H

#include "../../BaseTank.h"
#include <SDL.h>
#include "AStar.h"
#include <string>
#include <iostream>
#include <thread>
#include "../../ObstacleManager.h"
#include "../../Commons.h"

enum Steering
{
	SEEK = 1, ARRIVE = 2, AVOIDOB = 3, PURSUIT = 4,
};
//---------------------------------------------------------------

class b023200hTank : public BaseTank
{
	//---------------------------------------------------------------
public:
	b023200hTank(SDL_Renderer* renderer, TankSetupDetails details);
	~b023200hTank();

	void ChangeState(BASE_TANK_STATE newState);
	void RunHighPrior(float deltaTime);
	void Update(float deltaTime, SDL_Event e);
	void Render();
	void RotateHeadingByRadian(double radian, int sign);
	void DrawCircle(Vector2D pos);
	Steering GetLowPrior() { return LowPriorBehavior; }
	Vector2D GetTargetPos() { return TargLastSeen;  }
	//---------------------------------------------------------------
protected:
	void	MoveInHeadingDirection(float deltaTime);
	int x = 0, y = 0;
	class AStar* astar;
	void findpath();
	std::thread t1;
	Steering LowPriorBehavior;
	void RunLowPrior(float deltaTime);
	Vector2D TargLastSeen;
	std::vector<Vector2D> corners;
	int startT;
	bool ShouldAvoidL(Vector2D TopLeft,Vector2D BotLeft, Vector2D up, Vector2D down, Vector2D left, Vector2D right);
	bool ShouldAvoidR(Vector2D TopLeft,Vector2D BotLeft, Vector2D up, Vector2D down, Vector2D left, Vector2D right);
	bool ShouldAvoidU(Vector2D TopLeft, Vector2D BotLeft, Vector2D up, Vector2D down, Vector2D left, Vector2D right);
	bool ShouldAvoidD(Vector2D TopLeft, Vector2D BotLeft, Vector2D up, Vector2D down, Vector2D left, Vector2D right);
	Vector2D Force;
	bool ishit = false;
	bool ishit1 = false;
	bool ishit2 = false;
	bool ishit3 = false;
	bool ishit4 = false;
	bool ishit5 = false;
	bool ishit6 = false;
	bool ishit7 = false;


	float xy = 0;
	float yx = 0;
};

//---------------------------------------------------------------

#endif //b023200hTank_H