#ifndef b023200hTank_H
#define b023200hTank_H

#include "../../BaseTank.h"
#include <SDL.h>
#include "AStar.h"
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

	class AStar* astar;
};

//---------------------------------------------------------------

#endif //b023200hTank_H