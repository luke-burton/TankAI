//------------------------------------------------------------------------
//  Author: Paul Roberts (2019)
//------------------------------------------------------------------------
#ifndef COMMONS_H
#define COMMONS_H

#include <iostream>
#include "Constants.h"

using namespace::std;

#define WAYPOINTS_VISIBLE
#define AUDIO_VISIBLE 
#define DEBUG_LINES_VISIBLE

//-------------------------------------------------------------------------------------------

const string kTilemapPath			= "XML Data Files/Arena.xml";
const string kScorePath				= "Scores/Scores.txt";
const string kTankPath				= "XML Data Files/TankData.xml";
const string kBulletPath			= "Images/Bullet.png";
const string kRocketPath			= "Images/Rocket.png";
const string kMinePath				= "Images/Mine.png";
const string kCannonPath			= "Images/Cannon.png";
const string kWaypointPath			= "Images/Waypoint.png";
const string kHealthPickUpPath		= "Images/CrateHealth.png";
const string kRocketPickUpPath		= "Images/CrateRockets.png";
const string kBulletPickUpPath		= "Images/CrateBullets.png";
const string kMinePickUpPath		= "Images/CrateMines.png";
const string kExplosionImagePath	= "Images/Explosion.png";

//-------------------------------------------------------------------------------------------

enum Deceleration
{ 
	slow = 3, 
	normal = 2, 
	fast = 1 
};

//-------------------------------------------------------------------------------------------

enum GAMEOBJECT_TYPE
{
	GAMEOBJECT_UNKNOWN,
	GAMEOBJECT_OBSTACLE,
	GAMEOBJECT_OBSTACLE_BORDER,
	GAMEOBJECT_BULLET,
	GAMEOBJECT_MINE,
	GAMEOBJECT_ROCKET,
	GAMEOBJECT_TANK,
	GAMEOBJECT_PICKUP_HEALTH,
	GAMEOBJECT_PICKUP_BULLETS,
	GAMEOBJECT_PICKUP_ROCKETS,
	GAMEOBJECT_PICKUP_MINES
};

//-------------------------------------------------------------------------------------------

enum TANK_TYPE
{
	TANK_UNKNOWN,
	TANK_SMALL,
	TANK_MEDIUM,
	TANK_LARGE
};

//-------------------------------------------------------------------------------------------

enum SCORE_TYPE
{
	SCORE_BULLETHIT,
	SCORE_ROCKETHIT,
	SCORE_MINEHIT,
	SCORE_SURVIVAL,
	SCORE_DESTROYEDTANK,
	SCORE_COLLECTEDPICKUP
};

//-------------------------------------------------------------------------------------------

enum SCREENS
{
	SCREEN_INTRO = 0,
	SCREEN_MENU,
	SCREEN_PLAYGROUND,
	SCREEN_ARENA,
	SCREEN_GAMEOVER,
	SCREEN_HIGHSCORES
};

//-------------------------------------------------------------------------------------------

enum BASE_TANK_STATE
{
	TANKSTATE_IDLE,
	TANKSTATE_DEAD,
	TANKSTATE_MANFIRE,
	TANKSTATE_CANNONFIRE,
	TANKSTATE_DROPMINE,
	
	TANKSTATE_MAX
};

//-------------------------------------------------------------------------------------------

enum TURN_DIRECTION
{
	DIRECTION_UNKNOWN,
	DIRECTION_LEFT,
	DIRECTION_RIGHT
};

//-------------------------------------------------------------------------------------------

enum MOVE_DIRECTION
{
	DIRECTION_NONE,
	DIRECTION_FORWARD,
	DIRECTION_BACKWARD
};

//-------------------------------------------------------------------------------------------

enum STEERING_BEHAVIOUR
{
	STEERING_SEEK,
	STEERING_FLEE,
	STEERING_ARRIVE,
	STEERING_INTRPOSE,
	STEERING_OBSTACLEAVOIDANCE,
	STEERING_PATHFOLLOWING,
	STEERING_WANDER,
	STEERING_PURSUIT,
	STEERING_EVADE,
	STEERING_HIDE
};

