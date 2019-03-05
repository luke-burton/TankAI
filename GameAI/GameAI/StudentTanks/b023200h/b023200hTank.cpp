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


	for (GameObject* wall : ObstacleManager::Instance()->GetObstacles())
	{

		Vector2D newpos = wall->GetCentralPosition() - Vector2D(wall->GetAdjustedBoundingBox().width / 2, wall->GetAdjustedBoundingBox().height / 2);
		corners.push_back(newpos);	
		Vector2D newpos1 = wall->GetCentralPosition() - Vector2D(-wall->GetAdjustedBoundingBox().width / 2, wall->GetAdjustedBoundingBox().height / 2);
		corners.push_back(newpos1);
		Vector2D newpos2 = wall->GetCentralPosition() + Vector2D(-wall->GetAdjustedBoundingBox().width / 2, wall->GetAdjustedBoundingBox().height / 2);
		corners.push_back(newpos2);
		Vector2D newpos3 = wall->GetCentralPosition() + Vector2D(wall->GetAdjustedBoundingBox().width / 2, wall->GetAdjustedBoundingBox().height / 2);
		corners.push_back(newpos3);
		//Vector2D newpos1 = wall->GetCentralPosition() - Vector2D(-wall->GetAdjustedBoundingBox().width / 2, wall->GetAdjustedBoundingBox().height / 2);
		//Vector2D newpos2 = wall->GetCentralPosition() + Vector2D(-wall->GetAdjustedBoundingBox().width / 2, wall->GetAdjustedBoundingBox().height / 2);
		////Vector2D newpos3 = wall->GetCentralPosition() + Vector2D(wall->GetAdjustedBoundingBox().width / 2, wall->GetAdjustedBoundingBox().height / 2);
	



	}
	LowPriorBehavior = PURSUIT;
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
void b023200hTank::RunHighPrior(float deltaTime)
{
	BaseTank* target = mTanksICanSee[0];

	float radian = (atan2(y - this->GetCentralPosition().y, x - this->GetCentralPosition().x));
	Vector2D toTarget = Vec2DNormalize(GetCentralPosition() - Vector2D(x, y));
	double angle = acos(mHeading.Dot(toTarget));
	if (angle != angle)
		angle = 0.0f;
	if (target->GetCentralPosition().Distance(this->GetCentralPosition()) > 35)
	{
		if (Vec2DDistance(Vector2D(x, y), this->GetCentralPosition()) <= 35 || angle >  0.25)
		{
			if (mCurrentSpeed < 0)
				mCurrentSpeed += (Vec2DDistance(Vector2D(x, y), this->GetCentralPosition()) * kSpeedIncrement) * deltaTime;
			if (mCurrentSpeed > 0)
				mCurrentSpeed = 0;

			RotateHeadingToFacePosition(Vector2D(x, y), deltaTime);
			RotateHeadingToFacePosition(Vector2D(x, y), deltaTime);
			RotateHeadingToFacePosition(Vector2D(x, y), deltaTime);
			if (angle < 0.05)
				FireRockets();
		//	RotateHeadingToFacePosition(Vector2D(x, y), deltaTime);
			return;
		}
		mCurrentSpeed -= (kSpeedIncrement  * Vec2DDistance(Vector2D(x, y), this->GetCentralPosition())) * deltaTime;
		if (mCurrentSpeed < -GetMaxSpeed() /2 )
			mCurrentSpeed = -GetMaxSpeed() / 2;

		RotateHeadingToFacePosition(Vector2D(x, y), deltaTime);
		RotateHeadingToFacePosition(Vector2D(x, y), deltaTime);
		RotateHeadingToFacePosition(Vector2D(x, y), deltaTime);
		toTarget = mTanksICanSee[0]->GetCentralPosition() - GetCentralPosition();
		toTarget.Normalize();
		double dot = toTarget.Dot(mManFireDirection);
		if (angle < 0.05)
			FireRockets();
		float angle = atan2(mManFireDirection.y - toTarget.y, mManFireDirection.x - toTarget.x) * 90 / Pi;

			angle = 3;

		RotateManByRadian(kManTurnRate, -angle, 1);
		FireABullet();
		
	}
	else
	{
		if (mCurrentSpeed < 0)
			mCurrentSpeed += (Vec2DDistance(Vector2D(x, y), this->GetCentralPosition()) * kSpeedIncrement) * deltaTime;
	
		mCurrentSpeed -=  kSpeedIncrement / 50 * deltaTime;
		mCurrentSpeed +=  kSpeedIncrement / 100 * deltaTime;
		if (mCurrentSpeed > 0)
			mCurrentSpeed = 0;
		x = target->GetCentralPosition().x;
		y = target->GetCentralPosition().y;
		float radian = (atan2(y - this->GetCentralPosition().y, x - this->GetCentralPosition().x));
		Vector2D toTarget = Vec2DNormalize(GetCentralPosition() - Vector2D(x, y));
		double angle = acos(mHeading.Dot(toTarget));
		if (angle != angle)
			angle = 0.0f;
		//DropAMine();
		
		FireRockets();
		if (angle > 0.01)
		{
			RotateHeadingToFacePosition(Vector2D(x, y), deltaTime);
			RotateHeadingToFacePosition(Vector2D(x, y), deltaTime);
			RotateHeadingToFacePosition(Vector2D(x, y), deltaTime);
		}
		

		toTarget = mTanksICanSee[0]->GetCentralPosition() - GetCentralPosition();
		toTarget.Normalize();
		double dot = toTarget.Dot(mManFireDirection);
		 angle = atan2(mManFireDirection.y - toTarget.y, mManFireDirection.x - toTarget.x) * 90 / Pi;
		if (angle > 0)
			angle = 3;
	
		RotateManByRadian(kManTurnRate, -angle, 1);


	}

}
void b023200hTank::RunLowPrior(float deltaTime)
{
	
	if (ishit || ishit1 || ishit2 || ishit3 || ishit4 || ishit5 )
	{
		 
		//if (LowPriorBehavior == SEEK || LowPriorBehavior == ARRIVE || LowPriorBehavior == PURSUIT)
		//	LowPriorBehavior = AVOIDOB;

	}
	else if (LowPriorBehavior == AVOIDOB)
		LowPriorBehavior = SEEK;

	
	float radian = (atan2(y - this->GetCentralPosition().y, x - this->GetCentralPosition().x));
	Vector2D toTarget = Vec2DNormalize( GetCentralPosition() - Vector2D(x,y) );
	double angle = acos(mHeading.Dot(toTarget));
	if (angle != angle)
		angle = 0.0f;


	if (LowPriorBehavior != PURSUIT && LowPriorBehavior != AVOIDOB)
	{
		if (Vec2DDistance(Vector2D(x, y), this->GetCentralPosition()) <= 35 || angle > 0.15 )
			LowPriorBehavior = ARRIVE;
		else
			LowPriorBehavior = SEEK;
	}
	std::cout << " angle = " << angle << " dist = " << (Vec2DDistance(Vector2D(x, y), this->GetCentralPosition())) << "\n";
	//std::cout << "type = " << ishit << " : " << ishit1 << " : " << ishit2 << " : " << ishit3 << " BEHAVIOR =  " << LowPriorBehavior << " \n";
	switch (LowPriorBehavior)
	{
	case SEEK:
		std::cout << "behavior = seek \n"<< " max speed = " << GetMaxSpeed() << "\n";
			mCurrentSpeed -= (kSpeedIncrement  * Vec2DDistance(Vector2D(x, y), this->GetCentralPosition())) * deltaTime;
			if (mCurrentSpeed < -GetMaxSpeed() / 2)
				mCurrentSpeed = -GetMaxSpeed() / 2;
			
				RotateHeadingToFacePosition(Vector2D(x, y), deltaTime);
				RotateHeadingToFacePosition(Vector2D(x, y), deltaTime);
		break;
	case ARRIVE:
		std::cout << "behavior = arrive \n";
		if (mCurrentSpeed < 0)
			mCurrentSpeed += (Vec2DDistance(Vector2D(x, y), this->GetCentralPosition()) * kSpeedIncrement ) * deltaTime;
		if (mCurrentSpeed > 0)
			mCurrentSpeed = 0;

		RotateHeadingToFacePosition(Vector2D(x, y), deltaTime);
		RotateHeadingToFacePosition(Vector2D(x, y), deltaTime);
		RotateHeadingToFacePosition(Vector2D(x, y), deltaTime);
		break;

	case AVOIDOB:
		if (!ishit6 && ishit2 && ishit4)
		{
		
			xy = GetCentralPosition().x;
			yx = GetCentralPosition().y + 10;
		}
		else if (!ishit7 && ishit3 && ishit5)
		{
			xy = GetCentralPosition().x;
			yx = GetCentralPosition().y - 10;
		}
		else if (ishit )
		{
			xy = GetCentralPosition().x ;
			yx = GetCentralPosition().y + (Force.y * 10);
		}
		else if (ishit1)
		{
			xy = GetCentralPosition().x ;
			yx = GetCentralPosition().y + (Force.y *  10);
		}
		else if (ishit2 )
		{
			xy = GetCentralPosition().x + (Force.y * 10);
			yx = GetCentralPosition().y ;
		}
		else if (ishit3 )
		{
			xy = GetCentralPosition().x + (Force.y * 10);
			yx = GetCentralPosition().y  ;
		}else if (ishit4 )
		{	
			xy = GetCentralPosition().x + (Force.y * 10);
			yx = GetCentralPosition().y ;
		}else if (ishit5 )
		{
			xy = GetCentralPosition().x + (Force.y * 10);
			yx = GetCentralPosition().y  ;
		}
			radian = (atan2(yx - this->GetCentralPosition().y, xy - this->GetCentralPosition().x));
			 toTarget = Vec2DNormalize(GetCentralPosition() - Vector2D(xy, yx));
			 angle = acos(mHeading.Dot(toTarget));
			if (angle != angle)
				angle = 0.0f;

			if ( angle > 0.1)
			{
				if (mCurrentSpeed < 0)
				{ 
					mCurrentSpeed += (Vec2DDistance(Vector2D(xy, yx), this->GetCentralPosition()) * kSpeedIncrement) * deltaTime;
				
					
				}
				if (mCurrentSpeed > 0)
					mCurrentSpeed = 0;
		
				RotateHeadingToFacePosition(Vector2D(xy, yx), deltaTime);
			//	RotateHeadingToFacePosition(Vector2D(xy, yx), deltaTime);
			//	RotateHeadingToFacePosition(Vector2D(xy, yx), deltaTime);
		
				break;
			}
			else
			{
				mCurrentSpeed -= (kSpeedIncrement  / Vec2DDistance(Vector2D(xy, yx), this->GetCentralPosition())) * deltaTime;
				if (mCurrentSpeed < -GetMaxSpeed() / 4)
					mCurrentSpeed = -GetMaxSpeed() / 4;
				RotateHeadingToFacePosition(Vector2D(xy, yx), deltaTime);
				//RotateHeadingToFacePosition(Vector2D(xy, yx), deltaTime);
				//RotateHeadingToFacePosition(Vector2D(xy, yx), deltaTime);
			}
		
		break;
	case PURSUIT:
	
		break;
	}
}
void b023200hTank::Update(float deltaTime, SDL_Event e)
{
	BaseTank::Update(deltaTime, e);
	


	SDL_GetMouseState(&x, &y);
	//std::cout << "x = " << x << " y = " << y << "\n";
	if (mTanksICanSee.size() > 0)
	{
		LowPriorBehavior = PURSUIT;
		TargLastSeen = mTanksICanSee[0]->GetCentralPosition();
	}
	else if (LowPriorBehavior == PURSUIT)
	{
		LowPriorBehavior = SEEK;
	}
	astar = new AStar(this, mCollisionMap);
	astar->Tick();
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
		if (astar->finalpath.back().position.Distance(GetCentralPosition()) <= 35)
			astar->finalpath.pop_back();
		if (astar->finalpath.size() > 0)
		{
			x = astar->finalpath.back().position.x;
			y = astar->finalpath.back().position.y;
		}
	}
	if (x == 0 && y == 0 )
		return;
	if (LowPriorBehavior == PURSUIT && mTanksICanSee.size() > 0)
	{ 
		RunHighPrior(deltaTime);
	
		return;
	}
	
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

