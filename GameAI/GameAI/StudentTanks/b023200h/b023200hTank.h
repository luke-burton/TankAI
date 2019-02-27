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

	int startT;
};

//---------------------------------------------------------------

#endif //b023200hTank_H