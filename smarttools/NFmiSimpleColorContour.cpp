#include "NFmiSimpleColorContour.h"

NFmiSimpleColorContour::NFmiSimpleColorContour()
    : NFmiSimpleIsoline(),
      itsContourGab(1.),
      itsContourColor(NFmiColor(0., 0., 0.)),
      itsSimpleContourWidth(0.2f),
      itsSimpleContourLineStyle(0),
      itsColorContouringColorShadeLowValue(0),
      itsColorContouringColorShadeMidValue(50),
      itsColorContouringColorShadeHighValue(100),
      itsColorContouringColorShadeHigh2Value(100),
      itsColorContouringColorShadeLowValueColor(0, 0, 1),
      itsColorContouringColorShadeMidValueColor(0, 1, 0),
      itsColorContouringColorShadeHighValueColor(0, 1, 0),
      itsColorContouringColorShadeHigh2ValueColor(0, 1, 0)
{
}

NFmiSimpleColorContour::NFmiSimpleColorContour(const NFmiSimpleColorContour& other)
    : NFmiSimpleIsoline(other),
      itsContourGab(other.ContourGab()),
      itsContourColor(other.ContourColor()),
      itsSimpleContourWidth(other.SimpleContourWidth()),
      itsSimpleContourLineStyle(other.SimpleContourLineStyle()),
      itsColorContouringColorShadeLowValue(other.ColorContouringColorShadeLowValue()),
      itsColorContouringColorShadeMidValue(other.ColorContouringColorShadeMidValue()),
      itsColorContouringColorShadeHighValue(other.ColorContouringColorShadeHighValue()),
      itsColorContouringColorShadeHigh2Value(other.ColorContouringColorShadeHigh2Value()),
      itsColorContouringColorShadeLowValueColor(other.ColorContouringColorShadeLowValueColor()),
      itsColorContouringColorShadeMidValueColor(other.ColorContouringColorShadeMidValueColor()),
      itsColorContouringColorShadeHighValueColor(other.ColorContouringColorShadeHighValueColor()),
      itsColorContouringColorShadeHigh2ValueColor(other.ColorContouringColorShadeHigh2ValueColor())

{
}

NFmiSimpleColorContour::~NFmiSimpleColorContour() {}

const NFmiColor& NFmiSimpleColorContour::ContourColor(void) const { return itsContourColor; }
void NFmiSimpleColorContour::ContourColor(const NFmiColor& newColor) { itsContourColor = newColor; }

float NFmiSimpleColorContour::SimpleContourWidth(void) const { return itsSimpleContourWidth; }
void NFmiSimpleColorContour::SimpleContourWidth(const float& newValue)
{
  itsSimpleContourWidth = newValue;
}

void NFmiSimpleColorContour::IsolineColor(const NFmiColor& newColor)
{
  NFmiSimpleIsoline::IsolineColor(newColor);
  itsContourColor = newColor;
}

const NFmiColor& NFmiSimpleColorContour::IsolineColor() const
{
  return NFmiSimpleIsoline::IsolineColor();
}

float NFmiSimpleColorContour::SimpleIsoLineWidth() const
{
  return NFmiSimpleIsoline::SimpleIsoLineWidth();
}

void NFmiSimpleColorContour::SimpleIsoLineWidth(const float& newValue)
{
  NFmiSimpleIsoline::SimpleIsoLineWidth(newValue);
  itsSimpleContourWidth = newValue;
}

int NFmiSimpleColorContour::SimpleContourLineStyle(void) const { return itsSimpleContourLineStyle; }

void NFmiSimpleColorContour::SimpleContourLineStyle(const int& newValue)
{
  itsSimpleContourLineStyle = newValue;
}

int NFmiSimpleColorContour::SimpleIsoLineLineStyle() const
{
  return NFmiSimpleIsoline::SimpleIsoLineLineStyle();
}
void NFmiSimpleColorContour::SimpleIsoLineLineStyle(const int& newValue)
{
  NFmiSimpleIsoline::SimpleIsoLineLineStyle(newValue);
  itsSimpleContourLineStyle = newValue;
}

double NFmiSimpleColorContour::ContourGab(void) const { return itsContourGab; }
void NFmiSimpleColorContour::ContourGab(const double theContourGab)
{
  itsContourGab = theContourGab;
  if (itsContourGab == 0) itsContourGab = 1;
}

float NFmiSimpleColorContour::ColorContouringColorShadeLowValue(void) const
{
  return itsColorContouringColorShadeLowValue;
}
void NFmiSimpleColorContour::ColorContouringColorShadeLowValue(const float& newValue)
{
  itsColorContouringColorShadeLowValue = newValue;
}
float NFmiSimpleColorContour::ColorContouringColorShadeMidValue(void) const
{
  return itsColorContouringColorShadeMidValue;
}
void NFmiSimpleColorContour::ColorContouringColorShadeMidValue(const float& newValue)
{
  itsColorContouringColorShadeMidValue = newValue;
}
float NFmiSimpleColorContour::ColorContouringColorShadeHighValue(void) const
{
  return itsColorContouringColorShadeHighValue;
}
void NFmiSimpleColorContour::ColorContouringColorShadeHighValue(const float& newValue)
{
  itsColorContouringColorShadeHighValue = newValue;
}
float NFmiSimpleColorContour::ColorContouringColorShadeHigh2Value(void) const
{
  return itsColorContouringColorShadeHigh2Value;
}
void NFmiSimpleColorContour::ColorContouringColorShadeHigh2Value(const float& newValue)
{
  itsColorContouringColorShadeHigh2Value = newValue;
}
const NFmiColor& NFmiSimpleColorContour::ColorContouringColorShadeLowValueColor(void) const
{
  return itsColorContouringColorShadeLowValueColor;
}
void NFmiSimpleColorContour::ColorContouringColorShadeLowValueColor(const NFmiColor& newValue)
{
  itsColorContouringColorShadeLowValueColor = newValue;
}
const NFmiColor& NFmiSimpleColorContour::ColorContouringColorShadeMidValueColor(void) const
{
  return itsColorContouringColorShadeMidValueColor;
}
void NFmiSimpleColorContour::ColorContouringColorShadeMidValueColor(const NFmiColor& newValue)
{
  itsColorContouringColorShadeMidValueColor = newValue;
}
const NFmiColor& NFmiSimpleColorContour::ColorContouringColorShadeHighValueColor(void) const
{
  return itsColorContouringColorShadeHighValueColor;
}
void NFmiSimpleColorContour::ColorContouringColorShadeHighValueColor(const NFmiColor& newValue)
{
  itsColorContouringColorShadeHighValueColor = newValue;
}
const NFmiColor& NFmiSimpleColorContour::ColorContouringColorShadeHigh2ValueColor(void) const
{
  return itsColorContouringColorShadeHigh2ValueColor;
}
void NFmiSimpleColorContour::ColorContouringColorShadeHigh2ValueColor(const NFmiColor& newValue)
{
  itsColorContouringColorShadeHigh2ValueColor = newValue;
}
