#include"BonesRemoval.h"
#include"Utils.h"
#include"itkJPEGImageIO.h"

void  BonesRemoval(SingleInputType::Pointer inputPtr, SingleInputType::Pointer fusionImage, int size[])
{
	itk::ImageRegionIterator<SingleInputType> itrInput(inputPtr, inputPtr->GetRequestedRegion());
	itk::ImageRegionIterator<SingleInputType> itrFusion(fusionImage, fusionImage->GetRequestedRegion());
	for (int i = 0; i <= size[0]; i++)
	{
		for (int j = 0; j < size[1]; j++)
		{
			SingleInputType::IndexType index;
			index[0] = i;
			index[1] = j;
			itrFusion.SetIndex(index);
			itrInput.SetIndex(index);
			if (itrFusion.Get() > 0)
				itrInput.Set(MIN_PIXEL_VALUE);
		}
	}
}
void  BonesVisual(SingleInputType::Pointer inputPtr, SingleInputType::Pointer boneImage, int size[])
{
	itk::ImageRegionIterator<SingleInputType> itrInput(inputPtr, inputPtr->GetRequestedRegion());
	itk::ImageRegionIterator<SingleInputType> itrBone(boneImage, boneImage->GetRequestedRegion());
	for (int i = 0; i <= size[0]; i++)
	{
		for (int j = 0; j < size[1]; j++)
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

vector<SingleInputType::Pointer>  ConvertToTransverseImage(InputType::Pointer inputPtr, int size[])
{
	vector<SingleInputType::Pointer> TransverseContainer;
	itk::ImageRegionConstIterator<InputType> iter(inputPtr, inputPtr->GetLargestPossibleRegion());
	for (int i = 0; i < size[2]; i++) 
	{
		SingleInputType::Pointer slice = CreateEmptyTransverseFromVolume(inputPtr, size);
		itk::ImageRegionIterator<SingleInputType> iterSlice(slice,slice->GetLargestPossibleRegion());
		for (int j = 0; j < size[0]; j++)
		{
			for (int k = 0; k < size[1]; k++)
			{
				SingleInputType::IndexType dstIndex;
				InputType::IndexType srcIndex;
				srcIndex[0] = j;
				srcIndex[1] = k;
				srcIndex[2] = i;
				dstIndex[0] = j;
				dstIndex[1] = k;
				iter.SetIndex(srcIndex);
				iterSlice.SetIndex(dstIndex);
				iterSlice.Set(iter.Get());
			}
		}
		TransverseContainer.push_back(slice);
	}
	return TransverseContainer;
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
	//GradFilterType::Pointer gradFilter = GradFilterType::New();
	//itk::CannyEdgeDetectionImageFilter<SingleInputType, SingleInputType>::Pointer cannyF = itk::CannyEdgeDetectionImageFilter<SingleInputType, SingleInputType>::New();
	//cannyF->SetVariance(2);
	//cannyF->SetUpperThreshold(3000);
	//cannyF->SetLowerThreshold(1000);

	//gradFilter->SetInput(src);
	//cannyF->SetInput(src);
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

SingleInputType::Pointer  Fusion(SingleInputType::Pointer threImage, SingleInputType::Pointer lapImage, int size[])
{
	itk::ImageRegionIterator<SingleInputType> itrVolumeLap(lapImage, lapImage->GetLargestPossibleRegion());
	itk::ImageRegionIterator<SingleInputType> itrVolumeThre(threImage, threImage->GetLargestPossibleRegion());
	for (int i = 0; i <= size[0]; i++)
	{
		for (int j = 0; j < size[1]; j++)
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
SingleInputType::Pointer  OpOpen(SingleInputType::Pointer src)
{
	StructureElementType structuringElement;
	structuringElement.SetRadius(BALL_STRUCT_RADIUS);
	structuringElement.CreateStructuringElement();
	ErodeFilterType::Pointer erodeF = ErodeFilterType::New();
	DilateFilterType::Pointer dilateF = DilateFilterType::New();
	GrayDilateFilterType::Pointer gdilateF = GrayDilateFilterType::New();
	GrayErodeFilterType::Pointer gerodeF = GrayErodeFilterType::New();
	gdilateF->SetKernel(structuringElement);
	gerodeF->SetKernel(structuringElement);
	gerodeF->SetInput(src);
	gdilateF->SetInput(gerodeF->GetOutput());
	gdilateF->Update();
	return gdilateF->GetOutput();
	//erodeF->SetKernel(structuringElement);
	//dilateF->SetKernel(structuringElement);
	//erodeF->SetErodeValue(MIN_PIXEL_VALUE);
	//dilateF->SetDilateValue(MIN_PIXEL_VALUE);
	//erodeF->SetInput(src);
	//dilateF->SetInput(erodeF->GetOutput());
	//dilateF->Update();
	//return dilateF->GetOutput();
}
SingleInputType::Pointer  OpClose(SingleInputType::Pointer src)
{
	StructureElementType structuringElement;
	structuringElement.SetRadius(BALL_STRUCT_RADIUS+5);
	structuringElement.CreateStructuringElement();
	ErodeFilterType::Pointer erodeF = ErodeFilterType::New();
	DilateFilterType::Pointer dilateF = DilateFilterType::New();
	GrayDilateFilterType::Pointer gdilateF = GrayDilateFilterType::New();
	GrayErodeFilterType::Pointer gerodeF = GrayErodeFilterType::New();
	gdilateF->SetKernel(structuringElement);
	gerodeF->SetKernel(structuringElement);
	//erodeF->SetKernel(structuringElement);
	//dilateF->SetKernel(structuringElement);
	gdilateF->SetInput(src);
	gerodeF->SetInput(gdilateF->GetOutput());
	gerodeF->Update();
	return gerodeF->GetOutput();
	//erodeF->SetErodeValue(MIN_PIXEL_VALUE);
	//dilateF->SetDilateValue(MIN_PIXEL_VALUE);
	//dilateF->SetInput(src);
	//erodeF->SetInput(dilateF->GetOutput());
	//erodeF->Update();
	//return erodeF->GetOutput();
}
InputType::Pointer  BonesRemovalPipe(InputType::Pointer inputPtr, int size[],bool head,bool bonesExtract)
{
	itk::ImageFileWriter<SingleInputType>::Pointer singlep = itk::ImageFileWriter<SingleInputType>::New();
	itk::CastImageFilter<SingleInputType, itk::Image<unsigned char, 2>>::Pointer cast = itk::CastImageFilter<SingleInputType, itk::Image<unsigned char, 2>>::New();
	DcmioType::Pointer dcm = DcmioType::New();
	singlep->SetImageIO(dcm);
	MeanFilter::Pointer meanFilter = MeanFilter::New();
	GradFilterType::Pointer gradF = GradFilterType::New();

	vector<SingleInputType::Pointer>transContainer = ConvertToTransverseImage(inputPtr, size);

	vector<SingleInputType::Pointer>::const_iterator itr;
	int count = 0;
	//create a empty volume for bones visualization
	InputType::Pointer bones = CreateEmptyVolume(inputPtr,size);
	vector<SingleInputType::Pointer>bonesTransContainer = ConvertToTransverseImage(bones, size);
	vector<SingleInputType::Pointer>::const_iterator itrBones;

	for (itr=transContainer.begin(), itrBones=bonesTransContainer.begin(); itr!=transContainer.end(); itr++,itrBones++)
	{
		std::cout << "this is epoch"<<count <<std::endl;
		SingleInputType::Pointer sliceForLap = CreateEmptyTransverseFromVolume(inputPtr, size);
		SingleInputType::Pointer sliceForThresh = CreateEmptyTransverseFromVolume(inputPtr, size);

		CopyTransverse(*itr,sliceForLap,size);
		CopyTransverse(*itr,sliceForThresh ,size);


		//ContourFilterType::Pointer binaryContourFilter = ContourFilterType::New();
		//binaryContourFilter->SetInput(sliceForLap);
		//binaryContourFilter->FullyConnectedOn();
		//binaryContourFilter->SetForegroundValue(-1000);
		//binaryContourFilter->SetBackgroundValue(1000);
		//binaryContourFilter->Update();
		//sliceForLap = binaryContourFilter->GetOutput();
		//gradF->SetInput(sliceForThresh);
		
		//meanFilter->SetInput(sliceForThresh);
		//singlep->SetInput(meanFilter->GetOutput());
		//singlep->SetFileName("qqqqqqqqqqqqqq.dcm");
		//singlep->Update();

		if (head == true)
		{
			sliceForThresh= ThresholdProcess(sliceForThresh, HEAD_BONES_THRESHOLD);
			sliceForLap = ThresholdProcess(sliceForLap, HEAD_BONES_THRESHOLD);
			sliceForLap = LaplacianProcess(sliceForLap);
			//meanFilter->SetInput(sliceForLap);
			//meanFilter->Update();
			sliceForLap = Fusion(sliceForThresh, sliceForLap, size);
			//sliceForLap = OpOpen(sliceForLap);
			sliceForLap = OpClose(sliceForLap);
			BonesRemoval(*itr, sliceForLap, size);
		}
		else
		{
			sliceForThresh = ThresholdProcess(sliceForThresh, BONES_THRESHOLD);
			//meanFilter->SetInput(sliceForThresh);
			sliceForThresh = OpClose(sliceForThresh);
			sliceForThresh = HoleFill(sliceForThresh);
			BonesRemoval(*itr, sliceForThresh, size);
			BonesVisual(*itrBones, sliceForThresh, size);
		}
		count++;
	}
	CopyFromSeries(inputPtr, transContainer, size);
	CopyFromSeries(bones, bonesTransContainer, size);
	if (bonesExtract)
		return bones;
	else
		return inputPtr;
}

