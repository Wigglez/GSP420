#include "DecisionTree.h"


DecisionTreeNode* Decision::makeDecision()
{
	DecisionTreeNode* branch = getBranch();
	if(branch!= NULL)
	{
		return branch->makeDecision();
	}
	else
	{
		return NULL;
	}
}


DecisionTreeNode* MultiDecision::makeDecision()
{
	DecisionTreeNode* branch = getBranch();
	if(branch!= NULL)
	{
		return branch->makeDecision();
	}
	else
	{
		return NULL;
	}
}


DecisionTreeNode* DecisionTreeAction::makeDecision()
{
	return this;
}


