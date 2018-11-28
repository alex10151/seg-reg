%module SegAndRegPipe
%inline %{
#include "Pipeline.h"
%}
%include"carrays.i"
%array_class(float,FloatArray);
%array_class(unsigned int,UintArray);
%array_class(short,ShortArray);
%typemap(in) short * {
if (!SWIG_IsOK(SWIG_ConvertPtr($input, (void **) &$1, $descriptor(short *), 0))) {
SWIG_exception_fail(SWIG_TypeError, "in method '$symname', expecting type Foo");
}
}
%typemap(out) short * {
$result = SWIG_NewPointerObj($1,$descriptor(short*),1);
}
int RemovalPipeline(char*type, char*src, char*dst, char*dstBone, unsigned int* sizep, float* spacep, float* originp, bool isLittleEnd, bool is2Binary);
int RegistrationPipeline(char*fix, char*mov, char*dst, unsigned int* sizeFP, float* spaceFP, float* originFP, bool isLittleEndF, bool is2BinaryF, unsigned int* sizeMP, float* spaceMP, float* originMP, bool isLittleEndM, bool is2BinaryM);
int RemovalPipelinev2(char*type,short*src, short*dst, short*bones,unsigned int* sizep, float* spacep, float* originp);
int RegistrationPipelinev2(short*fix, short*mov, short*dst, unsigned int* sizeFP, float* spaceFP, float* originFP, unsigned int* sizeMP, float* spaceMP, float* originMP);