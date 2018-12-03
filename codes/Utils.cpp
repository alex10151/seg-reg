
#include"Utils.h"

vector<SingleInputType::Pointer>  ConvertToTransverseImage(InputType::Pointer inputPtr)
{
	InputType::SizeType size = inputPtr->GetLargestPossibleRegion().GetSize();
	vector<SingleInputType::Pointer> TransverseContainer;
	itk::ImageRegionConstIterator<InputType> iter(inputPtr, inputPtr->GetLargestPossibleRegion());
	for (unsigned int i = 0; i < size[2]; i++)
	{
		SingleInputType::Pointer slice = CreateEmptyTransverseFromVolume(inputPtr);
		itk::ImageRegionIterator<SingleInputType> iterSlice(slice, slice->GetLargestPossibleRegion());
		for (unsigned int j = 0; j < size[0]; j++)
		{
			for (unsigned int k = 0; k < size[1]; k++)
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
vector<SingleInputType::Pointer>  ConvertToSagittalImage(InputType::Pointer inputPtr)
{
	InputType::SizeType size = inputPtr->GetLargestPossibleRegion().GetSize();
	vector<SingleInputType::Pointer> sagittalContainer;
	itk::ImageRegionConstIterator<InputType> iter(inputPtr, inputPtr->GetRequestedRegion());
	int i = 0;
	for (unsigned int i = 0; i < size[0]; i++)
	{
		SingleInputType::Pointer slice = CreateEmptySagittalFromVolume(inputPtr);
		itk::ImageRegionIterator<SingleInputType> iterSlice(slice, slice->GetLargestPossibleRegion());
		for (unsigned int j = 0; j < size[1]; j++)
		{
			for (unsigned int k = 0; k < size[2]; k++)
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
bool cmp(vector<float>a, vector<float>b)
{
	return a.at(1) < b.at(1);
}
vector<vector<float>> PostProcess(vector<vector<float>> &points)
{
	for (unsigned int i = 0; i < points.size(); i++)
	{
		points.at(i).at(1) -= 5;
	}
	//volatile float head;
	//volatile float tail;
	//for (unsigned int i = 0; i < points.size(); i++)
	//{
	//	if (points.at(i).at(1) != LIMIT)
	//	{
	//		head = points.at(i).at(0);
	//		break;
	//	}
	//}
	//for (unsigned int i = points.size() - 1; i>=0; i--)
	//{
	//	if (points.at(i).at(1) != LIMIT)
	//	{
	//		tail = points.at(i).at(0);
	//		break;
	//	}
	//}
	// set threshold (base point :median of lines)
	/*vector<vector<float>> tempPoints = points;
	sort(tempPoints.begin(), tempPoints.end(), cmp);
	for (unsigned int i = 0; i < points.size(); i++)
	{
		bool flag = false;
		if (((points.at(i).at(1) - tempPoints.at(points.size() / 2).at(1)) <= -MAX_THRESHOLD) && !flag)
		{
			points.at(i).at(1) += DEFAULT_RESIDUAL;
			flag = true;
		}
		if (((points.at(i).at(1) - tempPoints.at(points.size() / 2).at(1)) >= MAX_THRESHOLD) && !flag)
		{
			points.at(i).at(1) -= DEFAULT_RESIDUAL;
			flag = true;
		}
		if (flag)
			i--;
	}*/

	//median filter
	//vector<float>temp;
	//for (int i = head + WINDOW_SIZE / 2; i <= tail - WINDOW_SIZE / 2; i++)
	//{

	//	for (int j = -(WINDOW_SIZE / 2); j <= WINDOW_SIZE / 2; j++)
	//		temp.push_back(points.at(i + j).at(1));
	//	sort(temp.begin(), temp.end());
	//	points.at(i).at(1) = temp.at(WINDOW_SIZE / 2)-DEFAULT_RESIDUAL;
	//	temp.clear();
	//}

	////mean filter
	//vector<float>temp;
	//float sum;
	//float mean;
	//for (int i = head + WINDOW_SIZE/2; i <= tail - WINDOW_SIZE/2; i++)
	//{
	//	for (int j = -(WINDOW_SIZE/2); j <= WINDOW_SIZE/2; j++)
	//		temp.push_back(points.at(i + j).at(1));
	//	sort(temp.begin(), temp.end());
	//	sum = accumulate(temp.begin(), temp.end(), 0);
	//	mean = sum / temp.size();
	//	cout << "points at (" << i << ")(1)) is:    " << points.at(i).at(1) << endl;
	//	cout << "mean(points at (" << i << ")(1)) is:    " << mean - DEFAULT_RESIDUAL << endl;
	//	points.at(i).at(1) = mean - DEFAULT_RESIDUAL;
	//	temp.clear();
	//}
	return points;
}
SingleInputType::Pointer CropImage(SingleInputType::Pointer src, int originX, int originY, int cutX, int cutY)
{
	SingleInputType::Pointer crop = SingleInputType::New();
	SingleInputType::RegionType region;
	SingleInputType::RegionType::IndexType startIndex;
	startIndex[0] = 0;
	startIndex[1] = 0;
	SingleInputType::PointType origin;
	origin[0] = 0;
	origin[1] = 0;
	SingleInputType::RegionType::SizeType sliceSize;
	sliceSize[0] = cutX;
	sliceSize[1] = cutY;
	SingleInputType::SpacingType sliceSpacing;
	sliceSpacing[0] = src->GetSpacing()[0];
	sliceSpacing[1] = src->GetSpacing()[1];
	region.SetIndex(startIndex);
	region.SetSize(sliceSize);
	crop->SetRegions(region);
	crop->SetOrigin(origin);
	crop->SetSpacing(sliceSpacing);
	crop->Allocate();
	itk::ImageRegionIterator<SingleInputType> itrSrc(src,src->GetLargestPossibleRegion());
	itk::ImageRegionIterator<SingleInputType> itrCrop(crop, crop->GetLargestPossibleRegion());
	for (int i = originX; i < originX+cutX; i++)
	{
		for (int j = originY; j < originY+cutY; j++)
		{
			SingleInputType::IndexType indexSrc;
			indexSrc[0] = i;
			indexSrc[1] = j;
			SingleInputType::IndexType indexCrop;
			indexCrop[0] = i - originX;
			indexCrop[1] = j - originY;
			itrSrc.SetIndex(indexSrc);
			itrCrop.SetIndex(indexCrop);
			itrCrop.Set(itrSrc.Get());
		}
	}
	return crop;
}

void CopyFromSeriesTransverse(InputType::Pointer src, vector<SingleInputType::Pointer>transContainer)
{
	InputType::SizeType size = src->GetRequestedRegion().GetSize();
	itk::ImageRegionIterator<InputType> iterSrc(src, src->GetRequestedRegion());
	for (unsigned int i = 0; i < size[2]; i++)
	{
		itk::ImageRegionIterator<SingleInputType> iterSingle(transContainer.at(i), transContainer.at(i)->GetRequestedRegion());
		for (unsigned int j = 0; j < size[0]; j++)
		{
			for (unsigned int k = 0; k < size[1]; k++)
			{
				InputType::IndexType indexSrc;
				SingleInputType::IndexType indexSingle;
				indexSrc[0] = j;
				indexSrc[1] = k;
				indexSrc[2] = i;
				indexSingle[0] = j;
				indexSingle[1] = k;
				iterSrc.SetIndex(indexSrc);
				iterSingle.SetIndex(indexSingle);
				iterSrc.Set(iterSingle.Get());

			}
		}
	}
}
void CopyFromSeriesSagittal(InputType::Pointer src, vector<SingleInputType::Pointer>transContainer)
{
	InputType::SizeType size = src->GetRequestedRegion().GetSize();
	itk::ImageRegionIterator<InputType> iterSrc(src, src->GetRequestedRegion());
	for (unsigned int i = 0; i < size[0]; i++)
	{
		itk::ImageRegionIterator<SingleInputType> iterSingle(transContainer.at(i), transContainer.at(i)->GetRequestedRegion());
		for (unsigned int j = 0; j < size[1]; j++)
		{
			for (unsigned int k = 0; k < size[2]; k++)
			{
				InputType::IndexType indexSrc;
				SingleInputType::IndexType indexSingle;
				indexSrc[1] = j;
				indexSrc[2] = k;
				indexSrc[0] = i;
				indexSingle[0] = j;
				indexSingle[1] = k;
				iterSrc.SetIndex(indexSrc);
				iterSingle.SetIndex(indexSingle);
				iterSrc.Set(iterSingle.Get());

			}
		}
	}
}
void CopyVolume(InputType::Pointer src, InputType::Pointer dst)
{
	InputType::SizeType size = src->GetLargestPossibleRegion().GetSize();
	itk::ImageRegionIterator<InputType> iterSrc(src, src->GetLargestPossibleRegion());
	itk::ImageRegionIterator<InputType> iterDst(dst, dst->GetLargestPossibleRegion());
	for (unsigned int j = 0; j < size[0]; j++)
	{
		for (unsigned int k = 0; k < size[1]; k++)
		{
			for (unsigned int i = 0; i < size[2]; i++)
			{
				InputType::IndexType dstIndex;
				InputType::IndexType srcIndex;
				srcIndex[0] = j;
				srcIndex[1] = k;
				srcIndex[2] = i;
				dstIndex[0] = j;
				dstIndex[1] = k;
				dstIndex[2] = i;
				iterSrc.SetIndex(srcIndex);
				iterDst.SetIndex(dstIndex);
				iterDst.Set(iterSrc.Get());
			}
		}
	}
}
void CopySlice(SingleInputType::Pointer src, SingleInputType::Pointer dst)
{
	SingleInputType::SizeType size = src->GetLargestPossibleRegion().GetSize();
	itk::ImageRegionIterator<SingleInputType> iterSrc(src, src->GetLargestPossibleRegion());
	itk::ImageRegionIterator<SingleInputType> iterDst(dst, dst->GetLargestPossibleRegion());
	for (unsigned int j = 0; j < size[0]; j++)
	{
		for (unsigned int k = 0; k < size[1]; k++)
		{
			SingleInputType::IndexType dstIndex;
			SingleInputType::IndexType srcIndex;
			srcIndex[0] = j;
			srcIndex[1] = k;
			dstIndex[0] = j;
			dstIndex[1] = k;
			iterSrc.SetIndex(srcIndex);
			iterDst.SetIndex(dstIndex);
			iterDst.Set(iterSrc.Get());
		}
	}
}
SingleInputType::Pointer CreateEmptySagittalFromVolume(InputType::Pointer inputPtr)
{
	InputType::SizeType size = inputPtr->GetLargestPossibleRegion().GetSize();
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
	return slice;
}
InputType::Pointer CreateEmptyVolume(InputType::Pointer inputPtr)
{
	InputType::SizeType size = inputPtr->GetLargestPossibleRegion().GetSize();
	InputType::Pointer volume = InputType::New();

	InputType::RegionType region;
	InputType::RegionType::IndexType start;
	start[0] = 0;
	start[1] = 0;
	start[2] = 0;
	InputType::RegionType::SizeType vsize;
	vsize[0] = size[0];
	vsize[1] = size[1];
	vsize[2] = size[2];
	region.SetSize(vsize);
	region.SetIndex(start);
	volume->SetRegions(region);
	InputType::PointType origin;
	origin[0] = inputPtr->GetOrigin()[0];
	origin[1] = inputPtr->GetOrigin()[1];
	origin[2] = inputPtr->GetOrigin()[2];
	volume->SetOrigin(origin);
	InputType::SpacingType spacing;
	spacing[0] = inputPtr->GetSpacing()[0];
	spacing[1] = inputPtr->GetSpacing()[1];
	spacing[2] = inputPtr->GetSpacing()[2];
	volume->SetSpacing(spacing);
	volume->Allocate();
	return volume;
}
SingleInputType::Pointer CreateEmptyTransverseFromVolume(InputType::Pointer inputPtr)
{
	InputType::SizeType size = inputPtr->GetLargestPossibleRegion().GetSize();
	SingleInputType::Pointer slice = SingleInputType::New();

	SingleInputType::RegionType sliceRegion;
	SingleInputType::RegionType::IndexType sliceStart;
	sliceStart[0] = 0;
	sliceStart[1] = 0;
	SingleInputType::RegionType::SizeType sliceSize;
	sliceSize[0] = size[0];
	sliceSize[1] = size[1];
	sliceRegion.SetSize(sliceSize);
	sliceRegion.SetIndex(sliceStart);
	slice->SetRegions(sliceRegion);
	SingleInputType::PointType sliceOrigin;
	sliceOrigin[0] = inputPtr->GetOrigin()[0];
	sliceOrigin[1] = inputPtr->GetOrigin()[1];
	slice->SetOrigin(sliceOrigin);
	SingleInputType::SpacingType sliceSpacing;
	sliceSpacing[0] = inputPtr->GetSpacing()[0];
	sliceSpacing[1] = inputPtr->GetSpacing()[1];
	slice->SetSpacing(sliceSpacing);
	slice->Allocate();
	return slice;
}
int NumberOfPixels(const int size[])
{
	return size[0] * size[1] * size[2];
}
unsigned int NumberOfPixels(vector<unsigned int> size)
{
	return size[0] * size[1] * size[2];
}
unsigned int NumberOfPixels(unsigned int size[])
{
	return size[0] * size[1] * size[2];
}

int Cordinates(const Point cartesian, const int size[])
{
	return cartesian.z*size[0] * size[1] + cartesian.y*size[0] + cartesian.x;
}

bool CheckIndex(int index[], const int image_size[])
{
	for (int i = 0; i < 3; i++)
	{
		if ((index[i] >= image_size[i]) || (index[i] < 0))
			return false;
	}
	return true;
}
PixelType* GetResidualElement3D(const PixelType *src, const PixelType *structureElement, const Point &base, const int imgSize[])
{
	Point baseOfElement(1, 1, 1);
	PixelType resElement[27] = { 0 };
	int sizeOfelement[3] = { 3,3,3 };
	for (int i = -1; i < 3; i++)
	{
		for (int j = -1; j < 3; j++)
		{
			for (int k = -1; k < 3; k++)
			{
				Point baseNeighbor(base.x + i, base.y + j, base.z + k);
				Point baseOfElementNeighbor(baseOfElement.x + i, baseOfElement.y + j, baseOfElement.z + k);
				resElement[Cordinates(baseOfElementNeighbor, sizeOfelement)] = src[Cordinates(baseNeighbor, imgSize)] - structureElement[Cordinates(baseOfElementNeighbor, sizeOfelement)];
			}
		}
	}
	return resElement;
}
PixelType StructureMin3D(const PixelType *structureElement)
{
	PixelType min = MAX_PIXEL_VALUE;
	for (int i = 0; i < 27; i++)
	{
		if (structureElement[i] < min)
			min = structureElement[i];
	}
	return min;
}
bool IsEqual3D(const PixelType *src, const PixelType *structureElement, const Point &base, const int imgSize[])
{
	Point baseOfElement(1, 1, 1);
	int sizeOfElement[3] = { 3,3,3 };
	for (int i = -1; i < 3; i++)
	{
		for (int j = -1; j < 3; j++)
		{
			for (int k = -1; k < 3; k++)
			{
				Point baseNeighbor(base.x + i, base.y + j, base.z + k);
				Point baseElementNeighbor(baseOfElement.x + i, baseOfElement.y + j, baseOfElement.z + k);
				if (src[Cordinates(baseNeighbor, imgSize)] != structureElement[Cordinates(baseElementNeighbor, sizeOfElement)])
					return false;
			}
		}
	}
	return true;
}
int RearrangeDCMFiles(const vector<string>filenames, string outputDir)
{
	vector<string>::const_iterator itrBegin = filenames.begin();
	vector<string>::const_iterator itrEnd = filenames.end();
	int size = filenames.size();
	while (itrBegin != itrEnd)
	{
		SingleInputReader::Pointer singleInputPtr = SingleInputReader::New();
		SingleOutputWriter::Pointer singleOutputPtr = SingleOutputWriter::New();
		DcmioType::Pointer dcmPtr = DcmioType::New();
		singleInputPtr->SetImageIO(dcmPtr);
		singleOutputPtr->SetImageIO(dcmPtr);
		singleInputPtr->SetFileName(*itrBegin);
		singleOutputPtr->SetInput(singleInputPtr->GetOutput());
		try
		{
			singleInputPtr->Update();
		}
		catch (itk::ExceptionObject &excp)
		{
			std::cerr << excp << std::endl;
			return EXIT_FAILURE;
		}
		std::string value;
		string a = "\\";
		string dir = outputDir;
		if (dcmPtr->GetValueFromTag(IMAGE_INSTANCE_NUMBER_TAG, value))
		{
			value = value.substr(0, value.find_first_of(' '));
			value = to_string(stoi(value) - 1);
			for (unsigned int i = 0; i < 6 - (value).length(); i++)
			{
				a.append("0");
			}
			a.append(value);
			a.append(".dcm");
			singleOutputPtr->SetFileName(dir.append(a));
			try
			{
				singleOutputPtr->Update();
			}
			catch (itk::ExceptionObject excep)
			{
				std::cerr << excep << std::endl;
				return EXIT_FAILURE;
			}

		}
		itrBegin++;
	}
	return 1;
}
// binarize the image of the target connected components and background.
PixelType* BinaryThresholdProcess(const PixelType *src, const Graph&graph, int low , int high)
{
	int imgSize[3] = { graph.valueOfAxisX,graph.valueOfAxisY,graph.valueOfAxisZ };
	PixelType *res = new PixelType[NumberOfPixels(imgSize)];
	for (int i = 0; i < NumberOfPixels(imgSize); i++)
	{
		if (graph.weightMetrix[i] == 0)
			res[i] = low;
		else
			res[i] = high;
	}
	return res;
}