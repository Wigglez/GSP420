#ifndef _GRAPH_H_
#define _GRAPH_H_
#include "Typedefs.h"


class Connection;
struct NodeRecord;

//Individual Nodes that make up a graph
class PathNode
{
private:
	Bool open;
	list<Connection*> connections;
public:
	//constructor
	//return type: none
	//parameters : none
	PathNode();

	//isOpen()
	//return type: Bool
	//parameters : none
	//returns a bool of whether or not this node is currently open
	Bool isOpen();

	//addConnection()
	//return type: Void
	//parameters : PathNode*, Float
	//creates a connections between this and another node with the specified cost, if the connection already exists it skips over it
	Void addConnection(PathNode* toNode, Float cost);

	//getConnections()
	//return type: Void
	//parameters : list<Connection*> *
	//fills out the list of connections that it was passed with all connections from this node
	Void getConnections(list<Connection*> * connections);

	//closeNode()
	//return type: Void
	//parameters : none
	//Closes this node
	Void closeNode();

	//openNode()
	//return type: Void
	//parameters : none
	//Opens this node
	Void openNode();

	//Overloading boolean operaters 
	Bool operator==(PathNode node);
	Bool operator!=(PathNode node);
};


class Connection
{
private:
	PathNode* toNode;
	PathNode* fromNode;
	Float cost;
	Connection();
public:
	
	//Constructor
	//return type: Void
	//parameters : PathNode*, PathNode*, Float
	//creates a connection from fromNode to toNode with the specified cost
	Connection( PathNode* toNode, PathNode* fromNode, Float cost);

	//getCost()
	//return type: Float
	//parameters : none
	//returns the cost of this connection
	Float getCost();

	//getToNode()
	//return type: PathNode*
	//parameters : none
	//Returns the toNode from this connection
	PathNode* getToNode();

	//getFromNode()
	//return type: PathNode*
	//parameters : none
	//Returns the fromNode from this connection
	PathNode* getFromNode();
};



class Graph
{
private:
	list<PathNode*> nodes;

	PathNode* currentNode;

public:
	//Graph()
	//return type: none
	//parameters : none
	//constructor
	Graph();

	//addNode()
	//return type: Void
	//parameters : PathNode*
	//Adds a node the graph
	Void addNode(PathNode* node);

	//traverse()
	//return type: Bool
	//parameters : PathNode*, PathNode*, list<Connection*>*
	//Using the dijkstra algorithm, finds a path of lowest cost from start to end and fills out path with connections if a path was found
	//Will return true if a path was found, false if it wasn't
	Bool traverse(PathNode* start, PathNode* end, list<Connection*>* path);
};


#endif