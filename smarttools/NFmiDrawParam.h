//**********************************************************
// C++ Class Name : NFmiDrawParam
// ---------------------------------------------------------
// Filetype: (HEADER)
// Filepath: D:/projekti/GDPro/GDTemp/NFmiDrawParam.h
//
//
// GDPro Properties
// ---------------------------------------------------
//  - GD Symbol Type    : CLD_Class
//  - GD Method         : UML ( 2.1.4 )
//  - GD System Name    : Met-editor Plan 2
//  - GD View Type      : Class Diagram
//  - GD View Name      : Markon ehdotus
// ---------------------------------------------------
//  Author         : pietarin
//  Creation Date  : Thur - Jan 28, 1999
//
//
//  Description:
//   En ole vielä varma tämän luokan tarkoituksesta/toiminnasta,
//   tämä on
//   Persan idea.
//
//  Change Log:
// Changed 1999.09.28/Marko	Lisäsin SecondaryIsoLineColor:in käytön (käytetään
//							vertailtaessa samantyyppistä dataa
// päällekkäin)
//
//**********************************************************
#pragma once

#include "GeneralData.h"
#include "GeneralVisualization.h"

#include "NFmiColor.h"
#include "NFmiMetEditorTypes.h"

#include <newbase/NFmiParameterName.h>
#include <newbase/NFmiPoint.h>
#include <newbase/NFmiDataMatrix.h>  // täältä tulee myös checkedVector
#include <boost/shared_ptr.hpp>

class NFmiDrawingEnvironment;

class NFmiDrawParam : public GeneralData, public GeneralVisualization
{
 public:
  NFmiDrawParam(void);
  NFmiDrawParam(const NFmiDataIdent& theParam,
                const NFmiLevel& theLevel,
                int thePriority,
                NFmiInfoData::Type theDataType =
                    NFmiInfoData::kNoDataType);  //, NFmiMetEditorCoordinatorMapOptions*
  // theMetEditorCoordinatorMapOptions=0);
  NFmiDrawParam(const NFmiDrawParam& other);
  virtual ~NFmiDrawParam(void);

  void Init(const NFmiDrawParam* theDrawParam, bool fInitOnlyDrawingOptions = false);
  void Init(const boost::shared_ptr<NFmiDrawParam>& theDrawParam,
            bool fInitOnlyDrawingOptions = false);
  bool UseArchiveModelData(void) const;
  bool IsModelRunDataType(void) const;
  static bool IsModelRunDataType(NFmiInfoData::Type theDataType);
  bool DoDataComparison(void);

  bool Init(const std::string& theFilename);
  bool StoreData(const std::string& theFilename);

  // --------------- "set" ja "get" metodit -----------------

