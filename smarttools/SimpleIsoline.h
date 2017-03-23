#pragma once

#include "NFmiColor.h"

class SimpleIsoline
{
 public:
  SimpleIsoline();
  SimpleIsoline(const SimpleIsoline& other);
  virtual ~SimpleIsoline();

  const NFmiColor& IsolineLabelBoxFillColor(void) const;
  void IsolineLabelBoxFillColor(const NFmiColor& theColor);

  const NFmiColor& ContourLabelBoxFillColor(void) const;
  void ContourLabelBoxFillColor(const NFmiColor& theColor);

  double IsoLineGab(void) const;
  void IsoLineGab(const double theIsoLineGab);

  virtual const NFmiColor& IsolineColor(void) const;
  virtual void IsolineColor(const NFmiColor& newColor);

  void IsolineTextColor(const NFmiColor& newColor);
  const NFmiColor& IsolineTextColor(void) const;

  float SimpleIsoLineGap(void) const;
  void SimpleIsoLineGap(const float& newValue);

  /* Relative height or in millimeters? (0=is not showed at all) */
  float SimpleIsoLineLabelHeight(void) const;
  void SimpleIsoLineLabelHeight(const float& newValue);

  /* Box around a number indicates that there is not yet any
     other attributes for isoline. */
  bool ShowSimpleIsoLineLabelBox(void) const;
  void ShowSimpleIsoLineLabelBox(const bool& newValue);

  /* Is this relatiivinen vai in millimeters? */
  virtual float SimpleIsoLineWidth(void) const;
  virtual void SimpleIsoLineWidth(const float& newValue);

  // 1=solid line, 2=dashed line, and so on... */
  virtual int SimpleIsoLineLineStyle(void) const;
  virtual void SimpleIsoLineLineStyle(const int& newValue);

  /* true=same color for all isolines,
     false=do isoline color scaling */
  bool UseSingleColorsWithSimpleIsoLines(void) const;
  void UseSingleColorsWithSimpleIsoLines(const bool& newValue);

  /* Lower limit of color scale */
  float SimpleIsoLineColorShadeLowValue(void) const;
  void SimpleIsoLineColorShadeLowValue(const float& newValue);

  /* Mean value of color scale */
  float SimpleIsoLineColorShadeMidValue(void) const;
  void SimpleIsoLineColorShadeMidValue(const float& newValue);

  /* Highest value of color scale */
  float SimpleIsoLineColorShadeHighValue(void) const;
  void SimpleIsoLineColorShadeHighValue(const float& newValue);

  float SimpleIsoLineColorShadeHigh2Value(void) const;
  void SimpleIsoLineColorShadeHigh2Value(const float& newValue);

  const NFmiColor& SimpleIsoLineColorShadeLowValueColor(void) const;
  void SimpleIsoLineColorShadeLowValueColor(const NFmiColor& newValue);

  const NFmiColor& SimpleIsoLineColorShadeMidValueColor(void) const;
  void SimpleIsoLineColorShadeMidValueColor(const NFmiColor& newValue);

  const NFmiColor& SimpleIsoLineColorShadeHighValueColor(void) const;
  void SimpleIsoLineColorShadeHighValueColor(const NFmiColor& newValue);

  const NFmiColor& SimpleIsoLineColorShadeHigh2ValueColor(void) const;
  void SimpleIsoLineColorShadeHigh2ValueColor(const NFmiColor& newValue);

  /* How many color categories are used in color scaling.
     Special isoline settings is took into operation if
     GeneralVisualization::UseSimpleIsoLineDefinitions() is false */
  int SimpleIsoLineColorShadeClassCount(void) const;
  void SimpleIsoLineColorShadeClassCount(const int& newValue);

 private:
  SimpleIsoline& operator=(const SimpleIsoline& other) = delete;

  NFmiColor itsIsolineLabelBoxFillColor;
  NFmiColor itsContourLabelBoxFillColor;
  double itsIsoLineGab;
  NFmiColor itsIsolineColor;
  NFmiColor itsIsolineTextColor;
  float itsSimpleIsoLineGap;
  float itsSimpleIsoLineLabelHeight;
  bool fShowSimpleIsoLineLabelBox;
  float itsSimpleIsoLineWidth;
  int itsSimpleIsoLineLineStyle;
  bool fUseSingleColorsWithSimpleIsoLines;
  float itsSimpleIsoLineColorShadeLowValue;
  float itsSimpleIsoLineColorShadeMidValue;
  float itsSimpleIsoLineColorShadeHighValue;
  float itsSimpleIsoLineColorShadeHigh2Value;
  NFmiColor itsSimpleIsoLineColorShadeLowValueColor;
  NFmiColor itsSimpleIsoLineColorShadeMidValueColor;
  NFmiColor itsSimpleIsoLineColorShadeHighValueColor;
  NFmiColor itsSimpleIsoLineColorShadeHigh2ValueColor;
  int itsSimpleIsoLineColorShadeClassCount;
  bool fUseSecondaryColors;
  NFmiColor itsSecondaryIsolineColor;
  NFmiColor itsSecondaryIsolineTextColor;
};
