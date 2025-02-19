include("${complex_SOURCE_DIR}/cmake/Plugin.cmake")

set(PLUGIN_NAME "ComplexCore")
set(${PLUGIN_NAME}_SOURCE_DIR ${complex_SOURCE_DIR}/src/Plugins/${PLUGIN_NAME})

# ------------------------------------------------------------------------------
# These are all the filters in the plugin. All filters should be kept in the
# PLUGIN_NAME/src/PLUGIN_NAME/Filters/ directory.
set(FilterList
  AlignGeometries
  AlignSectionsFeatureCentroidFilter
  AlignSectionsListFilter
  ApplyTransformationToGeometryFilter
  ApproximatePointCloudHull
  ArrayCalculatorFilter
  CalculateArrayHistogramFilter
  CalculateFeatureSizesFilter
  CalculateTriangleAreasFilter
  ChangeAngleRepresentation
  CombineAttributeArraysFilter
  ComputeMomentInvariants2DFilter
  ConditionalSetValue
  ConvertColorToGrayScaleFilter
  ConvertDataFilter
  CopyDataObjectFilter
  CopyFeatureArrayToElementArray
  CreateAttributeMatrixFilter
  CreateDataArray
  CreateDataGroup
  CreateFeatureArrayFromElementArray
  CreateGeometryFilter
  CreateImageGeometry
  CropImageGeometry
  CropVertexGeometry
  DeleteData
  ErodeDilateBadDataFilter
  ErodeDilateCoordinationNumberFilter
  ErodeDilateMaskFilter
  ExecuteProcessFilter
  ExportDREAM3DFilter
  ExtractComponentAsArrayFilter
  ExtractInternalSurfacesFromTriangleGeometry
  ExtractVertexGeometryFilter
  FeatureDataCSVWriterFilter
  FillBadDataFilter
  FindArrayStatisticsFilter
  FindBiasedFeaturesFilter
  FindBoundaryCellsFilter
  FindDifferencesMap
  FindEuclideanDistMapFilter
  FindFeatureCentroidsFilter
  FindFeaturePhasesFilter
  FindNeighborhoodsFilter
  FindNeighborListStatistics
  FindNeighbors
  FindNumFeaturesFilter
  FindSurfaceAreaToVolumeFilter
  FindSurfaceFeatures
  FindTriangleGeomCentroidsFilter
  FindTriangleGeomSizesFilter
  FindVertexToTriangleDistancesFilter
  FindVolFractionsFilter
  GenerateColorTableFilter
  IdentifySample
  ImportBinaryCTNorthstarFilter
  ImportCSVDataFilter
  ImportDREAM3DFilter
  ImportHDF5Dataset
  ImportTextFilter
  ImportVolumeGraphicsFileFilter
  InitializeData
  InterpolatePointCloudToRegularGridFilter
  IterativeClosestPointFilter
  LaplacianSmoothingFilter
  MapPointCloudToRegularGridFilter
  MinNeighbors
  MoveData
  MultiThresholdObjects
  PartitionGeometryFilter
  PointSampleTriangleGeometryFilter
  QuickSurfaceMeshFilter
  RawBinaryReaderFilter
  RemoveFlaggedFeaturesFilter
  RemoveFlaggedVertices
  RemoveMinimumSizeFeaturesFilter
  RenameDataObject
  ReplaceElementAttributesWithNeighborValuesFilter
  ResampleImageGeomFilter
  RobustAutomaticThreshold
  RotateSampleRefFrameFilter
  ScalarSegmentFeaturesFilter
  SetImageGeomOriginScalingFilter
  SharedFeatureFaceFilter
  SplitAttributeArrayFilter
  StlFileReaderFilter
  TriangleCentroidFilter
  TriangleDihedralAngleFilter
  TriangleNormalFilter
  WriteASCIIDataFilter
  WriteBinaryDataFilter
)

set(ActionList
)

set(AlgorithmList
  AlignSectionsFeatureCentroid
  AlignSectionsList
  ApplyTransformationToGeometry
  ArrayCalculator
  CalculateArrayHistogram
  CombineAttributeArrays
  ComputeMomentInvariants2D
  ConvertColorToGrayScale
  ConvertData
  ErodeDilateBadData
  ErodeDilateCoordinationNumber
  ErodeDilateMask
  ExecuteProcess
  ExtractComponentAsArray
  ExtractVertexGeometry
  FillBadData
  FindArrayStatistics
  FindBiasedFeatures
  FindBoundaryCells
  FindEuclideanDistMap
  FindFeatureCentroids
  FindNeighborhoods
  FindSurfaceAreaToVolume
  FindTriangleGeomCentroids
  FindTriangleGeomSizes
  FindVertexToTriangleDistances
  GenerateColorTable
  ImportVolumeGraphicsFile
  ImportBinaryCTNorthstar
  LaplacianSmoothing
  PartitionGeometry
  PointSampleTriangleGeometry
  QuickSurfaceMesh
  RawBinaryReader
  RemoveFlaggedFeatures
  ReplaceElementAttributesWithNeighborValues
  ResampleImageGeom
  ScalarSegmentFeatures
  SharedFeatureFace
  SplitAttributeArray
  StlFileReader
  TriangleCentroid
  TupleTransfer
)