  void Priority(int thePriority) { itsPriority = thePriority; };
  int Priority(void) const { return itsPriority; };
  void FrameColor(const NFmiColor& theFrameColor) { itsFrameColor = theFrameColor; };
  const NFmiColor& FrameColor(void) const { return itsFrameColor; };
  void FillColor(const NFmiColor& theFillColor) { itsFillColor = theFillColor; };
  const NFmiColor& FillColor(void) const { return itsFillColor; };
  void IsolineLabelBoxFillColor(const NFmiColor& theColor)
  {
    itsIsolineLabelBoxFillColor = theColor;
    itsContourLabelBoxFillColor =
        theColor;  // **** Versio 3 parametri asetetaan toistaiseksi myös näin ****
  };
  const NFmiColor& IsolineLabelBoxFillColor(void) const { return itsIsolineLabelBoxFillColor; };
  void ContourLabelBoxFillColor(const NFmiColor& theColor)
  {
    itsContourLabelBoxFillColor = theColor;
  };
  const NFmiColor& ContourLabelBoxFillColor(void) const { return itsContourLabelBoxFillColor; };
  void RelativeSize(const NFmiPoint& theRelativeSize) { itsRelativeSize = theRelativeSize; };
  const NFmiPoint& RelativeSize(void) const { return itsRelativeSize; };
  void RelativePositionOffset(const NFmiPoint& theRelativePositionOffset)
  {
    itsRelativePositionOffset = theRelativePositionOffset;
  };
  const NFmiPoint& RelativePositionOffset(void) const { return itsRelativePositionOffset; };
  void OnlyOneSymbolRelativeSize(const NFmiPoint& theOnlyOneSymbolRelativeSize)
  {
    itsOnlyOneSymbolRelativeSize = theOnlyOneSymbolRelativeSize;
  };
  const NFmiPoint& OnlyOneSymbolRelativeSize(void) const { return itsOnlyOneSymbolRelativeSize; };
  void OnlyOneSymbolRelativePositionOffset(const NFmiPoint& theOnlyOneSymbolRelativePositionOffset)
  {
    itsOnlyOneSymbolRelativePositionOffset = theOnlyOneSymbolRelativePositionOffset;
  };
  const NFmiPoint& OnlyOneSymbolRelativePositionOffset(void) const
  {
    return itsOnlyOneSymbolRelativePositionOffset;
  };
  void UseIsoLineGabWithCustomContours(const bool newState)
  {
    fUseIsoLineGabWithCustomContours = newState;
    fUseContourGabWithCustomContours =
        newState;  // **** Versio 3 parametri asetetaan toistaiseksi myös näin ****
  };
  bool UseIsoLineGabWithCustomContours(void) const { return fUseIsoLineGabWithCustomContours; };
  void UseContourGabWithCustomContours(const bool newState)
  {
    fUseContourGabWithCustomContours = newState;
  };
  bool UseContourGabWithCustomContours(void) const { return fUseContourGabWithCustomContours; };
  void IsoLineGab(const double theIsoLineGab)
  {
    itsIsoLineGab = theIsoLineGab;
    if (itsIsoLineGab == 0)
      itsIsoLineGab = 1;  // gappi ei voi olla 0
  }
  double IsoLineGab(void) const { return itsIsoLineGab; };
  void ContourGab(const double theContourGab)
  {
    itsContourGab = theContourGab;
    if (itsContourGab == 0)
      itsContourGab = 1;
  }
  double ContourGab(void) const { return itsContourGab; };
  void ModifyingStep(const double theModifyingStep) { itsModifyingStep = theModifyingStep; };
  double ModifyingStep(void) const { return itsModifyingStep; };
  //	void				 ModifyingUnit (bool theModifyingUnit) { fModifyingUnit =
  // theModifyingUnit; }
  //	bool			 ModifyingUnit (void) const { return fModifyingUnit; }
  const NFmiMetEditorTypes::View* PossibleViewTypeList(void) const
  {
    return itsPossibleViewTypeList;
  }
  int PossibleViewTypeCount(void) const { return itsPossibleViewTypeCount; };
  double AbsoluteMinValue(void) const { return itsAbsoluteMinValue; }
  void AbsoluteMinValue(double theAbsoluteMinValue) { itsAbsoluteMinValue = theAbsoluteMinValue; }
  double AbsoluteMaxValue(void) const { return itsAbsoluteMaxValue; }
  void AbsoluteMaxValue(double theAbsoluteMaxValue) { itsAbsoluteMaxValue = theAbsoluteMaxValue; }
  double TimeSeriesScaleMin(void) const { return itsTimeSeriesScaleMin; };
  double TimeSeriesScaleMax(void) const { return itsTimeSeriesScaleMax; };
  void TimeSeriesScaleMin(double theValue) { itsTimeSeriesScaleMin = theValue; };
  void TimeSeriesScaleMax(double theValue) { itsTimeSeriesScaleMax = theValue; };
  const NFmiColor& IsolineColor(void) const
  {
    return fUseSecondaryColors ? itsSecondaryIsolineColor : itsIsolineColor;
  };
  void IsolineColor(const NFmiColor& newColor)
  {
    itsIsolineColor = newColor;
    itsContourColor = newColor;  // **** Versio 3 parametri asetetaan toistaiseksi myös näin ****
  }

  const NFmiColor& ContourColor(void) const { return itsContourColor; };
  void ContourColor(const NFmiColor& newColor) { itsContourColor = newColor; };
  void IsolineTextColor(const NFmiColor& newColor)
  {
    itsIsolineTextColor = newColor;
    itsContourTextColor =
        newColor;  // **** Versio 3 parametri asetetaan toistaiseksi myös näin ****
  }
  const NFmiColor& IsolineTextColor(void) const
  {
    return fUseSecondaryColors ? itsSecondaryIsolineTextColor : itsIsolineTextColor;
  };

  void ContourTextColor(const NFmiColor& newColor) { itsContourTextColor = newColor; };
  const NFmiColor& ContourTextColor(void) const { return itsContourTextColor; };
  //	double TimeSerialModifyingLimit(void) const {return fModifyingUnit ?
  // itsTimeSerialModifyingLimit : 100;};
  double TimeSerialModifyingLimit(void) const { return itsTimeSerialModifyingLimit; };
  NFmiMetEditorTypes::View StationDataViewType(void) const { return itsStationDataViewType; };
  void TimeSerialModifyingLimit(double newValue) { itsTimeSerialModifyingLimit = newValue; };
  void StationDataViewType(NFmiMetEditorTypes::View newValue)
  {
    itsStationDataViewType = newValue;
  };

