#include"Pipeline.h"
#include"PreDef.h"
#include"RawReader.h"
#include"RawWriter.h"
#include"TableRemoval.h"
#include"BonesRemoval.h"
#include"Registration.h"
#include"RegistrationInterfaceCommand.h"
#include"Utils.h"
using namespace predef;

template<typename T>
vector<T> Parse2Vec(T*params)
{
	vector<T>temp;
	for (unsigned int i = 0; i < inputDim; i++)
		temp.push_back(params[i]);
	return temp;
}
vector<unsigned int> Parse2VecUint(char *params)
{
	vector<unsigned int> temp;
	const char*split = ",";
	char*p=strtok(params,split);
	while (p != NULL)
	{
		temp.push_back(atoi(p));
		p = strtok(NULL, split);
	}
	return temp;
}
vector<float> Parse2VecFloat(char *params)
{
	vector<float> temp;
	const char*split = ",";
	char*p = strtok(params, split);
	while (p != NULL)
	{
		temp.push_back(atof(p));
		p = strtok(NULL, split);
	}
	return temp;
}
vector<double> Parse2VecDouble(char *params)
{
	vector<double> temp;
	const char*split = ",";
	char*p = strtok(params, split);
	while (p != NULL)
	{
		temp.push_back(atof(p));
		p = strtok(NULL, split);
	}
	return temp;
}
int RemovalPipelinev2(char*type,short*src,short*dst,short*bones,unsigned int* sizep, float* spacep, float* originp)
{ 
	ImportImageFilterType::Pointer ptr = ImportImageFilterType::New();
	ImportImageFilterType::RegionType region;
	ImportImageFilterType::IndexType start = { 0,0,0 };
	ImportImageFilterType::SizeType sizei = { sizep[0],sizep[1],sizep[2] };
	region.SetIndex(start);
	region.SetSize(sizei);
	ptr->SetRegion(region);
	itk::SpacePrecisionType ori[inputDim] = { originp[0],originp[1],originp[2] };
	itk::SpacePrecisionType spacing[inputDim] = { spacep[0],spacep[1],spacep[2] };
	ptr->SetSpacing(spacing);
	ptr->SetOrigin(ori);
	ptr->SetImportPointer(src,NumberOfPixels(sizep),false);
	ptr->Update();
	InputType::Pointer inputPtr =  ptr->GetOutput();
	InputType::Pointer resBone = CreateEmptyVolume(inputPtr);
	if (!strcmp(type, "-table"))
		TableRemovalPipe(inputPtr);
	if (!strcmp(type, "-headbone"))
	{
		BonesRemovalPipe(inputPtr, resBone, true);
	}
	if (!strcmp(type, "-bodybone"))
	{
		BonesRemovalPipe(inputPtr, resBone, false);
	}
	for (unsigned int i = 0; i < NumberOfPixels(sizep); i++)
		dst[i] = inputPtr->GetBufferPointer()[i];
	if (strcmp(type, "-table"))
	{
		for (unsigned int i = 0; i < NumberOfPixels(sizep); i++)
			bones[i] = resBone->GetBufferPointer()[i];
	}
	return EXIT_SUCCESS;
}
int RemovalPipeline(char*type,char*src,char*dst,char*dstBone,unsigned int* sizep,float* spacep,float* originp,bool isLittleEnd,bool is2Binary)
{
	vector<unsigned int> size = Parse2Vec<unsigned int>(sizep);
	vector<float> space = Parse2Vec<float>(spacep);
	vector<float> origin = Parse2Vec<float>(originp);
	InputType::Pointer inputPtr = InputType::New();
	RawReader<PixelType, inputDim> rawRead(size,space,origin,is2Binary,isLittleEnd);
	RawWriter<PixelType, inputDim> rawWrite(size, space, origin, is2Binary, isLittleEnd);
	RawWriter<PixelType, inputDim> rawWriteB(size, space, origin, is2Binary, isLittleEnd);

	rawRead.SetFilename(src);
	rawWrite.SetFilename(dst);
	rawWriteB.SetFilename(dstBone);
	rawRead.Run();
	inputPtr = rawRead.GetOutput();
	InputType::Pointer resBone = CreateEmptyVolume(inputPtr);
	if (!strcmp(type, "-table"))
		TableRemovalPipe(inputPtr);
	if (!strcmp(type, "-headbone"))
	{
		BonesRemovalPipe(inputPtr,resBone, true );
	}
	if (!strcmp(type, "-bodybone"))
	{
		BonesRemovalPipe(inputPtr,resBone, false);
	}
	rawWrite.SetInput(inputPtr);
	rawWrite.Run();
	if (strcmp(type,"-table"))
	{
		rawWriteB.SetInput(resBone);
		rawWriteB.Run();
	}
	return EXIT_SUCCESS;
}
int RegistrationPipelinev2(short*fix, short*mov, short*dst, unsigned int* sizeFP, float* spaceFP, float* originFP, unsigned int* sizeMP, float* spaceMP, float* originMP)
{
	//cast raw array to itk image type
	ImportRegType::Pointer ptr1 = ImportRegType::New();
	ImportRegType::RegionType region1;
	ImportRegType::IndexType start1 = { 0,0,0 };
	ImportRegType::SizeType size1 = { sizeFP[0],sizeFP[1],sizeFP[2] };
	region1.SetIndex(start1);
	region1.SetSize(size1);
	ptr1->SetRegion(region1);
	itk::SpacePrecisionType ori1[inputDim] = { originFP[0],originFP[1],originFP[2] };
	itk::SpacePrecisionType spacing1[inputDim] = { spaceFP[0],spaceFP[1],spaceFP[2] };
	ptr1->SetSpacing(spacing1);
	ptr1->SetOrigin(ori1);
	ptr1->SetImportPointer(fix, NumberOfPixels(sizeFP), false);
	ptr1->Update();
	FixedImageType::Pointer inputPtr1 = ptr1->GetOutput();

	ImportRegType::Pointer ptr2 = ImportRegType::New();
	ImportRegType::RegionType region2;
	ImportRegType::IndexType start2 = { 0,0,0 };
	ImportRegType::SizeType size2 = { sizeMP[0],sizeMP[1],sizeMP[2] };
	region2.SetIndex(start2);
	region2.SetSize(size2);
	ptr2->SetRegion(region2);
	itk::SpacePrecisionType ori2[inputDim] = { originMP[0],originMP[1],originMP[2] };
	itk::SpacePrecisionType spacing2[inputDim] = { spaceMP[0],spaceMP[1],spaceMP[2] };
	ptr2->SetSpacing(spacing2);
	ptr2->SetOrigin(ori2);
	ptr2->SetImportPointer(mov, NumberOfPixels(sizeMP), false);
	ptr2->Update();
	MovingImageType::Pointer inputPtr2 = ptr2->GetOutput();
	//registration
	RegistrationType::Pointer registration = RegistrationType::New();
	OptimizeStepPipe(inputPtr1, inputPtr2,registration);
	// post process
	ResampleFilterType::Pointer resample = ResampleFilterType::New();
	ResamplePipe(registration, inputPtr1, inputPtr2, resample);
	for (unsigned int i = 0; i < NumberOfPixels(sizeFP); i++)
		dst[i] = resample->GetOutput()->GetBufferPointer()[i];
	return EXIT_SUCCESS;
}

