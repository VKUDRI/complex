#include "ExampleFilter2.hpp"

#include "complex/Common/StringLiteral.hpp"
#include "complex/Parameters/ArrayCreationParameter.hpp"
#include "complex/Parameters/ArraySelectionParameter.hpp"
#include "complex/Parameters/BoolParameter.hpp"
#include "complex/Parameters/ChoicesParameter.hpp"
#include "complex/Parameters/DataGroupCreationParameter.hpp"
#include "complex/Parameters/DataGroupSelectionParameter.hpp"
#include "complex/Parameters/DataPathSelectionParameter.hpp"
#include "complex/Parameters/DynamicTableParameter.hpp"
#include "complex/Parameters/GeometrySelectionParameter.hpp"
#include "complex/Parameters/MultiArraySelectionParameter.hpp"
#include <any>

using namespace complex;

namespace
{
constexpr StringLiteral k_Param1 = "param1";
constexpr StringLiteral k_Param2 = "param2";
constexpr StringLiteral k_Param3 = "param3";
constexpr StringLiteral k_Param4 = "param4";
constexpr StringLiteral k_Param5 = "param5";
constexpr StringLiteral k_Param6 = "param6";
constexpr StringLiteral k_Param7 = "param7";
constexpr StringLiteral k_Param8 = "param8";
constexpr StringLiteral k_Param9 = "param9";
constexpr StringLiteral k_Param10 = "param10";
constexpr StringLiteral k_Param11 = "param11";
constexpr StringLiteral k_Param12 = "param12";
constexpr StringLiteral k_Param13 = "param13";
constexpr StringLiteral k_Param14 = "param14";
constexpr StringLiteral k_Param15 = "param15";
constexpr StringLiteral k_Param16 = "param16";
} // namespace

namespace complex
{
//------------------------------------------------------------------------------
std::string ExampleFilter2::name() const
{
  return FilterTraits<ExampleFilter2>::name;
}

//------------------------------------------------------------------------------
std::string ExampleFilter2::className() const
{
  return FilterTraits<ExampleFilter2>::className;
}

//------------------------------------------------------------------------------
Uuid ExampleFilter2::uuid() const
{
  return FilterTraits<ExampleFilter2>::uuid;
}

//------------------------------------------------------------------------------
std::string ExampleFilter2::humanName() const
{
  return "Example Filter 2";
}

//------------------------------------------------------------------------------
std::vector<std::string> ExampleFilter2::defaultTags() const
{
  return {"Example", "Test"};
}

//------------------------------------------------------------------------------
Parameters ExampleFilter2::parameters() const
{
  Parameters params;
  params.insertSeparator({"1rst Group of Parameters"});
  params.insertLinkableParameter(std::make_unique<BoolParameter>(k_Param7, "Bool Parameter", "Example bool help text", true));
  params.insertLinkableParameter(std::make_unique<ChoicesParameter>(k_Param3, "ChoicesParameter", "Example choices help text", 0, ChoicesParameter::Choices{"foo", "bar", "baz"}));

  params.insertSeparator({"2nd Group of Parameters"});
  DynamicTableInfo tableInfo;
  tableInfo.setColsInfo(DynamicTableInfo::DynamicVectorInfo(2, "Col {}"));
  tableInfo.setRowsInfo(DynamicTableInfo::DynamicVectorInfo(0, "Row {}"));
  DynamicTableInfo::TableDataType defaultTable{{{10, 20}, {30, 40}}};
  params.insert(std::make_unique<DynamicTableParameter>(k_Param13, "DynamicTableParameter", "DynamicTableParameter Example Help Text", defaultTable, tableInfo));

  // These should show up under the "Required Objects" Section in the GUI
  params.insert(std::make_unique<DataGroupSelectionParameter>(k_Param9, "DataGroupSelectionParameter", "Example data group selection help text", DataPath{},
                                                              DataGroupSelectionParameter::AllowedTypes{BaseGroup::GroupType::DataGroup}));
  params.insert(std::make_unique<DataPathSelectionParameter>(k_Param10, "DataPathSelectionParameter", "Example data path selection help text", DataPath{}));
  params.insert(std::make_unique<ArraySelectionParameter>(k_Param6, "Array Selection", "Example array selection help text", ArraySelectionParameter::ValueType{}, complex::GetAllDataTypes()));
  params.insert(std::make_unique<GeometrySelectionParameter>(k_Param11, "GeometrySelectionParameter", "Example geometry selection help text", DataPath{}, GeometrySelectionParameter::AllowedTypes{}));
  params.insert(std::make_unique<MultiArraySelectionParameter>(k_Param12, "MultiArraySelectionParameter", "Example multiarray selection help text", MultiArraySelectionParameter::ValueType{},
                                                               MultiArraySelectionParameter::AllowedTypes{IArray::ArrayType::Any}, complex::GetAllDataTypes()));

  params.linkParameters(k_Param7, k_Param9, std::make_any<BoolParameter::ValueType>(true));

  params.linkParameters(k_Param3, k_Param10, std::make_any<ChoicesParameter::ValueType>(0));
  params.linkParameters(k_Param3, k_Param6, std::make_any<ChoicesParameter::ValueType>(1));
  params.linkParameters(k_Param3, k_Param11, std::make_any<ChoicesParameter::ValueType>(2));

  // These should show up under the "Created Objects" section in the GUI
  params.insert(std::make_unique<DataGroupCreationParameter>(k_Param8, "DataGroupCreationParameter", "Example data group creation help text", DataPath{}));
  params.insert(std::make_unique<ArrayCreationParameter>(k_Param5, "Array Creation", "Example array creation help text", ArrayCreationParameter::ValueType{}));

  return params;
}

//------------------------------------------------------------------------------
IFilter::UniquePointer ExampleFilter2::clone() const
{
  return std::make_unique<ExampleFilter2>();
}

//------------------------------------------------------------------------------
IFilter::PreflightResult ExampleFilter2::preflightImpl(const DataStructure& data, const Arguments& args, const MessageHandler& messageHandler, const std::atomic_bool& shouldCancel) const
{
  return {};
}

//------------------------------------------------------------------------------
Result<> ExampleFilter2::executeImpl(DataStructure& data, const Arguments& args, const PipelineFilter* pipelineFilter, const MessageHandler& messageHandler, const std::atomic_bool& shouldCancel) const
{
  return {};
}
} // namespace complex
