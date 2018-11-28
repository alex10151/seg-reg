#pragma once
#include "IOhandleBase.h"
#include "ReaderBase.h"

//unfinished class
template<typename PixelType, unsigned int inputDim=2>
class DCMSingleReader :public SingleReaderBase<PixelType, inputDim>
{
	using InputType = itk::Image<PixelType, inputDim>;
	using InputReader = itk::ImageFileReader<InputType>;
	using ImageIObase = itk::ImageIOBase;
};


template<typename PixelType, unsigned int inputDim = 3>
class DCMSeriesReader:public SeriesReaderBase<PixelType,inputDim>, public SeriesIOHandler
{
	using InputType = itk::Image<PixelType, inputDim>;
	using InputReader = itk::ImageSeriesReader<InputType>;
	using ImageIObase = itk::ImageIOBase;
	using FilenamesContainer = itk::FilenamesContainer;
public:
	itk::GDCMImageIO::Pointer dcm;
	DCMSeriesReader():SeriesReaderBase<PixelType, inputDim>()
	{
		this->dcm = itk::GDCMImageIO::New();
		this->SetImageIO(dcm.GetPointer());
	};
	DCMSeriesReader(typename InputReader::Pointer readPtr)
	{
		if (readPtr.GetPointer())
			this->readPtr = readPtr;
	}
	bool SetImageIO(ImageIObase::Pointer base)
	{
		if (this->readPtr.GetPointer())
		{
			this->readPtr->SetImageIO(base);
			return true;
		}
		return false;
	}
	bool SetFilenames(FilenamesContainer filenames)
	{
		if (this->readPtr.GetPointer())
		{
			this->readPtr->SetFileNames(filenames);
			return true;
		}
		return false;
	}
	const typename InputReader::DictionaryArrayType* GetMetaDicArray()
	{
		return this->readPtr->GetMetaDataDictionaryArray();
	}
	int Run()
	{
		try
		{
			this->readPtr->Update();
		}
		catch (itk::ExceptionObject excep)
		{
			std::cerr << "reading files failed" << std::endl;
			std::cerr << excep << std::endl;
			return EXIT_FAILURE;
		}
		return EXIT_SUCCESS;
	}
};

