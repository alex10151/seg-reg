#pragma once
#include "IOhandleBase.h"
#include "WriterBase.h"
#include"itkRawImageIO.h"

template<typename PixelType, unsigned int outputDim>
class RawWriter :public SingleWriterBase<PixelType, outputDim>, public SeriesIOHandler
{
	using OutputType = itk::Image<PixelType, outputDim>;
	using OutputWriter = itk::ImageFileWriter <OutputType>;
	using ImageIObase = itk::ImageIOBase;
private:
	typename itk::RawImageIO<PixelType, outputDim>::Pointer raw;
public:
	RawWriter
	(vector<unsigned int>size, vector<float>spacing, vector<float>origin, bool is2Binary = true, bool isLittleEndian = true)
		:SingleWriterBase<PixelType, outputDim>()
	{
		this->raw = itk::RawImageIO<PixelType, outputDim>::New();
		this->SetImageIO(raw.GetPointer());
		this->SetRawIOConfiguration(size, spacing, origin, is2Binary, isLittleEndian);
	};
	RawWriter(typename OutputWriter::Pointer writePtr)
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
	bool SetRawIOConfiguration(vector<unsigned int>size, vector<float>spacing, vector<float>origin, bool is2Binary, bool isLittleEndian)
	{
		this->raw->SetFileDimensionality(outputDim);
		this->raw->SetPixelType(itk::ImageIOBase::SCALAR);
		this->raw->SetNumberOfComponents(1);
		this->raw->SetHeaderSize(0);
		if (is2Binary)
			this->raw->SetFileTypeToBinary();
		if (isLittleEndian)
			this->raw->SetByteOrderToLittleEndian();
		else
			this->raw->SetByteOrderToBigEndian();
		for (unsigned int i = 0; i < size.size(); i++)
		{
			this->raw->SetDimensions(i, size.at(i));
		}
		for (unsigned int i = 0; i < spacing.size(); i++)
		{
			this->raw->SetSpacing(i, spacing.at(i));
		}
		for (unsigned int i = 0; i < origin.size(); i++)
		{
			this->raw->SetOrigin(i, origin.at(i));
		}
		return true;
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