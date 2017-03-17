#pragma once

#include "CustomIsoline.h"

class CustomColorContour : public CustomIsoline
{
 public:
  CustomColorContour();
  CustomColorContour(const CustomColorContour& other);
  virtual ~CustomColorContour();

  void UseIsoLineGabWithCustomContours(const bool newState);
  bool UseIsoLineGabWithCustomContours(void) const;

  void UseContourGabWithCustomContours(const bool newState);
  bool UseContourGabWithCustomContours(void) const;

  /* List of boundary values to divide entire range to classes. */
  const checkedVector<float>& SpecialColorContouringValues(void) const;
  void SetSpecialColorContouringValues(const checkedVector<float>& newValue);

  /* Specifies color indices for contours. */
  const checkedVector<int>& SpecialColorContouringColorIndexies(void) const;
  void SpecialColorContouringColorIndexies(const checkedVector<int>& newValue);

 private:
  CustomColorContour& operator=(const CustomColorContour& other) = delete;

  bool fUseIsoLineGabWithCustomContours;
  bool fUseContourGabWithCustomContours;
  checkedVector<float> itsSpecialColorContouringValues;
  checkedVector<int> itsSpecialColorContouringColorIndexies;
};
