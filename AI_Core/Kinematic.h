#ifndef _KINEMATIC_H_
#define _KINEMATIC_H_

#include "Typedefs.h"
#include "Vector2D.h"


//Sturcture that is returned from the kinematic behaviors
//contains the linear Velocity and angular velocity to be applied to the object
struct SteeringOutput
{
public:
	//Linear Velocity
	Vector2D linearVel;
	//Angular Velocity
	float angularVel;

	//overload operator += for SteeringOutput
	SteeringOutput operator +=(SteeringOutput steering)
	{
		this->linearVel += steering.linearVel;
		this->angularVel += steering.angularVel ;
		return *this;
	}
};


//Class Kinematic
//used for simple kinematic behaviors(Seek, Flee, Wander)
class Kinematic
{
private:
	//Current position
	Vector2D position;
	//Current rotation
	Float rotation;
	//Target to seek/flee
	Vector2D target;
	//Max Speed to travel
	Float maxSpeed;
	//How close to target should the object start to slow
	Float slowRadius;
	//angle of rotation to wander
	Float wanderRotation;

	//used to find a random number (0,1)
	Float random();
	//used to find a random number (-1,1)
	Float randomBinomial();

public:
	//Empty constructor
	Kinematic();
	//Destructor
	virtual ~Kinematic();
	//Constuctor
	//parameters: Vector2D, Float, Vector2D, Float, Float, Float)
	//sets parameters to corresponding data members
	Kinematic(Vector2D position,
			  Float rotation,
			  Vector2D target,
			  Float maxSpeed,
		  	  Float slowRadius,
			  Float wanderRadius);
	//Constuctor
	//parameters: Vector2D, Float
	//sets parameters to corresponding data members
	Kinematic(Vector2D position,
			  Float rotation);
	//Constuctor
	//parameters: Vector2D, Vector2D)
	//sets parameters to corresponding data members
	Kinematic(Vector2D position,
			  Vector2D target);
	//Constuctor
	//parameters: Vector2D, Float, Float)
	//sets parameters to corresponding data members
	Kinematic(Vector2D position,
			  Float rotation,
			  Float wanderRadius);

	
	/////////////////////////////////////////////////////////////////////
	//Position()
	//return type: Vector2D
	//parameters : none
	//returns the position
	Vector2D Position();

	//Rotation()
	//return type: float
	//parameters : none
	//returns the rotation
	Float Rotation();

	//Target()
	//return type: Vector2D
	//parameters : none
	//returns the target
	Vector2D Target();

	//MaxSpeed()
	//return type: float
	//parameters : none
	//returns the max speed
	Float MaxSpeed();

	//SlowRadius()
	//return type: float
	//parameters : none
	//return the radius that the object starts to slow
	Float SlowRadius();

	//WanderRotation()
	//return type: float
	//parameters : none
	//returns the rotation of the wander
	Float WanderRotation();


	/////////////////////////////////////////////////////////////////////
	//setPosition()
	//return type: none
	//parameters : Vector2D
	//sets the position
	Void setPosition(Vector2D position);
	
	
	//setRotation()
	//return type: none
	//parameters : float
	//sets the rotation
	Void setRotation(Float rotation);
	
	//setTarget()
	//return type: none
	//parameters : Vector2D
	//sets the target
	Void setTarget(Vector2D target);
	
	//setMaxSpeed()
	//return type: none
	//parameters : float
	//sets the max speed
	Void setMaxSpeed(Float maxSpeed);
	
	//setSlowRadius()
	//return type: none
	//parameters : float
	//sets the radius to start slowing when you seek something
	Void setSlowRadius(Float radius);
	
	//setWanderRotation()
	//return type: none
	//parameters : float
	//sets the angle to wander
	Void setWanderRotation(Float angle);


	/////////////////////////////////////////////////////////////////////
	//Seek()
	//return type: SteeringOutput
	//parameters : none
	//Returns a linear velocity and angular velocity to go from your position to your taget
	SteeringOutput Seek();
	
	//Flee()
	//return type: SteeringOutput
	//parameters : none
	//Returns a linear velocity and angular velocity to get away from your taget
	SteeringOutput Flee();
	
	//Wander()
	//return type: SteeringOutput
	//parameters : none
	//Returns a linear velocity and angular velocity that simulates wandering 
	SteeringOutput Wander();
};


#endif