bool b023200hTank::ShouldAvoidL(Vector2D TopLeft, Vector2D BotLeft, Vector2D up, Vector2D down, Vector2D left, Vector2D right)
{

	if (right.y > TopLeft.y && right.y < BotLeft.y && right.x < TopLeft.x + 55 && right.x > TopLeft.x - 25)
	{
		if (x <= BotLeft.x)
			return false;
		if (Vector2D(x, y).Distance(TopLeft) < Vector2D(x, y).Distance(BotLeft))
			Force.y = -1;
		else
			Force.y = 1;

		return true;
	}
	else
		return false;
}
bool b023200hTank::ShouldAvoidR(Vector2D TopLeft, Vector2D BotLeft, Vector2D up, Vector2D down, Vector2D left, Vector2D right)
{

	if (left.y > TopLeft.y && left.y < BotLeft.y && left.x < TopLeft.x + 25  && left.x > TopLeft.x - 55)
	{
		if (x >= BotLeft.x)
			return false;
		if (Vector2D(x, y).Distance(TopLeft) < Vector2D(x, y).Distance(BotLeft))
			Force.y = -1;
		else
			Force.y = 1;
		
		return true;
	}
		
	else
		return false;
}
bool b023200hTank::ShouldAvoidU(Vector2D TopLeft, Vector2D BotLeft, Vector2D up, Vector2D down, Vector2D left, Vector2D right)
{

	if (up.x > TopLeft.x && up.x < BotLeft.x && left.y < TopLeft.y + 15  && left.y > TopLeft.y - 45)
	{
		if (GetCentralPosition().y > BotLeft.y)
			return false;
		if (y <= TopLeft.y)
			return false;
		if (Vector2D(x, y).Distance(TopLeft) < Vector2D(x, y).Distance(BotLeft))
			Force.y = -1;
		else
			Force.y = 1;
		
		return true;
	}
		
	else
		return false;
}


