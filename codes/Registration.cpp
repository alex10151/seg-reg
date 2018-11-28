#include"Registration.h"
#include"PreDef.h"
#include"CmdIterationUpdate.h"
#include"RegistrationInterfaceCommand.h"
using namespace predef;

int OptimizeStepPipe(FixedImageType::Pointer fix,MovingImageType::Pointer mov, RegistrationType::Pointer registration)
{
	//registration components def
	TransformType::Pointer      transform = TransformType::New();
	OptimizerType::Pointer      optimizer = OptimizerType::New();
	InterpolatorType::Pointer   interpolator = InterpolatorType::New();
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
	//cast inputs to internal type
	FixedCastFilterType::Pointer fixedCaster = FixedCastFilterType::New();
	MovingCastFilterType::Pointer movingCaster = MovingCastFilterType::New();
	fixedCaster->SetInput(fix);
	movingCaster->SetInput(mov);
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
	metric->SetNumberOfSpatialSamples(fix->GetLargestPossibleRegion().GetNumberOfPixels()*0.001);
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
		cout << "===============Start registration:" << endl;
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
	return EXIT_SUCCESS;
}
int ResamplePipe(RegistrationType::Pointer registration,FixedImageType::Pointer fix,MovingImageType::Pointer mov, ResampleFilterType::Pointer resample)
{
	ParametersType finalParameters = registration->GetLastTransformParameters();
	//resample process
	TransformType::Pointer finalTransform = TransformType::New();
	finalTransform->SetParameters(finalParameters);
	finalTransform->SetFixedParameters(registration->GetTransform()->GetFixedParameters());
	resample->SetTransform(finalTransform);
	resample->SetInput(mov);
	resample->SetSize(fix->GetLargestPossibleRegion().GetSize());
	resample->SetOutputOrigin(fix->GetOrigin());
	resample->SetOutputSpacing(fix->GetSpacing());
	resample->SetOutputDirection(fix->GetDirection());
	resample->SetDefaultPixelValue(BACKGROUD_GRAY_LEVEL);
	try
	{
		cout << "===============Start resample:" << endl;
		resample->Update();
	}
	catch (itk::ExceptionObject & err)
	{
		std::cout << "ExceptionObject caught !" << std::endl;
		std::cout << err << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}


