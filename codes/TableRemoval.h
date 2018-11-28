#pragma once
#include "PreDef.h"
using namespace predef;

float GetIntersection(float diag, float yMax, float zMax, float y1, float z1, float vy, float vz);
bool cmp(vector<float>a, vector<float>b);
vector<short> Convert2Int(InputType::SizeType size);
void TableRemoval(InputType::Pointer inputPtr, vector<vector<float>>structureSlice);
HT2DFilterType::LinesListType HoughTransformOnGradientImage(SingleInputType::Pointer srcImg);
float FindYofPoint(HT2DFilterType::LinesListType &lines, vector<short> size);
void TableRemovalPipe(InputType::Pointer inputPtr);
