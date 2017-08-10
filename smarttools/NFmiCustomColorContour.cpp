#include "NFmiCustomColorContour.h"

NFmiCustomColorContour::NFmiCustomColorContour()
    : NFmiCustomIsoline(),
      fUseIsoLineGabWithCustomContours(false),
      fUseContourGabWithCustomContours(false)

{
}

NFmiCustomColorContour::NFmiCustomColorContour(const NFmiCustomColorContour& other)
    : NFmiCustomIsoline(other),
      fUseIsoLineGabWithCustomContours(other.UseIsoLineGabWithCustomContours()),
      fUseContourGabWithCustomContours(other.UseContourGabWithCustomContours()),
      itsSpecialColorContouringValues(other.SpecialColorContouringValues()),
      itsSpecialColorContouringColorIndexies(other.SpecialColorContouringColorIndexies())
{
}

NFmiCustomColorContour::~NFmiCustomColorContour() {}

void NFmiCustomColorContour::UseIsoLineGabWithCustomContours(const bool newState)
{
  fUseIsoLineGabWithCustomContours = newState;
  fUseContourGabWithCustomContours = newState;
};
bool NFmiCustomColorContour::UseIsoLineGabWithCustomContours(void) const
{
  return fUseIsoLineGabWithCustomContours;
};

void NFmiCustomColorContour::UseContourGabWithCustomContours(const bool newState)
{
  fUseContourGabWithCustomContours = newState;
};
bool NFmiCustomColorContour::UseContourGabWithCustomContours(void) const
{
  return fUseContourGabWithCustomContours;
};

const checkedVector<float>& NFmiCustomColorContour::SpecialColorContouringValues(void) const
{
  return itsSpecialColorContouringValues;
}
void NFmiCustomColorContour::SetSpecialColorContouringValues(const checkedVector<float>& newValue)
{
  itsSpecialColorContouringValues = newValue;
}

const checkedVector<int>& NFmiCustomColorContour::SpecialColorContouringColorIndexies(void) const
{
  return itsSpecialColorContouringColorIndexies;
}
void NFmiCustomColorContour::SpecialColorContouringColorIndexies(const checkedVector<int>& newValue)
{
  itsSpecialColorContouringColorIndexies = newValue;
}
