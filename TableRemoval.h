#pragma once
#include "PreDef.h"
using namespace predef;

float GetIntersection(float diag, float yMax, float zMax, float y1, float z1, float vy, float vz);
bool cmp(vector<float>a, vector<float>b);
InputType::Pointer  TableRemoval(InputType::Pointer inputPtr, int size[], vector<vector<float>>structureSlice);
vector<SingleInputType::Pointer>  ConvertToSagittalImage(InputType::Pointer inputPtr, int size[]);
HT2DFilterType::LinesListType HoughTransformOnGradientImage(SingleInputType::Pointer srcImg);
float FindYofPoint(HT2DFilterType::LinesListType &lines, int size[]);
InputType::Pointer  TableRemovalPipe(InputType::Pointer inputPtr, int size[]);
