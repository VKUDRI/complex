#include "ComplexCore/utils/CosOperator.hpp"
#include "ComplexCore/utils/CalculatorArray.hpp"

using namespace complex;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
CosOperator::CosOperator()
{
  setNumberOfArguments(1);
  setInfixToken("cos");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
CosOperator::~CosOperator() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CosOperator::calculate(DataStructure& dataStructure, CalculatorParameter::AngleUnits units, DataPath calculatedArrayPath, std::stack<ICalculatorArray::Pointer>& executionStack)
{
  CreateNewArrayTrig(dataStructure, units, calculatedArrayPath, executionStack, [](double num) -> double { return cos(num); });
}

// -----------------------------------------------------------------------------
CosOperator::Pointer CosOperator::NullPointer()
{
  return Pointer(static_cast<Self*>(nullptr));
}
