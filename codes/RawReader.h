#pragma once
#include "IOhandleBase.h"
#include "ReaderBase.h"
#include"itkRawImageIO.h"
#include"itkImage.h"
#include<vector>

using namespace std;
template<typename PixelType, unsigned int inputDim = 3>
class RawReader :public SingleReaderBase<PixelType, inputDim>, public SeriesIOHandler
{
	using InputType = itk::Image<PixelType, inputDim>;
	using InputReader = itk::ImageFileReader<InputType>;
	using ImageIObase = itk::ImageIOBase;
private:
	typename itk::RawImageIO<PixelType, inputDim>::Pointer raw;
public:
	RawReader
	(vector<unsigned int>size, vector<float>spacing, vector<float>origin, bool is2Binary = true, bool isLittleEndian = true)
		:SingleReaderBase<PixelType, inputDim>()
	{
		this->raw = itk::RawImageIO<PixelType, inputDim>::New();
		this->SetImageIO(raw.GetPointer());
		this->SetRawIOConfiguration(size, spacing, origin, is2Binary, isLittleEndian);
	};
	RawReader(typename InputReader::Pointer readPtr)
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
	bool SetRawIOConfiguration(vector<unsigned int>size, vector<float>spacing, vector<float>origin, bool is2Binary, bool isLittleEndian)
	{
		this->raw->SetFileDimensionality(inputDim);
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