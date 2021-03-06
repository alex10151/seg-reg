#pragma once
#include"itkImage.h"
#include "itkImageSeriesReader.h"
#include "itkImageSeriesWriter.h"
#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"
#include "itkConnectedThresholdImageFilter.h"
#include "itkConfidenceConnectedImageFilter.h"
#include "itkCurvatureFlowImageFilter.h"
#include "itkImageFileWriter.h"
#include "itkCastImageFilter.h"
#include "itkThresholdImageFilter.h"
#include "itkGradientMagnitudeImageFilter.h"
#include "itkHoughTransform2DLinesImageFilter.h"
#include "itkBinaryThresholdImageFilter.h"
#include"itkSobelEdgeDetectionImageFilter.h"
#include "itkMultiResolutionImageRegistrationMethod.h"
#include "itkMattesMutualInformationImageToImageMetric.h"
#include"itkCannyEdgeDetectionImageFilter.h"
#include "itkBinaryErodeImageFilter.h"
#include "itkBinaryDilateImageFilter.h"
#include "itkBinaryBallStructuringElement.h"
#include "itkLaplacianRecursiveGaussianImageFilter.h"
#include"itkBinaryContourImageFilter.h"
#include"itkGrayscaleFillholeImageFilter.h"
#include"itkMeanImageFilter.h"
#include"itkGrayscaleDilateImageFilter.h"
#include"itkGrayscaleErodeImageFilter.h"
#include"itkRegularStepGradientDescentOptimizerv4.h"
#include"itkRegularStepGradientDescentOptimizer.h"
#include"itkMeanSquaresImageToImageMetricv4.h"
#include"itkMeanSquaresImageToImageMetric.h"
#include"itkLinearInterpolateImageFunction.h"
#include"itkImageRegistrationMethod.h"
#include"itkImageRegistrationMethodv4.h"
#include"itkCenteredTransformInitializer.h"
#include"itkResampleImageFilter.h"
#include"itkSubtractImageFilter.h"
#include"itkRescaleIntensityImageFilter.h"
#include"itkAffineTransform.h"
#include"itkEuler3DTransform.h"
#include"itkCenteredTransformInitializer.h"
#include"itkTranslationTransform.h"
#include"itkBinaryPruningImageFilter.h"
#include"itkGrayscaleGrindPeakImageFilter.h"
#include"itkNormalizedCorrelationImageToImageMetric.h"
#include"itkGradientDescentOptimizer.h"
#include "itkCheckerBoardImageFilter.h"
#include"itkMedianImageFilter.h"
#include"itkAntiAliasBinaryImageFilter.h"
#include"itkBilateralImageFilter.h"
#include"itkImportImageFilter.h"
#include"itkShiftScaleImageFilter.h"
#include <time.h>
#include "iostream"
#include <string>
#include <fstream>
#include <stack>
#include <vector>
#include <queue>
using namespace std;
namespace predef
{
	//params of configurations(for segmentation)
	constexpr float BALL_STRUCT_RADIUS =5;
	constexpr float THRESHOLD_OUTSIDE_VALUE = 0;
	constexpr float THRESHOLD_LOW = 200;
	constexpr float THRESHOLD_UP = 1500;
	constexpr float HOUGHTRANSFORM_RADIUS= 1;
	constexpr float HOUGHTRANSFORM_LINES=40;
	constexpr float HOUGHTRANSFORM_VAR = 2;
	constexpr float LAPLACE_SIGMA = 0.5;
	constexpr int LIMIT = 10000;
	constexpr int WINDOW_SIZE = 7;
	constexpr int BONES_THRESHOLD = 180;
	constexpr int HEAD_BONES_THRESHOLD = 180;
	constexpr int MAX_THRESHOLD = 20;
	constexpr short DEFAULT_RESIDUAL = 5;
	constexpr int MAX_PIXEL_VALUE = 2000;
	constexpr int MIN_PIXEL_VALUE = -1000;
	constexpr int PRUNE_ITERATION = 100;
	constexpr int DEFAULT_SHIFT = -1024;
	constexpr int DEFAULT_SCALE = 1;

	//optimizer params(registration)
	constexpr int NB_OF_BINS = 64;
	constexpr bool USE_PDF_DERIVATIVE = false;   //true may cause problem
	constexpr int BACKGROUD_GRAY_LEVEL = 255;
	constexpr double RELAX_FACTOR = 0.5;
	constexpr unsigned int NB_ITERATION = 100;
	constexpr unsigned int NB_OF_LEVELS = 5;
	constexpr unsigned int REINIT_SEED = 76926294;

	//dicom tag
	const  string IMAGE_INSTANCE_NUMBER_TAG =  "0020|0013";

	//dims and pixeltype

