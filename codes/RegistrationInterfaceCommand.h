#pragma once
#include"itkCommand.h"
#include"itkRegularStepGradientDescentOptimizer.h"

template <typename TRegistration>
class RegInterfaceCommand : public itk::Command
{
protected:
	RegInterfaceCommand() {};
public:
	using Self = RegInterfaceCommand;
	using Superclass = itk::Command;
	using Pointer = itk::SmartPointer<Self>;	
	using RegistrationType = TRegistration;
	using RegistrationPointer = RegistrationType * ;
	using OptimizerType = itk::RegularStepGradientDescentOptimizer;
	using OptimizerPointer = OptimizerType * ;
	itkNewMacro(Self);
	void Execute(itk::Object * object, const itk::EventObject & event)
	{
		using RegistrationType = TRegistration;
		using RegistrationPointer = RegistrationType * ;
		using OptimizerType = itk::RegularStepGradientDescentOptimizer;
		using OptimizerPointer = OptimizerType * ;
		if (!(itk::IterationEvent().CheckEvent(&event)))
		{
			return;
		}
		RegistrationPointer registration = static_cast<RegistrationPointer>(object);
		OptimizerPointer optimizer = static_cast<OptimizerPointer>(registration->GetModifiableOptimizer());
		std::cout << "-------------------------------------" << std::endl;
		std::cout << "MultiResolution Level : "
			<< registration->GetCurrentLevel() << std::endl;
		std::cout << std::endl;
		if (registration->GetCurrentLevel() == 0)
		{
			optimizer->SetMaximumStepLength(16.00);
			optimizer->SetMinimumStepLength(0.01);
		}
		else
		{
			optimizer->SetMaximumStepLength(
				optimizer->GetMaximumStepLength() * 0.25);
			optimizer->SetMinimumStepLength(
				optimizer->GetMinimumStepLength() * 0.1);
		}
	}
	void Execute(const itk::Object *, const itk::EventObject &)
	{
		return;
	}
};