create_complex_plugin(NAME ${PLUGIN_NAME}
                      FILTER_LIST ${FilterList}
                      ACTION_LIST ${ActionList}
                      ALGORITHM_LIST ${AlgorithmList}
                      DESCRIPTION "${PLUGIN_NAME} Plugin"
                      VERSION "0.1.0"
                      DOC_CHECK
                      ADD_TO_GLOBAL_LIST
)

#------------------------------------------------------------------------------
# Add in the unit tests for the filters included in this plugin
enable_testing()
add_subdirectory(${${PLUGIN_NAME}_SOURCE_DIR}/test)

#------------------------------------------------------------------------------
# If there are additional library packages that need to be found, there is where
# to do that using the usual find_package(....) call
# ------------------------------------------------------------------------------
# The ExecuteProcess Filter requires the `reproc` library
# ------------------------------------------------------------------------------
find_package("reproc++" CONFIG REQUIRED)
get_target_property(reproc_dll_path reproc++ IMPORTED_LOCATION_RELEASE)
get_filename_component(reproc_dll_path "${reproc_dll_path}" DIRECTORY)
file(REAL_PATH "${reproc_dll_path}" reproc_dll_path)
file(TO_CMAKE_PATH "${reproc_dll_path}" reproc_dll_path)
get_property(COMPLEX_EXTRA_LIBRARY_DIRS GLOBAL PROPERTY COMPLEX_EXTRA_LIBRARY_DIRS)
set_property(GLOBAL PROPERTY COMPLEX_EXTRA_LIBRARY_DIRS ${COMPLEX_EXTRA_LIBRARY_DIRS} ${reproc_dll_path})


#------------------------------------------------------------------------------
# If there are additional libraries that this plugin needs to link against you
# can use the target_link_libraries() cmake call
target_link_libraries(${PLUGIN_NAME} PRIVATE reproc++)

#------------------------------------------------------------------------------
# If there are additional source files that need to be compiled for this plugin
# can use the target_sources(...) cmake call

