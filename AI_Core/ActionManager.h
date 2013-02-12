#ifndef _ACTIONMANAGER_H_
#define _ACTIONMANAGER_H_

#include "Typedefs.h"


//Abstract class: Action
//actions are various things that can be done in the game
class Action
{
protected:
	//Priority of the action
	Float priority ;
	//bool of if the action is complete
	Bool complete;
	//bool if this action is interruptable(false by default)
	Bool interrupt;
public:
	//isComplete()
	//return type: bool
	//parameters : none
	//Must override this function, determines when the action is complete
	virtual Bool isComplete() = 0;

	//canInterrupt()
	//return type: bool
	//parameters : none
	//returns if this action can be interrupetd
	Bool canInterrupt();

	//act()
	//return type: none
	//parameters : none
	//Must override this function, this is the actual actions that happens(such as play animations and/or sounds)
	virtual Void act() = 0;

	//getPriority()
	//return type: float
	//parameters : none
	//reutrns the priority of the action
	Float getPriority();
};


//Class ActionSequence
//Holds multiple actions to execute in order
class ActionSequence
{
private:
	//holds all the actions for this sequence
	list<Action*> actions;
	//Current action in the sequence
	uInt currentIndex;
public:
	//empty constructors
	ActionSequence();
	//Destructor
	virtual ~ActionSequence();
	//Constructor that takes a list of actions to start with
	ActionSequence(list<Action*> actions);

	//canInterrupt()
	//return type: bool
	//parameters : none
	//tests if any of the actions can be interrupted, if one action can be then the entire sequence can be interrupted
	Bool canInterrupt();

	//isComplete()
	//return type: bool
	//parameters : none
	//tests to see if all the actions in the sequence is complete
	Bool isComplete();

	//execute()
	//return type: none
	//parameters : none
	//Goes through each action and performs one individually
	Void execute();

	//addAction()
	//return type: none
	//parameters : Action*
	//adds an individual action to the sequence
	Void addAction(Action* action);

	//addAction()
	//return type: none
	//parameters : ActionSequence
	//adds a sperate action sequence to the this sequence
	Void addAction(ActionSequence actions);

	//getAction()
	//return type: none
	//parameters : list<Action*>
	//Takes in a list of actions and fills it out with its current actions
	Void getActions(list<Action*>* actions);
};



#endif