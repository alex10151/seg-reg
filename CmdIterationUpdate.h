#pragma once
#include"itkCommand.h"
#include"itkMacro.h"
#include"PreDef.h"
using namespace predef;
class CommandIterationUpdate : public itk::Command
{
public:
	using Self = CommandIterationUpdate;
	using Superclass = itk::Command;
	using Pointer = itk::SmartPointer<Self>;
	itkNewMacro(Self);
protected:
	CommandIterationUpdate() = default;
public:
	using OptimizerPointer = const OptimizerType *;
	void Execute(itk::Object *caller, const itk::EventObject & event);
	void Execute(const itk::Object * object, const itk::EventObject & event);
};

