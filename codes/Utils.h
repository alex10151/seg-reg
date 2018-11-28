#pragma once
#include "Graph.h"
#include"PreDef.h"
using namespace predef;

//convert a volume to sagittal series
vector<SingleInputType::Pointer>  ConvertToSagittalImage(InputType::Pointer inputPtr);
//convert a volume to transverse series
vector<SingleInputType::Pointer>  ConvertToTransverseImage(InputType::Pointer inputPtr);
//post process of table removal
vector<vector<float>> PostProcess(vector<vector<float>> &points);
//crop 2d image 
SingleInputType::Pointer CropImage(SingleInputType::Pointer src, int originX, int originY, int cutX, int cutY);
//copy a transverse series image into a volume
void CopyFromSeriesTransverse(InputType::Pointer src, vector<SingleInputType::Pointer>transContainer);
//copy a sagittal series image into a volume
void CopyFromSeriesSagittal(InputType::Pointer src, vector<SingleInputType::Pointer>transContainer);
//copy volume
void CopyVolume(InputType::Pointer src, InputType::Pointer dst);
//copy slice
void CopySlice(SingleInputType::Pointer src, SingleInputType::Pointer dst);
//create a empty volume from a volume
InputType::Pointer CreateEmptyVolume(InputType::Pointer inputPtr);
//create a slice transverse image from a volume
SingleInputType::Pointer CreateEmptyTransverseFromVolume(InputType::Pointer inputPtr);
//create a slice sagittal image from a volume
SingleInputType::Pointer CreateEmptySagittalFromVolume(InputType::Pointer inputPtr);
//get total pixels
unsigned int NumberOfPixels(vector<unsigned int> size);
int NumberOfPixels(const int size[]);
unsigned int NumberOfPixels(unsigned int size[]);
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