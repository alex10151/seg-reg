#pragma once
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageSeriesReader.h"

template<typename PixelType,unsigned int inputDim=3>
class ReaderBase
{
	using InputType = itk::Image<PixelType, inputDim>;
public:
	virtual typename InputType::Pointer GetOutput()=0;
};

template<typename PixelType>
class SingleReaderBase :public ReaderBase<PixelType, 2>
{
	using InputType = itk::Image<PixelType, 2>;
	using SingleInputReader = itk::ImageFileReader<InputType>;
protected:
	typename SingleInputReader::Pointer readPtr;
public:
	SingleReaderBase()
	{
		this->readPtr = SingleInputReader::New();
	}
	typename InputType::Pointer GetOutput()
	{
		return this->readPtr->GetOutput();
	}
	typename SingleInputReader::Pointer GetReader()
	{
		return this->readPtr;
	}
};
template<typename PixelType,unsigned int inputDim=3>
class SeriesReaderBase:public ReaderBase<PixelType, inputDim>
{
	using InputType = itk::Image<PixelType, inputDim>;
	using InputReader = itk::ImageSeriesReader<InputType>;
protected:
	typename InputReader::Pointer readPtr;
public:
	SeriesReaderBase()
	{
		this->readPtr = InputReader::New();

	}
	typename InputReader::Pointer GetReader()
	{
		return this->readPtr;
	}
	typename InputType::Pointer GetOutput()
	{
		return this->readPtr->GetOutput();
	}
};

