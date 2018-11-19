#pragma once
#include "PreDef.h"
#include"CmdIterationUpdate.h"
#include"DCMSeriesReader.h"
using namespace predef;

//mov img size fits to fixed img
ResampleFilterType::Pointer SizeMatch(DCMSeriesReader<PixelType, 3>fixReader, DCMSeriesReader<PixelType, 3>movReader);

ResampleFilterType::Pointer Resample(TransformType::Pointer transform, itk::ImageFileReader<itk::Image<signed short, 2>>::Pointer fixReader, itk::ImageFileReader<itk::Image<signed short, 2>>::Pointer movReader);
//resample step
ResampleFilterType::Pointer Resample(TransformType::Pointer transform, DCMSeriesReader<PixelType, 3>fixReader, DCMSeriesReader<PixelType, 3>movReader);
// opt step
int OptimizeStep(RegistrationType::Pointer registration, TransformType::Pointer transform, OptimizerType::Pointer optimizer, CommandIterationUpdate::Pointer observer);
