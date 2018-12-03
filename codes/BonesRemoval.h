#pragma once
#include "PreDef.h"
using namespace predef;

void  BonesVisual(SingleInputType::Pointer inputPtr, SingleInputType::Pointer boneImage);
InputType::Pointer  BonesRemoval(InputType::Pointer inputPtr, InputType::Pointer rmvStruct);
vector<InputType::Pointer> BonesRemovalPipe(InputType::Pointer inputPtr, bool head = false);
SingleInputType::Pointer  HoleFill(SingleInputType::Pointer src);
SingleInputType::Pointer  ThresholdProcess(SingleInputType::Pointer inputPtr, int bonesThresh);
SingleInputType::Pointer  LaplacianProcess(SingleInputType::Pointer src);
SingleInputType::Pointer  Fusion(SingleInputType::Pointer threImage, SingleInputType::Pointer lapImage);
SingleInputType::Pointer  OpOpen(SingleInputType::Pointer src,float radius);
SingleInputType::Pointer  OpClose(SingleInputType::Pointer src,float radius);