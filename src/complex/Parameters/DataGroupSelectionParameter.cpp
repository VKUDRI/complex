#include "DataGroupSelectionParameter.hpp"

#include "complex/Common/Any.hpp"
#include "complex/DataStructure/DataGroup.hpp"

#include <fmt/core.h>
#include <fmt/ranges.h>
#include <nlohmann/json.hpp>

namespace complex
{
DataGroupSelectionParameter::DataGroupSelectionParameter(const std::string& name, const std::string& humanName, const std::string& helpText, const ValueType& defaultValue,
                                                         const AllowedTypes& allowedTypes)
: MutableDataParameter(name, humanName, helpText, Category::Required)
, m_DefaultValue(defaultValue)
, m_AllowedTypes(allowedTypes)
{
}

Uuid DataGroupSelectionParameter::uuid() const
{
  return ParameterTraits<DataGroupSelectionParameter>::uuid;
}

IParameter::AcceptedTypes DataGroupSelectionParameter::acceptedTypes() const
{
  return {typeid(ValueType)};
}

nlohmann::json DataGroupSelectionParameter::toJson(const std::any& value) const
{
  const auto& path = GetAnyRef<ValueType>(value);
  nlohmann::json json = path.toString();
  return json;
}

Result<std::any> DataGroupSelectionParameter::fromJson(const nlohmann::json& json) const
{
  static constexpr StringLiteral prefix = "FilterParameter 'DataGroupSelectionParameter' JSON Error: ";

  if(!json.is_string())
  {
    return MakeErrorResult<std::any>(FilterParameter::Constants::k_Json_Value_Not_String, fmt::format("{}The JSON data entry for key '{}' is not a string.", prefix.view(), name()));
  }

  auto valueString = json.get<std::string>();
  auto path = DataPath::FromString(valueString);
  if(!path.has_value())
  {
    return MakeErrorResult<std::any>(FilterParameter::Constants::k_Json_Value_Not_Value_Type, fmt::format("{}Failed to parse '{}' as DataPath for key '{}'", prefix, valueString, name()));
  }
  return {{std::move(*path)}};
}

IParameter::UniquePointer DataGroupSelectionParameter::clone() const
{
  return std::make_unique<DataGroupSelectionParameter>(name(), humanName(), helpText(), m_DefaultValue, m_AllowedTypes);
}

std::any DataGroupSelectionParameter::defaultValue() const
{
  return defaultPath();
}

const DataGroupSelectionParameter::AllowedTypes& DataGroupSelectionParameter::allowedTypes() const
{
  return m_AllowedTypes;
}

typename DataGroupSelectionParameter::ValueType DataGroupSelectionParameter::defaultPath() const
{
  return m_DefaultValue;
}

Result<> DataGroupSelectionParameter::validate(const DataStructure& dataStructure, const std::any& value) const
{
  const auto& path = GetAnyRef<ValueType>(value);

  return validatePath(dataStructure, path);
}

Result<> DataGroupSelectionParameter::validatePath(const DataStructure& dataStructure, const DataPath& value) const
{
  const std::string prefix = fmt::format("FilterParameter '{}' Validation Error: ", humanName());

  if(value.empty())
  {
    return MakeErrorResult(FilterParameter::Constants::k_Validate_Empty_Value, fmt::format("{}DataPath cannot be empty", prefix));
  }

  const DataObject* dataObject = dataStructure.getData(value);
  if(dataObject == nullptr)
  {
    return MakeErrorResult(FilterParameter::Constants::k_Validate_DuplicateValue, fmt::format("{}Object does not exist at path '{}'", prefix, value.toString()));
  }

  const auto baseGroupObj = dataStructure.getDataAs<BaseGroup>(value);
  if(baseGroupObj == nullptr)
  {
    return MakeErrorResult(FilterParameter::Constants::k_Validate_DuplicateValue, fmt::format("{}Object at path '{}' is not a BaseGroup type", prefix, value.toString()));
  }

  // Look for the actual group type that the user selected in the allowed set
  if(m_AllowedTypes.count(baseGroupObj->getGroupType()) > 0)
  {
    return {};
  }

  return MakeErrorResult(FilterParameter::Constants::k_Validate_AllowedType_Error, fmt::format("{}Group at path '{}' was of type '{}', but only {} are allowed", prefix, value.toString(),
                                                                                               dataObject->getTypeName(), BaseGroup::StringListFromGroupType(m_AllowedTypes)));
}

Result<std::any> DataGroupSelectionParameter::resolve(DataStructure& dataStructure, const std::any& value) const
{
  const auto& path = GetAnyRef<ValueType>(value);
  DataObject* object = dataStructure.getData(path);
  return {{object}};
}
} // namespace complex
