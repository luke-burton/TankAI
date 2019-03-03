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

	if (ishit || ishit1 || ishit2 || ishit3)
	{
		if (LowPriorBehavior == SEEK || LowPriorBehavior == ARRIVE || LowPriorBehavior == PURSUIT)
			LowPriorBehavior = AVOIDOB;

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
		if (Vec2DDistance(Vector2D(x, y), this->GetCentralPosition()) <= 35 ||  angle >  0.1)
			LowPriorBehavior = ARRIVE;
		else
			LowPriorBehavior = SEEK;
	}
	//std::cout << "type = " << ishit << " : " << ishit1 << " : " << ishit2 << " : " << ishit3 << " BEHAVIOR =  " << LowPriorBehavior << " \n";
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
	//	std::cout << "behavior = arrive \n";
		if (mCurrentSpeed < 0)
			mCurrentSpeed += (Vec2DDistance(Vector2D(x, y), this->GetCentralPosition()) * kSpeedIncrement ) * deltaTime;
		if (mCurrentSpeed > 0)
			mCurrentSpeed = 0;

		RotateHeadingToFacePosition(Vector2D(x, y), deltaTime);
		RotateHeadingToFacePosition(Vector2D(x, y), deltaTime);
		RotateHeadingToFacePosition(Vector2D(x, y), deltaTime);
		break;

	case AVOIDOB:
		//std::cout << "behavior = cunt \n" << " max speed = " << GetMaxSpeed() << "\n";
		if (ishit)
		{
			float xy = GetCentralPosition().x;
			float  yx = GetCentralPosition().y + (Force.y * 50);
			
				
			radian = (atan2(yx - this->GetCentralPosition().y, xy - this->GetCentralPosition().x));
			 toTarget = Vec2DNormalize(GetCentralPosition() - Vector2D(xy, yx));
			 angle = acos(mHeading.Dot(toTarget));
			if (angle != angle)
				angle = 0.0f;

			if (Vec2DDistance(Vector2D(xy, yx), this->GetCentralPosition()) <= 35 || angle > 0.1)
			{
				if (mCurrentSpeed < 0)
				{ 
					mCurrentSpeed += (kSpeedIncrement  * Vec2DDistance(Vector2D(xy, yx), this->GetCentralPosition())) * deltaTime;
					mCurrentSpeed += (kSpeedIncrement  * Vec2DDistance(Vector2D(xy, yx), this->GetCentralPosition())) * deltaTime;
					mCurrentSpeed += (kSpeedIncrement  * Vec2DDistance(Vector2D(xy, yx), this->GetCentralPosition())) * deltaTime;
					mCurrentSpeed += (kSpeedIncrement  * Vec2DDistance(Vector2D(xy, yx), this->GetCentralPosition())) * deltaTime;
					mCurrentSpeed += (kSpeedIncrement  * Vec2DDistance(Vector2D(xy, yx), this->GetCentralPosition())) * deltaTime;
				}
				if (mCurrentSpeed > 0)
					mCurrentSpeed = 0;
				RotateHeadingToFacePosition(Vector2D(xy, yx), deltaTime);
				break;
			}
			mCurrentSpeed -= (kSpeedIncrement  * Vec2DDistance(Vector2D(xy, yx), this->GetCentralPosition())) * deltaTime;
			if (mCurrentSpeed < -GetMaxSpeed() / 2)
				mCurrentSpeed = -GetMaxSpeed() / 2;
		}
		break;
	case PURSUIT:
	//	std::cout << "behavior = pursuit \n" << " max speed = " << GetMaxSpeed() << "\n";
		if (Vec2DDistance(Vector2D(x, y), this->GetCentralPosition()) <= 35 || angle > 0.1) 
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
				break;
			}
		}
	break;
	}


	if (x == 0 && y == 0 )
		return;
	RunLowPrior(deltaTime);
	//mCurrentSpeed -= 1 * deltaTime;		
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

	if (right.y > TopLeft.y && right.y < BotLeft.y && right.x < TopLeft.x + 30 && right.x > TopLeft.x - 30)
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

	if (left.y > TopLeft.y && left.y < BotLeft.y && left.x < TopLeft.x + 30 && left.x > TopLeft.x - 30)
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
void b023200hTank::Render()
{
	Force = Vector2D(0, 0);
	int feelerlength = 40;
	int walllength = 10;
	Vector2D right = Vector2D(feelerlength / 1.3, 0) + GetCentralPosition();
	Vector2D left = Vector2D(-feelerlength / 1.3, 0) + GetCentralPosition();
	Vector2D up = Vector2D(0, feelerlength) + GetCentralPosition();
	Vector2D down = Vector2D(0,-feelerlength) + GetCentralPosition();
	 ishit = false;
	 ishit1 = false;
	 ishit2 = false;
	 ishit3 = false;
	for(int i = 0; i < corners.size() ; i+= 4)
	{
	Vector2D newpos = corners[i];
	Vector2D newpos1 = corners[i + 1];
	Vector2D newpos2 = corners[i + 2];
	Vector2D newpos3 = corners[i + 3];
	if (ShouldAvoidL(newpos, newpos2, up,down,left,right))
		ishit = true;
	if (ShouldAvoidR(newpos1, newpos3, up,down,left,right))
		ishit1 = true;


	continue;

	if (right.x > newpos2.x && right.x < newpos2.x + 60)
		ishit = true;
	//std::cout << up.y   << " : " << newpos.y << " : " << (up.y > newpos.y) << " : " <<(up.y < newpos.y + 30) << " : "  <<  "\n";
	if (left.x > newpos1.x - 60 && left.x < newpos1.x )
		ishit1 = true;
	if (up.y > newpos.y && up.y < newpos.y + 60  )
		ishit2 = true;	
	if (down.y > newpos2.y - 60 && down.y < newpos2.y  )
		ishit3 = true;

	continue;
	/* TOP OF COLLISIONS*/
	DrawDebugLine(newpos - Vector2D(0, walllength), newpos1 - Vector2D(0, walllength), 255, 255, 255);//top line from left corner to right corner
	DrawDebugLine(newpos - Vector2D(0,-walllength), newpos1 - Vector2D(0, -walllength), 255, 255, 255); // bot line from left corner to right corner
	/* TOP OF COLLISIONS END*/

	/* BOT OF COLLISIONS*/
	
	DrawDebugLine(newpos2 - Vector2D(0, walllength), newpos3 - Vector2D(0, walllength), 255, 255, 255);
	DrawDebugLine(newpos2 - Vector2D(0, -walllength), newpos3 - Vector2D(0, -walllength), 255, 255, 255);
	/* BOT OF COLLISIONS END*/
	/* left side of collision start*/
	DrawDebugLine(newpos - Vector2D(0, walllength), newpos2 - Vector2D(0, -walllength), 255, 255, 255);
	DrawDebugLine(newpos - Vector2D(-walllength, walllength), newpos2 - Vector2D(-walllength, -walllength), 255, 255, 255);
	/* left side of collision end*/
	/* right side of collision start*/
	DrawDebugLine(newpos1 - Vector2D(0, walllength), newpos3 - Vector2D(0, -walllength), 255, 255, 255);
	DrawDebugLine(newpos1 - Vector2D(walllength, walllength), newpos3 - Vector2D(walllength, -walllength), 255, 255, 255);
	/* right side of collision end*/
	}
	std::cout << "force = " << Force.y << "\n";
	if (Force.y != 0)
		y += 50 *  Force.y;
	DrawDebugCircle( Vector2D(x, y), 16, 255, 0, 0);
	if (ishit)
	{
		
		DrawDebugLine(GetCentralPosition() + Vector2D(feelerlength / 2, 0), right, 255, 0, 0);
	}
	else
		DrawDebugLine(GetCentralPosition() + Vector2D(feelerlength / 2, 0), right, 0, 255, 0);
	if (ishit1)
		DrawDebugLine(GetCentralPosition() - Vector2D(feelerlength / 2, 0), left, 255, 0, 0);
	else
		DrawDebugLine(GetCentralPosition() - Vector2D(feelerlength / 2, 0), left, 0, 255, 0);
	if (ishit2)
		DrawDebugLine(GetCentralPosition() + Vector2D(0, feelerlength / 2), up, 255, 0, 0);
	else
		DrawDebugLine(GetCentralPosition() + Vector2D(0, feelerlength / 2), up, 0, 255, 0);
	if (ishit3)
		DrawDebugLine(GetCentralPosition() - Vector2D(0, feelerlength / 2), down, 255, 0, 0);
	else
		DrawDebugLine(GetCentralPosition() - Vector2D(0, feelerlength / 2), down, 0, 255, 0);
	BaseTank::Render();
}