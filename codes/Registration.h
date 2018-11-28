#pragma once
#include "PreDef.h"
#include"CmdIterationUpdate.h"
#include"DCMSeriesReader.h"
using namespace predef;

//resample step
int ResamplePipe(RegistrationType::Pointer registration, FixedImageType::Pointer fix, MovingImageType::Pointer mov, ResampleFilterType::Pointer resample);
// opt step
int OptimizeStepPipe(FixedImageType::Pointer fix, MovingImageType::Pointer mov, RegistrationType::Pointer registration);