set(PLUGIN_EXTRA_SOURCES
    ${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/CSVDataParser.hpp
    ${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/nanoflann.hpp
    ${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/StlUtilities.hpp
    ${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/StlUtilities.cpp
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/CalculatorItem.hpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/CalculatorItem.cpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/ICalculatorArray.hpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/ICalculatorArray.cpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/CalculatorArray.hpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/CalculatorOperator.hpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/CalculatorOperator.cpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/UnaryOperator.hpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/UnaryOperator.cpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/BinaryOperator.hpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/BinaryOperator.cpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/ABSOperator.hpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/ABSOperator.cpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/PowOperator.hpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/PowOperator.cpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/ExpOperator.hpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/ExpOperator.cpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/SinOperator.hpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/SinOperator.cpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/CosOperator.hpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/CosOperator.cpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/TanOperator.hpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/TanOperator.cpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/ASinOperator.hpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/ASinOperator.cpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/ACosOperator.hpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/ACosOperator.cpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/ATanOperator.hpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/ATanOperator.cpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/SqrtOperator.hpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/SqrtOperator.cpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/RootOperator.hpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/RootOperator.cpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/LogOperator.hpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/LogOperator.cpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/Log10Operator.hpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/Log10Operator.cpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/LnOperator.hpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/LnOperator.cpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/AdditionOperator.hpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/AdditionOperator.cpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/SubtractionOperator.hpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/SubtractionOperator.cpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/NegativeOperator.hpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/NegativeOperator.cpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/MultiplicationOperator.hpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/MultiplicationOperator.cpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/DivisionOperator.hpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/DivisionOperator.cpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/LeftParenthesisItem.hpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/LeftParenthesisItem.cpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/RightParenthesisItem.hpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/RightParenthesisItem.cpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/CalculatorSeparator.hpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/CalculatorSeparator.cpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/CommaSeparator.hpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/CommaSeparator.cpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/FloorOperator.hpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/FloorOperator.cpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/CeilOperator.hpp"
	"${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}/utils/CeilOperator.cpp"
)

target_sources(${PLUGIN_NAME}
  PRIVATE
    ${PLUGIN_EXTRA_SOURCES}
)

source_group(TREE "${${PLUGIN_NAME}_SOURCE_DIR}/src/${PLUGIN_NAME}" PREFIX ${PLUGIN_NAME}
  FILES
    ${PLUGIN_EXTRA_SOURCES}
)

#------------------------------------------------------------------------------
# If there are additional include directories that are needed for this plugin
# you can use the target_include_directories(.....) cmake call
# target_include_directories(${PLUGIN_NAME}
#     PUBLIC
#     additional include directories here
# )

# -----------------------------------------------------------------------
# Install example pipelines
# -----------------------------------------------------------------------
if(EXISTS "${${PLUGIN_NAME}_SOURCE_DIR}/pipelines")
  set(PIPELINE_DEST_DIR "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR}/pipelines/${PLUGIN_NAME}")
  add_custom_target(${PLUGIN_NAME}PipelineFolderCopy ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${${PLUGIN_NAME}_SOURCE_DIR}/pipelines"
    ${PIPELINE_DEST_DIR}
    COMMENT "Copying Pipeline Folder into Binary Directory")
  set_target_properties(${PLUGIN_NAME}PipelineFolderCopy PROPERTIES FOLDER ZZ_COPY_FILES)

  set(INSTALL_DESTINATION "pipelines/${PLUGIN_NAME}")
  install(DIRECTORY
    "${${PLUGIN_NAME}_SOURCE_DIR}/pipelines/"
    DESTINATION "${INSTALL_DESTINATION}"
    COMPONENT Applications
  )
endif()

# -----------------------------------------------------------------------
# Download Example Data Files
# -----------------------------------------------------------------------
include(${complex_SOURCE_DIR}/cmake/Utility.cmake)
set(DATA_DEST_DIR "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR}/Data")

file(TO_CMAKE_PATH "${DREAM3D_DATA_DIR}" DREAM3D_DATA_DIR_NORM)

# -----------------------------------------------------------------------
# These data files are pulled from the 'complex data archive' on github,
# decompressed and then copied to the build folder. Install rules are also
# generated for each of the data sets.
# -----------------------------------------------------------------------
download_test_data(DREAM3D_DATA_DIR ${DREAM3D_DATA_DIR} ARCHIVE_NAME ASCII_Data.tar.gz SHA512 4696b919510aa400b9de6c72d0abe66e7c8be2a4effd350bfd07c4fc24d25ccb45c22e7aca5ad162d88db3e04d700b86f7d1d7e954db90ef00b31b5b46955ed4)
download_test_data(DREAM3D_DATA_DIR ${DREAM3D_DATA_DIR} ARCHIVE_NAME STL_Models.tar.gz SHA512 14d5bbb1dcc241a5ad9b17e7647144926f22fca9af5e426798c2ed423d8d3b5d89d8c66ce9e6854bfcdbaea7f72b69980477ab837498e3dca89b5cd2870e2cff)

set(DREAM3D_DATA_ARCHIVES ASCII_Data STL_Models)
foreach(archive ${DREAM3D_DATA_ARCHIVES})
  add_custom_target(Copy_${archive} ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${DREAM3D_DATA_DIR}/TestFiles/${archive} ${DATA_DEST_DIR}/${archive}
    COMMENT "Copying ${archive} into Binary Directory"
    DEPENDS Fetch_${archive})
  set_target_properties(Copy_${archive} PROPERTIES FOLDER ZZ_COPY_FILES)

  install(DIRECTORY 
    ${DREAM3D_DATA_DIR_NORM}/TestFiles/${archive}
    DESTINATION Data/
    COMPONENT Applications
  )
endforeach()

# -----------------------------------------------------------------------
# Create build folder copy rules and install rules for the 'data' folder
# for this plugin
# -----------------------------------------------------------------------
# add_custom_target(Copy_${PLUGIN_NAME}_Data ALL
#   COMMAND ${CMAKE_COMMAND} -E copy_directory ${${PLUGIN_NAME}_SOURCE_DIR}/data ${DATA_DEST_DIR}/${PLUGIN_NAME}
#   COMMENT "Copying ${PLUGIN_NAME} data into Binary Directory"
#   )
# set_target_properties(Copy_${PLUGIN_NAME}_Data PROPERTIES FOLDER ZZ_COPY_FILES)

set(Installed_Data_Files
)

install(FILES 
  ${Installed_Data_Files}
  DESTINATION Data/${PLUGIN_NAME}
  COMPONENT Applications
)

