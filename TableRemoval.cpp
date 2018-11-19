#include"TableRemoval.h"
#include"Utils.h"
#include<numeric>

float GetIntersection(float diag, float yMax,float zMax,float y1, float z1, float vy, float vz)
{
	float res1 = -1;
	float res2 = -1;
	for (int i = static_cast<int>(-diag); i < static_cast<int>(diag); i++)
	{
		float y = y1 + i * vy;
		float z = z1 + i * vz;
		if ((abs(z) < 1) && (y < yMax) && (y > 0))
		{
			 res1=y;
			 break;
		}
	}
	for (int i = static_cast<int>(diag); i > static_cast<int>(-diag); i--)
	{
		float y = y1 + i * vy;
		float z = z1 + i * vz;
		if ((abs(z-zMax) < 1) && (y < yMax) && (y > 0))
		{
			res2 = y;
			break;
		}
	}
	return (res1<res2)?res2:res1;
}
InputType::Pointer  TableRemoval(InputType::Pointer inputPtr, int size[], vector<vector<float>>structureSlice)
{
	itk::ImageRegionIterator<InputType> itrVolume(inputPtr, inputPtr->GetLargestPossibleRegion());
	int initIndex = structureSlice.at(0).at(0);
	int endIndex = structureSlice.at(structureSlice.size() - 1).at(0);
	for (int k = 0; k < size[2]; k++)
	{
		for (int i = initIndex; i <= endIndex; i++)
		{
			for (int j = 0; j < size[1]; j++)
			{
				InputType::IndexType index;
				index[0] = i;
				index[1] = j;
				index[2] = k;
				itrVolume.SetIndex(index);
				if (j >= structureSlice.at(i).at(1))
					itrVolume.Set(MIN_PIXEL_VALUE);
			}
		}
	}
	return inputPtr;
}

