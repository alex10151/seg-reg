#pragma once
#include "Utils.h"
// binary erosion
PixelType* BinaryErode3D(const PixelType *src, const Graph &graph, const PixelType *structureElement);
// gray-value erosion
PixelType* GrayValueErode3D(const PixelType *src, const Graph &graph, const PixelType *structureElement);