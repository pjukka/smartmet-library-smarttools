#include "NFmiHatchSettings.h"

NFmiHatchSettings::NFmiHatchSettings()
    : fUseWithIsoLineHatch1(false),
      fDrawIsoLineHatchWithBorders1(false),
      itsIsoLineHatchLowValue1(0),
      itsIsoLineHatchHighValue1(10),
      itsIsoLineHatchType1(1),
      itsIsoLineHatchColor1(0, 0, 0),
      itsIsoLineHatchBorderColor1(0, 0, 0),
      fUseWithIsoLineHatch2(false),
      fDrawIsoLineHatchWithBorders2(false),
      itsIsoLineHatchLowValue2(50),
      itsIsoLineHatchHighValue2(60),
      itsIsoLineHatchType2(2),
      itsIsoLineHatchColor2(0.5f, 0.5f, 0.5f)
{
}
NFmiHatchSettings::NFmiHatchSettings(const NFmiHatchSettings& other)
    : fUseWithIsoLineHatch1(other.UseWithIsoLineHatch1()),
      fDrawIsoLineHatchWithBorders1(other.DrawIsoLineHatchWithBorders1()),
      itsIsoLineHatchLowValue1(other.IsoLineHatchLowValue1()),
      itsIsoLineHatchHighValue1(other.IsoLineHatchHighValue1()),
      itsIsoLineHatchType1(other.IsoLineHatchType1()),
      itsIsoLineHatchColor1(other.IsoLineHatchColor1()),
      itsIsoLineHatchBorderColor1(other.IsoLineHatchBorderColor1()),
      fUseWithIsoLineHatch2(other.UseWithIsoLineHatch2()),
      fDrawIsoLineHatchWithBorders2(other.DrawIsoLineHatchWithBorders2()),
      itsIsoLineHatchLowValue2(other.IsoLineHatchLowValue2()),
      itsIsoLineHatchHighValue2(other.IsoLineHatchHighValue2()),
      itsIsoLineHatchType2(other.IsoLineHatchType2()),
      itsIsoLineHatchColor2(other.IsoLineHatchColor2())
{
}
NFmiHatchSettings::~NFmiHatchSettings() {}

bool NFmiHatchSettings::UseWithIsoLineHatch1(void) const { return fUseWithIsoLineHatch1; }
void NFmiHatchSettings::UseWithIsoLineHatch1(const bool& newValue) { fUseWithIsoLineHatch1 = newValue; }
bool NFmiHatchSettings::DrawIsoLineHatchWithBorders1(void) const
{
  return fDrawIsoLineHatchWithBorders1;
}
void NFmiHatchSettings::DrawIsoLineHatchWithBorders1(const bool& newValue)
{
  fDrawIsoLineHatchWithBorders1 = newValue;
}
float NFmiHatchSettings::IsoLineHatchLowValue1(void) const { return itsIsoLineHatchLowValue1; }
void NFmiHatchSettings::IsoLineHatchLowValue1(const float& newValue)
{
  itsIsoLineHatchLowValue1 = newValue;
}
float NFmiHatchSettings::IsoLineHatchHighValue1(void) const { return itsIsoLineHatchHighValue1; }
void NFmiHatchSettings::IsoLineHatchHighValue1(const float& newValue)
{
  itsIsoLineHatchHighValue1 = newValue;
}
int NFmiHatchSettings::IsoLineHatchType1(void) const { return itsIsoLineHatchType1; }
void NFmiHatchSettings::IsoLineHatchType1(const int& newValue) { itsIsoLineHatchType1 = newValue; }
const NFmiColor& NFmiHatchSettings::IsoLineHatchColor1(void) const { return itsIsoLineHatchColor1; }
void NFmiHatchSettings::IsoLineHatchColor1(const NFmiColor& newValue)
{
  itsIsoLineHatchColor1 = newValue;
}
const NFmiColor& NFmiHatchSettings::IsoLineHatchBorderColor1(void) const
{
  return itsIsoLineHatchBorderColor1;
}
void NFmiHatchSettings::IsoLineHatchBorderColor1(const NFmiColor& newValue)
{
  itsIsoLineHatchBorderColor1 = newValue;
}

bool NFmiHatchSettings::UseWithIsoLineHatch2(void) const { return fUseWithIsoLineHatch2; }
void NFmiHatchSettings::UseWithIsoLineHatch2(const bool& newValue) { fUseWithIsoLineHatch2 = newValue; }
bool NFmiHatchSettings::DrawIsoLineHatchWithBorders2(void) const
{
  return fDrawIsoLineHatchWithBorders2;
}
void NFmiHatchSettings::DrawIsoLineHatchWithBorders2(const bool& newValue)
{
  fDrawIsoLineHatchWithBorders2 = newValue;
}
float NFmiHatchSettings::IsoLineHatchLowValue2(void) const { return itsIsoLineHatchLowValue2; }
void NFmiHatchSettings::IsoLineHatchLowValue2(const float& newValue)
{
  itsIsoLineHatchLowValue2 = newValue;
}
float NFmiHatchSettings::IsoLineHatchHighValue2(void) const { return itsIsoLineHatchHighValue2; }
void NFmiHatchSettings::IsoLineHatchHighValue2(const float& newValue)
{
  itsIsoLineHatchHighValue2 = newValue;
}
int NFmiHatchSettings::IsoLineHatchType2(void) const { return itsIsoLineHatchType2; }
void NFmiHatchSettings::IsoLineHatchType2(const int& newValue) { itsIsoLineHatchType2 = newValue; }
const NFmiColor& NFmiHatchSettings::IsoLineHatchColor2(void) const { return itsIsoLineHatchColor2; }
void NFmiHatchSettings::IsoLineHatchColor2(const NFmiColor& newValue)
{
  itsIsoLineHatchColor2 = newValue;
}
