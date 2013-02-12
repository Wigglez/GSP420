#include "Graph.h"

PathNode::PathNode()
{
	open = true;
	connections.clear();
}

Bool PathNode::isOpen()
{
	return open;
	this;
}

Void PathNode::addConnection(PathNode* toNode, Float cost)
{
	Bool newConnection = true;

	for(list<Connection*>::iterator conItr = connections.begin(); conItr != connections.end(); conItr++)
	{
		if(toNode == (*conItr)->getToNode())
			newConnection = false;
	}
	
	if(newConnection)
	{
		Connection* newC = new Connection(toNode, this, cost);

		connections.push_back(newC);

		toNode->addConnection(this, cost);
		
	}
}

Void PathNode::getConnections(list<Connection*> * outConnections)
{
	*outConnections = connections;
}

Void PathNode::closeNode()
{
	open = false;
}

Void PathNode::openNode()
{
	open = true;
}

Bool PathNode::operator==(PathNode node)
{
	list<Connection*> otherConnections;
	node.getConnections(&otherConnections);
	if(this->connections == otherConnections)
	{
		return true;
	}
	else{
		return false;
	}
}

Bool PathNode::operator!=(PathNode node)
{
	list<Connection*> otherConnections;
	node.getConnections(&otherConnections);
	if(this->connections == otherConnections)
	{
		return false;
	}
	else{
		return true;
	}
}


Connection::Connection( PathNode* toNode, PathNode* fromNode, Float cost)
{
	this->toNode = toNode;
	this->fromNode = fromNode;
	this->cost = cost;
}

Float Connection::getCost()
{
	return cost;
}

PathNode* Connection::getToNode()
{
	return toNode;
}

PathNode* Connection::getFromNode()
{
	return fromNode;
}


Graph::Graph()
{
	nodes.clear();
	currentNode = NULL;
}

Void Graph::addNode(PathNode* node)
{
	if(nodes.empty())
	{
		currentNode = node;
	}

	nodes.push_back(node);
}


Bool Graph::traverse(PathNode* start, PathNode* end, list<Connection*>* path)
{
	//Make sure there is nothing in the path
	path->clear();
	//Open up all nodes in this graph
	for(list<PathNode*>::iterator nodItr = nodes.begin(); nodItr != nodes.end(); nodItr++)
	{
		(*nodItr)->openNode();
	}

	//structure to keep track of info for each node
	struct NodeRecord
	{
		PathNode* node;
		Connection* connection;
		NodeRecord* prevNode;
		Float costSoFar;

		Bool operator==(NodeRecord otherRecord)
		{
			if(this->node == otherRecord.node
				&& this->connection == otherRecord.connection
				&& this->costSoFar == otherRecord.costSoFar)
				return true;
			else
				return false;
		}

	};

	//Initialize record for start node
	NodeRecord startNode;
	startNode.node = start;
	startNode.connection = NULL;
	startNode.costSoFar = 0;

	//Create open and closed lists of node records
	list<NodeRecord> open;
	list<NodeRecord> closed;

	//Put the start node in the open list
	open.push_back(startNode);
	
	//Keep track of the current node
	NodeRecord current;
	while( open.size() > 0)
	{
		
		
		//find the lowest cost so far in the open set of nodes
		Float lowestCost = FLT_MAX;
		for(list<NodeRecord>::iterator nodeItr = open.begin(); nodeItr != open.end(); nodeItr++)
		{
			if(nodeItr->costSoFar < lowestCost)
			{
				lowestCost = nodeItr->costSoFar;
				current = *nodeItr;
			}
		}

		//If this is the end Node break
		if(current.node == end)
		{
			break;
		}

		//to store the connectinos of the currentNode
		list<Connection*> connections;
		//Fill out connections
		current.node->getConnections(&connections);

		//for each connection of this node...
		for(list<Connection*>::iterator conItr = connections.begin(); conItr != connections.end(); conItr++)
		{
			//get the toNode
			PathNode* endNode = (*conItr)->getToNode();
			//Figure the cost so far for this connection
			Float endNodeCost = current.costSoFar + (*conItr)->getCost();

			//If this node is closed, skip over it
			if(!endNode->isOpen())
			{
				continue;
			}

			//check if this node is in the open set and is a worse route
			Bool worseRoute = false;
			Bool alreadyInOpen = false;
			for(list<NodeRecord>::iterator nodeItr = open.begin(); nodeItr != open.end(); nodeItr++)
			{
				if(nodeItr->node == endNode && nodeItr->connection == *conItr)
				{
					alreadyInOpen = true;

					if(nodeItr->costSoFar < endNodeCost || nodeItr->costSoFar == endNodeCost)
					{
						worseRoute = true;
						break;
					}
				}
			}
			if(worseRoute) // if this route is worse, go on to next connection
			{
				continue;
			}

			//If the node is not closed and not in the open set then we have an unvisited node so add it to the open set
			if(!alreadyInOpen)
			{
				NodeRecord endNodeRecord;
				endNodeRecord.node = endNode;
				endNodeRecord.connection = *conItr;
				endNodeRecord.costSoFar = endNodeCost;

				endNodeRecord.prevNode = new NodeRecord();
				endNodeRecord.prevNode->node = current.node;
				endNodeRecord.prevNode->connection = current.connection;
				endNodeRecord.prevNode->prevNode = current.prevNode;

				open.push_back(endNodeRecord);
			}


		}

		//At this point we have gone through all connections of this node so close it and remove from open set
		current.node->closeNode();
		open.remove(current);
	}



	if(current.node != end)
	{
		return false;
	}

	while(current.node != start)
	{
		path->push_back(current.connection);
		current = *current.prevNode;
	}

	path->reverse();
	return true;
}