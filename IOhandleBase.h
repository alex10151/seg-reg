#pragma once
#include "itkImageIOBase.h"

//for now only series class base
class SeriesIOHandler
{
public: 
	using ImageIObase = itk::ImageIOBase;
	virtual bool SetImageIO(ImageIObase::Pointer base) = 0;
	virtual int Run() = 0;
};

