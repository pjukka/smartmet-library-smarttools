#pragma once

#include "NFmiMetEditorTypes.h"

class NFmiGeneralVisualization
{
  /* 5 is hardly visible */
  const float itsMinAlpha = 5.f;

 public:
  NFmiGeneralVisualization();
  NFmiGeneralVisualization(const NFmiGeneralVisualization& other);
  virtual ~NFmiGeneralVisualization();

  /* Specifies a type of view e.g. symbol, isoline, text..  */
  void ViewType(const NFmiMetEditorTypes::View& theViewType);
  NFmiMetEditorTypes::View ViewType(void) const;

  /* Specifies a type of view if grid data is in question.
     Possible values are defined in NFmiMetEditorTypes::View */
  int GridDataPresentationStyle(void) const;
  void GridDataPresentationStyle(int newValue);

  /* Isoline thinning option */
  bool UseIsoLineFeathering(void) const;
  void UseIsoLineFeathering(bool newValue);

  /* Every value has its special definitions if false.
     All values are handled in simplyfied way if true. */
  bool UseSimpleIsoLineDefinitions(void) const;
  void UseSimpleIsoLineDefinitions(bool newValue);

  bool UseSimpleContourDefinitions(void) const;
  void UseSimpleContourDefinitions(bool newValue);

  /* Draw lines between value and color */
  bool UseSeparatorLinesBetweenColorContourClasses(void) const;
  void UseSeparatorLinesBetweenColorContourClasses(bool newValue);

  /* Value range is 0-10, 0=no smoothing, 10=maximum corner rounding */
  float IsoLineSplineSmoothingFactor(void) const;
  void IsoLineSplineSmoothingFactor(float newValue);

  /* Draw data only over a mask if true */
  bool DrawOnlyOverMask(void) const;
  void DrawOnlyOverMask(bool newValue);

  /* It is allowed to define class boundaries and colors if true */
  bool UseCustomColorContouring(void) const;
  void UseCustomColorContouring(bool newValue);

  bool UseCustomIsoLineing(void) const;
  void UseCustomIsoLineing(bool newValue);

  /* Number of digits used in isoline label */
  int IsoLineLabelDigitCount(void) const;
  void IsoLineLabelDigitCount(int newValue);

  /* Value of transparency. 0 is fully transparent and 100 fully opaque.
     Lower limit is defined by itsMinAlpha.  */
  float Alpha(void) const;
  void Alpha(float newValue);

  /* Data (view) is hidden if true */
  void HideParam(bool newValue);
  bool IsParamHidden(void) const;

  /* Is value shown or a difference between this and previous timestamp.
     The state won't be stored in file while saving data. */
  bool ShowDifference(void) const;
  void ShowDifference(bool newState);

  bool ShowDifferenceToOriginalData(void) const;
  void ShowDifferenceToOriginalData(bool newValue);

 private:
  NFmiGeneralVisualization& operator=(const NFmiGeneralVisualization& other) = delete;

  NFmiMetEditorTypes::View itsViewType;
  int itsGridDataPresentationStyle;
  bool fUseIsoLineFeathering;
  bool fUseSimpleIsoLineDefinitions;
  bool fUseSimpleContourDefinitions;
  bool fUseSeparatorLinesBetweenColorContourClasses;
  float itsIsoLineSplineSmoothingFactor;
  bool fDrawOnlyOverMask;
  bool fUseCustomColorContouring;
  bool fUseCustomIsoLineing;
  int itsIsoLineLabelDigitCount;
  float itsAlpha;
  bool fHidden;
  bool fShowDifference;
  bool fShowDifferenceToOriginalData;
};
