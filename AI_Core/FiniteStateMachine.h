#ifndef _FINITESTATEMACHINE_H_
#define _FINITESTATEMACHINE_H_

#include "Typedefs.h"
#include "ActionManager.h"


class FSM_State;
class Transition;


//Abstract class : Condition
//Used to check whether it is triggered
//Override test() with specific conditions to be tested for
class Condition
{
public:
	virtual Bool test() = 0;
};


//Class: FSM_State
//State for the Finite State Machine to manage
class FSM_State
{
private:
	///List of transistions to other states
	list<Transition> transitions;
	//Actions to be executed when this state is entered
	ActionSequence entryAction;
	//Actions to be executed when this state is exited
	ActionSequence exitAction;
	//Actions to be executed when this state is currently active
	ActionSequence actions;

public:
	//Empty constructor
	FSM_State();
	//Destructor
	virtual ~FSM_State();

	//Constructor that takes 3 action sequences and list of type Transition
	//Takes in and sets entry actions, exit actions, and active actions and 
	//a list of transitions to other states 
	FSM_State(  ActionSequence entryAction,
				ActionSequence exitAction,
				ActionSequence actions,
				list<Transition> transition);
	
	//getActions()
	//return type: ActionSequence
	//parameters : none
	//returns the active actions of this state
	ActionSequence getAction();

	//getEntryAction()
	//return type: ActionSequence
	//parameters : none
	//returns the entry actions of this state
	ActionSequence getEntryAction();

	//getExitAction()
	//return type: ActionSequence
	//parameters : none
	//returns the exit actions of this state
	ActionSequence getExitAction();

	//getTransition()
	//return type: Void
	//parameters : list<Transition> 
	//takes in a list and fills it out with the state's transitions
	Void getTransition(list<Transition> transitions);

};

//Class Transition
//This is uesd to flag the state machine to change states
class Transition
{
protected:
	//Actions to be executed when this transition is triggered
	ActionSequence actions;
	//State to go to when this transition is triggered
	FSM_State targetState;
	//Test condition to determine if the transition is triggerd
	Condition* condition;
public:
	//Empty constructor
	Transition();
	//Destructor
	~Transition();

	//Constructor that takes in ActionSequence of actions, a FSM_State for the taget state and Condition* for its condition
	Transition(ActionSequence actions, FSM_State targetState, Condition* condition);
	
	//getActions()
	//return type: ActionSequence
	//parameters : none
	//returns actions
	ActionSequence getActions();

	//getTargetState()
	//return type: FSM_State
	//parameters : none
	//returns target state to transition to if this transition is triggered
	FSM_State getTargetState();

	//isTriggered()
	//return type: Bool
	//parameters : none
	//tests whether or not this transition's condition is met
	Bool isTriggered();
};


//Class FiniteStateMachine
//this is used to manage states and transition between them
class FiniteStateMachine
{
private:
	//initial state when the Finite state Machine gets created
	FSM_State initialState;
	//Holds the current state the finite state machine is in
	FSM_State currentState;
	//Empty constructor
	FiniteStateMachine();
public:	
	//Consturctor
	//parameter: FSM_State initial state
	//initialized state machine with an initialState
	FiniteStateMachine(FSM_State initialState);

	//update()
	//return type: actionSequence
	//parameters : none
	//checks the if the state should change and returns an ActionSequence of actions to execute
	ActionSequence update();

	//getCurrentState()
	//return type: FSM_State
	//parameters : none
	//returns the current state of the Finite State Machine
	FSM_State getCurrentState();
};

#endif