//-------------------------------------------------------------------------------------------

enum TileType
{
	TILETYPE_EMPTY,
	TILETYPE_EMPTY_CHECKED,
	TILETYPE_BLOCKED,
	TILETYPE_PATH
};

//-------------------------------------------------------------------------------------------

struct RotatedRect2D
{
	double minX;
	double maxX;
	double minY;
	double maxY;
};

struct Rect2D
{
	double x;
	double y;
	double width;
	double height;

	Rect2D(double initialX, double initialY, double initialWidth, double initialHeight)
	{
		x		= initialX;
		y		= initialY;
		width	= initialWidth;
		height	= initialHeight;
	}

	RotatedRect2D RotateRect(double theta)
	{
		RotatedRect2D rotatedRect;
		double centreX = x + width*0.5f;
		double centreY = y + height*0.5f;

		double cosTheta = cos( theta );
		double sinTheta = sin( theta );

		double heightCosTheta = (height*0.5) * cosTheta;
		double widthCosTheta  = (width*0.5)  * cosTheta;
		double heightSinTheta = (height*0.5) * sinTheta;
		double widthSinTheta  = (width*0.5)  * sinTheta;

		if ( theta > 0 )
		{
			if ( theta < 90.0f )
			{
				// 0 < theta < 90
				rotatedRect.minY = centreY;
				rotatedRect.maxY = centreY + heightCosTheta + widthSinTheta;
				rotatedRect.minX = centreX - heightSinTheta;
				rotatedRect.maxX = centreX + widthCosTheta;
			}
			else
			{
				// 90 <= theta <= 180
				rotatedRect.minY = centreY + heightCosTheta;
				rotatedRect.maxY = centreY + widthSinTheta;
				rotatedRect.minX = centreX - heightSinTheta + widthCosTheta;
				rotatedRect.maxX = centreX;
			}
		}
		else
		{
			if ( theta > -90.0f )
			{
				// -90 < theta <= 0
				rotatedRect.minY = centreY + widthSinTheta;
				rotatedRect.maxY = centreY + heightCosTheta;
				rotatedRect.minX = centreX;
				rotatedRect.maxX = centreX + widthCosTheta - heightSinTheta;
			}
			else
			{
				// -180 <= theta <= -90
				rotatedRect.minY = centreY + widthSinTheta + heightCosTheta;
				rotatedRect.maxY = centreY;
				rotatedRect.minX = centreX + widthCosTheta;
				rotatedRect.maxX = centreX - heightSinTheta;
			}
		}

		return rotatedRect;
	}
};

//compares two real numbers. Returns true if they are equal
inline bool isEqual(float a, float b)
{
	if (fabs(a-b) < 1E-12)
		return true;

	return false;
}

inline bool isEqual(double a, double b)
{
	if (fabs(a-b) < 1E-12)
		return true;
	
	return false;
}

//------------------------------------------------------------------------
//  Vector2d - Author: Mat Buckland
//------------------------------------------------------------------------
#include <windows.h>
#include <limits>

struct Vector2D
{
  double x;
  double y;

  Vector2D():x(0.0),y(0.0){}
  Vector2D(double a, double b):x(a),y(b){}

  //sets x and y to zero
  void Zero(){x=0.0; y=0.0;}

  //returns true if both x and y are zero
  bool isZero()const{return (x*x + y*y) < MinDouble;}

  //returns the length of the vector
  inline double    Length()const;

  //returns the squared length of the vector (thereby avoiding the sqrt)
  inline double    LengthSq()const;

  inline void      Normalize();

  inline double    Dot(const Vector2D& v2)const;

  //returns positive if v2 is clockwise of this vector,
  //negative if anticlockwise (assuming the Y axis is pointing down,
  //X axis to right like a Window app)
  inline int       Sign(const Vector2D& v2)const;

  //returns the vector that is perpendicular to this one.
  inline Vector2D  Perp()const;

  //adjusts x and y so that the length of the vector does not exceed max
  inline void      Truncate(double max);

