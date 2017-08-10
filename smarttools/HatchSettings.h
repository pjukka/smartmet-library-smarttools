#pragma once

#include "NFmiColor.h"

class HatchSettings
{
 public:
  HatchSettings();
  HatchSettings(const HatchSettings& other);
  virtual ~HatchSettings();

  bool UseWithIsoLineHatch1(void) const;
  void UseWithIsoLineHatch1(const bool& newValue);

  bool DrawIsoLineHatchWithBorders1(void) const;
  void DrawIsoLineHatchWithBorders1(const bool& newValue);

  float IsoLineHatchLowValue1(void) const;
  void IsoLineHatchLowValue1(const float& newValue);

  float IsoLineHatchHighValue1(void) const;
  void IsoLineHatchHighValue1(const float& newValue);

  int IsoLineHatchType1(void) const;
  void IsoLineHatchType1(const int& newValue);

  const NFmiColor& IsoLineHatchColor1(void) const;
  void IsoLineHatchColor1(const NFmiColor& newValue);

  const NFmiColor& IsoLineHatchBorderColor1(void) const;
  void IsoLineHatchBorderColor1(const NFmiColor& newValue);

  bool UseWithIsoLineHatch2(void) const;
  void UseWithIsoLineHatch2(const bool& newValue);

  bool DrawIsoLineHatchWithBorders2(void) const;
  void DrawIsoLineHatchWithBorders2(const bool& newValue);

  float IsoLineHatchLowValue2(void) const;
  void IsoLineHatchLowValue2(const float& newValue);

  float IsoLineHatchHighValue2(void) const;
  void IsoLineHatchHighValue2(const float& newValue);

  int IsoLineHatchType2(void) const;
  void IsoLineHatchType2(const int& newValue);

  const NFmiColor& IsoLineHatchColor2(void) const;
  void IsoLineHatchColor2(const NFmiColor& newValue);

 private:
  HatchSettings& operator=(const HatchSettings& other) = delete;

  bool fUseWithIsoLineHatch1;
  bool fDrawIsoLineHatchWithBorders1;
  float itsIsoLineHatchLowValue1;   // hatch alueen ala-arvo
  float itsIsoLineHatchHighValue1;  // hatch alueen yläarvo
  int itsIsoLineHatchType1;  // hatch tyyppi 1=vinoviiva oikealle, 2=vinoviiva vasemmalle jne.
  NFmiColor itsIsoLineHatchColor1;
  NFmiColor itsIsoLineHatchBorderColor1;
  bool fUseWithIsoLineHatch2;
  bool fDrawIsoLineHatchWithBorders2;
  float itsIsoLineHatchLowValue2;   // hatch alueen ala-arvo
  float itsIsoLineHatchHighValue2;  // hatch alueen yläarvo
  int itsIsoLineHatchType2;  // hatch tyyppi 1=vinoviiva oikealle, 2=vinoviiva vasemmalle jne.
  NFmiColor itsIsoLineHatchColor2;
};
