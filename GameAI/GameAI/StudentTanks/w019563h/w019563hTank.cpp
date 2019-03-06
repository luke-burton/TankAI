#include "w019563hTank.h"
#include "../../TankManager.h"
#include "../../Commons.h"
#include "../../C2DMatrix.h"

//--------------------------------------------------------------------------------------------------

w019563hTank::w019563hTank(SDL_Renderer* renderer, TankSetupDetails details)
	: BaseTank(renderer, details)
{
	for (int x = 0; x < kMapWidth; x++)
	{
		for (int y = 0; y < kMapHeight; y++)
		{
			mMap[x][y] = mCollisionMap[x][y];
		}
	}
	canfire = false;
}

//--------------------------------------------------------------------------------------------------

w019563hTank::~w019563hTank()
{
	
}

//--------------------------------------------------------------------------------------------------

void w019563hTank::ChangeState(BASE_TANK_STATE newState)
{
	BaseTank::ChangeState(newState);
}

//--------------------------------------------------------------------------------------------------

void w019563hTank::Update(float deltaTime, SDL_Event e)
{
	bool apressed;
	SDL_GetMouseState(&mx, &my);
	
	if (mTanksICanSee.size() != NULL)
	{
		
	}
	float tempdistance = 9999;

	SearchForTanks();
	
	if (Tanks.size() != NULL)
	{
		Attack(Tanks.front());
	}

	int xx = GetCentralPosition().x;
	int yy = GetCentralPosition().y;

	SetupNodes(mx, my, xx, yy);

	if (IsBlocked(Vector2D(mx, my)) == true)
	{
		if (Tanks.size() != NULL)
		{
			mx = Tanks.front().Position.x;
			my = Tanks.front().Position.y;
		}
	}

	FindBlocking();

	if (IsBlocked(Vector2D(mx, my)) == false)
	{
		DoPathFinding(StartNode, EndNode);
	}

	if (AtLocation(deltaTime, StartNode.Position, EndNode.Position) == false && IsBlocked(Vector2D(mx, my)) == false)
	{
		if (FinalList.size() != NULL)
		{
			MoveToLocation(deltaTime, FinalList.back().Position);
			FinalList.pop_back();
		}
	}

	else
	{
		mCurrentSpeed = 0;
	}

	cout << GetHealth() << endl;

	//cout << "distance" << Vec2DDistance(StartNode.Position, EndNode.Position) + Vec2DDistance(StartNode.Position, StartNode.Position) << endl;
	BaseTank::Update(deltaTime, e);
}


//--------------------------------------------------------------------------------------------------

