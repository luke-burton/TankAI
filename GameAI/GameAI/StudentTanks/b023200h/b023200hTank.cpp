#include "b023200hTank.h"
#include "../../TankManager.h"
#include "../../Commons.h"
#include "../../C2DMatrix.h"

//--------------------------------------------------------------------------------------------------

b023200hTank::b023200hTank(SDL_Renderer* renderer, TankSetupDetails details) 
	: BaseTank(renderer, details)
{
	astar = new AStar(this, mCollisionMap);
	//t1 = thread(&b023200hTank::findpath, this);
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
	return;
	while (true) {
		astar = new AStar(this, mCollisionMap);
		astar->Tick();
		std::this_thread::sleep_for(std::chrono::milliseconds(600));
	}
	
	
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
		break;
	case ARRIVE:
		//std::cout << "behavior = arrive \n";
		if (mCurrentSpeed < 0)
			mCurrentSpeed += (Vec2DDistance(Vector2D(x, y), this->GetCentralPosition()) * kSpeedIncrement ) * deltaTime;
		if (mCurrentSpeed > 0)
			mCurrentSpeed = 0;

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
				astar = new AStar(this, mCollisionMap);
				astar->Tick();
				break;
			case SDL_BUTTON_LEFT:
				if (LowPriorBehavior != PURSUIT)
					LowPriorBehavior = PURSUIT;
				else
					LowPriorBehavior = SEEK;
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
		if (Vec2DDistance(Vector2D(x, y), this->GetCentralPosition()) <= 34)
		{
			astar->finalpath.pop_back();
		}
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
	
	if (astar->finalpath.size() > 0)
	{
		DrawDebugLine(this->GetCentralPosition(), astar->finalpath.back().position, 255, 1, 1);
	}
	for (ANode node : astar->finalpath)
	{

		DrawDebugCircle(node.position, 35, 1, 255, 1);
		DrawDebugCircle(node.position, 16, 255, 1, 1);
	}
	
	BaseTank::Render();
}