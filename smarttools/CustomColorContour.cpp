#include "CustomColorContour.h"

CustomColorContour::CustomColorContour()
    : CustomIsoline(),
      fUseIsoLineGabWithCustomContours(false),
      fUseContourGabWithCustomContours(false)

{
}

CustomColorContour::CustomColorContour(const CustomColorContour& other)
    : CustomIsoline(other),
      fUseIsoLineGabWithCustomContours(other.UseIsoLineGabWithCustomContours()),
      fUseContourGabWithCustomContours(other.UseContourGabWithCustomContours()),
      itsSpecialColorContouringValues(other.SpecialColorContouringValues()),
      itsSpecialColorContouringColorIndexies(other.SpecialColorContouringColorIndexies())
{
}

CustomColorContour::~CustomColorContour() {}

void CustomColorContour::UseIsoLineGabWithCustomContours(const bool newState)
{
  fUseIsoLineGabWithCustomContours = newState;
  fUseContourGabWithCustomContours = newState;
};
bool CustomColorContour::UseIsoLineGabWithCustomContours(void) const
{
  return fUseIsoLineGabWithCustomContours;
};

void CustomColorContour::UseContourGabWithCustomContours(const bool newState)
{
  fUseContourGabWithCustomContours = newState;
};
bool CustomColorContour::UseContourGabWithCustomContours(void) const
{
  return fUseContourGabWithCustomContours;
};

const checkedVector<float>& CustomColorContour::SpecialColorContouringValues(void) const
{
  return itsSpecialColorContouringValues;
}
void CustomColorContour::SetSpecialColorContouringValues(const checkedVector<float>& newValue)
{
  itsSpecialColorContouringValues = newValue;
}

const checkedVector<int>& CustomColorContour::SpecialColorContouringColorIndexies(void) const
{
  return itsSpecialColorContouringColorIndexies;
}
void CustomColorContour::SpecialColorContouringColorIndexies(const checkedVector<int>& newValue)
{
  itsSpecialColorContouringColorIndexies = newValue;
}
