#include "a016430fTank.h"
#include "../../TankManager.h"
#include "../../Commons.h"
#include "../../C2DMatrix.h"

//--------------------------------------------------------------------------------------------------

a016430fTank::a016430fTank(SDL_Renderer* renderer, TankSetupDetails details) 
	: BaseTank(renderer, details)
{
	LowLevelStack = TankState::behavior::SEEK;
}

//--------------------------------------------------------------------------------------------------

a016430fTank::~a016430fTank()
{
}


//--------------------------------------------------------------------------------------------------

void a016430fTank::ChangeState(BASE_TANK_STATE newState)
{
	BaseTank::ChangeState(newState);
}

//--------------------------------------------------------------------------------------------------


void a016430fTank::Update(float deltaTime, SDL_Event e)
{
	BaseTank::Update(deltaTime, e);
	
	SDL_GetMouseState(&x, &y);
	
}


void a016430fTank::Render()
{
	DrawDebugCircle(Vector2D(x, y), 16, 255, 0, 255);
	BaseTank::Render();
	
}