#include "DataArrayUtilities.hpp"

#include "complex/Common/Types.hpp"
#include "complex/Common/TypesUtility.hpp"

#include <set>

using namespace complex;

namespace
{
template <class T>
Result<> ReplaceArray(DataStructure& dataStructure, const DataPath& dataPath, const std::vector<usize>& tupleShape, IDataAction::Mode mode, const IDataArray& inputDataArray)
{
  auto& castInputArray = dynamic_cast<const DataArray<T>&>(inputDataArray);
  const IDataStore::ShapeType componentShape = castInputArray.getDataStoreRef().getComponentShape();
  dataStructure.removeData(dataPath);
  return CreateArray<T>(dataStructure, tupleShape, componentShape, dataPath, mode);
}
} // namespace

namespace complex
{
//-----------------------------------------------------------------------------
Result<> CheckValueConverts(const std::string& value, NumericType numericType)
{
  switch(numericType)
  {
  case NumericType::int8: {
    return CheckValuesSignedInt<int8>(value, Constants::k_Int8);
  }
  case NumericType::uint8: {
    return CheckValuesUnsignedInt<uint8>(value, Constants::k_UInt8);
  }
  case NumericType::int16: {
    return CheckValuesSignedInt<int16>(value, Constants::k_Int16);
  }
  case NumericType::uint16: {
    return CheckValuesUnsignedInt<uint16>(value, Constants::k_UInt16);
  }
  case NumericType::int32: {
    return CheckValuesSignedInt<int32>(value, Constants::k_Int32);
  }
  case NumericType::uint32: {
    return CheckValuesUnsignedInt<uint32>(value, Constants::k_UInt32);
  }
  case NumericType::int64: {
    return CheckValuesSignedInt<int64>(value, Constants::k_Int64);
  }
  case NumericType::uint64: {
    return CheckValuesUnsignedInt<uint64>(value, Constants::k_UInt64);
  }
  case NumericType::float32: {
    return CheckValuesFloatDouble<float32>(value, Constants::k_Float32);
  }
  case NumericType::float64: {
    return CheckValuesFloatDouble<float64>(value, Constants::k_Float64);
  }
  }
  return MakeErrorResult(-10102, fmt::format("CheckInitValueConverts: Cannot convert input value '{}' to type '{}'", value, NumericTypeToString(numericType)));
}

//-----------------------------------------------------------------------------
Result<> CheckValueConvertsToArrayType(const std::string& value, const DataObject& inputDataArray)
{
  if(TemplateHelpers::CanDynamicCast<Float32Array>()(&inputDataArray))
  {
    return CheckValuesFloatDouble<float32>(value, Constants::k_Float32);
  }
  if(TemplateHelpers::CanDynamicCast<Float64Array>()(&inputDataArray))
  {
    return CheckValuesFloatDouble<float64>(value, Constants::k_Float64);
  }
  if(TemplateHelpers::CanDynamicCast<Int8Array>()(&inputDataArray))
  {
    return CheckValuesSignedInt<int8>(value, Constants::k_Int8);
  }
  if(TemplateHelpers::CanDynamicCast<UInt8Array>()(&inputDataArray))
  {
    return CheckValuesUnsignedInt<uint8>(value, Constants::k_UInt8);
  }
  if(TemplateHelpers::CanDynamicCast<Int16Array>()(&inputDataArray))
  {
    return CheckValuesSignedInt<int16>(value, Constants::k_Int16);
  }
  if(TemplateHelpers::CanDynamicCast<UInt16Array>()(&inputDataArray))
  {
    return CheckValuesUnsignedInt<uint16>(value, Constants::k_UInt16);
  }
  if(TemplateHelpers::CanDynamicCast<Int32Array>()(&inputDataArray))
  {
    return CheckValuesSignedInt<int32>(value, Constants::k_Int32);
  }
  if(TemplateHelpers::CanDynamicCast<UInt32Array>()(&inputDataArray))
  {
    return CheckValuesUnsignedInt<uint32>(value, Constants::k_UInt32);
  }
  if(TemplateHelpers::CanDynamicCast<Int64Array>()(&inputDataArray))
  {
    return CheckValuesSignedInt<int64>(value, Constants::k_Int64);
  }
  if(TemplateHelpers::CanDynamicCast<UInt64Array>()(&inputDataArray))
  {
    return CheckValuesUnsignedInt<uint64>(value, Constants::k_UInt64);
  }

  return {MakeErrorResult(-259, fmt::format("Input DataObject could not be cast to any primitive type."))};
}

//-----------------------------------------------------------------------------
bool CheckArraysAreSameType(const DataStructure& dataStructure, const std::vector<DataPath>& dataArrayPaths)
{
  std::set<complex::DataType> types;
  for(const auto& dataPath : dataArrayPaths)
  {
    const auto* dataArray = dataStructure.getDataAs<IDataArray>(dataPath);
    types.insert(dataArray->getDataType());
  }
  return types.size() == 1;
}

//-----------------------------------------------------------------------------
bool CheckArraysHaveSameTupleCount(const DataStructure& dataStructure, const std::vector<DataPath>& dataArrayPaths)
{
  std::set<size_t> types;
  for(const auto& dataPath : dataArrayPaths)
  {
    const auto* dataArray = dataStructure.getDataAs<IDataArray>(dataPath);
    types.insert(dataArray->getNumberOfTuples());
  }
  return types.size() == 1;
}

//-----------------------------------------------------------------------------
Result<> ConditionalReplaceValueInArray(const std::string& valueAsStr, DataObject& inputDataObject, const IDataArray& conditionalDataArray)
{
  const IDataArray& iDataArray = dynamic_cast<IDataArray&>(inputDataObject);
  const complex::DataType arrayType = iDataArray.getDataType();
  const Result<> resultFromConversion;
  switch(arrayType)
  {
  case complex::DataType::int8:
    return ConditionalReplaceValueInArrayFromString<int8>(valueAsStr, inputDataObject, conditionalDataArray);
    break;
  case complex::DataType::uint8:
    return ConditionalReplaceValueInArrayFromString<uint8>(valueAsStr, inputDataObject, conditionalDataArray);
    break;
  case complex::DataType::int16:
    return ConditionalReplaceValueInArrayFromString<int16>(valueAsStr, inputDataObject, conditionalDataArray);
    break;
  case complex::DataType::uint16:
    return ConditionalReplaceValueInArrayFromString<uint16>(valueAsStr, inputDataObject, conditionalDataArray);
    break;
  case complex::DataType::int32:
    return ConditionalReplaceValueInArrayFromString<int32>(valueAsStr, inputDataObject, conditionalDataArray);
    break;
  case complex::DataType::uint32:
    return ConditionalReplaceValueInArrayFromString<uint32>(valueAsStr, inputDataObject, conditionalDataArray);
    break;
  case complex::DataType::int64:
    return ConditionalReplaceValueInArrayFromString<int64>(valueAsStr, inputDataObject, conditionalDataArray);
    break;
  case complex::DataType::uint64:
    return ConditionalReplaceValueInArrayFromString<uint64>(valueAsStr, inputDataObject, conditionalDataArray);
    break;
  case complex::DataType::float32:
    return ConditionalReplaceValueInArrayFromString<float32>(valueAsStr, inputDataObject, conditionalDataArray);
    break;
  case complex::DataType::float64:
    return ConditionalReplaceValueInArrayFromString<float64>(valueAsStr, inputDataObject, conditionalDataArray);
    break;
  case complex::DataType::boolean:
    return ConditionalReplaceValueInArrayFromString<bool>(valueAsStr, inputDataObject, conditionalDataArray);
    break;
  }
  return {};
}

//-----------------------------------------------------------------------------
Result<> ResizeAndReplaceDataArray(DataStructure& dataStructure, const DataPath& dataPath, std::vector<usize>& tupleShape, IDataAction::Mode mode)
{
  auto* inputDataArray = dataStructure.getDataAs<IDataArray>(dataPath);

  if(TemplateHelpers::CanDynamicCast<Float32Array>()(inputDataArray))
  {
    return ReplaceArray<float32>(dataStructure, dataPath, tupleShape, mode, *inputDataArray);
  }
  if(TemplateHelpers::CanDynamicCast<Float64Array>()(inputDataArray))
  {
    return ReplaceArray<float64>(dataStructure, dataPath, tupleShape, mode, *inputDataArray);
  }
  if(TemplateHelpers::CanDynamicCast<Int8Array>()(inputDataArray))
  {
    return ReplaceArray<int8>(dataStructure, dataPath, tupleShape, mode, *inputDataArray);
  }
  if(TemplateHelpers::CanDynamicCast<UInt8Array>()(inputDataArray))
  {
    return ReplaceArray<uint8>(dataStructure, dataPath, tupleShape, mode, *inputDataArray);
  }
  if(TemplateHelpers::CanDynamicCast<Int16Array>()(inputDataArray))
  {
    return ReplaceArray<int16>(dataStructure, dataPath, tupleShape, mode, *inputDataArray);
  }
  if(TemplateHelpers::CanDynamicCast<UInt16Array>()(inputDataArray))
  {
    return ReplaceArray<uint16>(dataStructure, dataPath, tupleShape, mode, *inputDataArray);
  }
  if(TemplateHelpers::CanDynamicCast<Int32Array>()(inputDataArray))
  {
    return ReplaceArray<int32>(dataStructure, dataPath, tupleShape, mode, *inputDataArray);
  }
  if(TemplateHelpers::CanDynamicCast<UInt32Array>()(inputDataArray))
  {
    return ReplaceArray<uint32>(dataStructure, dataPath, tupleShape, mode, *inputDataArray);
  }
  if(TemplateHelpers::CanDynamicCast<Int64Array>()(inputDataArray))
  {
    return ReplaceArray<int64>(dataStructure, dataPath, tupleShape, mode, *inputDataArray);
  }
  if(TemplateHelpers::CanDynamicCast<UInt64Array>()(inputDataArray))
  {
    return ReplaceArray<uint64>(dataStructure, dataPath, tupleShape, mode, *inputDataArray);
  }
  if(TemplateHelpers::CanDynamicCast<BoolArray>()(inputDataArray))
  {
    return ReplaceArray<bool>(dataStructure, dataPath, tupleShape, mode, *inputDataArray);
  }

  return MakeErrorResult(-401, fmt::format("The input array at DataPath '{}' was of an unsupported type", dataPath.toString()));
}

//-----------------------------------------------------------------------------
void ResizeAttributeMatrix(AttributeMatrix& attributeMatrix, const std::vector<usize>& newShape)
{
  attributeMatrix.setShape(newShape);
  auto childArrays = attributeMatrix.findAllChildrenOfType<IArray>();
  for(const auto& array : childArrays)
  {
    array->reshapeTuples(newShape);
  }
}

//-----------------------------------------------------------------------------
Result<> ValidateNumFeaturesInArray(const DataStructure& dataStructure, const DataPath& arrayPath, const Int32Array& featureIds)
{
  const auto* featureArray = dataStructure.getDataAs<IDataArray>(arrayPath);
  if(featureArray == nullptr)
  {
    return MakeErrorResult(-5550, fmt::format("Could not find the input array path '{}' for validating number of features", arrayPath.toString()));
  }

  const usize numFeatures = featureArray->getNumberOfTuples();
  bool mismatchedFeatures = false;
  usize largestFeature = 0;
  for(const int32& featureId : featureIds)
  {
    if(static_cast<usize>(featureId) > largestFeature)
    {
      largestFeature = featureId;
      if(largestFeature >= numFeatures)
      {
        mismatchedFeatures = true;
        break;
      }
    }
  }

  Result<> results = {};
  if(mismatchedFeatures)
  {
    results.errors().push_back(Error{-5551, fmt::format("The largest Feature Id {} in the FeatureIds array is larger than the number of Features ({}) in the Feature Data array at path '{}'",
                                                        largestFeature, numFeatures, arrayPath.toString())});
  }

  if(largestFeature != (numFeatures - 1))
  {
    results.errors().push_back(Error{-5552, fmt::format("The number of Features ({}) in the Feature Data array at path '{}' does not match the largest Feature Id in the FeatureIds array {}",
                                                        numFeatures, arrayPath.toString(), largestFeature)});

    const auto* parentAM = dataStructure.getDataAs<AttributeMatrix>(arrayPath.getParent());
    if(parentAM != nullptr)
    {
      results.errors().push_back(Error{-5553, fmt::format("The input Attribute matrix at path '{}' has {} tuples which does not match the number of total features {}",
                                                          arrayPath.getParent().toString(), parentAM->getNumTuples(), largestFeature + 1)});
    }
  }

  return results;
}

//-----------------------------------------------------------------------------
std::unique_ptr<MaskCompare> InstantiateMaskCompare(DataStructure& dataStructure, const DataPath& maskArrayPath)
{
  auto& maskArray = dataStructure.getDataRefAs<IDataArray>(maskArrayPath);

  return InstantiateMaskCompare(maskArray);
}

//-----------------------------------------------------------------------------
std::unique_ptr<MaskCompare> InstantiateMaskCompare(IDataArray& maskArray)
{
  switch(maskArray.getDataType())
  {
  case DataType::boolean: {
    return std::make_unique<BoolMaskCompare>(dynamic_cast<BoolArray&>(maskArray));
  }
  case DataType::uint8: {
    return std::make_unique<UInt8MaskCompare>(dynamic_cast<UInt8Array&>(maskArray));
  }
  default:
    throw std::runtime_error("InstantiateMaskCompare: The Mask Array being used is NOT of type bool or uint8.");
  }
}

} // namespace complex
