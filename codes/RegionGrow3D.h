#pragma once
#include "itkConfidenceConnectedImageFilter.h"
#include"itkCurvatureFlowImageFilter.h"
#include "itkCastImageFilter.h"


//using pixelType = float;
//using pixelTypeInt = signed short;
template<typename PixelType= float, typename PixelTypeInt=signed short,unsigned int inputDim=3>
class RegionGrowWithConfidenceMethod
{
	using InternalType = itk::Image<PixelType, inputDim>;
	using CastType = itk::Image<PixelTypeInt, inputDim>; 
	using ConfidenceConnectedImageFilter = itk::ConfidenceConnectedImageFilter<InternalType, InternalType>;
	using CurvatureImageFiltertype = itk::CurvatureFlowImageFilter<InternalType, InternalType>;
	using CastFloatToUnsignedIntType = itk::CastImageFilter<InternalType, CastType >;
    typedef typename InternalType::IndexType  IndexType;
private:
	typename ConfidenceConnectedImageFilter::Pointer confidentPtr;
	typename CurvatureImageFiltertype::Pointer curvaturePtr;
	typename CastFloatToUnsignedIntType::Pointer castPtr;
	const InternalType *source;
public:
	RegionGrowWithConfidenceMethod(const InternalType *source)
	{
		this->confidentPtr = ConfidenceConnectedImageFilter::New();
		this->curvaturePtr = CurvatureImageFiltertype::New();
		this->castPtr = CastFloatToUnsignedIntType::New();
		this->source = source;
	};
	RegionGrowWithConfidenceMethod()
	{
		this->confidentPtr = ConfidenceConnectedImageFilter::New();
		this->curvaturePtr = CurvatureImageFiltertype::New();
		this->castPtr = CastFloatToUnsignedIntType::New();
	};

	bool SetInput(const InternalType *source)
	{
		if (!this->source)
		{
			this->source = source;
			return true;
		}
		else
		{
			return false;
		}
	};

	CastType* GetProcessedImage()
	{
		if (!this->source)
			return 0;
		this->curvaturePtr->SetInput(this->source);
		this->confidentPtr->SetInput(this->curvaturePtr->GetOutput());
		this->castPtr->SetInput(this->confidentPtr->GetOutput());
		return this->castPtr->GetOutput();
	};

	void configCurvatureFilter(float timeStep = 0.125, int itrNum = 5)
	{
		this->curvaturePtr->SetTimeStep(timeStep);
		this->curvaturePtr->SetNumberOfIterations(itrNum);
	}

	void configConfidenceConnectedImageFilter(const IndexType seed, unsigned int radius = 3, double mutiplier = 3, unsigned int iterNum = 3, PixelType replaceValue = 800)
	{
		this->confidentPtr->SetInitialNeighborhoodRadius(radius);
		this->confidentPtr->SetMultiplier(mutiplier);
		this->confidentPtr->SetNumberOfIterations(iterNum);
		this->confidentPtr->SetReplaceValue(replaceValue);
		this->confidentPtr->SetSeed(seed);
	}
};