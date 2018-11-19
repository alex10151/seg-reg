#include "CmdIterationUpdate.h"

void CommandIterationUpdate::Execute(itk::Object *caller, const itk::EventObject & event)
{
	Execute((const itk::Object *)caller, event);
}
void CommandIterationUpdate::Execute(const itk::Object * object, const itk::EventObject & event)
{
	auto optimizer = static_cast<OptimizerPointer>(object);
	if (!itk::IterationEvent().CheckEvent(&event))
	{
		return;
	}
	std::cout << optimizer->GetCurrentIteration() << "   ";
	std::cout << optimizer->GetValue() << "   ";
	std::cout << optimizer->GetCurrentPosition() << std::endl;
}
