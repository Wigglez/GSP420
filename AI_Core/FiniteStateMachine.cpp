#include "FiniteStateMachine.h"


Transition::Transition()
{
}

Transition::~Transition()
{
}

Transition::Transition(ActionSequence actions, FSM_State targetState, Condition* condition)
{
	this->actions = actions;
	this->targetState = targetState;
	this->condition = condition;
}

ActionSequence Transition::getActions()
{
	return actions;
}


FSM_State Transition::getTargetState()
{
	return targetState;
}


Bool Transition::isTriggered()
{
	return condition->test();
}




FSM_State::FSM_State()
{

}

FSM_State::~FSM_State()
{
}

FSM_State::FSM_State( ActionSequence entryAction, ActionSequence exitAction, ActionSequence actions, list<Transition> transition)
{
	this->entryAction = entryAction;
	this->exitAction = exitAction;
	this->actions = actions;
	this->transitions = transitions;
}

ActionSequence FSM_State::getAction()
{
	return actions;
}

ActionSequence FSM_State::getEntryAction()
{
	return entryAction;
}

ActionSequence FSM_State::getExitAction()
{
	return exitAction;
}

Void FSM_State::getTransition(list<Transition> transitions)
{
	transitions = this->transitions;
}


FiniteStateMachine::FiniteStateMachine()
{
}


FiniteStateMachine::FiniteStateMachine(FSM_State initialState)
{
	this->initialState = initialState;
	currentState = this->initialState;
}

ActionSequence FiniteStateMachine::update()
{
	ActionSequence returnActions;

	list<Transition>::iterator transItr;
	list<Transition> currentTrans; 
	currentState.getTransition(currentTrans);

	for(transItr = currentTrans.begin(); transItr != currentTrans.end(); transItr++)
	{
		if(transItr->isTriggered())
		{
			returnActions = currentState.getExitAction();

			currentState = transItr->getTargetState();

			returnActions.addAction(currentState.getEntryAction());

			return returnActions;
		}
	}


	return currentState.getAction();
	
}