#include "ArraySelectionParameter.hpp"

#include "complex/Common/Any.hpp"
#include "complex/Common/TypesUtility.hpp"
#include "complex/DataStructure/DataGroup.hpp"
#include "complex/DataStructure/IDataArray.hpp"
#include "complex/Utilities/StringUtilities.hpp"

#include <fmt/core.h>
#include <fmt/ranges.h>
#include <nlohmann/json.hpp>

using namespace complex;

template <>
struct fmt::formatter<complex::DataType>
{
  template <typename ParseContext>
  constexpr auto parse(ParseContext& ctx)
  {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const complex::DataType& value, FormatContext& ctx)
  {
    return fmt::format_to(ctx.out(), "{}", complex::DataTypeToString(value));
  }
};

namespace complex
{
ArraySelectionParameter::ArraySelectionParameter(const std::string& name, const std::string& humanName, const std::string& helpText, const ValueType& defaultValue, const AllowedTypes& allowedTypes,
                                                 AllowedComponentShapes requiredComps, DataLocation location)
: MutableDataParameter(name, humanName, helpText, Category::Required)
, m_DefaultValue(defaultValue)
, m_AllowedTypes(allowedTypes)
, m_RequiredComponentShapes(requiredComps)
, m_Location(location)
{
  if(allowedTypes.empty())
  {
    throw std::runtime_error(
        fmt::format("ArraySelectionParameter REQUIRES a non-empty AllowedTypes variable. Please report this to the developer. \n  Parameter Name:{}\n  Human Name:{}", name, humanName));
  }
}

Uuid ArraySelectionParameter::uuid() const
{
  return ParameterTraits<ArraySelectionParameter>::uuid;
}

IParameter::AcceptedTypes ArraySelectionParameter::acceptedTypes() const
{
  return {typeid(ValueType)};
}

nlohmann::json ArraySelectionParameter::toJson(const std::any& value) const
{
  const auto& path = GetAnyRef<ValueType>(value);
  nlohmann::json json = path.toString();
  return json;
}

Result<std::any> ArraySelectionParameter::fromJson(const nlohmann::json& json) const
{
  static constexpr StringLiteral prefix = "FilterParameter 'ArraySelectionParameter' JSON Error: ";
  if(!json.is_string())
  {
    return MakeErrorResult<std::any>(FilterParameter::Constants::k_Json_Value_Not_String, fmt::format("{}JSON value for key '{}' is not a string", prefix.view(), name()));
  }
  auto jsonStrValue = json.get<std::string>();
  if(jsonStrValue.empty())
  {
    return {{DataPath()}};
  }
  std::optional<DataPath> path = DataPath::FromString(jsonStrValue);
  if(!path.has_value())
  {
    return MakeErrorResult<std::any>(FilterParameter::Constants::k_Json_Value_Not_Value_Type, fmt::format("{}Failed to parse '{}' as DataPath", prefix.view(), jsonStrValue));
  }
  return {{std::move(*path)}};
}

IParameter::UniquePointer ArraySelectionParameter::clone() const
{
  return std::make_unique<ArraySelectionParameter>(name(), humanName(), helpText(), m_DefaultValue, m_AllowedTypes, m_RequiredComponentShapes);
}

std::any ArraySelectionParameter::defaultValue() const
{
  return defaultPath();
}

typename ArraySelectionParameter::ValueType ArraySelectionParameter::defaultPath() const
{
  return m_DefaultValue;
}

ArraySelectionParameter::AllowedTypes ArraySelectionParameter::allowedTypes() const
{
  return m_AllowedTypes;
}

ArraySelectionParameter::AllowedComponentShapes ArraySelectionParameter::requiredComponentShapes() const
{
  return m_RequiredComponentShapes;
}

bool ArraySelectionParameter::allowsOutOfCore() const
{
  return (m_Location == DataLocation::Any) || (m_Location == DataLocation::OutOfCore);
}

bool ArraySelectionParameter::allowsInMemory() const
{
  return (m_Location == DataLocation::Any) || (m_Location == DataLocation::InMemory);
}

ArraySelectionParameter::DataLocation ArraySelectionParameter::allowedDataLocations() const
{
  return m_Location;
}

Result<> ArraySelectionParameter::validate(const DataStructure& dataStructure, const std::any& value) const
{
  const auto& path = GetAnyRef<ValueType>(value);

  return validatePath(dataStructure, path);
}

Result<> ArraySelectionParameter::validatePath(const DataStructure& dataStructure, const DataPath& value) const
{
  const std::string prefix = fmt::format("FilterParameter '{}' Validation Error: ", humanName());

  if(value.empty())
  {
    return complex::MakeErrorResult(complex::FilterParameter::Constants::k_Validate_Empty_Value, fmt::format("{}DataPath cannot be empty", prefix));
  }
  const DataObject* object = dataStructure.getData(value);
  if(object == nullptr)
  {
    return complex::MakeErrorResult<>(complex::FilterParameter::Constants::k_Validate_Does_Not_Exist, fmt::format("{}Object does not exist at path '{}'", prefix, value.toString()));
  }

  const auto* dataArray = dynamic_cast<const IDataArray*>(object);
  if(dataArray == nullptr)
  {
    return complex::MakeErrorResult<>(complex::FilterParameter::Constants::k_Validate_Type_Error, fmt::format("{}Object at path '{}' must be a DataArray.", prefix, value.toString()));
  }

  if(!m_AllowedTypes.empty())
  {
    DataType dataType = dataArray->getDataType();
    if(m_AllowedTypes.count(dataType) == 0)
    {
      return complex::MakeErrorResult(complex::FilterParameter::Constants::k_Validate_AllowedType_Error,
                                      fmt::format("{}DataArray at path '{}' was of type '{}', but only {} are allowed", prefix, value.toString(), dataType, m_AllowedTypes));
    }
  }

  if(!m_RequiredComponentShapes.empty())
  {
    std::string compStr;
    bool foundMatch = false;
    for(const auto& compShape : m_RequiredComponentShapes)
    {
      if(compShape == dataArray->getComponentShape())
      {
        foundMatch = true;
        break;
      }
      compStr += StringUtilities::number(compShape[0]);
      for(usize i = 1; i < compShape.size(); ++i)
      {
        compStr += " x " + StringUtilities::number(compShape[i]);
      }
      compStr += " or ";
    }
    if(!foundMatch)
    {
      return complex::MakeErrorResult<>(complex::FilterParameter::Constants::k_Validate_TupleShapeValue,
                                        fmt::format("{}Object at path '{}' must have a component shape of {}.", prefix, value.toString(), compStr));
    }
  }

  if(m_Location != DataLocation::Any)
  {
    IDataStore::StoreType storeType = dataArray->getStoreType();

    if(allowsInMemory() && (storeType == IDataStore::StoreType::Empty))
    {
      return {};
    }
    else if(allowsOutOfCore() && (storeType == IDataStore::StoreType::EmptyOutOfCore))
    {
      return {};
    }

    return MakeErrorResult(FilterParameter::Constants::k_Validate_DataLocation_Error,
                           fmt::format("{}DataArray at path '{}' was stored at '{}', but only {} are allowed", prefix, value.toString(), storeType, m_Location));
  }

  return {};
}

Result<std::any> ArraySelectionParameter::resolve(DataStructure& dataStructure, const std::any& value) const
{
  const auto& path = GetAnyRef<ValueType>(value);
  DataObject* object = dataStructure.getData(path);
  return {{object}};
}
} // namespace complex
