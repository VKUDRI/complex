#include "ParallelData2DAlgorithm.hpp"

using namespace complex;

// -----------------------------------------------------------------------------
ParallelData2DAlgorithm::ParallelData2DAlgorithm() = default;

// -----------------------------------------------------------------------------
ParallelData2DAlgorithm::~ParallelData2DAlgorithm() = default;

// -----------------------------------------------------------------------------
bool ParallelData2DAlgorithm::getParallelizationEnabled() const
{
  return m_RunParallel;
}

// -----------------------------------------------------------------------------
void ParallelData2DAlgorithm::setParallelizationEnabled(bool doParallel)
{
  m_RunParallel = doParallel;
}

// -----------------------------------------------------------------------------
Range2D ParallelData2DAlgorithm::getRange() const
{
  return m_Range;
}

// -----------------------------------------------------------------------------
void ParallelData2DAlgorithm::setRange(const Range2D& range)
{
  m_Range = range;
}

// -----------------------------------------------------------------------------
void ParallelData2DAlgorithm::setRange(size_t minCols, size_t maxCols, size_t minRows, size_t maxRows)
{
  m_Range = {minCols, maxCols, minRows, maxRows};
}
