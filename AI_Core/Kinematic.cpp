#include "Kinematic.h"
#include <time.h>

Kinematic::Kinematic()
{
	position.Zero_Vector();
	rotation = 0;
	target.Zero_Vector();
	maxSpeed = 0;
	slowRadius = 0;
	wanderRotation = 0;
}

Kinematic::~Kinematic()
{
}

Kinematic::Kinematic(Vector2D position, Float rotation, Vector2D target, Float maxSpeed, Float slowRadius, Float wanderRadius)
{
	this->position = position;
	this->rotation = rotation;
	this->target = target;
	this->maxSpeed = maxSpeed;
	this->slowRadius = slowRadius;
	this->wanderRotation = wanderRadius;
}


Kinematic::Kinematic(Vector2D position, Float rotation)
{
	this->position = position;
	this->rotation = rotation;

	target.Zero_Vector();
	maxSpeed = 1;
	slowRadius = 0;
	wanderRotation = 0;
}

Kinematic::Kinematic(Vector2D position, Vector2D target)
{
	this->position = position;
	this->target = target;

	rotation = 0;
	target.Zero_Vector();
	maxSpeed = 1;
	slowRadius = 0;
	wanderRotation = 0;
}


Kinematic::Kinematic(Vector2D position, Float rotation, Float wanderRadius)
{
	this->position = position;
	this->rotation = rotation;
	this->wanderRotation = wanderRadius;

	target.Zero_Vector();
	maxSpeed = 1;
	slowRadius = 0;
}


Vector2D Kinematic::Position()
{
	return position;
}

Float Kinematic::Rotation()
{
	return rotation;
}

Vector2D Kinematic::Target()
{
	return target;
}

Float Kinematic::MaxSpeed()
{
	return maxSpeed;
}

Float Kinematic::SlowRadius()
{
	return slowRadius;
}

Float Kinematic::WanderRotation()
{
	return wanderRotation;
}



Void Kinematic::setPosition(Vector2D position)
{
	this->position = position;
}

Void Kinematic::setRotation(Float rotation)
{
	this->rotation = rotation;
}

Void Kinematic::setTarget(Vector2D target)	
{
	this->target = target;
}

Void Kinematic::setMaxSpeed(Float maxSpeed)
{
	this->maxSpeed = maxSpeed;
}

Void Kinematic::setSlowRadius(Float radius)
{
	this->slowRadius = slowRadius;
}

Void Kinematic::setWanderRotation(Float radius)
{
	this->wanderRotation = wanderRotation;
}



SteeringOutput Kinematic::Seek()
{
	//Create structure for output
	SteeringOutput steering;

	// Get direction of target
	steering.linearVel = target - position;

	// Set velocity 
	if(steering.linearVel.Length() == 0)
	{
		steering.linearVel = 0;
	}
	else if(steering.linearVel.Length() < slowRadius)
	{
		steering.linearVel.Normalize();
		steering.linearVel *= maxSpeed/2.0f;
	} 
	else
	{
		steering.linearVel.Normalize();
		steering.linearVel *= maxSpeed;
	}

	// Face in the direction we want to move
	rotation = atan2(-steering.linearVel[0], steering.linearVel[1]);

	// Output the steering
	steering.angularVel = 0;

	return steering;
}

SteeringOutput Kinematic::Flee()
{
	//Create structure for output
	SteeringOutput steering;

	// Get direction of target
	steering.linearVel = position - target;

	// Set velocity 
		steering.linearVel.Normalize();
		steering.linearVel *= maxSpeed;
	

	// Face in the direction we want to move
	rotation = -1*atan2(-steering.linearVel[0], steering.linearVel[1]);

	// Output the steering
	steering.angularVel = 0;

	return steering;
}

SteeringOutput Kinematic::Wander()
{
	//Create structure for output
	SteeringOutput steering;
	
	// gets the character orientation as a vector
	Vector2D charOrientation;
	charOrientation[0] = sin(rotation);
	charOrientation[1] = cos(rotation);

	//Get velocity from the vector form of the orientation
	steering.linearVel = maxSpeed * charOrientation;

	// Change orientation randomly
	steering.angularVel = randomBinomial() * wanderRotation;

	// Output steering
	return steering;
}


//Returns a number between -1 and 1
float Kinematic::randomBinomial()
{

	return random() - random();
}

// Returns a number between 0 and 1
float Kinematic::random()
{
		srand((unsigned int)time(NULL));
		int a  = rand();
		srand((unsigned int)time(NULL));
		int b  = rand();

		float c;
		if( a > b)
			c = (float)b/(float)a;
		else
			c = (float)a/(float)b;

		return c;
}