#include"BonesRemoval.h"
#include"Utils.h"
#include"itkMeanImageFilter.h"
void  BonesRemoval(InputType::Pointer inputPtr, InputType::Pointer rmvStruct)
{
	InputType::SizeType size = inputPtr->GetLargestPossibleRegion().GetSize();
	itk::ImageRegionIterator<InputType> itrInput(inputPtr, inputPtr->GetRequestedRegion());
	itk::ImageRegionIterator<InputType> itrRmv(rmvStruct, rmvStruct->GetRequestedRegion());
	for (unsigned int i = 0; i <= size[0]; i++)
	{
		for (unsigned int j = 0; j < size[1]; j++)
		{
			for (unsigned int k = 0; k < size[2]; k++)
			{
				InputType::IndexType index;
				index[0] = i;
				index[1] = j;
				index[2] = k;
				itrRmv.SetIndex(index);
				itrInput.SetIndex(index);
				if (itrRmv.Get() > 0)
					itrInput.Set(MIN_PIXEL_VALUE);
			}
		}
	}
}
void  BonesVisual(SingleInputType::Pointer inputPtr, SingleInputType::Pointer boneImage)
{
	SingleInputType::SizeType size = inputPtr->GetLargestPossibleRegion().GetSize();
	itk::ImageRegionIterator<SingleInputType> itrInput(inputPtr, inputPtr->GetRequestedRegion());
	itk::ImageRegionIterator<SingleInputType> itrBone(boneImage, boneImage->GetRequestedRegion());
	for (unsigned int i = 0; i <= size[0]; i++)
	{
		for (unsigned int j = 0; j < size[1]; j++)
		{
			SingleInputType::IndexType index;
			index[0] = i;
			index[1] = j;
			itrBone.SetIndex(index);
			itrInput.SetIndex(index);
			if (itrBone.Get() > 0)
				itrInput.Set(MAX_PIXEL_VALUE);
		}
	}
}
SingleInputType::Pointer  ThresholdProcess(SingleInputType::Pointer inputPtr, int bonesThresh)
{
	BinaryThresholdFilter::Pointer thresholdPtr = BinaryThresholdFilter::New();
	thresholdPtr->SetOutsideValue(MIN_PIXEL_VALUE);
	thresholdPtr->SetInsideValue(MAX_PIXEL_VALUE);
	thresholdPtr->SetUpperThreshold(MAX_PIXEL_VALUE);
	thresholdPtr->SetLowerThreshold(bonesThresh);
	thresholdPtr->SetInput(inputPtr);
	thresholdPtr->Update();
	return thresholdPtr->GetOutput();
}

SingleInputType::Pointer  HoleFill(SingleInputType::Pointer src)
{
	FillHoleType::Pointer fillHoleFilter = FillHoleType::New();
	fillHoleFilter->SetInput(src);
	fillHoleFilter->Update();
	return fillHoleFilter->GetOutput();
}
SingleInputType::Pointer  LaplacianProcess(SingleInputType::Pointer src)
{
	LoGFilterType::Pointer laplaceF = LoGFilterType::New();
	laplaceF->SetNormalizeAcrossScale(true);
	laplaceF->SetSigma(LAPLACE_SIGMA);
	laplaceF->SetInput(src);
	try
	{
		laplaceF->Update();
	}
	catch (itk::ExceptionObject& excep)
	{
		cout << excep << endl;
	}
	return laplaceF->GetOutput();
}