  //returns the distance between this vector and the one passed as a parameter
  inline double    Distance(const Vector2D &v2)const;

  //squared version of above.
  inline double    DistanceSq(const Vector2D &v2)const;

  inline void      Reflect(const Vector2D& norm);

  //returns the vector that is the reverse of this vector
  inline Vector2D  GetReverse()const;

  //we need some overloaded operators
  const Vector2D& operator+=(const Vector2D &rhs)
  {
    x += rhs.x;
    y += rhs.y;

    return *this;
  }

  const Vector2D& operator-=(const Vector2D &rhs)
  {
    x -= rhs.x;
    y -= rhs.y;

    return *this;
  }

  const Vector2D& operator*(const Vector2D &rhs)
  {
    x *= rhs.x;
    y *= rhs.y;

    return *this;
  }

  const Vector2D& operator*=(const Vector2D &rhs)
  {
    x *= rhs.x;
    y *= rhs.y;

    return *this;
  }

  const Vector2D& operator*=(const double& rhs)
  {
    x *= rhs;
    y *= rhs;

    return *this;
  }

  const Vector2D& operator/=(const double& rhs)
  {
    x /= rhs;
    y /= rhs;

    return *this;
  }

  bool operator==(const Vector2D& rhs)const
  {
    return (isEqual(x, rhs.x) && isEqual(y,rhs.y) );
  }

  bool operator!=(const Vector2D& rhs)const
  {
    return (x != rhs.x) || (y != rhs.y);
  }
  
};

//-----------------------------------------------------------------------some more operator overloads
inline Vector2D operator*(const Vector2D &lhs, double rhs);
inline Vector2D operator*(double lhs, const Vector2D &rhs);
inline Vector2D operator-(const Vector2D &lhs, const Vector2D &rhs);
inline Vector2D operator+(const Vector2D &lhs, const Vector2D &rhs);
inline Vector2D operator/(const Vector2D &lhs, double val);
std::ostream& operator<<(std::ostream& os, const Vector2D& rhs);
std::ifstream& operator>>(std::ifstream& is, Vector2D& lhs);


//------------------------------------------------------------------------member functions

//------------------------- Length ---------------------------------------
//
//  returns the length of a 2D vector
//------------------------------------------------------------------------
inline double Vector2D::Length()const
{
  return sqrt(x * x + y * y);
}


//------------------------- LengthSq -------------------------------------
//
//  returns the squared length of a 2D vector
//------------------------------------------------------------------------
inline double Vector2D::LengthSq()const
{
  return (x * x + y * y);
}


//------------------------- Vec2DDot -------------------------------------
//
//  calculates the dot product
//------------------------------------------------------------------------
inline double Vector2D::Dot(const Vector2D &v2)const
{
  return x*v2.x + y*v2.y;
}

//------------------------ Sign ------------------------------------------
//
//  returns positive if v2 is clockwise of this vector,
//  minus if anticlockwise (Y axis pointing down, X axis to right)
//------------------------------------------------------------------------
enum {clockwise = 1, anticlockwise = -1};

inline int Vector2D::Sign(const Vector2D& v2)const
{
  if (y*v2.x > x*v2.y)
  { 
    return anticlockwise;
  }
  else 
  {
    return clockwise;
  }
}

//------------------------------ Perp ------------------------------------
//
//  Returns a vector perpendicular to this vector
//------------------------------------------------------------------------
inline Vector2D Vector2D::Perp()const
{
  return Vector2D(-y, x);
}

//------------------------------ Distance --------------------------------
//
//  calculates the euclidean distance between two vectors
//------------------------------------------------------------------------
inline double Vector2D::Distance(const Vector2D &v2)const
{
  double ySeparation = v2.y - y;
  double xSeparation = v2.x - x;

  return sqrt(ySeparation*ySeparation + xSeparation*xSeparation);
}


