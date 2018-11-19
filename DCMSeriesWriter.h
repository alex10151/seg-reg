#pragma once
#include "IOhandleBase.h"
#include "WriterBase.h"

template<typename PixelType, unsigned int inputDim, unsigned int outputDim>
class DCMSeriesWriter :public SeriesWriterBase<PixelType,inputDim,outputDim>, public SeriesIOHandler
{
	using OutputType = itk::Image<PixelType, outputDim>;
	using CastType = itk::Image<PixelType, inputDim>;
	using OutputWriter = itk::ImageSeriesWriter<CastType, OutputType>;
	using ImageIObase = itk::ImageIOBase;
	using FilenamesContainer = itk::FilenamesContainer;
public:
	DCMSeriesWriter():SeriesWriterBase<PixelType,inputDim,outputDim>(){};
	DCMSeriesWriter(typename OutputWriter::Pointer writePtr)
	{
		if (writePtr.GetPointer())
			this->writePtr = writePtr;
	}
	bool SetImageIO(ImageIObase::Pointer base)
	{
		if (this->writePtr.GetPointer())
		{
			this->writePtr->SetImageIO(base);
			return true;
		}
		return false;
	}
	bool SetFilenames(FilenamesContainer filenames)
	{
		if (this->writePtr.GetPointer())
		{
			this->writePtr->SetFileNames(filenames);
			return true;
		}
		return false;
	}
	void SetMetaDicArray(const typename OutputWriter::DictionaryArrayType* dic)
	{
		this->writePtr->SetMetaDataDictionaryArray(dic);
	}
	int Run()
	{
		try
		{
			this->writePtr->Update();
		}
		catch (itk::ExceptionObject excep)
		{
			std::cerr << "writing files failed." << std::endl;
			std::cerr << excep << std::endl;
			return EXIT_FAILURE;
		}
		return EXIT_SUCCESS;
	}
};