bool b023200hTank::ShouldAvoidD(Vector2D TopLeft, Vector2D BotLeft, Vector2D up, Vector2D down, Vector2D left, Vector2D right)
{

	if (up.x > TopLeft.x && up.x < BotLeft.x && left.y < TopLeft.y + 45 && left.y > TopLeft.y - 15)
	{
		if (GetCentralPosition().y < BotLeft.y)
			return false;
		if (y >= BotLeft.y)
			return false;
		if (Vector2D(x, y).Distance(TopLeft) < Vector2D(x, y).Distance(BotLeft))
			Force.y = -1;
		else
			Force.y = 1;

		return true;
	}

	else
		return false;
}

void b023200hTank::Render()
{
	if (astar->finalpath.size() > 0)
	{
		DrawDebugCircle(astar->finalpath.back().position, 16, 255, 255, 155);
	}
	DrawDebugCircle(GetCentralPosition(), GetHealth(), 255, 255, 255);
	BaseTank::Render();
	return;//render disabled here
	//std::cout << " health = " << GetHealth() << " \n";
	Force = Vector2D(0, 0);
	int feelerlength = 50;
	int walllength = 10;
	Vector2D right = Vector2D(feelerlength  , 0) + GetCentralPosition();
	Vector2D left = Vector2D(-feelerlength , 0) + GetCentralPosition();
	Vector2D up = Vector2D(right.x - GetCentralPosition().x + 5, feelerlength - 10) + GetCentralPosition();
	Vector2D down = Vector2D(right.x - GetCentralPosition().x + 5,-feelerlength + 10) + GetCentralPosition();
	Vector2D up1 = Vector2D(left.x - GetCentralPosition().x - 5, feelerlength - 10) + GetCentralPosition();
	Vector2D down1 = Vector2D(left.x - GetCentralPosition().x - 5,-feelerlength + 10) + GetCentralPosition();
	Vector2D up2 = Vector2D(0 ,feelerlength + 10) + GetCentralPosition();
	Vector2D down2 = Vector2D(0 ,-feelerlength + 10) + GetCentralPosition();
	 ishit = false;
	 ishit1 = false;
	 ishit2 = false;
	 ishit3 = false;
	 ishit4 = false;
	 ishit5 = false;
	 ishit6 = false;
	 ishit7 = false;
	for(int i = 0; i < corners.size() ; i+= 4)
	{

		continue;///TODO OBS AVOIDANCE IS DISABLED HERE
	Vector2D newpos = corners[i];
	Vector2D newpos1 = corners[i + 1];
	Vector2D newpos2 = corners[i + 2];
	Vector2D newpos3 = corners[i + 3];
	if (ShouldAvoidL(newpos, newpos2, up,down,left,right))
		ishit = true;
	if (ShouldAvoidR(newpos1, newpos3, up,down,left,right))
		ishit1 = true;
	if (ShouldAvoidU(newpos, newpos1, up,down,left,right))
		ishit2 = true;
	if (ShouldAvoidD(newpos2, newpos3, up,down,left,right))
		ishit3 = true;
	if (ShouldAvoidU(newpos, newpos1, up1,down1,left,right))
		ishit4 = true;
	if (ShouldAvoidD(newpos2, newpos3, up1,down1,left,right))
		ishit5 = true;
	
	if (ShouldAvoidU(newpos, newpos1, up2,up,left,right))
		ishit6 = true;
	if (ShouldAvoidD(newpos2, newpos3, down2, down2,left,right))
		ishit7 = true;

	
	continue;

	}
	DrawDebugCircle(Vector2D(xy, yx), 16, 0, 255, 255);
	DrawDebugCircle(Vector2D(x, y), 16, 255, 0, 0);
	DrawDebugLine(GetCentralPosition(), up2, 255, 255, 255);
	if(ishit7)
		DrawDebugLine(GetCentralPosition(), down2, 255, 255, 255);
	else
		DrawDebugLine(GetCentralPosition(), down2, 255, 0, 255);
	if (ishit)
		DrawDebugLine(GetCentralPosition() , right, 255, 0, 0);
	else
		DrawDebugLine(GetCentralPosition() , right, 0, 255, 0);
	if (ishit1)
		DrawDebugLine(GetCentralPosition() , left, 255, 0, 0);
	else
		DrawDebugLine(GetCentralPosition() , left, 0, 255, 0);
	if (ishit2)
		DrawDebugLine(GetCentralPosition(), up, 255, 0, 0);
	else
		DrawDebugLine(GetCentralPosition() , up, 0, 255, 0);
	if (ishit3)
		DrawDebugLine(GetCentralPosition(), down, 255, 0, 0);
	else
		DrawDebugLine(GetCentralPosition() , down, 0, 255, 0);
	
	if (ishit4)
		DrawDebugLine(GetCentralPosition(), up1, 255, 0, 0);
	else
		DrawDebugLine(GetCentralPosition() , up1, 0, 255, 0);
	if (ishit5)
		DrawDebugLine(GetCentralPosition() , down1, 255, 0, 0);
	else
		DrawDebugLine(GetCentralPosition() , down1, 0, 255, 0);
	BaseTank::Render();
}