  void FileVersionNumber(const float theFileVersionNumber)
  {
    itsFileVersionNumber = theFileVersionNumber;
  };
  float FileVersionNumber(void) const { return itsFileVersionNumber; };
  bool ShowNumbers(void) const { return fShowNumbers; }
  void ShowNumbers(bool theValue) { fShowNumbers = theValue; }
  bool ShowMasks(void) const { return fShowMasks; }
  void ShowMasks(bool theValue) { fShowMasks = theValue; }
  bool ShowColors(void) const { return fShowColors; }
  void ShowColors(bool theValue) { fShowColors = theValue; }
  bool ShowColoredNumbers(void) const { return fShowColoredNumbers; }
  void ShowColoredNumbers(bool theValue) { fShowColoredNumbers = theValue; }
  bool ZeroColorMean(void) const { return fZeroColorMean; }
  void ZeroColorMean(bool theValue) { fZeroColorMean = theValue; }
  bool UseSecondaryColors(void) const { return fUseSecondaryColors; };
  void UseSecondaryColors(bool newState) { fUseSecondaryColors = newState; };
  //**************************************************************
  //********** 'versio 2' parametrien asetusfunktiot *************
  //**************************************************************
  float StationSymbolColorShadeLowValue(void) const { return itsStationSymbolColorShadeLowValue; }
  void StationSymbolColorShadeLowValue(float newValue)
  {
    itsStationSymbolColorShadeLowValue = newValue;
  }
  float StationSymbolColorShadeMidValue(void) const { return itsStationSymbolColorShadeMidValue; }
  void StationSymbolColorShadeMidValue(float newValue)
  {
    itsStationSymbolColorShadeMidValue = newValue;
  }
  float StationSymbolColorShadeHighValue(void) const { return itsStationSymbolColorShadeHighValue; }
  void StationSymbolColorShadeHighValue(float newValue)
  {
    itsStationSymbolColorShadeHighValue = newValue;
  }
  const NFmiColor& StationSymbolColorShadeLowValueColor(void) const
  {
    return itsStationSymbolColorShadeLowValueColor;
  }
  void StationSymbolColorShadeLowValueColor(const NFmiColor& newValue)
  {
    itsStationSymbolColorShadeLowValueColor = newValue;
  }
  const NFmiColor& StationSymbolColorShadeMidValueColor(void) const
  {
    return itsStationSymbolColorShadeMidValueColor;
  }
  void StationSymbolColorShadeMidValueColor(const NFmiColor& newValue)
  {
    itsStationSymbolColorShadeMidValueColor = newValue;
  }
  const NFmiColor& StationSymbolColorShadeHighValueColor(void) const
  {
    return itsStationSymbolColorShadeHighValueColor;
  }
  void StationSymbolColorShadeHighValueColor(const NFmiColor& newValue)
  {
    itsStationSymbolColorShadeHighValueColor = newValue;
  }
  int StationSymbolColorShadeClassCount(void) const { return itsStationSymbolColorShadeClassCount; }
  void StationSymbolColorShadeClassCount(int newValue)
  {
    itsStationSymbolColorShadeClassCount = newValue;
  }
  bool UseSymbolsInTextMode(void) const { return fUseSymbolsInTextMode; }
  void UseSymbolsInTextMode(bool newValue) { fUseSymbolsInTextMode = newValue; }
  int UsedSymbolListIndex(void) const { return itsUsedSymbolListIndex; }
  void UsedSymbolListIndex(int newValue) { itsUsedSymbolListIndex = newValue; }
  int SymbolIndexingMapListIndex(void) const { return itsSymbolIndexingMapListIndex; }
  void SymbolIndexingMapListIndex(int newValue) { itsSymbolIndexingMapListIndex = newValue; }
  bool UseContourFeathering(void) const { return fUseContourFeathering; }
  void UseContourFeathering(bool newValue) { fUseContourFeathering = newValue; }
  bool IsoLineLabelsOverLapping(void) const { return fIsoLineLabelsOverLapping; }
  void IsoLineLabelsOverLapping(bool newValue) { fIsoLineLabelsOverLapping = newValue; }
  bool ShowColorLegend(void) const { return fShowColorLegend; }
  void ShowColorLegend(bool newValue) { fShowColorLegend = newValue; }
  float SimpleIsoLineGap(void) const { return itsSimpleIsoLineGap; }
  void SimpleIsoLineGap(float newValue) { itsSimpleIsoLineGap = newValue; }
  float SimpleIsoLineZeroValue(void) const { return itsSimpleIsoLineZeroValue; }
  void SimpleIsoLineZeroValue(float newValue)
  {
    itsSimpleIsoLineZeroValue = newValue;
    itsSimpleContourZeroValue =
        newValue;  // **** Versio 3 parametri asetetaan toistaiseksi myös näin ****
  }

  float SimpleContourZeroValue(void) const { return itsSimpleContourZeroValue; }
  void SimpleContourZeroValue(float newValue) { itsSimpleContourZeroValue = newValue; }
  float SimpleIsoLineLabelHeight(void) const { return itsSimpleIsoLineLabelHeight; }
  void SimpleIsoLineLabelHeight(float newValue)
  {
    itsSimpleIsoLineLabelHeight = newValue;
    itsSimpleContourLabelHeight =
        newValue;  // **** Versio 3 parametri asetetaan toistaiseksi myös näin ****
  }

