#include"Pipeline.h"
#include"PreDef.h"
#include"DCMSeriesReader.h"
#include"DCMSeriesWriter.h"
#include"TableRemoval.h"
#include"BonesRemoval.h"
#include"CmdIterationUpdate.h"
#include"Registration.h"
#include"itkNiftiImageIO.h"
#include"itkRawImageIO.h"
#include"RegistrationInterfaceCommand.h"

using namespace predef;

int RemovalPipeline(char*type,char*src,char*dst)
{
	InputType::Pointer inputPtr = InputType::New();
	OutputType::Pointer outputPtr = OutputType::New();
	DcmioType::Pointer dcmioPtr = DcmioType::New();
	DCMSeriesReader<PixelType, inputDim> handleRead = DCMSeriesReader<PixelType, inputDim>();
	DCMSeriesWriter<PixelTypeInt, inputDim, outputDim> handleWrite = DCMSeriesWriter<PixelTypeInt, inputDim, outputDim>();
	NameGeneratorType::Pointer namePtrOfTemp = NameGeneratorType::New();
	CastFloatToUnsignedIntType::Pointer caster = CastFloatToUnsignedIntType::New();
	handleRead.SetImageIO(dcmioPtr.GetPointer());
	handleWrite.SetImageIO(dcmioPtr.GetPointer());

	itksys::SystemTools::MakeDirectory(dst);
	namePtrOfTemp->SetInputDirectory(src);
	namePtrOfTemp->SetOutputDirectory(dst);

	handleRead.SetFilenames(namePtrOfTemp->GetInputFileNames());
	handleWrite.SetFilenames(namePtrOfTemp->GetOutputFileNames());
	handleWrite.SetMetaDicArray(handleRead.GetMetaDicArray());
	handleRead.Run();
	handleRead.GetReader();
	inputPtr = handleRead.GetOutput();

	InputType::SizeType inputSize = inputPtr->GetLargestPossibleRegion().GetSize();
	int size[3] = { inputSize[0] , inputSize[1],inputSize[2] };
	InputType::Pointer removal;
	if (!strcmp(type, "-table"))
		removal = TableRemovalPipe(inputPtr, size);
	if (!strcmp(type, "-headbone"))
		removal = BonesRemovalPipe(inputPtr, size,true,false);
	if (!strcmp(type, "-bodybone"))
		removal = BonesRemovalPipe(inputPtr, size, false,false);
	//calculate seed cordinates
	//InternalType::IndexType seed;
	//if (!strcmp(argv[3], "-s"))
	//{
	//	seed[0] = atoi(argv[4]);
	//	seed[1] = atoi(argv[5]);
	//	seed[2] = atoi(argv[6]);
	//}
	//else
	//{
	//	seed = SeedChoose(Image, size);
	//	return EXIT_FAILURE;
	//}
	//RegionGrowWithConfidenceMethod<PixelType, PixelTypeInt> rgcm(handleRead.Getoutput());
	//rgcm.configCurvatureFilter();
	//rgcm.configConfidenceConnectedImageFilter(seed);
	//handleWrite.SetInput(rgcm.GetProcessedImage());
	caster->SetInput(removal);
	handleWrite.SetInput(caster->GetOutput());
	handleWrite.Run();
	return EXIT_SUCCESS;
}
int RegistrationPipeline(char*fix, char*mov, char*dst)
{
	//registration components def
	TransformType::Pointer      transform = TransformType::New();
	OptimizerType::Pointer      optimizer = OptimizerType::New();
	InterpolatorType::Pointer   interpolator = InterpolatorType::New();
	RegistrationType::Pointer   registration = RegistrationType::New();
	MetricType::Pointer         metric = MetricType::New();
	FixedImagePyramidType::Pointer fixedImagePyramid =
		FixedImagePyramidType::New();
	MovingImagePyramidType::Pointer movingImagePyramid =
		MovingImagePyramidType::New();
	registration->SetOptimizer(optimizer);
	registration->SetTransform(transform);
	registration->SetInterpolator(interpolator);
	registration->SetMetric(metric);
	registration->SetFixedImagePyramid(fixedImagePyramid);
	registration->SetMovingImagePyramid(movingImagePyramid);
	//image io def
	DCMSeriesReader<PixelType, RegInputDim> seriesFix;
	DCMSeriesReader<PixelType, RegInputDim> seriesMov;
	NameGeneratorType::Pointer filenames = NameGeneratorType::New();
	DcmioType::Pointer dcm = DcmioType::New();
	filenames->SetInputDirectory(fix);
	seriesFix.SetFilenames(filenames->GetInputFileNames());
	filenames->SetInputDirectory(mov);
	seriesMov.SetFilenames(filenames->GetInputFileNames());

	seriesFix.SetImageIO(dcm.GetPointer());
	seriesMov.SetImageIO(dcm.GetPointer());
	seriesFix.Run();
	seriesMov.Run();
	//cast inputs to internal type
	FixedCastFilterType::Pointer fixedCaster = FixedCastFilterType::New();
	MovingCastFilterType::Pointer movingCaster = MovingCastFilterType::New();
	fixedCaster->SetInput(seriesFix.GetOutput());
	movingCaster->SetInput(seriesMov.GetOutput());
	registration->SetFixedImage(fixedCaster->GetOutput());
	registration->SetMovingImage(movingCaster->GetOutput());
	fixedCaster->Update();
	movingCaster->Update();
	//set registration params
	registration->SetFixedImageRegion(fixedCaster->GetOutput()->GetLargestPossibleRegion());
	ParametersType initialParameters(transform->GetNumberOfParameters());
	for (unsigned int i = 0; i < transform->GetNumberOfParameters(); i++)
		initialParameters[i] = 1;

	registration->SetInitialTransformParameters(initialParameters);
	metric->SetNumberOfSpatialSamples(seriesFix.GetOutput()->GetLargestPossibleRegion().GetNumberOfPixels()*0.001);
	metric->SetNumberOfHistogramBins(NB_OF_BINS);
	metric->ReinitializeSeed(REINIT_SEED);
	metric->SetUseExplicitPDFDerivatives(USE_PDF_DERIVATIVE);
	optimizer->SetNumberOfIterations(NB_ITERATION);
	optimizer->SetRelaxationFactor(RELAX_FACTOR);
	CommandIterationUpdate::Pointer observer = CommandIterationUpdate::New();
	optimizer->AddObserver(itk::IterationEvent(), observer);
	RegInterfaceCommand<RegistrationType>::Pointer command = RegInterfaceCommand<RegistrationType>::New();
	registration->AddObserver(itk::IterationEvent(), command);
	registration->SetNumberOfLevels(NB_OF_LEVELS);
	try
	{
		registration->Update();
		std::cout << "Optimizer stop condition: "
			<< registration->GetOptimizer()->GetStopConditionDescription()
			<< std::endl;
	}
	catch (itk::ExceptionObject & err)
	{
		std::cout << "ExceptionObject caught !" << std::endl;
		std::cout << err << std::endl;
		return EXIT_FAILURE;
	}
	// post process
	ParametersType finalParameters = registration->GetLastTransformParameters();
	double TranslationAlongX = finalParameters[0];
	double TranslationAlongY = finalParameters[1];
	double TranslationAlongZ = finalParameters[2];
	unsigned int numberOfIterations = optimizer->GetCurrentIteration();
	double bestValue = optimizer->GetValue();
	std::cout << "Result = " << std::endl;
	std::cout << " Translation X = " << TranslationAlongX << std::endl;
	std::cout << " Translation Y = " << TranslationAlongY << std::endl;
	std::cout << " Iterations    = " << numberOfIterations << std::endl;
	std::cout << " Metric value  = " << bestValue << std::endl;

	//resample process
	TransformType::Pointer finalTransform = TransformType::New();
	finalTransform->SetParameters(finalParameters);
	finalTransform->SetFixedParameters(transform->GetFixedParameters());
	ResampleFilterType::Pointer resample = ResampleFilterType::New();
	resample->SetTransform(finalTransform);
	resample->SetInput(seriesMov.GetOutput());
	FixedImageType::Pointer fixedImage = seriesFix.GetOutput();
	resample->SetSize(fixedImage->GetLargestPossibleRegion().GetSize());
	resample->SetOutputOrigin(fixedImage->GetOrigin());
	resample->SetOutputSpacing(fixedImage->GetSpacing());
	resample->SetOutputDirection(fixedImage->GetDirection());
	resample->SetDefaultPixelValue(BACKGROUD_GRAY_LEVEL);
	//writer def
	DCMSeriesWriter<PixelType, RegInputDim, RegoutputDim> seriesW;
	seriesW.SetImageIO(dcm.GetPointer());
	filenames->SetOutputDirectory(dst);
	seriesW.SetFilenames(filenames->GetOutputFileNames());
	seriesW.SetMetaDicArray(seriesMov.GetMetaDicArray());
	seriesW.SetInput(resample->GetOutput());
	seriesW.Run();
	return EXIT_SUCCESS;
}