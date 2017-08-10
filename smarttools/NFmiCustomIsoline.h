#pragma once

#include <newbase/NFmiDataMatrix.h>

class NFmiCustomIsoline
{
 public:
  NFmiCustomIsoline();
  NFmiCustomIsoline(const NFmiCustomIsoline& other);
  virtual ~NFmiCustomIsoline();

  /* All the values where to set isoline. */
  const checkedVector<float>& SpecialIsoLineValues(void) const;
  void SetSpecialIsoLineValues(const checkedVector<float>& newValue);

  const checkedVector<float>& SpecialContourValues(void) const;
  void SetSpecialContourValues(const checkedVector<float>& newValue);

  /* 0=no label */
  const checkedVector<float>& SpecialIsoLineLabelHeight(void) const;
  void SetSpecialIsoLineLabelHeight(const checkedVector<float>& newValue);

  const checkedVector<float>& SpecialContourLabelHeight(void) const;
  void SetSpecialContourLabelHeight(const checkedVector<float>& newValue);

  const checkedVector<float>& SpecialIsoLineWidth(void) const;
  void SetSpecialIsoLineWidth(const checkedVector<float>& newValue);

  const checkedVector<float>& SpecialcontourWidth(void) const;
  void SetSpecialcontourWidth(const checkedVector<float>& newValue);

  const checkedVector<int>& SpecialIsoLineStyle(void) const;
  void SetSpecialIsoLineStyle(const checkedVector<int>& newValue);

  const checkedVector<int>& SpecialContourStyle(void) const;
  void SetSpecialContourStyle(const checkedVector<int>& newValue);

  /* Chosen color indices for isolines. */
  const checkedVector<int>& SpecialIsoLineColorIndexies(void) const;
  void SetSpecialIsoLineColorIndexies(const checkedVector<int>& newValue);

  const checkedVector<int>& SpecialContourColorIndexies(void) const;
  void SetSpecialContourColorIndexies(const checkedVector<int>& newValue);

  const checkedVector<bool>& SpecialIsoLineShowLabelBox(void) const;
  void SpecialIsoLineShowLabelBox(const checkedVector<bool>& newValue);

 private:
  NFmiCustomIsoline& operator=(const NFmiCustomIsoline& other) = delete;

  checkedVector<float> itsSpecialIsoLineValues;
  checkedVector<float> itsSpecialContourValues;
  checkedVector<float> itsSpecialIsoLineLabelHeight;
  checkedVector<float> itsSpecialContourLabelHeight;
  checkedVector<float> itsSpecialIsoLineWidth;
  checkedVector<float> itsSpecialContourWidth;
  checkedVector<int> itsSpecialIsoLineStyle;
  checkedVector<int> itsSpecialContourStyle;
  checkedVector<int> itsSpecialIsoLineColorIndexies;
  checkedVector<int> itsSpecialContourColorIndexies;
  checkedVector<bool> itsSpecialIsoLineShowLabelBox;
};
