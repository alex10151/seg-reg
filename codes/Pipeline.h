#pragma once
#include<iostream>
#include<vector>

using namespace std;
//for removal from files(dicom)
int RemovalPipeline(char*type, char*src, char*dst, char*dstBone, unsigned int* sizep, float* spacep, float* originp, bool isLittleEnd, bool is2Binary);
//for registraion from files(dicom)
int RegistrationPipeline(char*fix, char*mov, char*dst, unsigned int* sizeFP, float* spaceFP, float* originFP, bool isLittleEndF, bool is2BinaryF, unsigned int* sizeMP, float* spaceMP, float* originMP, bool isLittleEndM, bool is2BinaryM);
//for removal interface
int RemovalPipelinev2(char*type,short*src, short*dst, short*bones,unsigned int* sizep, float* spacep, float* originp);
//for registration interface
int RegistrationPipelinev2(short*fix, short*mov, short*dst, unsigned int* sizeFP, float* spaceFP, float* originFP, unsigned int* sizeMP, float* spaceMP, float* originMP);