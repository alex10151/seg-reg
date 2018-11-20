#include "itkResampleImageFilter.h"
#include "itkAffineTransform.h"
#include"itkTranslationTransform.h"
#include "itkNearestNeighborInterpolateImageFunction.h"
#include	"itkBSplineInterpolateImageFunction.h"
#include"itkWindowedSincInterpolateImageFunction.h"
#include"itkGDCMImageIO.h"
#include"itkImageFileReader.h"
#include"itkImageFileWriter.h"
#include"itkCastImageFilter.h"
#include"itkShiftScaleImageFilter.h"
#include"itkRawImageIO.h"
#include "itkImageSeriesReader.h"
#include "itkImageSeriesWriter.h"
#include"itkImage.h"
#include "itkGDCMSeriesFileNames.h"
#include<vector>


template<typename TP, unsigned int D>
typename itk::Image<TP, D>::Pointer shift(typename itk::Image<TP, D>::Pointer source, int value) {
	auto filter = typename itk::ShiftScaleImageFilter<itk::Image<TP, D>, itk::Image<TP, D>>::New();
	filter->SetShift(value);
	filter->SetInput(source);
	filter->Update();
	return filter->GetOutput();
}

int main(int argc, char*argv[])
{
	using type = signed short;
	constexpr unsigned int Dimension = 3;
	using InputPixelType = signed short;
	using OutputPixelType = signed short;
	using InputImageType = itk::Image<InputPixelType, Dimension >;
	using ResampleImageType = itk::Image<double, Dimension>;
	using OutputImageType = itk::Image< OutputPixelType, 2 >;
	using FilterType = itk::ResampleImageFilter<InputImageType, InputImageType>;
	using TransformType = itk::AffineTransform< double, Dimension >;
	using TranslationType = itk::TranslationTransform<double, Dimension>;
	//using InterpolatorType = itk::NearestNeighborInterpolateImageFunction<
	//	itk::Image<double, 2>, double>;
	using InterpolatorType = itk::BSplineInterpolateImageFunction<
		InputImageType, double>;
	//using InterpolatorType = itk::WindowedSincInterpolateImageFunction<
	//	InputImageType, 5>;
	InputImageType::Pointer pp = InputImageType::New();
	FilterType::Pointer ffffff = FilterType::New();
	//TranslationType::Pointer transform = TranslationType::New();
	//TranslationType::OutputVectorType translation;
	//translation[0] = 256; // X translation in millimeters
	//translation[1] = 256; // Y translation in millimeters
	//transform->Translate(translation);
	itk::ImageFileReader<InputImageType>::Pointer reader = itk::ImageFileReader<InputImageType>::New();
	itk::ImageFileWriter<OutputImageType>::Pointer writer = itk::ImageFileWriter<OutputImageType>::New();
	itk::ImageSeriesReader<InputImageType>::Pointer seriesR = itk::ImageSeriesReader<InputImageType>::New();
	itk::ImageSeriesWriter<InputImageType, OutputImageType>::Pointer seriesW = itk::ImageSeriesWriter<InputImageType, OutputImageType>::New();
	itk::GDCMImageIO::Pointer dcm = itk::GDCMImageIO::New();
	itk::GDCMSeriesFileNames::Pointer filenames = itk::GDCMSeriesFileNames::New();

	itk::CastImageFilter<itk::Image<float, Dimension>, itk::Image<signed short, Dimension>>::Pointer caster = itk::CastImageFilter<itk::Image<float, Dimension>, itk::Image<signed short, Dimension>>::New();
	reader->SetImageIO(dcm);
	writer->SetImageIO(dcm);
	seriesR->SetImageIO(dcm);
	seriesW->SetImageIO(dcm);
	seriesW->SetMetaDataDictionaryArray(seriesR->GetMetaDataDictionaryArray());
	filenames->SetInputDirectory(argv[1]);
	filenames->SetOutputDirectory(argv[2]);
	seriesR->SetFileNames(filenames->GetInputFileNames());
	seriesW->SetFileNames(filenames->GetOutputFileNames());
	seriesR->Update();
	//auto rawio = itk::RawImageIO<OutputPixelType, Dimension>::New();

	//writer->SetImageIO(rawio);
	//writer->SetMetaDataDictionary(reader->GetMetaDataDictionary());
	//reader->SetFileName(argv[1]);
	//writer->SetFileName(argv[2]);
	//reader->Update();
	//for (int i = 0; i < 2; ++i) {
	//	rawio->SetSpacing(i, reader->GetOutput()->GetSpacing()[i]*2.0);
	//}

	//auto imageAdded = shift<InputPixelType, Dimension>(seriesR->GetOutput(), 5000);

	//auto double2input = itk::CastImageFilter<itk::Image<double, Dimension>, itk::Image<OutputPixelType, Dimension>>::New();
	//auto input2double = itk::CastImageFilter<itk::Image<InputPixelType, Dimension>, itk::Image<double, Dimension>>::New();

	//input2double->SetInput(imageAdded);

	TransformType::OutputVectorType affine;
	TransformType::Pointer trans = TransformType::New();
	//InputImageType::PointType origin1 = reader->GetOutput()->GetOrigin();
	//InputImageType::SpacingType spacing = reader->GetOutput()->GetSpacing();
	//InputImageType::SizeType size1 = reader->GetOutput()->GetLargestPossibleRegion().GetSize();
	InputImageType::PointType origin1 = seriesR->GetOutput()->GetOrigin();
	InputImageType::SpacingType spacing = seriesR->GetOutput()->GetSpacing();
	InputImageType::SizeType size1 = seriesR->GetOutput()->GetLargestPossibleRegion().GetSize();
	const double imageCenterX = origin1[0] + spacing[0] * size1[0] / 2.0;
	const double imageCenterY = origin1[1] + spacing[1] * size1[1] / 2.0;
	const double imageCenterZ = origin1[2] + spacing[2] * size1[2] / 2.0;
	//std::cout << origin1[0] << "  " << origin1[1] << "   " << std::endl;
	//std::cout << imageCenterX << "  " << imageCenterY << "   " << std::endl;
	affine[0] = -(imageCenterX);
	affine[1] = -(imageCenterY);
	affine[2] = 1;
	trans->Translate(affine);

	//const double degreestoradians = std::atan(1.0) / 45.0;
	//itk::Vector<double> v1;
	//v1[0] = 0.0;
	//v1[1] = 0.0;
	//v1[2] = 1.0;
	//trans->Rotate3D(v1, 10 * degreestoradians, false);
	//affine[0] = imageCenterX;
	//affine[1] = imageCenterY;
	//affine[2] = imageCenterZ;
	//trans->Translate(affine);

	//params.set_x(1.0);
	//params.set_y(2.0);
	//params.set_z(3.0);
	//params.set_t(1.0);
	//translation->SetParameters(params);
	InterpolatorType::Pointer interpolator = InterpolatorType::New();
	//filter1->SetTransform(transform);
	ffffff->SetTransform(trans);
	ffffff->SetInterpolator(interpolator);
	ffffff->SetDefaultPixelValue(100);

	// pixel spacing in millimeters along X and Y
	//const double spacing[Dimension] = { 0.4, 0.4 };
	//filter1->SetOutputSpacing(reader->GetOutput()->GetSpacing()*0.8);
	ffffff->SetOutputSpacing(spacing);
	// Physical space coordinate of origin for X and Y
	const double origin[Dimension] = {0, 0 };
	//filter1->SetOutputOrigin(reader->GetOutput()->GetOrigin());
	ffffff->SetOutputOrigin(origin1);
	InputImageType::DirectionType direction;
	direction.SetIdentity();
	//filter1->SetOutputDirection(direction);
	//filter1->SetOutputDirection(reader->GetOutput()->GetDirection());
	ffffff->SetOutputDirection(seriesR->GetOutput()->GetDirection());
	//InputImageType::SizeType size;
	//size[0] = 512; // number of pixels along X
	//size[1] = 512; // number of pixels along Y
	//filter1->SetSize(InputImageType::SizeType{ 600, 600,300 });
	ffffff->SetSize(size1);
	ffffff->SetInput(seriesR->GetOutput());
	//filter1->SetInput(input2double->GetOutput());
	ffffff->UpdateLargestPossibleRegion();
	//double2input->SetInput(filter1->GetOutput());
	//caster->SetInput(filter1->GetOutput());
	//auto imageRestored = shift<OutputPixelType, Dimension>(double2input->GetOutput(), -5000);
	//auto imageRestored2 = shift<OutputPixelType, Dimension>(imageRestored, 5000);
	//writer->SetInput(imageRestored2);
	////writer->SetInput(reader->GetOutput());
	//writer->Update();
	seriesW->SetInput(ffffff->GetOutput());
	seriesW->Update();
	return 0;
}