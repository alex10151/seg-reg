
#include "Erode.h"
// binary erosion
PixelType* BinaryErode3D(const PixelType *src, const Graph &graph, const PixelType *structureElement)
{
	int imgSize[3] = { graph.valueOfAxisX,graph.valueOfAxisY,graph.valueOfAxisZ };
	PixelType *temp = BinaryThresholdProcess(src, graph, 0, 1);
	PixelType *res = new PixelType[NumberOfPixels(imgSize)]{ 0 };
	for (int i = 0; i < imgSize[0]; i++)
	{
		for (int j = 0; j < imgSize[1]; j++)
		{
			for (int k = 0; k < imgSize[2]; k++)
			{
				Point base(i, j, k);
				if (graph.weightMetrix[Cordinates(base, imgSize)] != 0)
				{
					if (IsEqual3D(temp, structureElement, base, imgSize))
						res[Cordinates(base, imgSize)] = MAX_PIXEL_VALUE;
				}
			}
		}
	}
	return res;
}
// gray-value erosion
PixelType* GrayValueErode3D(const PixelType *src, const Graph &graph, const PixelType *structureElement)
{
	int imgSize[3] = { graph.valueOfAxisX,graph.valueOfAxisY,graph.valueOfAxisZ };
	PixelType *res = new PixelType[NumberOfPixels(imgSize)];
	for (int i = 0; i < NumberOfPixels(imgSize); i++)
		res[i] = MIN_PIXEL_VALUE;
	for (int q = 0; q < imgSize[0]; q++)
	{
		for (int w = 0; w < imgSize[1]; w++)
		{
			for (int e = 0; e < imgSize[2]; e++)
			{
				Point center(q, w, e);
				PixelType *resElement = GetResidualElement3D(src, structureElement, center, imgSize);
				PixelType min = StructureMin3D(resElement);
				res[Cordinates(center, imgSize)] = min;
			}
		}
	}
	return res;
}