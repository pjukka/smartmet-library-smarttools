#include "NFmiSimpleIsoline.h"

NFmiSimpleIsoline::NFmiSimpleIsoline()
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

NFmiSimpleIsoline::NFmiSimpleIsoline(const NFmiSimpleIsoline& other)
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

NFmiSimpleIsoline::~NFmiSimpleIsoline() {}

void NFmiSimpleIsoline::IsolineLabelBoxFillColor(const NFmiColor& theColor)
{
  itsIsolineLabelBoxFillColor = theColor;
  itsContourLabelBoxFillColor =
      theColor;  // **** Versio 3 parametri asetetaan toistaiseksi myös näin ****
};
const NFmiColor& NFmiSimpleIsoline::IsolineLabelBoxFillColor(void) const
{
  return itsIsolineLabelBoxFillColor;
};

void NFmiSimpleIsoline::ContourLabelBoxFillColor(const NFmiColor& theColor)
{
  itsContourLabelBoxFillColor = theColor;
};
const NFmiColor& NFmiSimpleIsoline::ContourLabelBoxFillColor(void) const
{
  return itsContourLabelBoxFillColor;
};

void NFmiSimpleIsoline::IsoLineGab(const double theIsoLineGab)
{
  itsIsoLineGab = theIsoLineGab;
  if (itsIsoLineGab == 0) itsIsoLineGab = 1;  // gappi ei voi olla 0
}
double NFmiSimpleIsoline::IsoLineGab(void) const { return itsIsoLineGab; };

const NFmiColor& NFmiSimpleIsoline::IsolineColor(void) const
{
  return fUseSecondaryColors ? itsSecondaryIsolineColor : itsIsolineColor;
};
void NFmiSimpleIsoline::IsolineColor(const NFmiColor& newColor) { itsIsolineColor = newColor; }

void NFmiSimpleIsoline::IsolineTextColor(const NFmiColor& newColor) { itsIsolineTextColor = newColor; }
const NFmiColor& NFmiSimpleIsoline::IsolineTextColor(void) const
{
  return fUseSecondaryColors ? itsSecondaryIsolineTextColor : itsIsolineTextColor;
}

float NFmiSimpleIsoline::SimpleIsoLineGap(void) const { return itsSimpleIsoLineGap; }
void NFmiSimpleIsoline::SimpleIsoLineGap(const float& newValue) { itsSimpleIsoLineGap = newValue; }

float NFmiSimpleIsoline::SimpleIsoLineLabelHeight(void) const { return itsSimpleIsoLineLabelHeight; }
void NFmiSimpleIsoline::SimpleIsoLineLabelHeight(const float& newValue)
{
  itsSimpleIsoLineLabelHeight = newValue;
}

bool NFmiSimpleIsoline::ShowSimpleIsoLineLabelBox(void) const { return fShowSimpleIsoLineLabelBox; }
void NFmiSimpleIsoline::ShowSimpleIsoLineLabelBox(const bool& newValue)
{
  fShowSimpleIsoLineLabelBox = newValue;
}

float NFmiSimpleIsoline::SimpleIsoLineWidth(void) const { return itsSimpleIsoLineWidth; }
void NFmiSimpleIsoline::SimpleIsoLineWidth(const float& newValue) { itsSimpleIsoLineWidth = newValue; }

int NFmiSimpleIsoline::SimpleIsoLineLineStyle(void) const { return itsSimpleIsoLineLineStyle; }
void NFmiSimpleIsoline::SimpleIsoLineLineStyle(const int& newValue)
{
  itsSimpleIsoLineLineStyle = newValue;
}

bool NFmiSimpleIsoline::UseSingleColorsWithSimpleIsoLines(void) const
{
  return fUseSingleColorsWithSimpleIsoLines;
}
void NFmiSimpleIsoline::UseSingleColorsWithSimpleIsoLines(const bool& newValue)
{
  fUseSingleColorsWithSimpleIsoLines = newValue;
}

float NFmiSimpleIsoline::SimpleIsoLineColorShadeLowValue(void) const
{
  return itsSimpleIsoLineColorShadeLowValue;
}
void NFmiSimpleIsoline::SimpleIsoLineColorShadeLowValue(const float& newValue)
{
  itsSimpleIsoLineColorShadeLowValue = newValue;
}
float NFmiSimpleIsoline::SimpleIsoLineColorShadeMidValue(void) const
{
  return itsSimpleIsoLineColorShadeMidValue;
}
void NFmiSimpleIsoline::SimpleIsoLineColorShadeMidValue(const float& newValue)
{
  itsSimpleIsoLineColorShadeMidValue = newValue;
}
float NFmiSimpleIsoline::SimpleIsoLineColorShadeHighValue(void) const
{
  return itsSimpleIsoLineColorShadeHighValue;
}
void NFmiSimpleIsoline::SimpleIsoLineColorShadeHighValue(const float& newValue)
{
  itsSimpleIsoLineColorShadeHighValue = newValue;
  itsSimpleIsoLineColorShadeHigh2Value = newValue;
}

float NFmiSimpleIsoline::SimpleIsoLineColorShadeHigh2Value(void) const
{
  return itsSimpleIsoLineColorShadeHigh2Value;
}
void NFmiSimpleIsoline::SimpleIsoLineColorShadeHigh2Value(const float& newValue)
{
  itsSimpleIsoLineColorShadeHigh2Value = newValue;
}

const NFmiColor& NFmiSimpleIsoline::SimpleIsoLineColorShadeLowValueColor(void) const
{
  return itsSimpleIsoLineColorShadeLowValueColor;
}
void NFmiSimpleIsoline::SimpleIsoLineColorShadeLowValueColor(const NFmiColor& newValue)
{
  itsSimpleIsoLineColorShadeLowValueColor = newValue;
}
const NFmiColor& NFmiSimpleIsoline::SimpleIsoLineColorShadeMidValueColor(void) const
{
  return itsSimpleIsoLineColorShadeMidValueColor;
}
void NFmiSimpleIsoline::SimpleIsoLineColorShadeMidValueColor(const NFmiColor& newValue)
{
  itsSimpleIsoLineColorShadeMidValueColor = newValue;
}
const NFmiColor& NFmiSimpleIsoline::SimpleIsoLineColorShadeHighValueColor(void) const
{
  return itsSimpleIsoLineColorShadeHighValueColor;
}
void NFmiSimpleIsoline::SimpleIsoLineColorShadeHighValueColor(const NFmiColor& newValue)
{
  itsSimpleIsoLineColorShadeHighValueColor = newValue;
  itsSimpleIsoLineColorShadeHigh2ValueColor = newValue;
}

const NFmiColor& NFmiSimpleIsoline::SimpleIsoLineColorShadeHigh2ValueColor(void) const
{
  return itsSimpleIsoLineColorShadeHigh2ValueColor;
}
void NFmiSimpleIsoline::SimpleIsoLineColorShadeHigh2ValueColor(const NFmiColor& newValue)
{
  itsSimpleIsoLineColorShadeHigh2ValueColor = newValue;
}

int NFmiSimpleIsoline::SimpleIsoLineColorShadeClassCount(void) const
{
  return itsSimpleIsoLineColorShadeClassCount;
}
void NFmiSimpleIsoline::SimpleIsoLineColorShadeClassCount(const int& newValue)
{
  itsSimpleIsoLineColorShadeClassCount = newValue;
}
