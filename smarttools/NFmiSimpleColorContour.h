#pragma once

#include "NFmiSimpleIsoline.h"

class NFmiSimpleColorContour : public NFmiSimpleIsoline
{
 public:
  NFmiSimpleColorContour();
  NFmiSimpleColorContour(const NFmiSimpleColorContour& other);
  virtual ~NFmiSimpleColorContour();

  NFmiSimpleColorContour& operator=(const NFmiSimpleColorContour& other) = default;

  const NFmiColor& ContourColor(void) const;
  void ContourColor(const NFmiColor& newColor);

  float SimpleContourWidth(void) const;
  void SimpleContourWidth(const float& newValue);

  const NFmiColor& IsolineColor() const;
  void IsolineColor(const NFmiColor& newColor);

  float SimpleIsoLineWidth() const;
  void SimpleIsoLineWidth(const float& newValue);

  int SimpleContourLineStyle(void) const;
  void SimpleContourLineStyle(const int& newValue);

  int SimpleIsoLineLineStyle() const;
  void SimpleIsoLineLineStyle(const int& newValue);

  double ContourGab(void) const;
  void ContourGab(const double theContourGab);

  /* Lower limit of contour color scale */
  float ColorContouringColorShadeLowValue(void) const;
  void ColorContouringColorShadeLowValue(const float& newValue);

  /* Mean value of contour color scale */
  float ColorContouringColorShadeMidValue(void) const;
  void ColorContouringColorShadeMidValue(const float& newValue);

  float ColorContouringColorShadeHighValue(void) const;
  void ColorContouringColorShadeHighValue(const float& newValue);

  /* Upper limit of contour color scale */
  float ColorContouringColorShadeHigh2Value(void) const;
  void ColorContouringColorShadeHigh2Value(const float& newValue);

  const NFmiColor& ColorContouringColorShadeLowValueColor(void) const;
  void ColorContouringColorShadeLowValueColor(const NFmiColor& newValue);

  const NFmiColor& ColorContouringColorShadeMidValueColor(void) const;
  void ColorContouringColorShadeMidValueColor(const NFmiColor& newValue);

  const NFmiColor& ColorContouringColorShadeHighValueColor(void) const;
  void ColorContouringColorShadeHighValueColor(const NFmiColor& newValue);

  /* It is also possible to use hatched regions (2) with contours */
  const NFmiColor& ColorContouringColorShadeHigh2ValueColor(void) const;
  void ColorContouringColorShadeHigh2ValueColor(const NFmiColor& newValue);

 private:
  double itsContourGab;
  NFmiColor itsContourColor;
  float itsSimpleContourWidth;
  int itsSimpleContourLineStyle;

  float itsColorContouringColorShadeLowValue;
  float itsColorContouringColorShadeMidValue;
  float itsColorContouringColorShadeHighValue;
  float itsColorContouringColorShadeHigh2Value;
  NFmiColor itsColorContouringColorShadeLowValueColor;
  NFmiColor itsColorContouringColorShadeMidValueColor;
  NFmiColor itsColorContouringColorShadeHighValueColor;
  NFmiColor itsColorContouringColorShadeHigh2ValueColor;
};