SingleInputType::Pointer  Fusion(SingleInputType::Pointer threImage, SingleInputType::Pointer lapImage)
{
	SingleInputType::SizeType size = lapImage->GetLargestPossibleRegion().GetSize();
	itk::ImageRegionIterator<SingleInputType> itrVolumeLap(lapImage, lapImage->GetLargestPossibleRegion());
	itk::ImageRegionIterator<SingleInputType> itrVolumeThre(threImage, threImage->GetLargestPossibleRegion());
	for (unsigned int i = 0; i <= size[0]; i++)
	{
		for (unsigned int j = 0; j < size[1]; j++)
		{
			SingleInputType::IndexType index;
			index[0] = i;
			index[1] = j;
			itrVolumeThre.SetIndex(index);
			itrVolumeLap.SetIndex(index);
			if (itrVolumeThre.Get() >MIN_PIXEL_VALUE)
				itrVolumeLap.Set(itrVolumeThre.Get());
		}
	}
	return lapImage;
}
SingleInputType::Pointer  OpOpen(SingleInputType::Pointer src,float radius)
{
	StructureElementType structuringElement;
	structuringElement.SetRadius(radius);
	structuringElement.CreateStructuringElement();
	GrayDilateFilterType::Pointer gdilateF = GrayDilateFilterType::New();
	GrayErodeFilterType::Pointer gerodeF = GrayErodeFilterType::New();
	gdilateF->SetKernel(structuringElement);
	gerodeF->SetKernel(structuringElement);
	gerodeF->SetInput(src);
	gdilateF->SetInput(gerodeF->GetOutput());
	gdilateF->Update();
	return gdilateF->GetOutput();
}
SingleInputType::Pointer  OpClose(SingleInputType::Pointer src,float radius)
{
	StructureElementType structuringElement;
	structuringElement.SetRadius(radius);
	structuringElement.CreateStructuringElement();
	GrayDilateFilterType::Pointer gdilateF = GrayDilateFilterType::New();
	GrayErodeFilterType::Pointer gerodeF = GrayErodeFilterType::New();
	gdilateF->SetKernel(structuringElement);
	gerodeF->SetKernel(structuringElement);
	gdilateF->SetInput(src);
	gerodeF->SetInput(gdilateF->GetOutput());
	gerodeF->Update();
	return gerodeF->GetOutput();

}
InputType::Pointer SmoothProcess(InputType::Pointer src)
{
	vector<SingleInputType::Pointer> res  = ConvertToSagittalImage(src);
	vector<SingleInputType::Pointer>::iterator itr ;
	InputType::Pointer volume = InputType::New();
	MeanFilter::Pointer mean = MeanFilter::New();
	MedianType::Pointer median = MedianType::New();
	CurvatureImageFilterType::Pointer curve = CurvatureImageFilterType::New();
	itk::CastImageFilter < itk::Image<PixelType, 2>, itk::Image<float, 2> >::Pointer cast1 = itk::CastImageFilter < itk::Image<PixelType, 2>, itk::Image<float, 2> >::New();
	itk::CastImageFilter < itk::Image<float, 2>, itk::Image<PixelType, 2>>::Pointer cast2 = itk::CastImageFilter < itk::Image<float, 2>,itk::Image<PixelType, 2> >::New();

	for (itr = res.begin(); itr != res.end(); itr++)
	{
		mean->SetInput(*itr);
		mean->SetRadius(1.5);
		mean->Update();
		CopySlice(mean->GetOutput(), *itr);
	}
	volume = CreateEmptyVolume(src);
	CopyFromSeriesSagittal(volume,res);
	return volume;
}
void BonesRemovalPipe(InputType::Pointer inputPtr, InputType::Pointer resBone,bool head)
{
	InputType::SizeType size = inputPtr->GetLargestPossibleRegion().GetSize();
	vector<SingleInputType::Pointer>transContainer = ConvertToTransverseImage(inputPtr);
	vector<SingleInputType::Pointer>::const_iterator itr;
	int count = 0;
	int p = transContainer.size();
	//create a empty volume for bones visualization
	InputType::Pointer bones = CreateEmptyVolume(inputPtr);
	vector<SingleInputType::Pointer>bonesTransContainer = ConvertToTransverseImage(bones);
	vector<SingleInputType::Pointer>::const_iterator itrBones;
	cout << "======Start to Remove bones:" << endl;
	for (itr=transContainer.begin(), itrBones=bonesTransContainer.begin(); itr!=transContainer.end(); itr++,itrBones++)
	{
		std::cout << "===================== epoch"<<count <<std::endl;
		SingleInputType::Pointer sliceForLap = CreateEmptyTransverseFromVolume(inputPtr);
		SingleInputType::Pointer sliceForThresh = CreateEmptyTransverseFromVolume(inputPtr);

		CopySlice(*itr,sliceForLap);
		CopySlice(*itr,sliceForThresh);

		if (head)
		{
			sliceForThresh= ThresholdProcess(sliceForThresh, HEAD_BONES_THRESHOLD);
			sliceForLap = ThresholdProcess(sliceForLap, HEAD_BONES_THRESHOLD);
			sliceForLap = LaplacianProcess(sliceForLap);
			sliceForLap = Fusion(sliceForThresh, sliceForLap);
			sliceForLap = OpClose(sliceForLap, BALL_STRUCT_RADIUS);
			BonesVisual(*itrBones, sliceForLap);
		}
		else
		{
			sliceForThresh = ThresholdProcess(sliceForThresh, BONES_THRESHOLD);
			sliceForThresh = OpClose(sliceForThresh, BALL_STRUCT_RADIUS+5);
			sliceForThresh = HoleFill(sliceForThresh);
			BonesVisual(*itrBones, sliceForThresh);
		}
		count++;
	}
	CopyFromSeriesTransverse(bones, bonesTransContainer);
	if(!head)
		bones = SmoothProcess(bones);
	BonesRemoval(inputPtr, bones);
	CopyVolume(bones, resBone);
}

