#pragma once
#include "Graph.h"
#include"PreDef.h"
using namespace predef;

//post process of table removal
vector<vector<float>> PostProcess(vector<vector<float>> &points);
//crop 2d image 
SingleInputType::Pointer CropImage(SingleInputType::Pointer src, int originX, int originY, int cutX, int cutY);
//copy a series image into a volume
void CopyFromSeries(InputType::Pointer src, vector<SingleInputType::Pointer>transContainer, int size[]);
//for copy a transverse
void CopyTransverse(SingleInputType::Pointer src, SingleInputType::Pointer dst, int size[]);
//create a empty volume from a volume
InputType::Pointer CreateEmptyVolume(InputType::Pointer inputPtr, int size[]);
//create a slice transverse image from a volume
SingleInputType::Pointer CreateEmptyTransverseFromVolume(InputType::Pointer inputPtr, int size[]);
//create a slice sagittal image from a volume
SingleInputType::Pointer CreateEmptySagittalFromVolume(InputType::Pointer inputPtr, int size[]);
//get total pixels
int NumberOfPixels(const int size[]);
// cordinates conversion
int Cordinates(const Point cartesian, const int size[]);
// boundary check
bool CheckIndex(int index[], const int image_size[]);
//rearrange dcm files
int RearrangeDCMFiles(const vector<string>filenames, string outputDir);
//residual op of structure element and image
PixelType* GetResidualElement3D(const PixelType *src, const PixelType *structureElement, const Point &base, const int imgSize[]);
// get the max pixel value of a  structural element in source img
PixelType StructureMin3D(const PixelType *structureElement);
// binarize the image of the target connected components and background.
PixelType* BinaryThresholdProcess(const PixelType *src, const Graph&graph, int low = MIN_PIXEL_VALUE, int high = MAX_PIXEL_VALUE);
bool IsEqual3D(const PixelType *src, const PixelType *structureElement, const Point &base, const int imgSize[]);