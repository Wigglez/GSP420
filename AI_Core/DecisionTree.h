#ifndef _DECISIONTREE_H_
#define _DECISIONTREE_H_
#include "Typedefs.h"

//Abstract Class DecisionTreeNode
//can inherit from this class to create decisions
class DecisionTreeNode
{
public:
	//Makes a decision and decides on what branch to go to
	virtual DecisionTreeNode* makeDecision() = 0;
};

//Abstract Class Decision
//inherit from this to make a binary decision
class Decision: public DecisionTreeNode
{
protected:
	//branch to go to if condition is true
	DecisionTreeNode* trueNode;
	//branch to go to if condition if false
	DecisionTreeNode* falseNode;
public:
	//Must overide this function
	//Determins which branch to go to
	virtual DecisionTreeNode* getBranch() = 0;

	//makes decision and travels to next branch of tree
	DecisionTreeNode* makeDecision();
};


//Abstact class MultiDecision
//holds multiple branches to possibly travel to
class MultiDecision : public DecisionTreeNode
{
protected:
	//A list of possible branches to go to
	list<DecisionTreeNode*> branches;
public:
	//Must overide this function
	//Determins which branch to go to
	virtual DecisionTreeNode* getBranch() = 0;

	//makes decision and travels to next branch of tree
	DecisionTreeNode* makeDecision();
};


//Class DecisionTreeAction
//this is at the end of the branches and is the outcome of the decisions
class DecisionTreeAction : public DecisionTreeNode
{
public:
	DecisionTreeNode* makeDecision();
};

#endif