vector<SingleInputType::Pointer>  ConvertToSagittalImage(InputType::Pointer inputPtr, int size[])
{
	vector<SingleInputType::Pointer> sagittalContainer;
	itk::ImageRegionConstIterator<InputType> iter(inputPtr, inputPtr->GetRequestedRegion());
	int i = 0;
	for (int i = 0; i < size[0]; i++)
	{
		SingleInputType::Pointer slice = SingleInputType::New();

		SingleInputType::RegionType sliceRegion;
		SingleInputType::RegionType::IndexType sliceStart;
		sliceStart[0] = 0;
		sliceStart[1] = 0;
		SingleInputType::RegionType::SizeType sliceSize;
		sliceSize[0] = size[1];
		sliceSize[1] = size[2];
		sliceRegion.SetSize(sliceSize);
		sliceRegion.SetIndex(sliceStart);
		slice->SetRegions(sliceRegion);
		SingleInputType::PointType sliceOrigin;
		sliceOrigin[0] = inputPtr->GetOrigin()[1];
		sliceOrigin[1] = inputPtr->GetOrigin()[2];
		slice->SetOrigin(sliceOrigin);
		SingleInputType::SpacingType sliceSpacing;
		sliceSpacing[0] = inputPtr->GetSpacing()[1];
		sliceSpacing[1] = inputPtr->GetSpacing()[2];
		slice->SetSpacing(sliceSpacing);
		slice->Allocate();
		itk::ImageRegionIterator<SingleInputType> iterSlice(slice, sliceRegion);
		for (int j = 0; j < size[1]; j++)
		{
			for (int k = 0; k < size[2]; k++)
			{
				SingleInputType::IndexType dstIndex;
				InputType::IndexType srcIndex;
				srcIndex[0] = i;
				srcIndex[1] = j;
				srcIndex[2] = k;
				dstIndex[0] = j;
				dstIndex[1] = k;
				iter.SetIndex(srcIndex);
				iterSlice.SetIndex(dstIndex);
				iterSlice.Set(iter.Get());
			}
		}
		sagittalContainer.push_back(slice);
	}
	return sagittalContainer;
}
HT2DFilterType::LinesListType HoughTransformOnGradientImage(SingleInputType::Pointer srcImg)
{
	GradFilterType::Pointer gradFilter = GradFilterType::New();
	HT2DFilterType::Pointer htFilter = HT2DFilterType::New();
	BinaryThresholdFilter::Pointer bf = BinaryThresholdFilter::New();
	itk::ImageFileWriter<itk::Image<PixelTypeInt, 2>>::Pointer p1 = itk::ImageFileWriter<itk::Image<PixelTypeInt, 2>>::New();
	itk::GDCMImageIO::Pointer io = itk::GDCMImageIO::New();
	itk::CastImageFilter<SingleInputType, itk::Image<PixelTypeInt, 2>> ::Pointer cc = itk::CastImageFilter<SingleInputType, itk::Image<PixelTypeInt, 2>>::New();
	itk::SobelEdgeDetectionImageFilter<SingleInputType, SingleInputType>::Pointer sobelF = itk::SobelEdgeDetectionImageFilter<SingleInputType, SingleInputType>::New();
	//itk::CannyEdgeDetectionImageFilter<SingleInputType, SingleInputType>::Pointer cannyF = itk::CannyEdgeDetectionImageFilter<SingleInputType, SingleInputType>::New();
	         
	//using ball = itk::BinaryBallStructuringElement<PixelType>;
	//itk::BinaryErodeImageFilter<SingleInputType, SingleInputType, ball>::Pointer erodef = itk::BinaryErodeImageFilter<SingleInputType, SingleInputType, ball>::New();
	//ball element;
	//element.SetRadius(100);
	//element.CreateStructuringElement();
	//erodef->SetForegroundValue(1000);
	//erodef->SetKernel(element);

	gradFilter->SetInput(srcImg);
	bf->SetOutsideValue(MIN_PIXEL_VALUE);
	bf->SetInsideValue(MAX_PIXEL_VALUE);
	bf->SetLowerThreshold(THRESHOLD_LOW);
	bf->SetUpperThreshold(THRESHOLD_UP);
	//bf->SetInput(gradFilter->GetOutput());
	bf->SetInput(gradFilter->GetOutput());

	//cc->SetInput(bf1->GetOutput());
	//p1->SetFileName("ttttttt.dcm");
	//p1->SetImageIO(io);
	//p1->SetInput(cc->GetOutput());
	htFilter->SetDiscRadius(HOUGHTRANSFORM_RADIUS);
	htFilter->SetNumberOfLines(HOUGHTRANSFORM_LINES);
	htFilter->SetVariance(HOUGHTRANSFORM_VAR);
	//erodef->SetInput(bf1->GetOutput());
	//sobelF->SetInput(bf1->GetOutput());
	//cannyF->SetVariance(2);
	//cannyF->SetUpperThreshold(500);
	//cannyF->SetLowerThreshold(250);

	//cannyF->SetInput(srcImg);
	htFilter->SetInput(bf->GetOutput());
	cc->SetInput(htFilter->GetOutput());
	p1->SetInput(cc->GetOutput());
	//try
	//{
	//	p1->Update();
	//}
	//catch (itk::ExceptionObject &excep)
	//{
	//	cerr << excep << endl;
	//}
	try
	{
		htFilter->Update();
	}
	catch (itk::ExceptionObject &excep)
	{
		cerr << excep << endl;
	}

	return htFilter->GetLines();
}
float FindYofPoint(HT2DFilterType::LinesListType &lines, int size[])
{
	HT2DFilterType::LinesListType::const_iterator itr_lines = lines.begin();
	float diag = sqrt(size[1] * size[1] + size[2] * size[2]);
	vector<float> smin;
	int counter = 0;
	int targetIndex = 0;
	while (itr_lines != lines.end())
	{
		HT2DFilterType::LineType::PointListType::const_iterator itr_points = (*itr_lines)->GetPoints().begin();
		float u[2];
		float v[2];
		u[0] = itr_points->GetPosition()[0];
		u[1] = itr_points->GetPosition()[1];
		itr_points++;
		v[0] = u[0] - itr_points->GetPosition()[0];
		v[1] = u[1] - itr_points->GetPosition()[1];
		float norm = sqrt(v[0] * v[0] + v[1] * v[1]);
		v[0] = v[0] / norm;
		v[1] = v[1] / norm;
		if (abs(v[0]) > 0.2)
		{
			itr_lines++;
			counter++;
			continue;
		}
		float y = GetIntersection(diag, size[1], size[2], u[0], u[1], v[0], v[1]);
		if (y != -1)
			smin.push_back(y);
		itr_lines++;
		counter++;
	}
	for (unsigned int i = 0; i < smin.size(); i++)
		cout << smin.at(i) << endl;
	if (smin.size() == 0)
		return LIMIT;
	sort(smin.begin(), smin.end());
	float sum = accumulate(smin.begin(), smin.end(), 0);
	float mean = sum / smin.size();
	//return smin.at(10);
	//return mean;
	if (smin.size() <= 10)
	{
		return smin.at(smin.size() - 1);
	}
	else
	{
		return smin.at(10);
	}
}
InputType::Pointer  TableRemovalPipe(InputType::Pointer inputPtr, int size[])
{
	vector<SingleInputType::Pointer> sagiitalContainer = ConvertToSagittalImage(inputPtr, size);
	vector<vector<float>> pointsOfTransverse;
	for (unsigned int i = 0; i < sagiitalContainer.size(); i++)
	{
		vector<float> cord;
		//SingleInputType::Pointer crop = CropImage(sagiitalContainer.at(i),size[1]/2,0,size[1]/2,size[2]);
		HT2DFilterType::LinesListType lines = HoughTransformOnGradientImage(sagiitalContainer.at(i));
		cout << "the num of sagiital images:" << sagiitalContainer.size() << endl;
		cout << "this is epoch  :" << i << endl;
		if (lines.size() == 0)
			continue;
		//int cropSize[3] = {size[0],size[1]/2,size[2]};
		float y = FindYofPoint(lines, size);
		cout << "the y is :" << y << endl;
		cord.push_back(i);
		cord.push_back(y);
		pointsOfTransverse.push_back(cord);

	}
	pointsOfTransverse = PostProcess(pointsOfTransverse);
	//vector<vector<float>>::iterator itrs = pointsOfTransverse.begin();
	//while (itrs != pointsOfTransverse.end())
	//{
	//	(*itrs).at(1) = (*itrs).at(1) + size[1] / 2;
	//	itrs++;
	//}
	return TableRemoval(inputPtr, size, pointsOfTransverse);
}