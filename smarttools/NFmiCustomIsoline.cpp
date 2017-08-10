#include "NFmiCustomIsoline.h"

NFmiCustomIsoline::NFmiCustomIsoline() {}

NFmiCustomIsoline::NFmiCustomIsoline(const NFmiCustomIsoline& other)
    : itsSpecialIsoLineValues(other.SpecialIsoLineValues()),
      itsSpecialContourValues(other.SpecialContourValues()),
      itsSpecialIsoLineLabelHeight(other.SpecialIsoLineLabelHeight()),
      itsSpecialContourLabelHeight(other.SpecialContourLabelHeight()),
      itsSpecialIsoLineWidth(other.SpecialIsoLineWidth()),
      itsSpecialContourWidth(other.SpecialcontourWidth()),
      itsSpecialIsoLineStyle(other.SpecialIsoLineStyle()),
      itsSpecialContourStyle(other.SpecialContourStyle()),
      itsSpecialIsoLineColorIndexies(other.SpecialIsoLineColorIndexies()),
      itsSpecialContourColorIndexies(other.SpecialContourColorIndexies()),
      itsSpecialIsoLineShowLabelBox(other.SpecialIsoLineShowLabelBox())

{
}

NFmiCustomIsoline::~NFmiCustomIsoline() {}

const checkedVector<float>& NFmiCustomIsoline::SpecialIsoLineValues(void) const
{
  return itsSpecialIsoLineValues;
}
void NFmiCustomIsoline::SetSpecialIsoLineValues(const checkedVector<float>& newValue)
{
  itsSpecialIsoLineValues = newValue;
  itsSpecialContourValues = newValue;
}

const checkedVector<float>& NFmiCustomIsoline::SpecialContourValues(void) const
{
  return itsSpecialContourValues;
}
void NFmiCustomIsoline::SetSpecialContourValues(const checkedVector<float>& newValue)
{
  itsSpecialContourValues = newValue;
}

const checkedVector<float>& NFmiCustomIsoline::SpecialIsoLineLabelHeight(void) const
{
  return itsSpecialIsoLineLabelHeight;
}
void NFmiCustomIsoline::SetSpecialIsoLineLabelHeight(const checkedVector<float>& newValue)
{
  itsSpecialIsoLineLabelHeight = newValue;
  itsSpecialContourLabelHeight = newValue;
}

const checkedVector<float>& NFmiCustomIsoline::SpecialContourLabelHeight(void) const
{
  return itsSpecialContourLabelHeight;
}
void NFmiCustomIsoline::SetSpecialContourLabelHeight(const checkedVector<float>& newValue)
{
  itsSpecialContourLabelHeight = newValue;
}

const checkedVector<float>& NFmiCustomIsoline::SpecialIsoLineWidth(void) const
{
  return itsSpecialIsoLineWidth;
}
void NFmiCustomIsoline::SetSpecialIsoLineWidth(const checkedVector<float>& newValue)
{
  itsSpecialIsoLineWidth = newValue;
  itsSpecialContourWidth = newValue;
}

const checkedVector<float>& NFmiCustomIsoline::SpecialcontourWidth(void) const
{
  return itsSpecialContourWidth;
}
void NFmiCustomIsoline::SetSpecialcontourWidth(const checkedVector<float>& newValue)
{
  itsSpecialContourWidth = newValue;
}

const checkedVector<int>& NFmiCustomIsoline::SpecialIsoLineStyle(void) const
{
  return itsSpecialIsoLineStyle;
}
void NFmiCustomIsoline::SetSpecialIsoLineStyle(const checkedVector<int>& newValue)
{
  itsSpecialIsoLineStyle = newValue;
  itsSpecialContourStyle = newValue;
}

const checkedVector<int>& NFmiCustomIsoline::SpecialContourStyle(void) const
{
  return itsSpecialContourStyle;
}
void NFmiCustomIsoline::SetSpecialContourStyle(const checkedVector<int>& newValue)
{
  itsSpecialContourStyle = newValue;
}

const checkedVector<int>& NFmiCustomIsoline::SpecialIsoLineColorIndexies(void) const
{
  return itsSpecialIsoLineColorIndexies;
}
void NFmiCustomIsoline::SetSpecialIsoLineColorIndexies(const checkedVector<int>& newValue)
{
  itsSpecialIsoLineColorIndexies = newValue;
  itsSpecialContourColorIndexies = newValue;
}

const checkedVector<int>& NFmiCustomIsoline::SpecialContourColorIndexies(void) const
{
  return itsSpecialContourColorIndexies;
}
void NFmiCustomIsoline::SetSpecialContourColorIndexies(const checkedVector<int>& newValue)
{
  itsSpecialContourColorIndexies = newValue;
}

const checkedVector<bool>& NFmiCustomIsoline::SpecialIsoLineShowLabelBox(void) const
{
  return itsSpecialIsoLineShowLabelBox;
}
void NFmiCustomIsoline::SpecialIsoLineShowLabelBox(const checkedVector<bool>& newValue)
{
  itsSpecialIsoLineShowLabelBox = newValue;
}