  float SimpleContourLabelHeight(void) const { return itsSimpleContourLabelHeight; }
  void SimpleContourLabelHeight(float newValue) { itsSimpleContourLabelHeight = newValue; }
  bool ShowSimpleIsoLineLabelBox(void) const { return fShowSimpleIsoLineLabelBox; }
  void ShowSimpleIsoLineLabelBox(bool newValue)
  {
    fShowSimpleIsoLineLabelBox = newValue;
    fShowSimpleContourLabelBox = newValue;
  }

  bool ShowSimpleContourLabelBox(void) const { return fShowSimpleContourLabelBox; }
  void ShowSimpleContourLabelBox(bool newValue) { fShowSimpleContourLabelBox = newValue; }
  float SimpleIsoLineWidth(void) const { return itsSimpleIsoLineWidth; }
  void SimpleIsoLineWidth(float newValue)
  {
    itsSimpleIsoLineWidth = newValue;
    itsSimpleContourWidth = newValue;
  }

  float SimpleContourWidth(void) const { return itsSimpleContourWidth; }
  void SimpleContourWidth(float newValue) { itsSimpleContourWidth = newValue; }
  int SimpleIsoLineLineStyle(void) const { return itsSimpleIsoLineLineStyle; }
  void SimpleIsoLineLineStyle(int newValue)
  {
    itsSimpleIsoLineLineStyle = newValue;
    itsSimpleContourLineStyle = newValue;
  }

  int SimpleContourLineStyle(void) const { return itsSimpleContourLineStyle; }
  void SimpleContourLineStyle(int newValue) { itsSimpleContourLineStyle = newValue; }
  bool UseSingleColorsWithSimpleIsoLines(void) const { return fUseSingleColorsWithSimpleIsoLines; }
  void UseSingleColorsWithSimpleIsoLines(bool newValue)
  {
    fUseSingleColorsWithSimpleIsoLines = newValue;
  }
  float SimpleIsoLineColorShadeLowValue(void) const { return itsSimpleIsoLineColorShadeLowValue; }
  void SimpleIsoLineColorShadeLowValue(float newValue)
  {
    itsSimpleIsoLineColorShadeLowValue = newValue;
  }
  float SimpleIsoLineColorShadeMidValue(void) const { return itsSimpleIsoLineColorShadeMidValue; }
  void SimpleIsoLineColorShadeMidValue(float newValue)
  {
    itsSimpleIsoLineColorShadeMidValue = newValue;
  }
  float SimpleIsoLineColorShadeHighValue(void) const { return itsSimpleIsoLineColorShadeHighValue; }
  void SimpleIsoLineColorShadeHighValue(float newValue)
  {
    itsSimpleIsoLineColorShadeHighValue = newValue;
    itsSimpleIsoLineColorShadeHigh2Value = newValue;
  }

  float SimpleIsoLineColorShadeHigh2Value(void) const
  {
    return itsSimpleIsoLineColorShadeHigh2Value;
  }
  void SimpleIsoLineColorShadeHigh2Value(float newValue)
  {
    itsSimpleIsoLineColorShadeHigh2Value = newValue;
  }

  const NFmiColor& SimpleIsoLineColorShadeLowValueColor(void) const
  {
    return itsSimpleIsoLineColorShadeLowValueColor;
  }
  void SimpleIsoLineColorShadeLowValueColor(const NFmiColor& newValue)
  {
    itsSimpleIsoLineColorShadeLowValueColor = newValue;
  }
  const NFmiColor& SimpleIsoLineColorShadeMidValueColor(void) const
  {
    return itsSimpleIsoLineColorShadeMidValueColor;
  }
  void SimpleIsoLineColorShadeMidValueColor(const NFmiColor& newValue)
  {
    itsSimpleIsoLineColorShadeMidValueColor = newValue;
  }
  const NFmiColor& SimpleIsoLineColorShadeHighValueColor(void) const
  {
    return itsSimpleIsoLineColorShadeHighValueColor;
  }
  void SimpleIsoLineColorShadeHighValueColor(const NFmiColor& newValue)
  {
    itsSimpleIsoLineColorShadeHighValueColor = newValue;
    itsSimpleIsoLineColorShadeHigh2ValueColor = newValue;
  }

  const NFmiColor& SimpleIsoLineColorShadeHigh2ValueColor(void) const
  {
    return itsSimpleIsoLineColorShadeHigh2ValueColor;
  }
  void SimpleIsoLineColorShadeHigh2ValueColor(const NFmiColor& newValue)
  {
    itsSimpleIsoLineColorShadeHigh2ValueColor = newValue;
  }

