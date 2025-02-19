#include "ComplexCore/Filters/CopyFeatureArrayToElementArray.hpp"

#include "complex/DataStructure/DataArray.hpp"
#include "complex/DataStructure/DataStore.hpp"
#include "complex/Parameters/DataObjectNameParameter.hpp"
#include "complex/UnitTest/UnitTestCommon.hpp"

#include <catch2/catch.hpp>

using namespace complex;

namespace
{
const std::string k_CellFeatureIdsArrayName("FeatureIds");
const std::string k_CellTempArrayName("Cell Temperature");
const std::string k_FeatureDataArrayName("Feature Temperature");
} // namespace

TEST_CASE("ComplexCore::CopyFeatureArrayToElementArray: Parameter Check", "[Core][CopyFeatureArrayToElementArray]")
{
  // Instantiate the filter, a DataStructure object and an Arguments Object
  CopyFeatureArrayToElementArray filter;
  DataStructure dataStructure;
  Arguments args;

  // Create default Parameters for the filter.
  args.insertOrAssign(CopyFeatureArrayToElementArray::k_SelectedFeatureArrayPath_Key, std::make_any<DataPath>(DataPath{}));
  args.insertOrAssign(CopyFeatureArrayToElementArray::k_CellFeatureIdsArrayPath_Key, std::make_any<DataPath>(DataPath{}));
  args.insertOrAssign(CopyFeatureArrayToElementArray::k_CreatedArrayName_Key, std::make_any<DataObjectNameParameter::ValueType>(""));

  // Preflight the filter and check result
  auto preflightResult = filter.preflight(dataStructure, args);
  COMPLEX_RESULT_REQUIRE_INVALID(preflightResult.outputActions);
  REQUIRE(preflightResult.outputActions.errors().size() == 3);
  for(const Error& err : preflightResult.outputActions.errors())
  {
    REQUIRE(err.code == complex::FilterParameter::Constants::k_Validate_Empty_Value);
  }

  // Execute the filter and check the result
  auto executeResult = filter.execute(dataStructure, args);
  COMPLEX_RESULT_REQUIRE_INVALID(executeResult.result);
  REQUIRE(executeResult.result.errors().size() == 3);
  for(const Error& err : executeResult.result.errors())
  {
    REQUIRE(err.code == complex::FilterParameter::Constants::k_Validate_Empty_Value);
  }
}

using ListOfTypes = std::tuple<int8, uint8, int16, uint16, int32, uint32, int64, uint64, float32, float64>;
TEMPLATE_LIST_TEST_CASE("ComplexCore::CopyFeatureArrayToElementArray: Valid filter execution", "[Core][CopyFeatureArrayToElementArray]", ListOfTypes)
{
  DataStructure dataStructure;

  // Create Cell FeatureIds array
  Int32Array* cellFeatureIdsPtr = Int32Array::CreateWithStore<DataStore<int32>>(dataStructure, k_CellFeatureIdsArrayName, {{10, 3}}, {1});
  REQUIRE(cellFeatureIdsPtr != nullptr);
  Int32Array& cellFeatureIds = *cellFeatureIdsPtr;

  for(usize y = 0; y < 3; y++)
  {
    for(usize x = 0; x < 10; x++)
    {
      usize index = (10 * y) + x;
      cellFeatureIds[index] = static_cast<int32>(y);
    }
  }

  // Create a feature data array with 3 values
  DataArray<TestType>* avgTempValuePtr = DataArray<TestType>::template CreateWithStore<DataStore<TestType>>(dataStructure, k_FeatureDataArrayName, {3}, {1});
  REQUIRE(avgTempValuePtr != nullptr);
  DataArray<TestType>& avgTempValue = *avgTempValuePtr;

  for(int i = 0; i < 3; i++)
  {
    avgTempValue[i] = static_cast<TestType>(0);
  }

  // Create filter and set arguments
  CopyFeatureArrayToElementArray filter;
  Arguments args;

  args.insertOrAssign(CopyFeatureArrayToElementArray::k_SelectedFeatureArrayPath_Key, std::make_any<DataPath>(DataPath({k_FeatureDataArrayName})));
  args.insertOrAssign(CopyFeatureArrayToElementArray::k_CellFeatureIdsArrayPath_Key, std::make_any<DataPath>(DataPath({k_CellFeatureIdsArrayName})));
  args.insertOrAssign(CopyFeatureArrayToElementArray::k_CreatedArrayName_Key, std::make_any<DataObjectNameParameter::ValueType>(k_CellTempArrayName));

  // Preflight the filter
  auto preflightResult = filter.preflight(dataStructure, args);
  COMPLEX_RESULT_REQUIRE_VALID(preflightResult.outputActions);

  // Execute the filter
  auto executeResult = filter.execute(dataStructure, args);
  COMPLEX_RESULT_REQUIRE_VALID(executeResult.result);

  // Check the filter results
  DataArray<TestType>& createdElementArray = dataStructure.getDataRefAs<DataArray<TestType>>(DataPath({k_CellTempArrayName}));
  for(usize i = 0; i < createdElementArray.getNumberOfTuples(); i++)
  {
    int32 featureId = cellFeatureIds[i];
    TestType value = createdElementArray[i];
    TestType featureValue = avgTempValue[featureId];
    REQUIRE(value == featureValue);
  }
}
