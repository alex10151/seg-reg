#include"Registration.h"
#include"PreDef.h"
#include"DCMSeriesReader.h"
#include"DCMSeriesWriter.h"
#include"CmdIterationUpdate.h"
using namespace predef;

int OptimizeStep(RegistrationType::Pointer registration, TransformType::Pointer transform, OptimizerType::Pointer optimizer, CommandIterationUpdate::Pointer observer)
{
	//OptimizerScalesType optimizerScales(transform->GetNumberOfParameters());
	//for (int i = 0; i < 10; i++)
	//	optimizerScales[i] = DEFAULT_OPT_SCALE;
	//optimizerScales[10] = TRANSLATION_SCALE;
	//optimizerScales[11] = TRANSLATION_SCALE;
	////optimizerScales[4] = TRANSLATION_SCALE;
	////optimizerScales[5] = TRANSLATION_SCALE;
	//optimizer->SetScales(optimizerScales);
	////optimizer->SetLearningRate(MAX_STEP_LENGTH);
	////optimizer->SetMinimumStepLength(MIN_STEP_LENGTH);
	////optimizer->SetMaximumStepLength(MAX_STEP_LENGTH);
	//optimizer->SetLearningRate(0.01);
	//optimizer->SetNumberOfIterations(MAX_NB_ITERATION);
	//optimizer->MinimizeOn();
	//optimizer->AddObserver(itk::IterationEvent(), observer);
	////constexpr unsigned int numberOfLevels = 1;
	////Registrationv4Type::ShrinkFactorsArrayType shrinkFactorsPerLevel;
	////shrinkFactorsPerLevel.SetSize(1);
	////shrinkFactorsPerLevel[0] = 1;
	////Registrationv4Type::SmoothingSigmasArrayType smoothingSigmasPerLevel;
	////smoothingSigmasPerLevel.SetSize(1);
	////smoothingSigmasPerLevel[0] = 0;
	////registration->SetNumberOfLevels(numberOfLevels);
	////registration->SetSmoothingSigmasPerLevel(smoothingSigmasPerLevel);
	////registration->SetShrinkFactorsPerLevel(shrinkFactorsPerLevel);

	//try
	//{
	//	registration->Update();
	//	std::cout << "Optimizer stop conditon: " << registration->GetOptimizer()->GetStopConditionDescription() << std::endl;
	//}
	//catch (itk::ExceptionObject & err)
	//{
	//	std::cerr << "ExceptionObject caught !" << std::endl;
	//	std::cerr << err << std::endl;
	//	return EXIT_FAILURE;
	//}
	return NULL;
}
ResampleFilterType::Pointer Resample(TransformType::Pointer transform, DCMSeriesReader<PixelType, 3>fixReader, DCMSeriesReader<PixelType, 3>movReader)
{
	//ResampleFilterType::Pointer resampler=ResampleFilterType::New();
	//resampler->SetTransform(transform);
	//resampler->SetInput(movReader.GetOutput());
	//resampler->SetSize(fixReader.GetOutput()->GetLargestPossibleRegion().GetSize());
	//resampler->SetOutputOrigin(fixReader.GetOutput()->GetOrigin());
	//resampler->SetOutputSpacing(fixReader.GetOutput()->GetSpacing());
	//resampler->SetOutputDirection(fixReader.GetOutput()->GetDirection());
	//resampler->SetDefaultPixelValue(100);
	//try
	//{
	//	resampler->Update();
	//}
	//catch (itk::ExceptionObject & err)
	//{
	//	std::cerr << "ExceptionObject caught !" << std::endl;
	//	std::cerr << err << std::endl;
	//	return NULL;
	//}
	//return resampler;
	return NULL;
}

ResampleFilterType::Pointer SizeMatch(DCMSeriesReader<PixelType, 3>fixReader, DCMSeriesReader<PixelType, 3>movReader)
{
	//IdentityTransformType::Pointer identityP = IdentityTransformType::New();
	//ResampleFilterType::Pointer resampler = ResampleFilterType::New();
	//resampler->SetTransform(identityP);
	//resampler->SetInput(movReader.GetOutput());
	//resampler->SetSize(fixReader.GetOutput()->GetLargestPossibleRegion().GetSize());
	//resampler->SetOutputOrigin(movReader.GetOutput()->GetOrigin());
	//resampler->SetOutputSpacing(movReader.GetOutput()->GetSpacing());
	//resampler->SetOutputDirection(movReader.GetOutput()->GetDirection());
	//resampler->SetDefaultPixelValue(0);
	//try
	//{
	//	resampler->Update();
	//}
	//catch (itk::ExceptionObject & err)
	//{
	//	std::cerr << "ExceptionObject caught !" << std::endl;
	//	std::cerr << err << std::endl;
	//	return NULL;
	//}
	//return resampler;
	return NULL;
}
ResampleFilterType::Pointer Resample(TransformType::Pointer transform, itk::ImageFileReader<itk::Image<signed short, 2>>::Pointer fixReader, itk::ImageFileReader<itk::Image<signed short, 2>>::Pointer movReader)
{
	//ResampleFilterType::Pointer resampler= ResampleFilterType::New();
	//resampler->SetTransform(transform);
	//resampler->SetInput(movReader->GetOutput());
	//resampler->SetSize(fixReader->GetOutput()->GetLargestPossibleRegion().GetSize());
	//resampler->SetOutputOrigin(fixReader->GetOutput()->GetOrigin());
	//resampler->SetOutputSpacing(fixReader->GetOutput()->GetSpacing());
	//resampler->SetOutputDirection(fixReader->GetOutput()->GetDirection());
	//resampler->SetDefaultPixelValue(255);
	//try
	//{
	//	resampler->Update();
	//}
	//catch (itk::ExceptionObject & err)
	//{
	//	std::cerr << "ExceptionObject caught !" << std::endl;
	//	std::cerr << err << std::endl;
	//	return NULL;
	//}
	//return resampler;
	return NULL;
}