  int SimpleIsoLineColorShadeClassCount(void) const { return itsSimpleIsoLineColorShadeClassCount; }
  void SimpleIsoLineColorShadeClassCount(int newValue)
  {
    itsSimpleIsoLineColorShadeClassCount = newValue;
  }
  const checkedVector<float>& SpecialIsoLineValues(void) const { return itsSpecialIsoLineValues; }
  void SetSpecialIsoLineValues(const checkedVector<float>& newValue)
  {
    itsSpecialIsoLineValues = newValue;
    itsSpecialContourValues = newValue;
  }

  const checkedVector<float>& SpecialContourValues(void) const { return itsSpecialContourValues; }
  void SetSpecialContourValues(const checkedVector<float>& newValue)
  {
    itsSpecialContourValues = newValue;
  }

  const checkedVector<float>& SpecialIsoLineLabelHeight(void) const
  {
    return itsSpecialIsoLineLabelHeight;
  }
  void SetSpecialIsoLineLabelHeight(const checkedVector<float>& newValue)
  {
    itsSpecialIsoLineLabelHeight = newValue;
    itsSpecialContourLabelHeight = newValue;
  }

  const checkedVector<float>& SpecialContourLabelHeight(void) const
  {
    return itsSpecialContourLabelHeight;
  }
  void SetSpecialContourLabelHeight(const checkedVector<float>& newValue)
  {
    itsSpecialContourLabelHeight = newValue;
  }

  const checkedVector<float>& SpecialIsoLineWidth(void) const { return itsSpecialIsoLineWidth; }
  void SetSpecialIsoLineWidth(const checkedVector<float>& newValue)
  {
    itsSpecialIsoLineWidth = newValue;
    itsSpecialContourWidth = newValue;
  }

  const checkedVector<float>& SpecialcontourWidth(void) const { return itsSpecialContourWidth; }
  void SetSpecialcontourWidth(const checkedVector<float>& newValue)
  {
    itsSpecialContourWidth = newValue;
  }

  const checkedVector<int>& SpecialIsoLineStyle(void) const { return itsSpecialIsoLineStyle; }
  void SetSpecialIsoLineStyle(const checkedVector<int>& newValue)
  {
    itsSpecialIsoLineStyle = newValue;
    itsSpecialContourStyle = newValue;
  }

  const checkedVector<int>& SpecialContourStyle(void) const { return itsSpecialContourStyle; }
  void SetSpecialContourStyle(checkedVector<int>& newValue) { itsSpecialContourStyle = newValue; }
  const checkedVector<int>& SpecialIsoLineColorIndexies(void) const
  {
    return itsSpecialIsoLineColorIndexies;
  }
  void SetSpecialIsoLineColorIndexies(const checkedVector<int>& newValue)
  {
    itsSpecialIsoLineColorIndexies = newValue;
    itsSpecialContourColorIndexies = newValue;
  }

  const checkedVector<int>& SpecialContourColorIndexies(void) const
  {
    return itsSpecialContourColorIndexies;
  }
  void SetSpecialContourColorIndexies(const checkedVector<int>& newValue)
  {
    itsSpecialContourColorIndexies = newValue;
  }

  const checkedVector<bool>& SpecialIsoLineShowLabelBox(void) const
  {
    return itsSpecialIsoLineShowLabelBox;
  }
  void SpecialIsoLineShowLabelBox(checkedVector<bool>& newValue)
  {
    itsSpecialIsoLineShowLabelBox = newValue;
  }