//------------------------------ DistanceSq ------------------------------
//
//  calculates the euclidean distance squared between two vectors 
//------------------------------------------------------------------------
inline double Vector2D::DistanceSq(const Vector2D &v2)const
{
  double ySeparation = v2.y - y;
  double xSeparation = v2.x - x;

  return ySeparation*ySeparation + xSeparation*xSeparation;
}

//----------------------------- Truncate ---------------------------------
//
//  truncates a vector so that its length does not exceed max
//------------------------------------------------------------------------
inline void Vector2D::Truncate(double max)
{
  if (this->Length() > max)
  {
    this->Normalize();

    *this *= max;
  } 
}

//--------------------------- Reflect ------------------------------------
//
//  given a normalized vector this method reflects the vector it
//  is operating upon. (like the path of a ball bouncing off a wall)
//------------------------------------------------------------------------
inline void Vector2D::Reflect(const Vector2D& norm)
{
  *this += 2.0 * this->Dot(norm) * norm.GetReverse();
}

//----------------------- GetReverse ----------------------------------------
//
//  returns the vector that is the reverse of this vector
//------------------------------------------------------------------------
inline Vector2D Vector2D::GetReverse()const
{
  return Vector2D(-this->x, -this->y);
}


//------------------------- Normalize ------------------------------------
//
//  normalizes a 2D Vector
//------------------------------------------------------------------------
inline void Vector2D::Normalize()
{ 
  double vector_length = this->Length();

  if (vector_length > std::numeric_limits<double>::epsilon())
  {
    this->x /= vector_length;
    this->y /= vector_length;
  }
}


//------------------------------------------------------------------------non member functions

inline Vector2D Vec2DNormalize(const Vector2D &v)
{
  Vector2D vec = v;

  double vector_length = vec.Length();

  if (vector_length > std::numeric_limits<double>::epsilon())
  {
    vec.x /= vector_length;
    vec.y /= vector_length;
  }

  return vec;
}


inline double Vec2DDistance(const Vector2D &v1, const Vector2D &v2)
{

  double ySeparation = v2.y - v1.y;
  double xSeparation = v2.x - v1.x;

  return sqrt(ySeparation*ySeparation + xSeparation*xSeparation);
}

inline double Vec2DDistanceSq(const Vector2D &v1, const Vector2D &v2)
{

  double ySeparation = v2.y - v1.y;
  double xSeparation = v2.x - v1.x;

  return ySeparation*ySeparation + xSeparation*xSeparation;
}

inline double Vec2DLength(const Vector2D& v)
{
  return sqrt(v.x*v.x + v.y*v.y);
}

inline double Vec2DLengthSq(const Vector2D& v)
{
  return (v.x*v.x + v.y*v.y);
}


inline Vector2D POINTStoVector(const POINTS& p)
{
  return Vector2D(p.x, p.y);
}

inline Vector2D POINTtoVector(const POINT& p)
{
  return Vector2D((double)p.x, (double)p.y);
}

inline POINTS VectorToPOINTS(const Vector2D& v)
{
  POINTS p;
  p.x = (short)v.x;
  p.y = (short)v.y;

  return p;
}

inline POINT VectorToPOINT(const Vector2D& v)
{
  POINT p;
  p.x = (long)v.x;
  p.y = (long)v.y;

  return p;
}



//------------------------------------------------------------------------operator overloads
inline Vector2D operator*(const Vector2D &lhs, double rhs)
{
  Vector2D result(lhs);
  result *= rhs;
  return result;
}

inline Vector2D operator*(double lhs, const Vector2D &rhs)
{
  Vector2D result(rhs);
  result *= lhs;
  return result;
}

//overload the - operator
inline Vector2D operator-(const Vector2D &lhs, const Vector2D &rhs)
{
  Vector2D result(lhs);
  result.x -= rhs.x;
  result.y -= rhs.y;
  
  return result;
}

//overload the + operator
inline Vector2D operator+(const Vector2D &lhs, const Vector2D &rhs)
{
  Vector2D result(lhs);
  result.x += rhs.x;
  result.y += rhs.y;
  
  return result;
}

