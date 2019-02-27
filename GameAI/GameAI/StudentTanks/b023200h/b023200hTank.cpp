#include "b023200hTank.h"
#include "../../TankManager.h"
#include "../../Commons.h"
#include "../../C2DMatrix.h"

//--------------------------------------------------------------------------------------------------

b023200hTank::b023200hTank(SDL_Renderer* renderer, TankSetupDetails details) 
	: BaseTank(renderer, details)
{
	LowPriorBehavior = SEEK;
	astar = new AStar(this, mCollisionMap);

	
	//t1.join();
	//t1.detach();


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

void b023200hTank::findpath()
{
	
	//while (true) {
		//LowPriorBehavior = SEEK;
		//astar = new AStar(this, mCollisionMap);
	//	astar->Tick();
	//	std::this_thread::sleep_for(std::chrono::milliseconds(600));
	//}
	
	
}

void b023200hTank::RunLowPrior(float deltaTime)
{



	float radian = (atan2(y - this->GetCentralPosition().y, x - this->GetCentralPosition().x));
	Vector2D toTarget = Vec2DNormalize( GetCentralPosition() - Vector2D(x,y) );
	double angle = acos(mHeading.Dot(toTarget));
	if (angle != angle)
		angle = 0.0f;

	std::cout << "angle = " << angle << " \n";
	if (LowPriorBehavior != PURSUIT)
	{
		if (Vec2DDistance(Vector2D(x, y), this->GetCentralPosition()) <= 35 ||  angle >  0.1)
			LowPriorBehavior = ARRIVE;
		else
			LowPriorBehavior = SEEK;
	}
	switch (LowPriorBehavior)
	{
	case SEEK:
		//std::cout << "behavior = seek \n"<< " max speed = " << GetMaxSpeed() << "\n";
			mCurrentSpeed -= (kSpeedIncrement  * Vec2DDistance(Vector2D(x, y), this->GetCentralPosition())) * deltaTime;
			if (mCurrentSpeed < -GetMaxSpeed() / 2)
				mCurrentSpeed = -GetMaxSpeed() / 2;
				RotateHeadingToFacePosition(Vector2D(x, y), deltaTime);
				RotateHeadingToFacePosition(Vector2D(x, y), deltaTime);
				RotateHeadingToFacePosition(Vector2D(x, y), deltaTime);
		break;
	case ARRIVE:
		//std::cout << "behavior = arrive \n";
		if (mCurrentSpeed < 0)
			mCurrentSpeed += (Vec2DDistance(Vector2D(x, y), this->GetCentralPosition()) * kSpeedIncrement ) * deltaTime;
		if (mCurrentSpeed > 0)
			mCurrentSpeed = 0;

		RotateHeadingToFacePosition(Vector2D(x, y), deltaTime);
		RotateHeadingToFacePosition(Vector2D(x, y), deltaTime);
		RotateHeadingToFacePosition(Vector2D(x, y), deltaTime);
		break;

	case AVOIDOB:

		break;
	case PURSUIT:
		//std::cout << "behavior = pursuit \n" << " max speed = " << GetMaxSpeed() << "\n";
		if (Vec2DDistance(Vector2D(x, y), this->GetCentralPosition()) <= 35 || angle < 0.95) 
		{
			if (mCurrentSpeed < 0)
				mCurrentSpeed += (kSpeedIncrement  * Vec2DDistance(Vector2D(x, y), this->GetCentralPosition())) * deltaTime;
			if (mCurrentSpeed > 0)
				mCurrentSpeed = 0;
			RotateHeadingToFacePosition(Vector2D(x, y), deltaTime);
			break;
		}
		mCurrentSpeed -= (Vec2DDistance(Vector2D(x, y), this->GetCentralPosition()) * kSpeedIncrement   ) * deltaTime;
		if (mCurrentSpeed < -GetMaxSpeed())
			mCurrentSpeed = -GetMaxSpeed();
		break;
	}
}
void b023200hTank::Update(float deltaTime, SDL_Event e)
{
	BaseTank::Update(deltaTime, e);

	SDL_GetMouseState(&x, &y);
	if (mTanksICanSee.size() > 0)
	{
		TargLastSeen = mTanksICanSee[0]->GetCentralPosition();
	}
	else if (LowPriorBehavior == PURSUIT)
	{
		LowPriorBehavior = SEEK;
	}
	switch (e.type)
	{
		case SDL_MOUSEBUTTONUP:
		{
			switch (e.button.button)
			{
			case SDL_BUTTON_RIGHT:
				LowPriorBehavior = SEEK;
				//astar = new AStar(this, mCollisionMap);
				//astar->Tick();
				break;
			case SDL_BUTTON_LEFT:
				if (LowPriorBehavior != PURSUIT)
					LowPriorBehavior = PURSUIT;
				else
					LowPriorBehavior = SEEK;
				//astar = new AStar(this, mCollisionMap);
				//astar->Tick();

				break;
			}
		}
	break;
	}


	if (x == 0 && y == 0 )
		return;
	RunLowPrior(deltaTime);
	mCurrentSpeed -= 1 * deltaTime;		
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
	int fuck = 30;
	Vector2D right = Vector2D(fuck / 1.3, 0) + GetCentralPosition();
	Vector2D left = Vector2D(-fuck / 1.3, 0) + GetCentralPosition();	
	Vector2D up = Vector2D(0,fuck) + GetCentralPosition();
	Vector2D down = Vector2D(0,-fuck) + GetCentralPosition();
	
	int object = 0;
	for (GameObject* fucks : ObstacleManager::Instance()->GetObstacles())
	{
		if (object == 0)
			continue;
		if (object == 1)
		{
			Vector2D newpos = fucks->GetCentralPosition() + Vector2D(fucks->GetCollisionRadius(), -fucks->GetCollisionRadius());
			Vector2D newpos1 = fucks->GetCentralPosition() + Vector2D(-fucks->GetCollisionRadius(), fucks->GetCollisionRadius());
			Vector2D newpos2 = fucks->GetCentralPosition() + Vector2D(fucks->GetCollisionRadius(), fucks->GetCollisionRadius());
			Vector2D newpos3 = fucks->GetCentralPosition() + Vector2D(-fucks->GetCollisionRadius(), -fucks->GetCollisionRadius());
			DrawDebugLine(newpos3, newpos1, 0, 255, 0);
			DrawDebugLine(newpos1, newpos2, 0, 255, 0);
			DrawDebugLine(newpos2, newpos, 0, 255, 0);
			DrawDebugLine(newpos, newpos3, 0, 255, 0);

		}
		object++;
	
	}
	/*
	if(!shit)
		DrawDebugLine(GetCentralPosition() + Vector2D(fuck / 2,0), right, 0, 255, 0);
	else
		DrawDebugLine(GetCentralPosition() + Vector2D(fuck / 2, 0), right, 255, 0, 0);
	DrawDebugLine(GetCentralPosition() - Vector2D(fuck / 2, 0), left, 0, 255, 0);
	DrawDebugLine(GetCentralPosition() + Vector2D(0, fuck / 2), up, 0, 255, 0);
	DrawDebugLine(GetCentralPosition() - Vector2D(0, fuck / 2), down, 0, 255, 0);

	DrawDebugCircle(Vector2D(x,y), 16, 1, 255, 1);
	*/
	BaseTank::Render();
}