	using PixelType = short;
	using PixelTypeInt = signed short;
	constexpr unsigned int inputDim = 3;
	constexpr unsigned int outputDim = 2;
	//basic image type for segmentation
	using InputType = itk::Image<PixelType, inputDim>;
	using OutputType = itk::Image<PixelTypeInt, outputDim>;
	using CastType = itk::Image<PixelTypeInt, inputDim>;
	using InternalType = itk::Image<PixelType, inputDim>;
	using SingleInputType = itk::Image<PixelType, outputDim>;
	//the various filters available
	using BinaryThresholdFilter = itk::BinaryThresholdImageFilter<SingleInputType, SingleInputType>;
	using ConnectedFilterType = itk::ConnectedThresholdImageFilter<InternalType, InternalType>;
	using ConfidenceConnectedImageFilter = itk::ConfidenceConnectedImageFilter<InternalType, InternalType>;
	using CurvatureImageFilterType = itk::CurvatureFlowImageFilter<itk::Image<float,2>, itk::Image<float,2>>;
	using HT2DFilterType = itk::HoughTransform2DLinesImageFilter<PixelType, PixelType>;
	using GradFilterType = itk::GradientMagnitudeImageFilter<SingleInputType, SingleInputType>;
	using ThreshFilterType = itk::ThresholdImageFilter<SingleInputType>;
	using LoGFilterType = itk::LaplacianRecursiveGaussianImageFilter<SingleInputType, SingleInputType >;
	using StructureElementType = itk::BinaryBallStructuringElement<PixelType, outputDim>;
	using ErodeFilterType = itk::BinaryErodeImageFilter<SingleInputType,SingleInputType,StructureElementType>;
	using DilateFilterType = itk::BinaryDilateImageFilter<SingleInputType, SingleInputType, StructureElementType>;
	using GrayDilateFilterType = itk::GrayscaleDilateImageFilter<SingleInputType, SingleInputType, StructureElementType >;
	using GrayErodeFilterType = itk::GrayscaleErodeImageFilter <SingleInputType, SingleInputType, StructureElementType >;
	using ContourFilterType= itk::BinaryContourImageFilter< SingleInputType, SingleInputType>;
	using FillHoleType= itk::GrayscaleFillholeImageFilter<SingleInputType, SingleInputType> ;
	using MeanFilter = itk::MeanImageFilter<SingleInputType, SingleInputType>;
	using ImportImageFilterType = itk::ImportImageFilter<PixelType, inputDim>;
	using AntiStackFilterType = itk::AntiAliasBinaryImageFilter<itk::Image<float, 3>, itk::Image<float, 3>>;
	using PruneType= itk::BinaryPruningImageFilter <SingleInputType, SingleInputType >;
	using GrindType = itk::GrayscaleGrindPeakImageFilter <SingleInputType, SingleInputType >;
	using MedianType = itk::MedianImageFilter<SingleInputType, SingleInputType>;
	using RescaleFilterType = itk::RescaleIntensityImageFilter<InputType, InputType>;
	using ShiftFilterType = itk::ShiftScaleImageFilter<InputType,InputType>;
	// for IO
	using InputReader = itk::ImageSeriesReader<InputType>;
	using OutputWriter = itk::ImageSeriesWriter<CastType, OutputType>;
	using DcmioType = itk::GDCMImageIO;
	using NameGeneratorType = itk::GDCMSeriesFileNames;
	using SingleInputReader = itk::ImageFileReader<SingleInputType>;
	using SingleOutputWriter = itk::ImageFileWriter<SingleInputType>;
	using DictionaryType = itk::MetaDataDictionary;
	using MetaDataStringType = itk::MetaDataObject<string>;
	using IndexType = typename InternalType::IndexType;
	using ImageIObase = itk::ImageIOBase;
	using FilenamesContainer = itk::FilenamesContainer;

	//for registration
	constexpr unsigned int RegInputDim = 3;
	constexpr unsigned int RegoutputDim = 2;
	using InternalPixelType = float;
	using FixedImageType = itk::Image< PixelType, RegInputDim >;
	using MovingImageType = itk::Image< PixelType, RegInputDim >;
	using OutPutImageType = itk::Image<PixelType, RegoutputDim>;
	using InternalImageType = itk::Image< InternalPixelType, RegInputDim >;
	//using TransformType = itk::TranslationTransform< double, RegInputDim >;
	using ReTransformType = itk::Euler3DTransform<double>;
	using TransformType = itk::AffineTransform<double, RegInputDim>;
	using TransformInitializeType = itk::CenteredTransformInitializer<TransformType, InternalImageType, InternalImageType>;
	using OptimizerType = itk::RegularStepGradientDescentOptimizer;
	using InterpolatorType = itk::LinearInterpolateImageFunction<InternalImageType,double>;
	using MetricType = itk::MattesMutualInformationImageToImageMetric<InternalImageType,InternalImageType >;
	using RegistrationType = itk::MultiResolutionImageRegistrationMethod<InternalImageType,InternalImageType >;
	using FixedImagePyramidType = itk::MultiResolutionPyramidImageFilter<InternalImageType, InternalImageType >;
	using MovingImagePyramidType = itk::MultiResolutionPyramidImageFilter<InternalImageType, InternalImageType >;
	using FixedCastFilterType = itk::CastImageFilter<FixedImageType, InternalImageType >;
	using MovingCastFilterType = itk::CastImageFilter<MovingImageType, InternalImageType >;
	using ParametersType = RegistrationType::ParametersType;
	using ResampleFilterType = itk::ResampleImageFilter<MovingImageType, FixedImageType >;
	using ImportRegType = itk::ImportImageFilter<PixelType, RegInputDim>;
}