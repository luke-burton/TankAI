#ifndef b023200hTank_H
#define b023200hTank_H

#include "../../BaseTank.h"
#include <SDL.h>
#include "AStar.h"
#include <string>
#include <iostream>
#include <thread>
#include "../../Commons.h"


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
	//---------------------------------------------------------------
protected:
	void	MoveInHeadingDirection(float deltaTime);
	int x = 0, y = 0;
	class AStar* astar;
	void findpath();
	std::thread t1;
};

//---------------------------------------------------------------

#endif //b023200hTank_H