//overload the / operator
inline Vector2D operator/(const Vector2D &lhs, double val)
{
  Vector2D result(lhs);
  result.x /= val;
  result.y /= val;

  return result;
}

///////////////////////////////////////////////////////////////////////////////


//treats a window as a toroid
inline void WrapAround(Vector2D &pos, int MaxX, int MaxY)
{
  if (pos.x > MaxX) {pos.x = 0.0;}

  if (pos.x < 0)    {pos.x = (double)MaxX;}

  if (pos.y < 0)    {pos.y = (double)MaxY;}

  if (pos.y > MaxY) {pos.y = 0.0;}
}

//returns true if the point p is not inside the region defined by top_left
//and bot_rgt
inline bool NotInsideRegion(Vector2D p,
                            Vector2D top_left,
                            Vector2D bot_rgt)
{
  return (p.x < top_left.x) || (p.x > bot_rgt.x) || 
         (p.y < top_left.y) || (p.y > bot_rgt.y);
}

inline bool InsideRegion(Vector2D p,
                         Vector2D top_left,
                         Vector2D bot_rgt)
{
  return !((p.x < top_left.x) || (p.x > bot_rgt.x) || 
         (p.y < top_left.y) || (p.y > bot_rgt.y));
}

inline bool InsideRegion(Vector2D p, int left, int top, int right, int bottom)
{
  return !( (p.x < left) || (p.x > right) || (p.y < top) || (p.y > bottom) );
}

//------------------ isSecondInFOVOfFirst -------------------------------------
//
//  returns true if the target position is in the field of view of the entity
//  positioned at posFirst facing in facingFirst
//-----------------------------------------------------------------------------
inline bool isSecondInFOVOfFirst(Vector2D posFirst,
                                 Vector2D facingFirst,
                                 Vector2D posSecond,
                                 double    fov)
{
  Vector2D toTarget = Vec2DNormalize(posSecond - posFirst);

  return facingFirst.Dot(toTarget) >= cos(fov/2.0);
}

//-----------------------------------------------------------------------------

struct TankSetupDetails
{
	std::string StudentName;
	int			TankType;
	Vector2D	StartPosition;
	std::string TankImagePath;
	std::string ManImagePath;
	float		TurnRate;
	int			Health;
	float		Fuel;
	float		Mass;
	float		MaxSpeed;
	int			NumOfBullets;
	int			NumOfRockets;
	int			NumOfMines;
	bool		LeftCannonAttached;
	bool		RightCannonAttached;
	TileType**  CollisionMap;
};

//-----------------------------------------------------------------------------

struct ProjectileSetupDetails
{
	GAMEOBJECT_TYPE GameObjectType;
	std::string		ImagePath;
	Vector2D		StartPosition;
	Vector2D		Direction;
	double			RotationAngle;
	//BaseTank*		Firer;
};

//--------------------------------------------------------------------------------------------------
// Collision functions.
//--------------------------------------------------------------------------------------------------

inline bool Circle(Vector2D pos1, double radius1, Vector2D pos2, double radius2)
{
	Vector2D vec = pos1 - pos2;
	double distance = sqrt((vec.x*vec.x) + (vec.y*vec.y));

	double combinedDistance = (radius1 + radius2);
	return distance < combinedDistance;
}

//--------------------------------------------------------------------------------------------------

inline bool Box(Rect2D rect1, Rect2D rect2)
{
	if(rect1.x > (rect2.x+rect2.width))
		return false;
	else if((rect1.x+rect1.width) < rect2.x)
		return false;
	else if(rect1.y > (rect2.y+rect2.height))
		return false;
	else if((rect1.y+rect1.height) < rect2.y)
		return false;

	return true;
}

//--------------------------------------------------------------------------------------------------

inline bool PointInBox(Vector2D point, Rect2D rect2)
{
	if (point.x > rect2.x &&
		point.x < (rect2.x + rect2.width) &&
		point.y > rect2.y &&
		point.y < (rect2.y + rect2.height))
	{
		return true;
	}

	return false;
}

//--------------------------------------------------------------------------------------------------

#endif //COMMONS_H