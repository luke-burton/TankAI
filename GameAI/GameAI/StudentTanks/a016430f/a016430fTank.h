#ifndef a016430fTank_H
#define a016430fTank_H

#include "../../BaseTank.h"
#include <SDL.h>
#include <string>
#include <iostream>
#include <thread>
#include "../../ObstacleManager.h"
#include "../../Commons.h"

namespace TankState
{
	enum behavior{SEEK = 0,};
}
//---------------------------------------------------------------

class a016430fTank : public BaseTank
{
	//---------------------------------------------------------------
public:
	a016430fTank(SDL_Renderer* renderer, TankSetupDetails details);
	~a016430fTank();

	void ChangeState(BASE_TANK_STATE newState);
	void Update(float deltaTime, SDL_Event e);
	void Render();

	//---------------------------------------------------------------
protected:
	TankState::behavior LowLevelStack;
	int x = 0, y = 0;

};

//---------------------------------------------------------------

#endif //a016430fTank_H