#pragma once
#include "itkImage.h"
#include "itkImageSeriesWriter.h"
#include "itkImageFileWriter.h"

template<typename PixelType, unsigned int inputDim = 3>
class WriterBase
{
	using CastType = itk::Image<PixelType, inputDim>;
	virtual void SetInput(CastType *src) = 0;
};

template<typename PixelType>
class SingleWriterBase :public WriterBase<PixelType, 2>
{
	using OutputType = itk::Image<PixelType, 2>;
	using SingleOutputWriter = itk::ImageFileWriter<OutputType>;
protected:
	typename SingleOutputWriter::Pointer writePtr;
public:
	SingleWriterBase()
	{
		writePtr = SingleOutputWriter::New();
	}
	typename SingleOutputWriter::Pointer GetWriter()
	{
		return this->writePtr;
	}
	void SetInput(OutputType *src)
	{
		if (this->writePtr.GetPointer())
			this->writePtr->SetInput(src);
	}
};
template<typename PixelType,unsigned int inputDim =3,unsigned int outputDim=2>
class SeriesWriterBase:public WriterBase<PixelType, inputDim>
{	
	using OutputType = itk::Image<PixelType, outputDim>;
	using CastType = itk::Image<PixelType, inputDim>;
	using OutputWriter = itk::ImageSeriesWriter<CastType, OutputType>;
protected:
	typename OutputWriter::Pointer writePtr;
public:
	SeriesWriterBase()
	{
		 writePtr = OutputWriter::New();
	}
	typename OutputWriter::Pointer GetWriter()
	{
		return this->writePtr;
	}
	void SetInput(CastType *src)
	{
		if (this->writePtr.GetPointer())
			this->writePtr->SetInput(src);
	}
};

