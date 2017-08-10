#include "NFmiGeneralVisualization.h"

NFmiGeneralVisualization::NFmiGeneralVisualization()
    : itsViewType(NFmiMetEditorTypes::kFmiIsoLineView),
      itsGridDataPresentationStyle(2),
      fUseIsoLineFeathering(false),
      fUseSimpleIsoLineDefinitions(true),
      fUseSimpleContourDefinitions(true),
      fUseSeparatorLinesBetweenColorContourClasses(true),
      itsIsoLineSplineSmoothingFactor(6),
      fDrawOnlyOverMask(false),
      fUseCustomColorContouring(false),
      fUseCustomIsoLineing(false),
      itsIsoLineLabelDigitCount(0),
      itsAlpha(100.f),  // dafault means 100 fully opaque
      fHidden(false),
      fShowDifference(false),
      fShowDifferenceToOriginalData(false)

{
}

NFmiGeneralVisualization::NFmiGeneralVisualization(const NFmiGeneralVisualization& other)
    : itsViewType(other.ViewType()),
      itsGridDataPresentationStyle(other.GridDataPresentationStyle()),
      fUseIsoLineFeathering(other.UseIsoLineFeathering()),
      fUseSimpleIsoLineDefinitions(other.UseSimpleIsoLineDefinitions()),
      fUseSimpleContourDefinitions(other.UseSimpleContourDefinitions()),
      fUseSeparatorLinesBetweenColorContourClasses(
          other.UseSeparatorLinesBetweenColorContourClasses()),
      itsIsoLineSplineSmoothingFactor(other.IsoLineSplineSmoothingFactor()),
      fDrawOnlyOverMask(other.DrawOnlyOverMask()),
      fUseCustomColorContouring(other.UseCustomColorContouring()),
      fUseCustomIsoLineing(other.UseCustomIsoLineing()),
      itsIsoLineLabelDigitCount(other.IsoLineLabelDigitCount()),
      itsAlpha(other.Alpha()),
      fHidden(other.IsParamHidden()),
      fShowDifference(other.ShowDifference()),
      fShowDifferenceToOriginalData(other.ShowDifferenceToOriginalData())

{
}

NFmiGeneralVisualization::~NFmiGeneralVisualization() {}

void NFmiGeneralVisualization::ViewType(const NFmiMetEditorTypes::View& theViewType)
{
  itsViewType = theViewType;
}

NFmiMetEditorTypes::View NFmiGeneralVisualization::ViewType(void) const { return itsViewType; }

int NFmiGeneralVisualization::GridDataPresentationStyle(void) const
{
  return itsGridDataPresentationStyle;
}
void NFmiGeneralVisualization::GridDataPresentationStyle(int newValue)
{
  itsGridDataPresentationStyle = newValue;
}

bool NFmiGeneralVisualization::UseIsoLineFeathering(void) const { return fUseIsoLineFeathering; }
void NFmiGeneralVisualization::UseIsoLineFeathering(bool newValue) { fUseIsoLineFeathering = newValue; }

bool NFmiGeneralVisualization::UseSimpleIsoLineDefinitions(void) const
{
  return fUseSimpleIsoLineDefinitions;
}
void NFmiGeneralVisualization::UseSimpleIsoLineDefinitions(bool newValue)
{
  fUseSimpleIsoLineDefinitions = newValue;
  fUseSimpleContourDefinitions = newValue;
}

bool NFmiGeneralVisualization::UseSimpleContourDefinitions(void) const
{
  return fUseSimpleContourDefinitions;
}
void NFmiGeneralVisualization::UseSimpleContourDefinitions(bool newValue)
{
  fUseSimpleContourDefinitions = newValue;
}

bool NFmiGeneralVisualization::UseSeparatorLinesBetweenColorContourClasses(void) const
{
  return fUseSeparatorLinesBetweenColorContourClasses;
}
void NFmiGeneralVisualization::UseSeparatorLinesBetweenColorContourClasses(bool newValue)
{
  fUseSeparatorLinesBetweenColorContourClasses = newValue;
}

float NFmiGeneralVisualization::IsoLineSplineSmoothingFactor(void) const
{
  return itsIsoLineSplineSmoothingFactor;
}
void NFmiGeneralVisualization::IsoLineSplineSmoothingFactor(float newValue)
{
  itsIsoLineSplineSmoothingFactor = newValue;
}

bool NFmiGeneralVisualization::DrawOnlyOverMask(void) const { return fDrawOnlyOverMask; }
void NFmiGeneralVisualization::DrawOnlyOverMask(bool newValue) { fDrawOnlyOverMask = newValue; }

bool NFmiGeneralVisualization::UseCustomColorContouring(void) const
{
  return fUseCustomColorContouring;
}
void NFmiGeneralVisualization::UseCustomColorContouring(bool newValue)
{
  fUseCustomColorContouring = newValue;
  fUseCustomIsoLineing = newValue;
}

bool NFmiGeneralVisualization::UseCustomIsoLineing(void) const { return fUseCustomIsoLineing; }
void NFmiGeneralVisualization::UseCustomIsoLineing(bool newValue) { fUseCustomIsoLineing = newValue; }

int NFmiGeneralVisualization::IsoLineLabelDigitCount(void) const { return itsIsoLineLabelDigitCount; }
void NFmiGeneralVisualization::IsoLineLabelDigitCount(int newValue)
{
  itsIsoLineLabelDigitCount = newValue;
  if (itsIsoLineLabelDigitCount > 10) itsIsoLineLabelDigitCount = 10;
}

float NFmiGeneralVisualization::Alpha(void) const { return itsAlpha; }
void NFmiGeneralVisualization::Alpha(float newValue)
{
  itsAlpha = newValue;
  if (itsAlpha < NFmiGeneralVisualization::itsMinAlpha) itsAlpha = NFmiGeneralVisualization::itsMinAlpha;
  if (itsAlpha > 100.f) itsAlpha = 100.f;
}

void NFmiGeneralVisualization::HideParam(bool newValue) { fHidden = newValue; }
bool NFmiGeneralVisualization::IsParamHidden(void) const { return fHidden; }

bool NFmiGeneralVisualization::ShowDifference(void) const { return fShowDifference; }
void NFmiGeneralVisualization::ShowDifference(bool newState) { fShowDifference = newState; }

bool NFmiGeneralVisualization::ShowDifferenceToOriginalData(void) const
{
  return fShowDifferenceToOriginalData;
}
void NFmiGeneralVisualization::ShowDifferenceToOriginalData(bool newValue)
{
  fShowDifferenceToOriginalData = newValue;
}
