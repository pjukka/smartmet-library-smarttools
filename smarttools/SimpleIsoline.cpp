#include "SimpleIsoline.h"

SimpleIsoline::SimpleIsoline()
    : itsIsolineLabelBoxFillColor(NFmiColor(1., 1., 0.)),  // yellow
      itsContourLabelBoxFillColor(NFmiColor(1., 1., 0.)),
      itsIsoLineGab(1.),
      itsIsolineColor(NFmiColor(0., 0., 0.)),
      itsIsolineTextColor(NFmiColor(0., 0., 0.)),
      itsSimpleIsoLineGap(1),
      itsSimpleIsoLineLabelHeight(4),
      fShowSimpleIsoLineLabelBox(false),
      itsSimpleIsoLineWidth(0.2f),
      itsSimpleIsoLineLineStyle(0),
      fUseSingleColorsWithSimpleIsoLines(true),
      itsSimpleIsoLineColorShadeLowValue(0),
      itsSimpleIsoLineColorShadeMidValue(50),
      itsSimpleIsoLineColorShadeHighValue(100),
      itsSimpleIsoLineColorShadeHigh2Value(100),
      itsSimpleIsoLineColorShadeLowValueColor(0, 0, 1),
      itsSimpleIsoLineColorShadeMidValueColor(0, 1, 0),
      itsSimpleIsoLineColorShadeHighValueColor(0, 1, 0),
      itsSimpleIsoLineColorShadeHigh2ValueColor(0, 1, 0),
      itsSimpleIsoLineColorShadeClassCount(9),
      fUseSecondaryColors(false),
      itsSecondaryIsolineColor(0.6f, 0.6f, 0.6f),
      itsSecondaryIsolineTextColor(0.6f, 0.6f, 0.6f)
{
}

SimpleIsoline::SimpleIsoline(const SimpleIsoline& other)
    : itsIsolineLabelBoxFillColor(other.IsolineLabelBoxFillColor()),
      itsContourLabelBoxFillColor(other.ContourLabelBoxFillColor()),
      itsIsoLineGab(other.IsoLineGab()),
      itsIsolineColor(other.IsolineColor()),
      itsIsolineTextColor(other.IsolineTextColor()),
      itsSimpleIsoLineGap(other.SimpleIsoLineGap()),
      itsSimpleIsoLineLabelHeight(other.SimpleIsoLineLabelHeight()),
      fShowSimpleIsoLineLabelBox(other.ShowSimpleIsoLineLabelBox()),
      itsSimpleIsoLineWidth(other.SimpleIsoLineWidth()),
      itsSimpleIsoLineLineStyle(other.SimpleIsoLineLineStyle()),
      fUseSingleColorsWithSimpleIsoLines(other.UseSingleColorsWithSimpleIsoLines()),
      itsSimpleIsoLineColorShadeLowValue(other.SimpleIsoLineColorShadeLowValue()),
      itsSimpleIsoLineColorShadeMidValue(other.SimpleIsoLineColorShadeMidValue()),
      itsSimpleIsoLineColorShadeHighValue(other.SimpleIsoLineColorShadeHighValue()),
      itsSimpleIsoLineColorShadeHigh2Value(other.SimpleIsoLineColorShadeHigh2Value()),
      itsSimpleIsoLineColorShadeLowValueColor(other.SimpleIsoLineColorShadeLowValueColor()),
      itsSimpleIsoLineColorShadeMidValueColor(other.SimpleIsoLineColorShadeMidValueColor()),
      itsSimpleIsoLineColorShadeHighValueColor(other.SimpleIsoLineColorShadeHighValueColor()),
      itsSimpleIsoLineColorShadeHigh2ValueColor(other.SimpleIsoLineColorShadeHigh2ValueColor()),
      itsSimpleIsoLineColorShadeClassCount(other.SimpleIsoLineColorShadeClassCount()),
      fUseSecondaryColors(other.fUseSecondaryColors),
      itsSecondaryIsolineColor(other.itsSecondaryIsolineColor),
      itsSecondaryIsolineTextColor(other.itsSecondaryIsolineTextColor)
{
}

SimpleIsoline::~SimpleIsoline()
{
}

void SimpleIsoline::IsolineLabelBoxFillColor(const NFmiColor& theColor)
{
  itsIsolineLabelBoxFillColor = theColor;
  itsContourLabelBoxFillColor =
      theColor;  // **** Versio 3 parametri asetetaan toistaiseksi myös näin ****
};
const NFmiColor& SimpleIsoline::IsolineLabelBoxFillColor(void) const
{
  return itsIsolineLabelBoxFillColor;
};

void SimpleIsoline::ContourLabelBoxFillColor(const NFmiColor& theColor)
{
  itsContourLabelBoxFillColor = theColor;
};
const NFmiColor& SimpleIsoline::ContourLabelBoxFillColor(void) const
{
  return itsContourLabelBoxFillColor;
};

void SimpleIsoline::IsoLineGab(const double theIsoLineGab)
{
  itsIsoLineGab = theIsoLineGab;
  if (itsIsoLineGab == 0)
    itsIsoLineGab = 1;  // gappi ei voi olla 0
}
double SimpleIsoline::IsoLineGab(void) const
{
  return itsIsoLineGab;
};

const NFmiColor& SimpleIsoline::IsolineColor(void) const
{
  return fUseSecondaryColors ? itsSecondaryIsolineColor : itsIsolineColor;
};
void SimpleIsoline::IsolineColor(const NFmiColor& newColor)
{
  itsIsolineColor = newColor;
}

