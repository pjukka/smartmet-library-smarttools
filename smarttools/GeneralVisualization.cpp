#include "GeneralVisualization.h"

GeneralVisualization::GeneralVisualization()
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

GeneralVisualization::GeneralVisualization(const GeneralVisualization& other)
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

GeneralVisualization::~GeneralVisualization()
{
}

void GeneralVisualization::ViewType(const NFmiMetEditorTypes::View& theViewType)
{
  itsViewType = theViewType;
}

NFmiMetEditorTypes::View GeneralVisualization::ViewType(void) const
{
  return itsViewType;
}

int GeneralVisualization::GridDataPresentationStyle(void) const
{
  return itsGridDataPresentationStyle;
}
void GeneralVisualization::GridDataPresentationStyle(int newValue)
{
  itsGridDataPresentationStyle = newValue;
}

bool GeneralVisualization::UseIsoLineFeathering(void) const
{
  return fUseIsoLineFeathering;
}
void GeneralVisualization::UseIsoLineFeathering(bool newValue)
{
  fUseIsoLineFeathering = newValue;
}

bool GeneralVisualization::UseSimpleIsoLineDefinitions(void) const
{
  return fUseSimpleIsoLineDefinitions;
}
void GeneralVisualization::UseSimpleIsoLineDefinitions(bool newValue)
{
  fUseSimpleIsoLineDefinitions = newValue;
  fUseSimpleContourDefinitions = newValue;
}

bool GeneralVisualization::UseSimpleContourDefinitions(void) const
{
  return fUseSimpleContourDefinitions;
}
void GeneralVisualization::UseSimpleContourDefinitions(bool newValue)
{
  fUseSimpleContourDefinitions = newValue;
}

bool GeneralVisualization::UseSeparatorLinesBetweenColorContourClasses(void) const
{
  return fUseSeparatorLinesBetweenColorContourClasses;
}
void GeneralVisualization::UseSeparatorLinesBetweenColorContourClasses(bool newValue)
{
  fUseSeparatorLinesBetweenColorContourClasses = newValue;
}

float GeneralVisualization::IsoLineSplineSmoothingFactor(void) const
{
  return itsIsoLineSplineSmoothingFactor;
}
void GeneralVisualization::IsoLineSplineSmoothingFactor(float newValue)
{
  itsIsoLineSplineSmoothingFactor = newValue;
}

bool GeneralVisualization::DrawOnlyOverMask(void) const
{
  return fDrawOnlyOverMask;
}
void GeneralVisualization::DrawOnlyOverMask(bool newValue)
{
  fDrawOnlyOverMask = newValue;
}

bool GeneralVisualization::UseCustomColorContouring(void) const
{
  return fUseCustomColorContouring;
}
void GeneralVisualization::UseCustomColorContouring(bool newValue)
{
  fUseCustomColorContouring = newValue;
  fUseCustomIsoLineing = newValue;
}

bool GeneralVisualization::UseCustomIsoLineing(void) const
{
  return fUseCustomIsoLineing;
}
void GeneralVisualization::UseCustomIsoLineing(bool newValue)
{
  fUseCustomIsoLineing = newValue;
}

int GeneralVisualization::IsoLineLabelDigitCount(void) const
{
  return itsIsoLineLabelDigitCount;
}
void GeneralVisualization::IsoLineLabelDigitCount(int newValue)
{
  itsIsoLineLabelDigitCount = newValue;
  if (itsIsoLineLabelDigitCount > 10)
    itsIsoLineLabelDigitCount = 10;
}

float GeneralVisualization::Alpha(void) const
{
  return itsAlpha;
}
void GeneralVisualization::Alpha(float newValue)
{
  itsAlpha = newValue;
  if (itsAlpha < GeneralVisualization::itsMinAlpha)
    itsAlpha = GeneralVisualization::itsMinAlpha;
  if (itsAlpha > 100.f)
    itsAlpha = 100.f;
}

void GeneralVisualization::HideParam(bool newValue)
{
  fHidden = newValue;
}
bool GeneralVisualization::IsParamHidden(void) const
{
  return fHidden;
}

bool GeneralVisualization::ShowDifference(void) const
{
  return fShowDifference;
}
void GeneralVisualization::ShowDifference(bool newState)
{
  fShowDifference = newState;
}

bool GeneralVisualization::ShowDifferenceToOriginalData(void) const
{
  return fShowDifferenceToOriginalData;
}
void GeneralVisualization::ShowDifferenceToOriginalData(bool newValue)
{
  fShowDifferenceToOriginalData = newValue;
}