int RegistrationPipeline(char*fix, char*mov, char*dst, unsigned int* sizeFP, float* spaceFP, float* originFP, bool isLittleEndF, bool is2BinaryF, unsigned int* sizeMP, float* spaceMP, float* originMP, bool isLittleEndM, bool is2BinaryM)
{
	//python type convert to cxx type (default to process dim =3)
	vector<unsigned int> sizeF = Parse2Vec<unsigned int>(sizeFP);
	vector<float> spaceF = Parse2Vec<float>(spaceFP);
	vector<float> originF = Parse2Vec<float>(originFP);
	vector<unsigned int> sizeM = Parse2Vec<unsigned int>(sizeMP);
	vector<float> spaceM = Parse2Vec<float>(spaceMP);
	vector<float> originM = Parse2Vec<float>(originMP);
	//image io def
	RawReader<PixelType, RegInputDim> seriesFix(sizeF, spaceF, originF, is2BinaryF, isLittleEndF);
	RawReader<PixelType, RegInputDim> seriesMov(sizeM, spaceM, originM, is2BinaryM, isLittleEndM);
	seriesFix.SetFilename(fix);
	seriesMov.SetFilename(mov);
	seriesFix.Run();
	seriesMov.Run();
	//regitration
	RegistrationType::Pointer registration = RegistrationType::New();
	OptimizeStepPipe(seriesFix.GetOutput(), seriesMov.GetOutput(),registration);
	// post process
	ResampleFilterType::Pointer resample = ResampleFilterType::New();
	ResamplePipe(registration, seriesFix.GetOutput(), seriesMov.GetOutput(),resample);
	//writer def
	RawWriter<PixelType, RegInputDim> seriesW(sizeF,spaceF,originF,is2BinaryF,isLittleEndF);
	seriesW.SetFilename(dst);
	seriesW.SetInput(resample->GetOutput());
	seriesW.Run();
	return EXIT_SUCCESS;
}