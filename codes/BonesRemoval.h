#pragma once
#include "PreDef.h"
using namespace predef;

void  BonesVisual(SingleInputType::Pointer inputPtr, SingleInputType::Pointer boneImage);
void  BonesRemoval(InputType::Pointer inputPtr, InputType::Pointer rmvStruct);
void BonesRemovalPipe(InputType::Pointer inputPtr,InputType::Pointer resBone, bool head = false);
SingleInputType::Pointer  HoleFill(SingleInputType::Pointer src);
SingleInputType::Pointer  ThresholdProcess(SingleInputType::Pointer inputPtr, int bonesThresh);
SingleInputType::Pointer  LaplacianProcess(SingleInputType::Pointer src);
SingleInputType::Pointer  Fusion(SingleInputType::Pointer threImage, SingleInputType::Pointer lapImage);
SingleInputType::Pointer  OpOpen(SingleInputType::Pointer src,float radius);
SingleInputType::Pointer  OpClose(SingleInputType::Pointer src,float radius);