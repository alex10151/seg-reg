#pragma once
#include "PreDef.h"
using namespace predef;

void  BonesVisual(SingleInputType::Pointer inputPtr, SingleInputType::Pointer boneImage, int size[]);
InputType::Pointer  BonesRemoval(InputType::Pointer inputPtr, InputType::Pointer fusionImage, int size[]);
InputType::Pointer  BonesRemovalPipe(InputType::Pointer inputPtr, int size[], bool head = false, bool bonesExtract=false);
SingleInputType::Pointer  HoleFill(SingleInputType::Pointer src);
SingleInputType::Pointer  ThresholdProcess(SingleInputType::Pointer inputPtr, int bonesThresh);
vector<SingleInputType::Pointer>  ConvertToTransverseImage(InputType::Pointer inputPtr, int size[]);
SingleInputType::Pointer  LaplacianProcess(SingleInputType::Pointer src);
SingleInputType::Pointer  Fusion(SingleInputType::Pointer threImage, SingleInputType::Pointer lapImage, int size[]);
SingleInputType::Pointer  OpOpen(SingleInputType::Pointer src);
SingleInputType::Pointer  OpClose(SingleInputType::Pointer src);