  const checkedVector<float>& SpecialColorContouringValues(void) const
  {
    return itsSpecialColorContouringValues;
  }
  void SetSpecialColorContouringValues(const checkedVector<float>& newValue)
  {
    itsSpecialColorContouringValues = newValue;
  }
  const checkedVector<int>& SpecialColorContouringColorIndexies(void) const
  {
    return itsSpecialColorContouringColorIndexies;
  }
  void SpecialColorContouringColorIndexies(checkedVector<int>& newValue)
  {
    itsSpecialColorContouringColorIndexies = newValue;
  }
  float ColorContouringColorShadeLowValue(void) const
  {
    return itsColorContouringColorShadeLowValue;
  }
  void ColorContouringColorShadeLowValue(float newValue)
  {
    itsColorContouringColorShadeLowValue = newValue;
  }
  float ColorContouringColorShadeMidValue(void) const
  {
    return itsColorContouringColorShadeMidValue;
  }
  void ColorContouringColorShadeMidValue(float newValue)
  {
    itsColorContouringColorShadeMidValue = newValue;
  }
  float ColorContouringColorShadeHighValue(void) const
  {
    return itsColorContouringColorShadeHighValue;
  }
  void ColorContouringColorShadeHighValue(float newValue)
  {
    itsColorContouringColorShadeHighValue = newValue;
  }
  float ColorContouringColorShadeHigh2Value(void) const
  {
    return itsColorContouringColorShadeHigh2Value;
  }
  void ColorContouringColorShadeHigh2Value(float newValue)
  {
    itsColorContouringColorShadeHigh2Value = newValue;
  }
  const NFmiColor& ColorContouringColorShadeLowValueColor(void) const
  {
    return itsColorContouringColorShadeLowValueColor;
  }
  void ColorContouringColorShadeLowValueColor(const NFmiColor& newValue)
  {
    itsColorContouringColorShadeLowValueColor = newValue;
  }
  const NFmiColor& ColorContouringColorShadeMidValueColor(void) const
  {
    return itsColorContouringColorShadeMidValueColor;
  }
  void ColorContouringColorShadeMidValueColor(const NFmiColor& newValue)
  {
    itsColorContouringColorShadeMidValueColor = newValue;
  }
  const NFmiColor& ColorContouringColorShadeHighValueColor(void) const
  {
    return itsColorContouringColorShadeHighValueColor;
  }
  void ColorContouringColorShadeHighValueColor(const NFmiColor& newValue)
  {
    itsColorContouringColorShadeHighValueColor = newValue;
  }
  const NFmiColor& ColorContouringColorShadeHigh2ValueColor(void) const
  {
    return itsColorContouringColorShadeHigh2ValueColor;
  }
  void ColorContouringColorShadeHigh2ValueColor(const NFmiColor& newValue)
  {
    itsColorContouringColorShadeHigh2ValueColor = newValue;
  }
  bool UseWithIsoLineHatch1(void) const { return fUseWithIsoLineHatch1; }
  void UseWithIsoLineHatch1(bool newValue) { fUseWithIsoLineHatch1 = newValue; }
  bool DrawIsoLineHatchWithBorders1(void) const { return fDrawIsoLineHatchWithBorders1; }
  void DrawIsoLineHatchWithBorders1(bool newValue) { fDrawIsoLineHatchWithBorders1 = newValue; }
  float IsoLineHatchLowValue1(void) const { return itsIsoLineHatchLowValue1; }
  void IsoLineHatchLowValue1(float newValue) { itsIsoLineHatchLowValue1 = newValue; }
  float IsoLineHatchHighValue1(void) const { return itsIsoLineHatchHighValue1; }
  void IsoLineHatchHighValue1(float newValue) { itsIsoLineHatchHighValue1 = newValue; }
  int IsoLineHatchType1(void) const { return itsIsoLineHatchType1; }
  void IsoLineHatchType1(int newValue) { itsIsoLineHatchType1 = newValue; }
  const NFmiColor& IsoLineHatchColor1(void) const { return itsIsoLineHatchColor1; }
  void IsoLineHatchColor1(const NFmiColor& newValue) { itsIsoLineHatchColor1 = newValue; }
  const NFmiColor& IsoLineHatchBorderColor1(void) const { return itsIsoLineHatchBorderColor1; }
  void IsoLineHatchBorderColor1(const NFmiColor& newValue)
  {
    itsIsoLineHatchBorderColor1 = newValue;
  }

  bool UseWithIsoLineHatch2(void) const { return fUseWithIsoLineHatch2; }
  void UseWithIsoLineHatch2(bool newValue) { fUseWithIsoLineHatch2 = newValue; }
  bool DrawIsoLineHatchWithBorders2(void) const { return fDrawIsoLineHatchWithBorders2; }
  void DrawIsoLineHatchWithBorders2(bool newValue) { fDrawIsoLineHatchWithBorders2 = newValue; }
  float IsoLineHatchLowValue2(void) const { return itsIsoLineHatchLowValue2; }
  void IsoLineHatchLowValue2(float newValue) { itsIsoLineHatchLowValue2 = newValue; }
  float IsoLineHatchHighValue2(void) const { return itsIsoLineHatchHighValue2; }
  void IsoLineHatchHighValue2(float newValue) { itsIsoLineHatchHighValue2 = newValue; }
  int IsoLineHatchType2(void) const { return itsIsoLineHatchType2; }
  void IsoLineHatchType2(int newValue) { itsIsoLineHatchType2 = newValue; }
  const NFmiColor& IsoLineHatchColor2(void) const { return itsIsoLineHatchColor2; }
  void IsoLineHatchColor2(const NFmiColor& newValue) { itsIsoLineHatchColor2 = newValue; }
  int ContourLabelDigitCount(void) const { return itsContourLabelDigitCount; }
  void ContourLabelDigitCount(int newValue)
  {
    itsContourLabelDigitCount = newValue;
    if (itsContourLabelDigitCount > 10)
      itsContourLabelDigitCount = 10;
  }

  //**************************************************************
  //********** 'versio 2' parametrien asetusfunktiot *************
  //**************************************************************

  // ---------------------- operators ------------------------
  bool operator==(const NFmiDrawParam& theDrawParam) const;
  bool operator<(const NFmiDrawParam& theDrawParam) const;
  virtual std::ostream& Write(std::ostream& file) const;
  virtual std::istream& Read(std::istream& file);

  const static float itsMinAlpha;

  static bool IsMacroParamCase(NFmiInfoData::Type theDataType);
  bool IsMacroParamCase(bool justCheckDataType);

