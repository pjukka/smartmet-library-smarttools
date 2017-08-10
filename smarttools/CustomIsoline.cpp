#include "CustomIsoline.h"

CustomIsoline::CustomIsoline() {}

CustomIsoline::CustomIsoline(const CustomIsoline& other)
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

CustomIsoline::~CustomIsoline() {}

const checkedVector<float>& CustomIsoline::SpecialIsoLineValues(void) const
{
  return itsSpecialIsoLineValues;
}
void CustomIsoline::SetSpecialIsoLineValues(const checkedVector<float>& newValue)
{
  itsSpecialIsoLineValues = newValue;
  itsSpecialContourValues = newValue;
}

const checkedVector<float>& CustomIsoline::SpecialContourValues(void) const
{
  return itsSpecialContourValues;
}
void CustomIsoline::SetSpecialContourValues(const checkedVector<float>& newValue)
{
  itsSpecialContourValues = newValue;
}

const checkedVector<float>& CustomIsoline::SpecialIsoLineLabelHeight(void) const
{
  return itsSpecialIsoLineLabelHeight;
}
void CustomIsoline::SetSpecialIsoLineLabelHeight(const checkedVector<float>& newValue)
{
  itsSpecialIsoLineLabelHeight = newValue;
  itsSpecialContourLabelHeight = newValue;
}

const checkedVector<float>& CustomIsoline::SpecialContourLabelHeight(void) const
{
  return itsSpecialContourLabelHeight;
}
void CustomIsoline::SetSpecialContourLabelHeight(const checkedVector<float>& newValue)
{
  itsSpecialContourLabelHeight = newValue;
}

const checkedVector<float>& CustomIsoline::SpecialIsoLineWidth(void) const
{
  return itsSpecialIsoLineWidth;
}
void CustomIsoline::SetSpecialIsoLineWidth(const checkedVector<float>& newValue)
{
  itsSpecialIsoLineWidth = newValue;
  itsSpecialContourWidth = newValue;
}

const checkedVector<float>& CustomIsoline::SpecialcontourWidth(void) const
{
  return itsSpecialContourWidth;
}
void CustomIsoline::SetSpecialcontourWidth(const checkedVector<float>& newValue)
{
  itsSpecialContourWidth = newValue;
}

const checkedVector<int>& CustomIsoline::SpecialIsoLineStyle(void) const
{
  return itsSpecialIsoLineStyle;
}
void CustomIsoline::SetSpecialIsoLineStyle(const checkedVector<int>& newValue)
{
  itsSpecialIsoLineStyle = newValue;
  itsSpecialContourStyle = newValue;
}

const checkedVector<int>& CustomIsoline::SpecialContourStyle(void) const
{
  return itsSpecialContourStyle;
}
void CustomIsoline::SetSpecialContourStyle(const checkedVector<int>& newValue)
{
  itsSpecialContourStyle = newValue;
}

const checkedVector<int>& CustomIsoline::SpecialIsoLineColorIndexies(void) const
{
  return itsSpecialIsoLineColorIndexies;
}
void CustomIsoline::SetSpecialIsoLineColorIndexies(const checkedVector<int>& newValue)
{
  itsSpecialIsoLineColorIndexies = newValue;
  itsSpecialContourColorIndexies = newValue;
}

const checkedVector<int>& CustomIsoline::SpecialContourColorIndexies(void) const
{
  return itsSpecialContourColorIndexies;
}
void CustomIsoline::SetSpecialContourColorIndexies(const checkedVector<int>& newValue)
{
  itsSpecialContourColorIndexies = newValue;
}

const checkedVector<bool>& CustomIsoline::SpecialIsoLineShowLabelBox(void) const
{
  return itsSpecialIsoLineShowLabelBox;
}
void CustomIsoline::SpecialIsoLineShowLabelBox(const checkedVector<bool>& newValue)
{
  itsSpecialIsoLineShowLabelBox = newValue;
}
