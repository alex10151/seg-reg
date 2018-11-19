#pragma once
#include"Graph.h"
#include"Utils.h"
#include"Erode.h"

//  floodfill for finding all connected components within tresholds 
Graph FindConnectedComponent(const PixelType *src, Graph&graph, const PixelType threshold[])
{
	queue<Point>temp;
	int imgSize[3] = { graph.valueOfAxisX,graph.valueOfAxisY,graph.valueOfAxisZ };
	bool *flag = new bool[NumberOfPixels(imgSize)]{ false };
	int markOfConnected = 0;
	for (int i = 0; i < imgSize[0]; i++)
	{
		for (int j = 0; j < imgSize[1]; j++)
		{
			for (int k = 0; k < imgSize[2]; k++)
			{
				Point p = Point(i, j, k);
				if ((src[Cordinates(p, imgSize)] < threshold[0]) || (src[Cordinates(p, imgSize)] > threshold[1]))
				{
					flag[Cordinates(p, imgSize)] = true;
					continue;
				}
				if (flag[Cordinates(p, imgSize)] != true)
				{
					temp.push(p);
					markOfConnected++;
					while (!temp.empty())
					{
						Point base = temp.front();
						graph.weightMetrix[Cordinates(base, imgSize)] = markOfConnected;
						temp.pop();
						for (int q = -1; q < 3; q++)
						{
							for (int w = -1; w < 3; w++)
							{
								for (int e = -1; e < 3; e++)
								{
									if ((q == 0) && (w == 0) && (e == 0))
									{
										flag[Cordinates(base, imgSize)] = true;
										continue;
									}
									Point nextNeighbor(base.x + q, base.y + w, base.z + e);
									if (flag[Cordinates(nextNeighbor, imgSize)] == true)
										continue;
									if ((src[Cordinates(nextNeighbor, imgSize)] < threshold[0]) || (src[Cordinates(nextNeighbor, imgSize)] > threshold[1]))
									{
										flag[Cordinates(nextNeighbor, imgSize)] = true;
										continue;
									}
									else
									{
										temp.push(nextNeighbor);
										flag[Cordinates(nextNeighbor, imgSize)] = true;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return graph;
}
// compare two stack 
template<typename T>
bool CompareStack(stack<T>s1, stack<T>s2)
{
	stack<T>op1 = s1;
	stack<T>op2 = s2;
	while (!op1.empty() && !op2.empty())
	{
		T temp1 = op1.top();
		T temp2 = op2.top();
		op1.pop();
		op2.pop();
		if (!(temp1 == temp2))
			return false;
	}
	if (!op1.empty())
		return false;
	if (!op2.empty())
		return false;
	return true;
}
Point RandSample(PixelType*src, const int imgSize[])
{
	int maxStep = 10000;
	while (maxStep--)
	{
		srand((int)time(0));
		int rand_x = rand() % imgSize[0];
		int rand_y = rand() % imgSize[1];
		int rand_z = rand() % imgSize[2];
		Point res(rand_x, rand_y, rand_z);
		if (src[Cordinates(res, imgSize)] != 0)
			return res;
	}
}
// for auto choose seed ,base point is for threshold value, base point choosing not implented,default is (0,0,0).
//use binaryerode,gray-value erode not tested.
InternalType::IndexType SeedChoose(const PixelType*Image, const int size[], int residual = DEFAULT_RESIDUAL, Point base = Point(0, 0, 0))
{
	Graph connected(size[0], size[1], size[2], 0);
	int threshold_residual[2] = { -residual,residual };
	assert((Image[Cordinates(base, size)] + threshold_residual[0]) >= MIN_PIXEL_VALUE);
	assert((Image[Cordinates(base, size)] + threshold_residual[1]) <= MAX_PIXEL_VALUE);
	PixelType threshold[2] = { Image[Cordinates(base,size)] + threshold_residual[0] ,Image[Cordinates(base,size)] + threshold_residual[1] };
	connected = FindConnectedComponent(Image, connected, threshold);
	PixelType structureElement[27];
	for (int i = 0; i < 27; i++)
		structureElement[i] = 1;
	PixelType * erodeImage = BinaryErode3D(Image, connected, structureElement);
	Point target = RandSample(erodeImage, size);
	InternalType::IndexType seed;

	seed[0] = target.x;
	seed[1] = target.y;
	seed[2] = target.z;
	return seed;
}