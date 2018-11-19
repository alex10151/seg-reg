#pragma once
#include "Graph.h"
#include<vector>
using namespace std;
Graph::Graph(const Graph&another)
{
	this->weightMetrix = another.weightMetrix;
	this->valueOfAxisX = another.valueOfAxisX;
	this->valueOfAxisY = another.valueOfAxisY;
	this->valueOfAxisZ = another.valueOfAxisZ;
	this->amountOfConnected = another.amountOfConnected;
}
Graph::Graph(int x, int y, int z,int origin_value)
{
	this->weightMetrix = vector<int>(x*y*z, origin_value);
	this->valueOfAxisX = x;
	this->valueOfAxisY = y;
	this->valueOfAxisZ = z;
	this->amountOfConnected = 0;
}


Point::Point(int x, int y, int z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}
Point::Point(int cord[3])
{
	this->x = cord[0];
	this->y = cord[1];
	this->z = cord[2];
}
Point::Point(const Point &another)
{
	this->x = another.x;
	this->y = another.y;
	this->z = another.z;
}
bool Point::operator==(Point&op) const
{
	if (this->x != op.x)
		return false;
	if (this->y != op.y)
		return true;
	if (this->z != op.z)
		return false;
	else
		return true;
}