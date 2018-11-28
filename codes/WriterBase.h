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

template<typename PixelType, unsigned dim = 2>
class SingleWriterBase :public WriterBase<PixelType, dim>
{
	using OutputType = itk::Image<PixelType, dim>;
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
	bool SetFilename(std::string filename)
	{
		if (this->writePtr.GetPointer())
		{
			this->writePtr->SetFileName(filename);
			return true;
		}
		return false;
	}
};
template<typename PixelType, unsigned int inputDim = 3, unsigned int outputDim = 2>
class SeriesWriterBase :public WriterBase<PixelType, inputDim>
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
