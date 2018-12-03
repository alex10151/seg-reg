#include"TableRemoval.h"
#include"Utils.h"
#include<numeric>
#include"RawWriter.h"
#include"math.h"

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
InputType::Pointer TableRemoval(InputType::Pointer inputPtr, vector<vector<float>>structureSlice)
{
	InputType::SizeType size = inputPtr->GetLargestPossibleRegion().GetSize();
	itk::ImageRegionIterator<InputType> itrVolume(inputPtr, inputPtr->GetLargestPossibleRegion());
	int initIndex = structureSlice.at(0).at(0);
	int endIndex = structureSlice.at(structureSlice.size() - 1).at(0);
	for (unsigned int k = 0; k < size[2]; k++)
	{
		for (int i = initIndex; i <= endIndex; i++)
		{
			for (unsigned int j = 0; j < size[1]; j++)
			{
				InputType::IndexType index;
				index[0] = i;
				index[1] = j;
				index[2] = k;
				itrVolume.SetIndex(index);
				if (j >= structureSlice.at(i-initIndex).at(1))
					itrVolume.Set(MIN_PIXEL_VALUE);
			}
		}
	}
	return inputPtr;
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

	gradFilter->SetInput(srcImg);
	bf->SetOutsideValue(MIN_PIXEL_VALUE);
	bf->SetInsideValue(MAX_PIXEL_VALUE);
	bf->SetLowerThreshold(THRESHOLD_LOW);
	bf->SetUpperThreshold(THRESHOLD_UP);
	bf->SetInput(gradFilter->GetOutput());
	htFilter->SetDiscRadius(HOUGHTRANSFORM_RADIUS);
	htFilter->SetNumberOfLines(HOUGHTRANSFORM_LINES);
	htFilter->SetVariance(HOUGHTRANSFORM_VAR);
	htFilter->SetInput(bf->GetOutput());
	cc->SetInput(htFilter->GetOutput());
	p1->SetInput(cc->GetOutput());
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
float FindYofPoint(HT2DFilterType::LinesListType &lines, vector<short> size)
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
	if (smin.size() == 0)
		return LIMIT;
	sort(smin.begin(), smin.end());
	float sum = accumulate(smin.begin(), smin.end(), 0);
	float mean = sum / smin.size();
	//if (smin.size() <= 3)
	//{
	//	return smin.at(smin.size() - 1);
	//}
	//else
	//{
	//	return smin.at(3);
	//}
	return smin.at(0);
}
vector<short> Convert2Vec(InputType::SizeType size)
{
	vector<short>res;
	for(unsigned int i =0;i<size.Dimension;i++)
			res.push_back(size[i]);
	return res;
 }
InputType::Pointer TableRemovalPipe(InputType::Pointer inputPtr)
{
	InputType::SizeType size = inputPtr->GetLargestPossibleRegion().GetSize();
	vector<SingleInputType::Pointer> sagiitalContainer = ConvertToSagittalImage(inputPtr);
	vector<vector<float>> pointsOfTransverse;
	cout << "========Start to remove table:" << endl;
	for (unsigned int i = 0; i < sagiitalContainer.size(); i++)
	{
		vector<float> cord;
		HT2DFilterType::LinesListType lines = HoughTransformOnGradientImage(sagiitalContainer.at(i));
		cout << "======================== epoch  :" << i << endl;
		if (lines.size() == 0)
		{
			cord.push_back(i);
			cord.push_back(LIMIT);
			pointsOfTransverse.push_back(cord);
			continue; 
		}
		float y = FindYofPoint(lines, Convert2Vec(size));
		cord.push_back(i);
		cord.push_back(y);
		pointsOfTransverse.push_back(cord);

	}
	pointsOfTransverse = PostProcess(pointsOfTransverse);

	//table seg lines
	OutputType::Pointer temp = CreateEmptyTransverseFromVolume(inputPtr);
	itk::ImageRegionIterator<OutputType> lineItr(temp, temp->GetLargestPossibleRegion());
	OutputType::SizeType sizeL = temp->GetLargestPossibleRegion().GetSize();
	for (unsigned int i = 0; i < pointsOfTransverse.size(); i++)
	{
		if (pointsOfTransverse.at(i)[1] >sizeL[0])
			continue;
		OutputType::IndexType index;
		index[0] = pointsOfTransverse.at(i)[0];
		index[1] = floor(pointsOfTransverse.at(i)[1]); 
		lineItr.SetIndex(index);
		lineItr.Set(MAX_PIXEL_VALUE);
	}
	vector<unsigned int > size1 = { 512,512 };
	vector<float> space = { 0.9765625 ,0.9765625  };
	vector<float> ori = { 0,0 };
	RawWriter<PixelType, 2> raw(size1, space, ori, true, false);
	raw.SetFilename("asdsadsadqwewqe213213.raw");
	raw.SetInput(temp);
	raw.Run();

	inputPtr = TableRemoval(inputPtr, pointsOfTransverse);
	return inputPtr;
}