 protected:
  int itsPriority;
  NFmiMetEditorTypes::View itsStationDataViewType;  // jos viewtype on isoviiva, mutta data on asema
  // dataa, pitää olla varalla joku näyttötyyppi
  // että voidaan piirtää tällöin
  NFmiColor itsFrameColor;
  NFmiColor itsFillColor;

  NFmiColor itsIsolineLabelBoxFillColor;
  NFmiColor itsContourLabelBoxFillColor;  // **** Versio 3 parametri ****
  //   Minkä kokoinen näyttöön piirrettävä 'symbolidata'
  //   on suhteessa
  //   annettuun asemalle/hilalle varattuun 'datalaatikkoon'.
  //   (oletusarvo)
  NFmiPoint itsRelativeSize;  // nämä ovat asemadata symboli kokoja
  NFmiPoint itsRelativePositionOffset;
  NFmiPoint itsOnlyOneSymbolRelativeSize;
  NFmiPoint itsOnlyOneSymbolRelativePositionOffset;
  bool fUseIsoLineGabWithCustomContours;
  bool fUseContourGabWithCustomContours;  // **** Versio 3 parametri ****
  double itsIsoLineGab;
  double itsContourGab;  // **** Versio 3 parametri ****
  double itsModifyingStep;
  //	bool fModifyingUnit;	//(= 0, jos yksikkö on %, = 1, jos yksikkö on sama kuin itsUnit)
  double itsTimeSerialModifyingLimit;  // aikasarjanäytön muutos akselin minimi ja maksimi arvo
  NFmiColor itsIsolineColor;
  NFmiColor itsContourColor;  // **** Versio 3 parametri ****
  NFmiColor itsIsolineTextColor;
  NFmiColor itsContourTextColor;  // **** Versio 3 parametri ****
  NFmiColor itsSecondaryIsolineColor;
  NFmiColor itsSecondaryIsolineTextColor;
  bool fUseSecondaryColors;

  double itsAbsoluteMinValue;
  double itsAbsoluteMaxValue;

  double itsTimeSeriesScaleMin;  // käytetään aikasarjaeditorissa
  double itsTimeSeriesScaleMax;  // käytetään aikasarjaeditorissa

  //   Lista mahdollisista näyttötyypeistä kyseiselle
  //   parametrille.
  NFmiMetEditorTypes::View itsPossibleViewTypeList[5];
  int itsPossibleViewTypeCount;

  // Tekstinäyttö:
  bool fShowNumbers;
  bool fShowMasks;           // tämä on turha!!!!!
  bool fShowColors;          // asema tekstiä varten
  bool fShowColoredNumbers;  // asema tekstiä varten
  bool fZeroColorMean;       // asema tekstiä varten

  //***********************************************
  //********** 'versio 2' parametreja *************
  //***********************************************
  float itsStationSymbolColorShadeLowValue;   // väri skaalaus alkaa tästä arvosta
  float itsStationSymbolColorShadeMidValue;   // väri skaalauksen keskiarvo
  float itsStationSymbolColorShadeHighValue;  // väri skaalaus loppuu tähän arvoon
  NFmiColor itsStationSymbolColorShadeLowValueColor;
  NFmiColor itsStationSymbolColorShadeMidValueColor;
  NFmiColor itsStationSymbolColorShadeHighValueColor;
  int itsStationSymbolColorShadeClassCount;  // kuinka monta väri luokkaa tehdään skaalaukseen
  bool fUseSymbolsInTextMode;  // käytetäänkö tekstiä vai mapataanko arvoja kohden jokin symboli
                               // ruudulle?
  int itsUsedSymbolListIndex;         // 0=ei mitään, 1=synopfont, 2=hessaa, ...
  int itsSymbolIndexingMapListIndex;  // indeksi johonkin symbolilistaan, jossa on mapattu arvot
                                      // haluttuihin symboleihin

