#include "ActionManager.h"


Bool Action::canInterrupt()
{
	return false;
}

Float Action::getPriority()
{
	if(priority == NULL)
		priority = 0;

	return priority;
}


ActionSequence::ActionSequence()
{
}

ActionSequence::~ActionSequence()
{
}

ActionSequence::ActionSequence(list<Action*> actions)
{
	this->actions = actions;
}

Bool ActionSequence::canInterrupt()
{
	list<Action*>::iterator actionItr;

	for(actionItr = actions.begin(); actionItr != actions.end(); actionItr++)
	{
		//If any of the actions can be intereupted then the sequence can
		if((*actionItr)->canInterrupt())
		{
			return true;
		}
	}
	
	return false;
}

Bool ActionSequence::isComplete()
{
	list<Action*>::iterator actionItr;

	for(actionItr = actions.begin(); actionItr != actions.end(); actionItr++)
	{
		//If one of the actions is not complete then the sequence is not complete
		if(!(*actionItr)->isComplete())
		{
			return false;
		}
	}

	return true;
}


Void ActionSequence::execute()
{
	list<Action*>::iterator actionItr;

	for(actionItr = actions.begin(); actionItr != actions.end(); actionItr++)
	{
		(*actionItr)->act();
	}
}

Void ActionSequence::addAction(Action* action)
{
	Bool higherPriority = false;

	for(list<Action*>::iterator actItr = actions.begin(); actItr != actions.end(); actItr++)
	{
		if(action->getPriority() > (*actItr)->getPriority())
		{
			actions.insert(actItr, action);
			higherPriority = true;
			break;
		}
	}

	if(!higherPriority)
	{
		actions.push_back(action);
	}
}

Void ActionSequence::addAction(ActionSequence actions)
{
	list<Action*> newActions;

	actions.getActions(&newActions);

	list<Action*>::iterator actionItr;

	for(actionItr = newActions.begin(); actionItr != newActions.end(); actionItr++)
	{
		this->addAction((*actionItr));
	}

}

Void ActionSequence::getActions(list<Action*> *actions)
{
	*actions = this->actions;
}