void SimpleIsoline::IsolineTextColor(const NFmiColor& newColor)
{
  itsIsolineTextColor = newColor;
}
const NFmiColor& SimpleIsoline::IsolineTextColor(void) const
{
  return fUseSecondaryColors ? itsSecondaryIsolineTextColor : itsIsolineTextColor;
}

float SimpleIsoline::SimpleIsoLineGap(void) const
{
  return itsSimpleIsoLineGap;
}
void SimpleIsoline::SimpleIsoLineGap(const float& newValue)
{
  itsSimpleIsoLineGap = newValue;
}

float SimpleIsoline::SimpleIsoLineLabelHeight(void) const
{
  return itsSimpleIsoLineLabelHeight;
}
void SimpleIsoline::SimpleIsoLineLabelHeight(const float& newValue)
{
  itsSimpleIsoLineLabelHeight = newValue;
}

bool SimpleIsoline::ShowSimpleIsoLineLabelBox(void) const
{
  return fShowSimpleIsoLineLabelBox;
}
void SimpleIsoline::ShowSimpleIsoLineLabelBox(const bool& newValue)
{
  fShowSimpleIsoLineLabelBox = newValue;
}

float SimpleIsoline::SimpleIsoLineWidth(void) const
{
  return itsSimpleIsoLineWidth;
}
void SimpleIsoline::SimpleIsoLineWidth(const float& newValue)
{
  itsSimpleIsoLineWidth = newValue;
}

int SimpleIsoline::SimpleIsoLineLineStyle(void) const
{
  return itsSimpleIsoLineLineStyle;
}
void SimpleIsoline::SimpleIsoLineLineStyle(const int& newValue)
{
  itsSimpleIsoLineLineStyle = newValue;
}

bool SimpleIsoline::UseSingleColorsWithSimpleIsoLines(void) const
{
  return fUseSingleColorsWithSimpleIsoLines;
}
void SimpleIsoline::UseSingleColorsWithSimpleIsoLines(const bool& newValue)
{
  fUseSingleColorsWithSimpleIsoLines = newValue;
}

float SimpleIsoline::SimpleIsoLineColorShadeLowValue(void) const
{
  return itsSimpleIsoLineColorShadeLowValue;
}
void SimpleIsoline::SimpleIsoLineColorShadeLowValue(const float& newValue)
{
  itsSimpleIsoLineColorShadeLowValue = newValue;
}
float SimpleIsoline::SimpleIsoLineColorShadeMidValue(void) const
{
  return itsSimpleIsoLineColorShadeMidValue;
}
void SimpleIsoline::SimpleIsoLineColorShadeMidValue(const float& newValue)
{
  itsSimpleIsoLineColorShadeMidValue = newValue;
}
float SimpleIsoline::SimpleIsoLineColorShadeHighValue(void) const
{
  return itsSimpleIsoLineColorShadeHighValue;
}
void SimpleIsoline::SimpleIsoLineColorShadeHighValue(const float& newValue)
{
  itsSimpleIsoLineColorShadeHighValue = newValue;
  itsSimpleIsoLineColorShadeHigh2Value = newValue;
}

float SimpleIsoline::SimpleIsoLineColorShadeHigh2Value(void) const
{
  return itsSimpleIsoLineColorShadeHigh2Value;
}
void SimpleIsoline::SimpleIsoLineColorShadeHigh2Value(const float& newValue)
{
  itsSimpleIsoLineColorShadeHigh2Value = newValue;
}

const NFmiColor& SimpleIsoline::SimpleIsoLineColorShadeLowValueColor(void) const
{
  return itsSimpleIsoLineColorShadeLowValueColor;
}
void SimpleIsoline::SimpleIsoLineColorShadeLowValueColor(const NFmiColor& newValue)
{
  itsSimpleIsoLineColorShadeLowValueColor = newValue;
}
const NFmiColor& SimpleIsoline::SimpleIsoLineColorShadeMidValueColor(void) const
{
  return itsSimpleIsoLineColorShadeMidValueColor;
}
void SimpleIsoline::SimpleIsoLineColorShadeMidValueColor(const NFmiColor& newValue)
{
  itsSimpleIsoLineColorShadeMidValueColor = newValue;
}
const NFmiColor& SimpleIsoline::SimpleIsoLineColorShadeHighValueColor(void) const
{
  return itsSimpleIsoLineColorShadeHighValueColor;
}
void SimpleIsoline::SimpleIsoLineColorShadeHighValueColor(const NFmiColor& newValue)
{
  itsSimpleIsoLineColorShadeHighValueColor = newValue;
  itsSimpleIsoLineColorShadeHigh2ValueColor = newValue;
}

const NFmiColor& SimpleIsoline::SimpleIsoLineColorShadeHigh2ValueColor(void) const
{
  return itsSimpleIsoLineColorShadeHigh2ValueColor;
}
void SimpleIsoline::SimpleIsoLineColorShadeHigh2ValueColor(const NFmiColor& newValue)
{
  itsSimpleIsoLineColorShadeHigh2ValueColor = newValue;
}

int SimpleIsoline::SimpleIsoLineColorShadeClassCount(void) const
{
  return itsSimpleIsoLineColorShadeClassCount;
}
void SimpleIsoline::SimpleIsoLineColorShadeClassCount(const int& newValue)
{
  itsSimpleIsoLineColorShadeClassCount = newValue;
}