void w019563hTank::MoveInHeadingDirection(float deltaTime)
{
	//Get the force that propels in current heading.
	Vector2D force = (mHeading*mCurrentSpeed)-mVelocity;

	//Acceleration = Force/Mass
	Vector2D acceleration = force/GetMass();

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

void w019563hTank::MoveToLocation(float deltaTime, Vector2D location)
{
	RotateHeadingToFacePosition(location, deltaTime);
	RotateHeadingToFacePosition(location, deltaTime);
	RotateHeadingToFacePosition(location, deltaTime);

	Vector2D toTarget = Vec2DNormalize(GetCentralPosition() - location);

	double angle = acos(mHeading.Dot(toTarget));

	if (angle != angle)
		angle = 0.0f;


	//if (angle < 0.2)
	{
		mCurrentSpeed += -kSpeedIncrement * deltaTime;//(-Vec2DDistance(GetCentralPosition(),location)+15);
		//cout << mCurrentSpeed << endl;
	}
	//else
	{
		//mCurrentSpeed = 0;
	}
	if (mCurrentSpeed < -GetMaxSpeed())
		mCurrentSpeed = -GetMaxSpeed();
	//cout << angle << endl;
}

bool w019563hTank::AtLocation(float deltaTime, Vector2D TankPosition, Vector2D EndPosition)
{
	if ((TankPosition.x / kTileDimensions) == (EndPosition.x / kTileDimensions) && (TankPosition.y / kTileDimensions) == (EndPosition.y / kTileDimensions))
	{
		//cout << "AtPosition" << endl;
		return true;
	}
	else
	{
		return false;
	}
}

void w019563hTank::Avoid(float deltaTime, Vector2D ObsticalPosition, Vector2D EndPosition)
{
	Vector2D toTarget = Vec2DNormalize(GetCentralPosition() - EndPosition);

	double angle = acos(mHeading.Dot(toTarget));

	if (angle != angle)
		angle = 0.0f;

	if (angle > 0 && angle < 1.8)
	{
		MoveToLocation(deltaTime,Vector2D(StartNode.Position.x,StartNode.Position.y + 1000));
	}
	else
	{
		MoveToLocation(deltaTime, Vector2D(StartNode.Position.x, StartNode.Position.y - 1000));
	}
	if (mCurrentSpeed < -GetMaxSpeed())
		mCurrentSpeed = -GetMaxSpeed();


	//RotateHeadingToFacePosition(location, 10);
}

void w019563hTank::DoPathFinding(Node start, Node end)
{
	reset();
	//ClosedList.push_back(start);
	CheckTiles(start);
	for (int i = 0; i < 100; i++)
	{
		CheckTiles(FindBestTile());
		if (FoundPath)
			break;
	} 
	FinalList.push_back(end);
	int path = ClosedList.back().thisnode;
	//cout << "path=" << path << endl;
	for (int i = 0; i < path; i++)
	{
		MakePath(FinalList.back().Position);
	}
	//cout << "PathLength" << ClosedList.back().thisnode << endl;
}

bool w019563hTank::IsBlocked(Vector2D TileLocation)
{
	int xpos = TileLocation.x / 32;
	int ypos = TileLocation.y / 32;
	if (mMap[xpos][ypos] == TILETYPE_BLOCKED)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool w019563hTank::IsChecked(Vector2D TileLocation)
{
	int xpos = TileLocation.x / kTileDimensions;
	int ypos = TileLocation.y / kTileDimensions;
	if (mMap[xpos][ypos] == TILETYPE_EMPTY_CHECKED)
	{
		return true;
	}
	else
	{
		return false;
	}
	return false;
}

bool w019563hTank::DangerousTile(Vector2D TileLocation)
{
	int xpos = TileLocation.x / kTileDimensions;
	int ypos = TileLocation.y / kTileDimensions;
	if (mMap[xpos][ypos] == TILETYPE_PATH)
	{
		return true;
	}
	else
	{
		return false;
	}
	return false;
}

void w019563hTank::Attack(Tank TankToAttack)
{
	//cout << canfire << endl;
	//if (canfire == false)
	{
		mx = Center(mTanksICanSee.front()->GetCentralPosition() + (mTanksICanSee.front()->GetHeading() * 32)).x;
		my = Center(mTanksICanSee.front()->GetCentralPosition() + (mTanksICanSee.front()->GetHeading() * 32)).y;
		
		Vector2D toTarget = Vec2DNormalize(GetCentralPosition() - mTanksICanSee.front()->GetCentralPosition());

		double angle = acos(mHeading.Dot(toTarget));

		if (angle != angle)
			angle = 0.0f;

		if (angle < 0.4)
		{
			FireRockets();
		}
		Vector2D toTarget2 = mTanksICanSee[0]->GetCentralPosition() - GetCentralPosition();
		toTarget2.Normalize();
		double dot = toTarget2.Dot(mManFireDirection);
		if (dot < 0.99f)
			RotateManByRadian(kManTurnRate, -3, 1);
		else
		{
			FireABullet();
		}
		//DropAMine();
		//if (AtLocation(1, GetCentralPosition(), Vector2D(mx, my)) == true)
		{
			//canfire == true;
		}
	}

}

void w019563hTank::FindTanks()
{
}


void w019563hTank::SetupNodes(int mx, int my, int xx, int yy)
{
	StartNode.Position.x = (((xx / kTileDimensions)*kTileDimensions) + kHalfTileDimensions);
	StartNode.Position.y = (((yy / kTileDimensions)*kTileDimensions) + kHalfTileDimensions);
	StartNode.GridPosition.x = StartNode.Position.x / kMapWidth;
	StartNode.GridPosition.y = StartNode.Position.y / kMapHeight;
	StartNode.thisnode = 0;

	EndNode.Position.x = (((mx / kTileDimensions)*kTileDimensions) + kHalfTileDimensions);
	EndNode.Position.y = (((my / kTileDimensions)*kTileDimensions) + kHalfTileDimensions);
	EndNode.GridPosition.x = EndNode.Position.x / kMapWidth;
	EndNode.GridPosition.y = EndNode.Position.y / kMapHeight;
}

void w019563hTank::CheckTiles(Node Start)
{
	int xpos = Start.Position.x / kTileDimensions;
	int ypos = Start.Position.y / kTileDimensions;
	mMap[xpos][ypos] = TILETYPE_EMPTY_CHECKED;
	if (IsBlocked(Vector2D(Start.Position.x+32, Start.Position.y)) != true && IsChecked(Vector2D(Start.Position.x + 32, Start.Position.y)) != true)// && DangerousTile(Vector2D(Start.Position.x + 32, Start.Position.y)) != true)
	{
		CurrentNode.Position.x = Start.Position.x+32;
		CurrentNode.Position.y = Start.Position.y;
		{
			CurrentNode.f = Vec2DDistance(CurrentNode.Position, EndNode.Position); -Vec2DDistance(CurrentNode.Position, StartNode.Position) / 2;
		}
		CurrentNode.lastnode = Start.thisnode;
		CurrentNode.thisnode = Start.thisnode + 1;
		OpenList.push_back(CurrentNode);
	}
	if (IsBlocked(Vector2D(Start.Position.x-32, Start.Position.y)) != true && IsChecked(Vector2D(Start.Position.x - 32, Start.Position.y)) != true)// && DangerousTile(Vector2D(Start.Position.x - 32, Start.Position.y)) != true)
	{
		CurrentNode.Position.x = Start.Position.x-32;
		CurrentNode.Position.y = Start.Position.y;
		{
			CurrentNode.f = Vec2DDistance(CurrentNode.Position, EndNode.Position); -Vec2DDistance(CurrentNode.Position, StartNode.Position) / 2;
		}
		CurrentNode.lastnode = Start.thisnode;
		CurrentNode.thisnode = Start.thisnode + 1;
		OpenList.push_back(CurrentNode);
	}
	if (IsBlocked(Vector2D(Start.Position.x, Start.Position.y+32)) != true && IsChecked(Vector2D(Start.Position.x, Start.Position.y + 32)) != true)// && DangerousTile(Vector2D(Start.Position.x, Start.Position.y + 32)) != true)
	{
		CurrentNode.Position.x = Start.Position.x;
		CurrentNode.Position.y = Start.Position.y+32;
		{
			CurrentNode.f = Vec2DDistance(CurrentNode.Position, EndNode.Position); -Vec2DDistance(CurrentNode.Position, StartNode.Position) / 2;
		}
		CurrentNode.lastnode = Start.thisnode;
		CurrentNode.thisnode = Start.thisnode + 1;
		OpenList.push_back(CurrentNode);
	}
	if (IsBlocked(Vector2D(Start.Position.x, Start.Position.y-32)) != true && IsChecked(Vector2D(Start.Position.x, Start.Position.y - 32)) != true)// && DangerousTile(Vector2D(Start.Position.x, Start.Position.y - 32)) != true)
	{
		CurrentNode.Position.x = Start.Position.x;
		CurrentNode.Position.y = Start.Position.y-32;
		{
			CurrentNode.f = Vec2DDistance(CurrentNode.Position, EndNode.Position); -Vec2DDistance(CurrentNode.Position, StartNode.Position) / 2;
		}
		CurrentNode.lastnode = Start.thisnode;
		CurrentNode.thisnode = Start.thisnode + 1;
		OpenList.push_back(CurrentNode);
	}
}

Node w019563hTank::FindBestTile()
{
	int ff = 99999;
	for (int i = 0; i < OpenList.size(); i++)
	{
		if (ff > OpenList[i].f)
		{
			ff = OpenList[i].f;
			CurrentNode = OpenList[i];
			NodeToDelete = i;
		}
	}
	//cout << OpenList.size() << endl;
	if (NodeToDelete != NULL)
		OpenList.erase(OpenList.begin() + NodeToDelete);
	ClosedList.push_back(CurrentNode);
	if (AtLocation(0, CurrentNode.Position, EndNode.Position) == true)
	{
		FoundPath = true;
	}
	else
	{
		FoundPath = false;
	}
	return CurrentNode;
}

void w019563hTank::MakePath(Vector2D EndPosition)
{
	int ff = FinalList.back().thisnode;
	int dist = 9999;

	for (int i = 0; i < ClosedList.size(); i++)
	{
		if (ClosedList[i].thisnode == ff-1)
		{
			NodesToSearch.push_back(ClosedList[i]);
			ClosedList.erase(ClosedList.begin() + i);
			//cout << "doing" << endl;
		}
	}
	for (int m = 0; m < NodesToSearch.size(); m++)
	{
		if (dist > Vec2DDistance(NodesToSearch[m].Position, EndPosition))
		{
			CurrentNode = NodesToSearch[m];
		}
	}
	FinalList.push_back(CurrentNode);
}


void w019563hTank::reset()
{
	for (int x = 0; x < kMapWidth; x++)
	{
		for (int y = 0; y < kMapHeight; y++)
		{
			if (mMap[x][y] == TILETYPE_EMPTY_CHECKED)
			{
				mMap[x][y] = TILETYPE_EMPTY;
			}
			if (mMap[x][y] == TILETYPE_PATH)
			{
				mMap[x][y] = TILETYPE_EMPTY;
			}
		}
	}
	FinalList.clear();
	OpenList.clear();
	ClosedList.clear();
	NodesToSearch.clear();
}
void w019563hTank::SearchForTanks()
{
	Tanks.clear();
	if (mTanksICanSee.size() != NULL)
	{
		//for (int i = 32; i < 192; i += 32)
		//{
			//EnemyFovTiles.push_back(Center(mTanksICanSee.front()->GetCentralPosition() + (mTanksICanSee.front()->GetHeading()*-i)));
		//}
		for (int i = 0; i < mTanksICanSee.size(); i++)
		{
			CurrentTank.Position = mTanksICanSee[i]->GetCentralPosition();
			CurrentTank.Strength = 9999;
			CurrentTank.Heading = mTanksICanSee[i]->GetHeading();
			CurrentTank.Side = mTanksICanSee[i]->GetSide();
			Tanks.push_back(CurrentTank);
		}
	}
}
void w019563hTank::FindBlocking()
{
	if (EnemyFovTiles.size() != NULL);
	{
		for (int i = 0; i < EnemyFovTiles.size(); i++)
		{
			int x = EnemyFovTiles[i].x / kTileDimensions;
			int y = EnemyFovTiles[i].y / kTileDimensions;
			//mMap[x][y] = TILETYPE_PATH;
		}
	}
}
Vector2D w019563hTank::Center(Vector2D position)
{	
	Vector2D pos;
	int xx = position.x;
	int yy = position.y;
	pos.x = (((xx / kTileDimensions)*kTileDimensions) + kHalfTileDimensions);
	pos.y = (((yy / kTileDimensions)*kTileDimensions) + kHalfTileDimensions);
	return pos;
}
//--------------------------------------------------------------------------------------------------

void w019563hTank::RotateHeadingByRadian(double radian, int sign)
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

void w019563hTank::Render()
{

	DrawDebugCircle(GetCentralPosition(), GetHealth(), 255, 255, 255);
	/*
	//DrawDebugCircle(StartNode.Position, 16, 255, 0, 0);
	if (IsBlocked(EndNode.Position) == false)
		//DrawDebugCircle(EndNode.Position, 16, 255, 0, 0);
	//cout << ClosedList.size() << endl;;
	for (int i = 0; i < OpenList.size(); i++)
	{
		//DrawDebugCircle(OpenList[i].Position, 16, 0, 0, 255);
	}
	for (int i = 0; i < ClosedList.size(); i++)
	{
		//DrawDebugCircle(ClosedList[i].Position, 16, 255, 255, 0);
	}
	for (int i = 0; i < FinalList.size(); i++)
	{
		//DrawDebugCircle(FinalList[i].Position, 16, 255, 255, 255);
	}
	//if (mTanksICanSee.size() != NULL)
	{
		//DrawDebugCircle(Center(GetCentralPosition()+(GetHeading()*-100)),16, 255, 0, 0);
		for (int i = 32; i < 512; i += 32)
		{
			//DrawDebugCircle(Center(GetCentralPosition() + (GetHeading()*-i)), 16, 255, 0, 0);
		}
	}
	//DrawDebugCircle(Center(((CurrentTank.Position)+ (CurrentTank.Heading*64)) + ((CurrentTank.Side)*64)), 16, 255, 255, 255);
	//DrawDebugCircle(Center(((CurrentTank.Position) + (CurrentTank.Heading*64)) + ((CurrentTank.Side) * -64)), 16, 255, 255, 255);
	*/
	EnemyFovTiles.clear();
	Tanks.clear();
	BaseTank::Render();
}
//--------------------------------------------------------------------------------------------------