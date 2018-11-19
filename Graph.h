#pragma once
#include<vector>
using namespace std;

//graph for connected components
class Graph
{
public:
	vector<int>weightMetrix;
	int valueOfAxisX ;
	int valueOfAxisY;
	int valueOfAxisZ;
	int amountOfConnected;
	Graph(const Graph&another);
	Graph(int x, int y,int z, int origin_value);
};

class Point
{
public:
	int x;
	int y;
	int z;
	Point(int x, int y, int z);
	Point(int cord[3]);
	Point(const Point &another);
	bool operator==(Point&op) const;
};