  bool fUseContourFeathering;      // **** Versio 3 parametri ****
  bool fIsoLineLabelsOverLapping;  // voivatko isoviiva labelit mennä päällekkäin vai ei?
  bool fShowColorLegend;  // piirretäänko rivin viimeiseen ruutuun color contouringin väri selitys
                          // laatikko?
  float itsSimpleIsoLineGap;
  float itsSimpleIsoLineZeroValue;    // tämän arvon kautta isoviivat joutuvat menemään
  float itsSimpleContourZeroValue;    // **** Versio 3 parametri ****
  float itsSimpleIsoLineLabelHeight;  // relatiivinen vai mm? (0=ei näytetä ollenkaan)
  float itsSimpleContourLabelHeight;  // **** Versio 3 parametri ****
  bool fShowSimpleIsoLineLabelBox;    // ei vielä muita attribuutteja isoviiva labelille (tämä
                                      // tarkoittaa lukua ympäroivää laatikkoa)
  bool fShowSimpleContourLabelBox;    // **** Versio 3 parametri ****
  float itsSimpleIsoLineWidth;        // relatiivinen vai mm?
  float itsSimpleContourWidth;        // **** Versio 3 parametri ****
  int itsSimpleIsoLineLineStyle;      // 1=yht. viiva, 2=pisteviiva jne.
  int itsSimpleContourLineStyle;      // **** Versio 3 parametri ****
  bool fUseSingleColorsWithSimpleIsoLines;  // true=sama väri kaikille isoviivoille, false=tehdään
                                            // väriskaala
                                            // isoviivan väriskaalaus asetukset
  float itsSimpleIsoLineColorShadeLowValue;    // väri skaalaus alkaa tästä arvosta
  float itsSimpleIsoLineColorShadeMidValue;    // väri skaalauksen keskiarvo
  float itsSimpleIsoLineColorShadeHighValue;   // väri skaalaus loppuu tähän arvoon
  float itsSimpleIsoLineColorShadeHigh2Value;  // **** Versio 3 parametri ****
  NFmiColor itsSimpleIsoLineColorShadeLowValueColor;
  NFmiColor itsSimpleIsoLineColorShadeMidValueColor;
  NFmiColor itsSimpleIsoLineColorShadeHighValueColor;
  NFmiColor itsSimpleIsoLineColorShadeHigh2ValueColor;  // **** Versio 3 parametri ****
  int itsSimpleIsoLineColorShadeClassCount;  // kuinka monta väri luokkaa tehdään skaalaukseen
  // speciaali isoviiva asetukset (otetaan käyttöön, jos
  // GeneralVisualization::UseSimpleIsoLineDefinitions() is false)
  checkedVector<float>
      itsSpecialIsoLineValues;  // tähän laitetaan kaikki arvot, johon halutaan isoviiva
  checkedVector<float> itsSpecialContourValues;       // **** Versio 3 parametri ****
  checkedVector<float> itsSpecialIsoLineLabelHeight;  // isoviivalabeleiden korkeudet (0=ei labelia)
  checkedVector<float> itsSpecialContourLabelHeight;  // **** Versio 3 parametri ****
  checkedVector<float> itsSpecialIsoLineWidth;        // viivan paksuudet
  checkedVector<float> itsSpecialContourWidth;        // **** Versio 3 parametri ****
  checkedVector<int> itsSpecialIsoLineStyle;          // viiva tyylit
  checkedVector<int> itsSpecialContourStyle;          // **** Versio 3 parametri ****
  checkedVector<int> itsSpecialIsoLineColorIndexies;  // eri viivojen väri indeksit (pitää tehdä
                                                      // näyttö taulukko käyttäjälle)
  checkedVector<int> itsSpecialContourColorIndexies;  // **** Versio 3 parametri ****
  checkedVector<bool> itsSpecialIsoLineShowLabelBox;  // eri viivojen väri indeksit (pitää tehdä
                                                      // näyttö taulukko käyttäjälle)
  // colorcontouring ja quick contouring asetukset
  checkedVector<float> itsSpecialColorContouringValues;  // tähän laitetaan kaikki arvot, johon
                                                         // halutaan color contour luokka rajoiksi
  checkedVector<int> itsSpecialColorContouringColorIndexies;  // eri viivojen väri indeksit (pitää
  // tehdä näyttö taulukko käyttäjälle)
  float itsColorContouringColorShadeLowValue;  // väri skaalaus alkaa tästä arvosta
  float itsColorContouringColorShadeMidValue;  // väri skaalauksen keskiarvo
  float itsColorContouringColorShadeHighValue;
  float itsColorContouringColorShadeHigh2Value;  // väri skaalaus loppuu tähän arvoon
  NFmiColor itsColorContouringColorShadeLowValueColor;
  NFmiColor itsColorContouringColorShadeMidValueColor;
  NFmiColor itsColorContouringColorShadeHighValueColor;
  NFmiColor itsColorContouringColorShadeHigh2ValueColor;
  // isoviivojen kanssa voi käyttää myös hatchättyjä alueita (2 kpl)
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
  int itsContourLabelDigitCount;  // **** Versio 3 parametri ****
                                  //***********************************************
                                  //********** 'versio 2' parametreja *************
                                  //***********************************************

 private:
  static float itsFileVersionNumber;
  float itsInitFileVersionNumber;

  //	bool fEditableParam;	// onko parametri suoraan editoitavissa ollenkaan? (esim. HESSAA tai
  // tuulivektori eivät ole)
};
//@{ \name Globaalit NFmiDrawParam-luokan uudelleenohjaus-operaatiot
inline std::ostream& operator<<(std::ostream& os, const NFmiDrawParam& item)
{
  return item.Write(os);
}
inline std::istream& operator>>(std::istream& is, NFmiDrawParam& item)
{
  return item.Read(is);
}
//@}

