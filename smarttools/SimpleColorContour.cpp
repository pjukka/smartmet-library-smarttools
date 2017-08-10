#include "SimpleColorContour.h"

SimpleColorContour::SimpleColorContour()
    : SimpleIsoline(),
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

SimpleColorContour::SimpleColorContour(const SimpleColorContour& other)
    : SimpleIsoline(other),
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

SimpleColorContour::~SimpleColorContour() {}

const NFmiColor& SimpleColorContour::ContourColor(void) const { return itsContourColor; }
void SimpleColorContour::ContourColor(const NFmiColor& newColor) { itsContourColor = newColor; }

float SimpleColorContour::SimpleContourWidth(void) const { return itsSimpleContourWidth; }
void SimpleColorContour::SimpleContourWidth(const float& newValue)
{
  itsSimpleContourWidth = newValue;
}

void SimpleColorContour::IsolineColor(const NFmiColor& newColor)
{
  SimpleIsoline::IsolineColor(newColor);
  itsContourColor = newColor;
}

const NFmiColor& SimpleColorContour::IsolineColor() const { return SimpleIsoline::IsolineColor(); }

float SimpleColorContour::SimpleIsoLineWidth() const { return SimpleIsoline::SimpleIsoLineWidth(); }

void SimpleColorContour::SimpleIsoLineWidth(const float& newValue)
{
  SimpleIsoline::SimpleIsoLineWidth(newValue);
  itsSimpleContourWidth = newValue;
}

int SimpleColorContour::SimpleContourLineStyle(void) const { return itsSimpleContourLineStyle; }

void SimpleColorContour::SimpleContourLineStyle(const int& newValue)
{
  itsSimpleContourLineStyle = newValue;
}

int SimpleColorContour::SimpleIsoLineLineStyle() const
{
  return SimpleIsoline::SimpleIsoLineLineStyle();
}
void SimpleColorContour::SimpleIsoLineLineStyle(const int& newValue)
{
  SimpleIsoline::SimpleIsoLineLineStyle(newValue);
  itsSimpleContourLineStyle = newValue;
}

double SimpleColorContour::ContourGab(void) const { return itsContourGab; }
void SimpleColorContour::ContourGab(const double theContourGab)
{
  itsContourGab = theContourGab;
  if (itsContourGab == 0) itsContourGab = 1;
}

float SimpleColorContour::ColorContouringColorShadeLowValue(void) const
{
  return itsColorContouringColorShadeLowValue;
}
void SimpleColorContour::ColorContouringColorShadeLowValue(const float& newValue)
{
  itsColorContouringColorShadeLowValue = newValue;
}
float SimpleColorContour::ColorContouringColorShadeMidValue(void) const
{
  return itsColorContouringColorShadeMidValue;
}
void SimpleColorContour::ColorContouringColorShadeMidValue(const float& newValue)
{
  itsColorContouringColorShadeMidValue = newValue;
}
float SimpleColorContour::ColorContouringColorShadeHighValue(void) const
{
  return itsColorContouringColorShadeHighValue;
}
void SimpleColorContour::ColorContouringColorShadeHighValue(const float& newValue)
{
  itsColorContouringColorShadeHighValue = newValue;
}
float SimpleColorContour::ColorContouringColorShadeHigh2Value(void) const
{
  return itsColorContouringColorShadeHigh2Value;
}
void SimpleColorContour::ColorContouringColorShadeHigh2Value(const float& newValue)
{
  itsColorContouringColorShadeHigh2Value = newValue;
}
const NFmiColor& SimpleColorContour::ColorContouringColorShadeLowValueColor(void) const
{
  return itsColorContouringColorShadeLowValueColor;
}
void SimpleColorContour::ColorContouringColorShadeLowValueColor(const NFmiColor& newValue)
{
  itsColorContouringColorShadeLowValueColor = newValue;
}
const NFmiColor& SimpleColorContour::ColorContouringColorShadeMidValueColor(void) const
{
  return itsColorContouringColorShadeMidValueColor;
}
void SimpleColorContour::ColorContouringColorShadeMidValueColor(const NFmiColor& newValue)
{
  itsColorContouringColorShadeMidValueColor = newValue;
}
const NFmiColor& SimpleColorContour::ColorContouringColorShadeHighValueColor(void) const
{
  return itsColorContouringColorShadeHighValueColor;
}
void SimpleColorContour::ColorContouringColorShadeHighValueColor(const NFmiColor& newValue)
{
  itsColorContouringColorShadeHighValueColor = newValue;
}
const NFmiColor& SimpleColorContour::ColorContouringColorShadeHigh2ValueColor(void) const
{
  return itsColorContouringColorShadeHigh2ValueColor;
}
void SimpleColorContour::ColorContouringColorShadeHigh2ValueColor(const NFmiColor& newValue)
{
  itsColorContouringColorShadeHigh2ValueColor = newValue;
}
