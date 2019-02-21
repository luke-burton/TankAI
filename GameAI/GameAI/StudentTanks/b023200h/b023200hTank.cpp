#include "b023200hTank.h"
#include "../../TankManager.h"
#include "../../Commons.h"
#include "../../C2DMatrix.h"

//--------------------------------------------------------------------------------------------------

b023200hTank::b023200hTank(SDL_Renderer* renderer, TankSetupDetails details) 
	: BaseTank(renderer, details)
{
	astar = new AStar(this,mCollisionMap);


}

//--------------------------------------------------------------------------------------------------

b023200hTank::~b023200hTank()
{
}
void b023200hTank::DrawCircle(Vector2D pos)
{
	
}

//--------------------------------------------------------------------------------------------------

void b023200hTank::ChangeState(BASE_TANK_STATE newState)
{
	BaseTank::ChangeState(newState);
}

//--------------------------------------------------------------------------------------------------

void b023200hTank::Update(float deltaTime, SDL_Event e)
{
	BaseTank::Update(deltaTime, e);
	
	
	//SDL_GetMouseState(&x, &y);


	switch (e.type)
	{
		case SDL_MOUSEBUTTONUP:
		{
			switch (e.button.button)
			{
			case SDL_BUTTON_LEFT:
				astar = new AStar(this, mCollisionMap);
				astar->Tick();
				break;
			}
		}
		break;
	}
	if (astar->finalpath.size() > 0)
	{
		x = astar->finalpath.back().position.x;
		y = astar->finalpath.back().position.y;
		if (Vec2DDistance(Vector2D(x, y), this->GetCentralPosition()) < 30)
		{
			astar->finalpath.pop_back();
		}
	}
	if (x == 0 && y == 0)
		return;
	std::cout << Vec2DDistance(Vector2D(x, y), this->GetCentralPosition())  << "\n";
		/*SEEK START*/
	float radian = (atan2(y - this->GetCentralPosition().y, x - this->GetCentralPosition().x));
	Vector2D toTarget = Vec2DNormalize(GetCentralPosition() - Vector2D(x,y));
	double angle = acos(mHeading.Dot(toTarget));
	if (angle != angle)
		angle = 0.0f;
	if (angle > 3)
	{

	
		if (Vec2DDistance(Vector2D(x, y), this->GetCentralPosition()) > 30)
		{
			mCurrentSpeed += kSpeedIncrement * deltaTime;
			if (mCurrentSpeed > GetMaxSpeed())
				mCurrentSpeed = GetMaxSpeed();

		}
		else
		{
			if (mCurrentSpeed > 0)
				mCurrentSpeed -= kSpeedIncrement * deltaTime;
			if (mCurrentSpeed < 0)
				mCurrentSpeed = 0;
		}

	}
	else
	{
		if (mCurrentSpeed > 0)
			mCurrentSpeed -= kSpeedIncrement * deltaTime;
		if (mCurrentSpeed < 0)
			mCurrentSpeed = 0;
	}

		RotateHeadingToFacePosition(Vector2D(x, y), -19);

		/*SEEK END*/
		
}

//--------------------------------------------------------------------------------------------------

void b023200hTank::MoveInHeadingDirection(float deltaTime)
{
	//Get the force that propels in current heading.
	Vector2D force = (mHeading*mCurrentSpeed) - mVelocity;

	//Acceleration = Force/Mass
	Vector2D acceleration = force / GetMass();

	//Update velocity.
	mVelocity += acceleration * deltaTime;

	//Don't allow the tank does not go faster than max speed.
	mVelocity.Truncate(GetMaxSpeed()); //TODO: Add Penalty for going faster than MAX Speed.

									   //Finally, update the position.
	Vector2D newPosition = GetPosition();
	newPosition.x += mVelocity.x*deltaTime;
	newPosition.y += (mVelocity.y/**-1.0f*/)*deltaTime;	//Y flipped as adding to Y moves down screen.
	SetPosition(newPosition);
}

//--------------------------------------------------------------------------------------------------

void b023200hTank::RotateHeadingByRadian(double radian, int sign)
{
	//Clamp the amount to turn to the max turn rate.
	if (radian > mMaxTurnRate) 
		radian = mMaxTurnRate;
	else if(radian < -mMaxTurnRate)
		radian = -mMaxTurnRate;
	//IncrementTankRotationAngle(RadsToDegs(radian));
    mRotationAngle += RadsToDegs(radian)*sign;

	//Usee a rotation matrix to rotate the player's heading
	C2DMatrix RotationMatrix;
  
	//Calculate the direction of rotation.
	RotationMatrix.Rotate(radian * sign);	
	//Get the new heading.
	RotationMatrix.TransformVector2Ds(mHeading);

	//cout << "RotateHeadingByRadian -- Heading x = " << mHeading.x << " y = " << mHeading.y << endl;

	//Get the new velocity.
	RotationMatrix.TransformVector2Ds(mVelocity);

	//Side vector must always be perpendicular to the heading.
	mSide = mHeading.Perp();
}

//--------------------------------------------------------------------------------------------------

void b023200hTank::Render()
{

	if (astar->finalpath.size() > 0)
	{

		//for (ANode node : astar->finalpath)
		////{
		///	DrawDebugCircle(node.position, 16, 255, 1, 1);
			DrawDebugCircle(astar->finalpath.back().position, 16, 255, 255, 1);
		//}
	}
	for (int i = 0; i < astar->NodeLen(); i++)
	{
		if(astar->getNode(i).Starting )
			DrawDebugCircle(astar->getNode(i).position, 16, 255, 1, 1);
		if(astar->getNode(i).Ending )
			DrawDebugCircle(astar->getNode(i).position, 16, 1, 255, 1);
	}
	BaseTank::Render();
}