//**********************************************************
// C++ Class Name : NFmiDrawParam
// ---------------------------------------------------------
// Filetype: (SOURCE)
// Filepath: D:/projekti/GDPro/GDTemp/NFmiDrawParam.cpp
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
// Changed 1999.08.31/Marko	Muutin kontruktoreita niin, että mahdollisia
//							näyttötyyppejä on 2 (teksti ja isoviiva).
// Changed 1999.09.28/Marko	Lisäsin SecondaryIsoLineColor:in käytön (käytetään
//							vertailtaessa samantyyppistä dataa
// päällekkäin)
//
//**********************************************************
#include "NFmiDrawParam.h"
#include "NFmiDataStoringHelpers.h"
#include "NFmiColorSpaces.h"

#include <fstream>

float NFmiDrawParam::itsFileVersionNumber = 3.0;
const float NFmiDrawParam::itsMinAlpha =
    5.f;  // 5:kaan ei oikein erota enää mitään, on niin läpinäkyvä

//--------------------------------------------------------
// NFmiDrawParam(void)
//--------------------------------------------------------
NFmiDrawParam::NFmiDrawParam()
    : GeneralData(),
      itsPriority(1),
      itsViewType(NFmiMetEditorTypes::kFmiIsoLineView),
      itsStationDataViewType(NFmiMetEditorTypes::kFmiTextView),
      itsFrameColor(NFmiColor(0., 0., 0.))  // musta
      ,
      itsFillColor(NFmiColor(0., 0., 0.))  // musta
      ,
      itsIsolineLabelBoxFillColor(NFmiColor(1., 1., 0.))  // keltainen
      ,
      itsContourLabelBoxFillColor(NFmiColor(1., 1., 0.))  // keltainen
      ,
      itsRelativeSize(NFmiPoint(1, 1)),
      itsRelativePositionOffset(NFmiPoint(0, 0)),
      itsOnlyOneSymbolRelativeSize(NFmiPoint(1, 1)),
      itsOnlyOneSymbolRelativePositionOffset(NFmiPoint(0, 0)),
      fUseIsoLineGabWithCustomContours(false),
      fUseContourGabWithCustomContours(false),
      itsIsoLineGab(1.),
      itsContourGab(1.),
      itsModifyingStep(1.)
      //, fModifyingUnit						 (true)
      ,
      itsTimeSerialModifyingLimit(10),
      itsIsolineColor(NFmiColor(0., 0., 0.)),
      itsContourColor(NFmiColor(0., 0., 0.)),
      itsIsolineTextColor(NFmiColor(0., 0., 0.)),
      itsContourTextColor(NFmiColor(0., 0., 0.)),
      itsSecondaryIsolineColor(0.6f, 0.6f, 0.6f)  // tehdään secondary väreistä aina harmaita
      ,
      itsSecondaryIsolineTextColor(0.6f, 0.6f, 0.6f)  // tehdään secondary väreistä aina harmaita
      ,
      fUseSecondaryColors(false),
      itsAbsoluteMinValue(-1000000000),
      itsAbsoluteMaxValue(1000000000),
      itsTimeSeriesScaleMin(0),
      itsTimeSeriesScaleMax(100),
      itsPossibleViewTypeCount(2),
      fShowNumbers(true),
      fShowMasks(false)  // tämä on turha
      ,
      fShowColors(false),
      fShowColoredNumbers(false),
      fZeroColorMean(false),
      itsStationSymbolColorShadeLowValue(0),
      itsStationSymbolColorShadeMidValue(50),
      itsStationSymbolColorShadeHighValue(100),
      itsStationSymbolColorShadeLowValueColor(0, 0, 1),
      itsStationSymbolColorShadeMidValueColor(0, 1, 0),
      itsStationSymbolColorShadeHighValueColor(0, 1, 0),
      itsStationSymbolColorShadeClassCount(9),
      fUseSymbolsInTextMode(false),
      itsUsedSymbolListIndex(0),
      itsSymbolIndexingMapListIndex(-1),
      itsGridDataPresentationStyle(2),
      fUseIsoLineFeathering(false),
      fUseContourFeathering(false),
      fIsoLineLabelsOverLapping(true),
      fShowColorLegend(false),
      fUseSimpleIsoLineDefinitions(true),
      fUseSimpleContourDefinitions(true),
      fUseSeparatorLinesBetweenColorContourClasses(true),
      itsSimpleIsoLineGap(1),
      itsSimpleIsoLineZeroValue(0),
      itsSimpleContourZeroValue(0),
      itsSimpleIsoLineLabelHeight(4),
      itsSimpleContourLabelHeight(4),
      fShowSimpleIsoLineLabelBox(false),
      fShowSimpleContourLabelBox(false),
      itsSimpleIsoLineWidth(0.2f),
      itsSimpleContourWidth(0.2f),
      itsSimpleIsoLineLineStyle(0),
      itsSimpleContourLineStyle(0),
      itsIsoLineSplineSmoothingFactor(6),
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
      itsSpecialIsoLineValues(),
      itsSpecialContourValues(),
      itsSpecialIsoLineLabelHeight(),
      itsSpecialContourLabelHeight(),
      itsSpecialIsoLineWidth(),
      itsSpecialContourWidth(),
      itsSpecialIsoLineStyle(),
      itsSpecialContourStyle(),
      itsSpecialIsoLineColorIndexies(),
      itsSpecialContourColorIndexies(),
      itsSpecialIsoLineShowLabelBox(),
      fDrawOnlyOverMask(false),
      fUseCustomColorContouring(false),
      fUseCustomIsoLineing(false),
      itsSpecialColorContouringValues(),
      itsSpecialColorContouringColorIndexies(),
      itsColorContouringColorShadeLowValue(0),
      itsColorContouringColorShadeMidValue(50),
      itsColorContouringColorShadeHighValue(100),
      itsColorContouringColorShadeHigh2Value(100),
      itsColorContouringColorShadeLowValueColor(0, 0, 1),
      itsColorContouringColorShadeMidValueColor(0, 1, 0),
      itsColorContouringColorShadeHighValueColor(0, 1, 0),
      itsColorContouringColorShadeHigh2ValueColor(0, 1, 0),
      fUseWithIsoLineHatch1(false),
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
      // protected osa
      ,
      itsIsoLineLabelDigitCount(0),
      itsContourLabelDigitCount(0),
      itsAlpha(100.f)  // dafault on 100 eli täysin läpinäkymätön
      ,
      fHidden(false),
      fShowDifference(false),
      fShowDifferenceToOriginalData(false)
{
  GeneralData::InitFileVersionNumber(itsFileVersionNumber);
  itsPossibleViewTypeList[0] = NFmiMetEditorTypes::kFmiTextView;
  itsPossibleViewTypeList[1] = NFmiMetEditorTypes::kFmiIsoLineView;
}

//-------------------------------------------------------
// NFmiDrawParam
//-------------------------------------------------------
NFmiDrawParam::NFmiDrawParam(const NFmiDataIdent& theParam,
                             const NFmiLevel& theLevel,
                             int thePriority,
                             NFmiInfoData::Type theDataType)
    : GeneralData(),
      itsPriority(thePriority),
      itsViewType(NFmiMetEditorTypes::kFmiIsoLineView),
      itsStationDataViewType(NFmiMetEditorTypes::kFmiTextView),
      itsFrameColor(NFmiColor(0., 0., 0.))  // musta
      ,
      itsFillColor(NFmiColor(0., 0., 0.))  // musta
      ,
      itsIsolineLabelBoxFillColor(NFmiColor(1., 1., 0.))  // keltainen
      ,
      itsContourLabelBoxFillColor(NFmiColor(1., 1., 0.))  // keltainen
      ,
      itsRelativeSize(NFmiPoint(1, 1)),
      itsRelativePositionOffset(NFmiPoint(0, 0)),
      itsOnlyOneSymbolRelativeSize(NFmiPoint(1, 1)),
      itsOnlyOneSymbolRelativePositionOffset(NFmiPoint(0, 0)),
      fUseIsoLineGabWithCustomContours(false),
      fUseContourGabWithCustomContours(false),
      itsIsoLineGab(1.),
      itsContourGab(1.),
      itsModifyingStep(1.)
      //, fModifyingUnit						 (true)
      ,
      itsTimeSerialModifyingLimit(10),
      itsIsolineColor(NFmiColor(0., 0., 0.)),
      itsContourColor(NFmiColor(0., 0., 0.)),
      itsIsolineTextColor(NFmiColor(0., 0., 0.)),
      itsContourTextColor(NFmiColor(0., 0., 0.)),
      itsSecondaryIsolineColor(0.6f, 0.6f, 0.6f)  // tehdään secondary väreistä aina harmaita
      ,
      itsSecondaryIsolineTextColor(0.6f, 0.6f, 0.6f)  // tehdään secondary väreistä aina harmaita
      ,
      fUseSecondaryColors(false),
      itsAbsoluteMinValue(-1000000000),
      itsAbsoluteMaxValue(1000000000),
      itsTimeSeriesScaleMin(0),
      itsTimeSeriesScaleMax(100),
      itsPossibleViewTypeCount(2),
      fShowNumbers(true),
      fShowMasks(false)  // tämä on turha
      ,
      fShowColors(false),
      fShowColoredNumbers(false),
      fZeroColorMean(false),
      itsStationSymbolColorShadeLowValue(0),
      itsStationSymbolColorShadeMidValue(50),
      itsStationSymbolColorShadeHighValue(100),
      itsStationSymbolColorShadeLowValueColor(0, 0, 1),
      itsStationSymbolColorShadeMidValueColor(0, 1, 0),
      itsStationSymbolColorShadeHighValueColor(0, 1, 0),
      itsStationSymbolColorShadeClassCount(9),
      fUseSymbolsInTextMode(false),
      itsUsedSymbolListIndex(0),
      itsSymbolIndexingMapListIndex(-1),
      itsGridDataPresentationStyle(2),
      fUseIsoLineFeathering(false),
      fUseContourFeathering(false),
      fIsoLineLabelsOverLapping(true),
      fShowColorLegend(false),
      fUseSimpleIsoLineDefinitions(true),
      fUseSimpleContourDefinitions(true),
      fUseSeparatorLinesBetweenColorContourClasses(true),
      itsSimpleIsoLineGap(1),
      itsSimpleIsoLineZeroValue(0),
      itsSimpleContourZeroValue(0),
      itsSimpleIsoLineLabelHeight(4),
      itsSimpleContourLabelHeight(4),
      fShowSimpleIsoLineLabelBox(false),
      fShowSimpleContourLabelBox(false),
      itsSimpleIsoLineWidth(0.2f),
      itsSimpleContourWidth(0.2f),
      itsSimpleIsoLineLineStyle(0),
      itsSimpleContourLineStyle(0),
      itsIsoLineSplineSmoothingFactor(6),
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
      itsSpecialIsoLineValues(),
      itsSpecialContourValues(),
      itsSpecialIsoLineLabelHeight(),
      itsSpecialContourLabelHeight(),
      itsSpecialIsoLineWidth(),
      itsSpecialContourWidth(),
      itsSpecialIsoLineStyle(),
      itsSpecialContourStyle(),
      itsSpecialIsoLineColorIndexies(),
      itsSpecialContourColorIndexies(),
      itsSpecialIsoLineShowLabelBox(),
      fDrawOnlyOverMask(false),
      fUseCustomColorContouring(false),
      fUseCustomIsoLineing(false),
      itsSpecialColorContouringValues(),
      itsSpecialColorContouringColorIndexies(),
      itsColorContouringColorShadeLowValue(0),
      itsColorContouringColorShadeMidValue(50),
      itsColorContouringColorShadeHighValue(100),
      itsColorContouringColorShadeHigh2Value(100),
      itsColorContouringColorShadeLowValueColor(0, 0, 1),
      itsColorContouringColorShadeMidValueColor(0, 1, 0),
      itsColorContouringColorShadeHighValueColor(0, 1, 0),
      itsColorContouringColorShadeHigh2ValueColor(0, 1, 0),
      fUseWithIsoLineHatch1(false),
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
      itsIsoLineHatchColor2(0.5f, 0.5f, 0.5f),
      itsIsoLineLabelDigitCount(0),
      itsContourLabelDigitCount(0),
      itsAlpha(100.f)  // dafault on 100 eli täysin läpinäkymätön
      ,
      fHidden(false),
      fShowDifference(false),
      //***********************************************
      //********** 'versio 3' parametreja *************
      //***********************************************
      fShowDifferenceToOriginalData(false)
{
  GeneralData::Param(theParam);
  GeneralData::Level(theLevel);
  GeneralData::InitFileVersionNumber(itsFileVersionNumber);
  GeneralData::DataType(theDataType);

  itsPossibleViewTypeList[0] = NFmiMetEditorTypes::kFmiTextView;
  itsPossibleViewTypeList[1] = NFmiMetEditorTypes::kFmiIsoLineView;
}

NFmiDrawParam::NFmiDrawParam(const NFmiDrawParam& other)
    : GeneralData(other),
      itsPriority(other.itsPriority),
      itsViewType(other.itsViewType),
      itsStationDataViewType(other.itsStationDataViewType),
      itsFrameColor(other.itsFrameColor),
      itsFillColor(other.itsFillColor),
      itsIsolineLabelBoxFillColor(other.itsIsolineLabelBoxFillColor),
      itsContourLabelBoxFillColor(other.itsContourLabelBoxFillColor),
      itsRelativeSize(other.itsRelativeSize),
      itsRelativePositionOffset(other.itsRelativePositionOffset),
      itsOnlyOneSymbolRelativeSize(other.itsOnlyOneSymbolRelativeSize),
      itsOnlyOneSymbolRelativePositionOffset(other.itsOnlyOneSymbolRelativePositionOffset),
      fUseIsoLineGabWithCustomContours(other.fUseIsoLineGabWithCustomContours),
      fUseContourGabWithCustomContours(other.fUseContourGabWithCustomContours),
      itsIsoLineGab(other.itsIsoLineGab),
      itsContourGab(other.itsContourGab),
      itsModifyingStep(other.itsModifyingStep)
      //, fModifyingUnit(other.fModifyingUnit)
      ,
      itsTimeSerialModifyingLimit(other.itsTimeSerialModifyingLimit),
      itsIsolineColor(other.itsIsolineColor),
      itsContourColor(other.itsContourColor),
      itsIsolineTextColor(other.itsIsolineTextColor),
      itsContourTextColor(other.itsContourTextColor),
      itsSecondaryIsolineColor(other.itsSecondaryIsolineColor),
      itsSecondaryIsolineTextColor(other.itsSecondaryIsolineTextColor),
      fUseSecondaryColors(other.fUseSecondaryColors),
      itsAbsoluteMinValue(other.itsAbsoluteMinValue),
      itsAbsoluteMaxValue(other.itsAbsoluteMaxValue),
      itsTimeSeriesScaleMin(other.itsTimeSeriesScaleMin),
      itsTimeSeriesScaleMax(other.itsTimeSeriesScaleMax),
      itsPossibleViewTypeCount(other.itsPossibleViewTypeCount),
      fShowNumbers(other.fShowNumbers),
      fShowMasks(other.fShowMasks),
      fShowColors(other.fShowColors),
      fShowColoredNumbers(other.fShowColoredNumbers),
      fZeroColorMean(other.fZeroColorMean)
      //***********************************************
      //********** 'versio 2' parametreja *************
      //***********************************************
      ,
      itsStationSymbolColorShadeLowValue(other.itsStationSymbolColorShadeLowValue),
      itsStationSymbolColorShadeMidValue(other.itsStationSymbolColorShadeMidValue),
      itsStationSymbolColorShadeHighValue(other.itsStationSymbolColorShadeHighValue),
      itsStationSymbolColorShadeLowValueColor(other.itsStationSymbolColorShadeLowValueColor),
      itsStationSymbolColorShadeMidValueColor(other.itsStationSymbolColorShadeMidValueColor),
      itsStationSymbolColorShadeHighValueColor(other.itsStationSymbolColorShadeHighValueColor),
      itsStationSymbolColorShadeClassCount(other.itsStationSymbolColorShadeClassCount),
      fUseSymbolsInTextMode(other.fUseSymbolsInTextMode),
      itsUsedSymbolListIndex(other.itsUsedSymbolListIndex),
      itsSymbolIndexingMapListIndex(other.itsSymbolIndexingMapListIndex),
      itsGridDataPresentationStyle(other.itsGridDataPresentationStyle),
      fUseIsoLineFeathering(other.fUseIsoLineFeathering),
      fUseContourFeathering(other.fUseContourFeathering),
      fIsoLineLabelsOverLapping(other.fIsoLineLabelsOverLapping),
      fShowColorLegend(other.fShowColorLegend),
      fUseSimpleIsoLineDefinitions(other.fUseSimpleIsoLineDefinitions),
      fUseSimpleContourDefinitions(other.fUseSimpleContourDefinitions),
      fUseSeparatorLinesBetweenColorContourClasses(
          other.fUseSeparatorLinesBetweenColorContourClasses),
      itsSimpleIsoLineGap(other.itsSimpleIsoLineGap),
      itsSimpleIsoLineZeroValue(other.itsSimpleIsoLineZeroValue),
      itsSimpleContourZeroValue(other.itsSimpleContourZeroValue),
      itsSimpleIsoLineLabelHeight(other.itsSimpleIsoLineLabelHeight),
      itsSimpleContourLabelHeight(other.itsSimpleContourLabelHeight),
      fShowSimpleIsoLineLabelBox(other.fShowSimpleIsoLineLabelBox),
      fShowSimpleContourLabelBox(other.fShowSimpleContourLabelBox),
      itsSimpleIsoLineWidth(other.itsSimpleIsoLineWidth),
      itsSimpleContourWidth(other.itsSimpleContourWidth),
      itsSimpleIsoLineLineStyle(other.itsSimpleIsoLineLineStyle),
      itsSimpleContourLineStyle(other.itsSimpleContourLineStyle),
      itsIsoLineSplineSmoothingFactor(other.itsIsoLineSplineSmoothingFactor),
      fUseSingleColorsWithSimpleIsoLines(other.fUseSingleColorsWithSimpleIsoLines),
      itsSimpleIsoLineColorShadeLowValue(other.itsSimpleIsoLineColorShadeLowValue),
      itsSimpleIsoLineColorShadeMidValue(other.itsSimpleIsoLineColorShadeMidValue),
      itsSimpleIsoLineColorShadeHighValue(other.itsSimpleIsoLineColorShadeHighValue),
      itsSimpleIsoLineColorShadeHigh2Value(other.itsSimpleIsoLineColorShadeHigh2Value),
      itsSimpleIsoLineColorShadeLowValueColor(other.itsSimpleIsoLineColorShadeLowValueColor),
      itsSimpleIsoLineColorShadeMidValueColor(other.itsSimpleIsoLineColorShadeMidValueColor),
      itsSimpleIsoLineColorShadeHighValueColor(other.itsSimpleIsoLineColorShadeHighValueColor),
      itsSimpleIsoLineColorShadeHigh2ValueColor(other.itsSimpleIsoLineColorShadeHigh2ValueColor),
      itsSimpleIsoLineColorShadeClassCount(other.itsSimpleIsoLineColorShadeClassCount),
      itsSpecialIsoLineValues(other.itsSpecialIsoLineValues),
      itsSpecialContourValues(other.itsSpecialContourValues),
      itsSpecialIsoLineLabelHeight(other.itsSpecialIsoLineLabelHeight),
      itsSpecialContourLabelHeight(other.itsSpecialContourLabelHeight),
      itsSpecialIsoLineWidth(other.itsSpecialIsoLineWidth),
      itsSpecialContourWidth(other.itsSpecialContourWidth),
      itsSpecialIsoLineStyle(other.itsSpecialIsoLineStyle),
      itsSpecialContourStyle(other.itsSpecialContourStyle),
      itsSpecialIsoLineColorIndexies(other.itsSpecialIsoLineColorIndexies),
      itsSpecialContourColorIndexies(other.itsSpecialContourColorIndexies),
      itsSpecialIsoLineShowLabelBox(other.itsSpecialIsoLineShowLabelBox),
      fDrawOnlyOverMask(other.fDrawOnlyOverMask),
      fUseCustomColorContouring(other.fUseCustomColorContouring),
      fUseCustomIsoLineing(other.fUseCustomIsoLineing),
      itsSpecialColorContouringValues(other.itsSpecialColorContouringValues),
      itsSpecialColorContouringColorIndexies(other.itsSpecialColorContouringColorIndexies),
      itsColorContouringColorShadeLowValue(other.itsColorContouringColorShadeLowValue),
      itsColorContouringColorShadeMidValue(other.itsColorContouringColorShadeMidValue),
      itsColorContouringColorShadeHighValue(other.itsColorContouringColorShadeHighValue),
      itsColorContouringColorShadeHigh2Value(other.itsColorContouringColorShadeHigh2Value),
      itsColorContouringColorShadeLowValueColor(other.itsColorContouringColorShadeLowValueColor),
      itsColorContouringColorShadeMidValueColor(other.itsColorContouringColorShadeMidValueColor),
      itsColorContouringColorShadeHighValueColor(other.itsColorContouringColorShadeHighValueColor),
      itsColorContouringColorShadeHigh2ValueColor(
          other.itsColorContouringColorShadeHigh2ValueColor),
      fUseWithIsoLineHatch1(other.fUseWithIsoLineHatch1),
      fDrawIsoLineHatchWithBorders1(other.fDrawIsoLineHatchWithBorders1),
      itsIsoLineHatchLowValue1(other.itsIsoLineHatchLowValue1),
      itsIsoLineHatchHighValue1(other.itsIsoLineHatchHighValue1),
      itsIsoLineHatchType1(other.itsIsoLineHatchType1),
      itsIsoLineHatchColor1(other.itsIsoLineHatchColor1),
      itsIsoLineHatchBorderColor1(other.itsIsoLineHatchBorderColor1),
      fUseWithIsoLineHatch2(other.fUseWithIsoLineHatch2),
      fDrawIsoLineHatchWithBorders2(other.fDrawIsoLineHatchWithBorders2),
      itsIsoLineHatchLowValue2(other.itsIsoLineHatchLowValue2),
      itsIsoLineHatchHighValue2(other.itsIsoLineHatchHighValue2),
      itsIsoLineHatchType2(other.itsIsoLineHatchType2),
      itsIsoLineHatchColor2(other.itsIsoLineHatchColor2),
      itsIsoLineLabelDigitCount(other.itsIsoLineLabelDigitCount),
      itsContourLabelDigitCount(other.itsContourLabelDigitCount)
      //***********************************************
      //********** 'versio 2' parametreja *************
      //***********************************************
      ,
      itsAlpha(other.itsAlpha),
      fHidden(other.fHidden),
      fShowDifference(other.fShowDifference),
      //***********************************************
      //********** 'versio 3' parametreja *************
      //***********************************************
      fShowDifferenceToOriginalData(other.fShowDifferenceToOriginalData)
{
  Alpha(itsAlpha);  // varmistus että pysytään rajoissa
  itsPossibleViewTypeList[0] = NFmiMetEditorTypes::kFmiTextView;
  itsPossibleViewTypeList[1] = NFmiMetEditorTypes::kFmiIsoLineView;
}

//-------------------------------------------------------
// ~NFmiDrawParam
//-------------------------------------------------------
NFmiDrawParam::~NFmiDrawParam(void)
{
}

//-------------------------------------------------------
// Init
//-------------------------------------------------------
void NFmiDrawParam::Init(const boost::shared_ptr<NFmiDrawParam>& theDrawParam,
                         bool fInitOnlyDrawingOptions)
{
  Init(theDrawParam.get(), fInitOnlyDrawingOptions);
}

void NFmiDrawParam::Init(const NFmiDrawParam* theDrawParam, bool fInitOnlyDrawingOptions)
{
  if (theDrawParam)
  {
    if (fInitOnlyDrawingOptions == false)
    {
      GeneralData::InitFileName(theDrawParam->InitFileName());
      // HUOM! GeneralData::MacroParamRelativePath -dataosaa ei saa initialisoida, koska sitä
      // käytetään vain
      // viewmakrojen yhteydessä
      GeneralData::ParameterAbbreviation(theDrawParam->ParameterAbbreviation());
      GeneralData::ViewMacroDrawParam(theDrawParam->ViewMacroDrawParam());
      GeneralData::Param(theDrawParam->Param());
      GeneralData::Level(const_cast<NFmiDrawParam*>(theDrawParam)->Level());
      GeneralData::DataType(theDrawParam->DataType());
      GeneralData::ModelOriginTime(theDrawParam->ModelOriginTime());
      GeneralData::ModelRunIndex(theDrawParam->ModelRunIndex());
      GeneralData::TimeSerialModelRunCount(theDrawParam->TimeSerialModelRunCount());
      GeneralData::ModelRunDifferenceIndex(theDrawParam->ModelRunDifferenceIndex());
      GeneralData::DataComparisonProdId(theDrawParam->DataComparisonProdId());
      GeneralData::DataComparisonType(theDrawParam->DataComparisonType());
    }
    itsPriority = theDrawParam->Priority();

    itsViewType = theDrawParam->ViewType();
    itsStationDataViewType = theDrawParam->StationDataViewType();

    itsFrameColor = NFmiColor(theDrawParam->FrameColor());
    itsFillColor = NFmiColor(theDrawParam->FillColor());
    itsIsolineLabelBoxFillColor = NFmiColor(theDrawParam->IsolineLabelBoxFillColor());

    itsRelativeSize = NFmiPoint(theDrawParam->RelativeSize());
    itsRelativePositionOffset = NFmiPoint(theDrawParam->RelativePositionOffset());
    itsOnlyOneSymbolRelativeSize = NFmiPoint(theDrawParam->OnlyOneSymbolRelativeSize());
    itsOnlyOneSymbolRelativePositionOffset =
        NFmiPoint(theDrawParam->OnlyOneSymbolRelativePositionOffset());

    fUseIsoLineGabWithCustomContours = theDrawParam->UseIsoLineGabWithCustomContours();
    fUseContourGabWithCustomContours = theDrawParam->UseContourGabWithCustomContours();
    itsIsoLineGab = theDrawParam->IsoLineGab();
    itsModifyingStep = theDrawParam->ModifyingStep();
    //		fModifyingUnit =  theDrawParam->ModifyingUnit();
    itsTimeSerialModifyingLimit = theDrawParam->TimeSerialModifyingLimit();

    itsIsolineColor = theDrawParam->IsolineColor();
    itsIsolineTextColor = theDrawParam->IsolineTextColor();

    // ei tarvitse toistaiseksi alustaa sekundaarisia värejä
    //	itsSecondaryIsolineColor = theDrawParam->IsolineSecondaryColor();
    //	itsSecondaryIsolineTextColor = theDrawParam->IsolineSecondaryTextColor();
    // ei tarvitse toistaiseksi alustaa sekundaarisia värejä

    itsAbsoluteMinValue = theDrawParam->AbsoluteMinValue();
    itsAbsoluteMaxValue = theDrawParam->AbsoluteMaxValue();

    itsTimeSeriesScaleMin = theDrawParam->TimeSeriesScaleMin();
    itsTimeSeriesScaleMax = theDrawParam->TimeSeriesScaleMax();

    itsPossibleViewTypeCount = theDrawParam->PossibleViewTypeCount();

    const NFmiMetEditorTypes::View* possibleViewTypeList = theDrawParam->PossibleViewTypeList();
    for (int typeNumber = 0; typeNumber < itsPossibleViewTypeCount; typeNumber++)
    {
      itsPossibleViewTypeList[typeNumber] = possibleViewTypeList[typeNumber];
    }

    fHidden = theDrawParam->IsParamHidden();

    GeneralData::Unit(theDrawParam->Unit());

    fShowNumbers = theDrawParam->ShowNumbers();
    fShowMasks = theDrawParam->ShowMasks();
    fShowColors = theDrawParam->ShowColors();
    fShowColoredNumbers = theDrawParam->ShowColoredNumbers();
    fZeroColorMean = theDrawParam->ZeroColorMean();
    fShowDifference = theDrawParam->ShowDifference();
    fShowDifferenceToOriginalData = theDrawParam->ShowDifferenceToOriginalData();

    //***********************************************
    //********** 'versio 2' parametreja *************
    //***********************************************
    itsStationSymbolColorShadeLowValue = theDrawParam->StationSymbolColorShadeLowValue();
    itsStationSymbolColorShadeMidValue = theDrawParam->StationSymbolColorShadeMidValue();
    itsStationSymbolColorShadeHighValue = theDrawParam->StationSymbolColorShadeHighValue();
    itsStationSymbolColorShadeLowValueColor = theDrawParam->StationSymbolColorShadeLowValueColor();
    itsStationSymbolColorShadeMidValueColor = theDrawParam->StationSymbolColorShadeMidValueColor();
    itsStationSymbolColorShadeHighValueColor =
        theDrawParam->StationSymbolColorShadeHighValueColor();
    itsStationSymbolColorShadeClassCount = theDrawParam->StationSymbolColorShadeClassCount();
    fUseSymbolsInTextMode = theDrawParam->UseSymbolsInTextMode();
    itsUsedSymbolListIndex = theDrawParam->UsedSymbolListIndex();
    itsSymbolIndexingMapListIndex = theDrawParam->SymbolIndexingMapListIndex();
    itsGridDataPresentationStyle = theDrawParam->GridDataPresentationStyle();
    fUseIsoLineFeathering = theDrawParam->UseIsoLineFeathering();
    fIsoLineLabelsOverLapping = theDrawParam->IsoLineLabelsOverLapping();
    fShowColorLegend = theDrawParam->ShowColorLegend();
    fUseSimpleIsoLineDefinitions = theDrawParam->UseSimpleIsoLineDefinitions();
    fUseSeparatorLinesBetweenColorContourClasses =
        theDrawParam->UseSeparatorLinesBetweenColorContourClasses();
    itsSimpleIsoLineGap = theDrawParam->SimpleIsoLineGap();
    itsSimpleIsoLineZeroValue = theDrawParam->SimpleIsoLineZeroValue();
    itsSimpleIsoLineLabelHeight = theDrawParam->SimpleIsoLineLabelHeight();
    fShowSimpleIsoLineLabelBox = theDrawParam->ShowSimpleIsoLineLabelBox();
    itsSimpleIsoLineWidth = theDrawParam->SimpleIsoLineWidth();
    itsSimpleIsoLineLineStyle = theDrawParam->SimpleIsoLineLineStyle();
    itsIsoLineSplineSmoothingFactor = theDrawParam->IsoLineSplineSmoothingFactor();
    fUseSingleColorsWithSimpleIsoLines = theDrawParam->UseSingleColorsWithSimpleIsoLines();
    itsSimpleIsoLineColorShadeLowValue = theDrawParam->SimpleIsoLineColorShadeLowValue();
    itsSimpleIsoLineColorShadeMidValue = theDrawParam->SimpleIsoLineColorShadeMidValue();
    itsSimpleIsoLineColorShadeHighValue = theDrawParam->SimpleIsoLineColorShadeHighValue();
    itsSimpleIsoLineColorShadeLowValueColor = theDrawParam->SimpleIsoLineColorShadeLowValueColor();
    itsSimpleIsoLineColorShadeMidValueColor = theDrawParam->SimpleIsoLineColorShadeMidValueColor();
    itsSimpleIsoLineColorShadeHighValueColor =
        theDrawParam->SimpleIsoLineColorShadeHighValueColor();
    itsSimpleIsoLineColorShadeClassCount = theDrawParam->SimpleIsoLineColorShadeClassCount();
    itsSpecialIsoLineValues = theDrawParam->SpecialIsoLineValues();
    itsSpecialIsoLineLabelHeight = theDrawParam->SpecialIsoLineLabelHeight();
    itsSpecialIsoLineWidth = theDrawParam->SpecialIsoLineWidth();
    itsSpecialIsoLineStyle = theDrawParam->SpecialIsoLineStyle();
    itsSpecialIsoLineColorIndexies = theDrawParam->SpecialIsoLineColorIndexies();
    itsSpecialIsoLineShowLabelBox = theDrawParam->SpecialIsoLineShowLabelBox();
    fDrawOnlyOverMask = theDrawParam->DrawOnlyOverMask();
    fUseCustomColorContouring = theDrawParam->UseCustomColorContouring();
    itsSpecialColorContouringValues = theDrawParam->SpecialColorContouringValues();
    itsSpecialColorContouringColorIndexies = theDrawParam->SpecialColorContouringColorIndexies();
    itsColorContouringColorShadeLowValue = theDrawParam->ColorContouringColorShadeLowValue();
    itsColorContouringColorShadeMidValue = theDrawParam->ColorContouringColorShadeMidValue();
    itsColorContouringColorShadeHighValue = theDrawParam->ColorContouringColorShadeHighValue();
    itsColorContouringColorShadeHigh2Value = theDrawParam->ColorContouringColorShadeHigh2Value();
    itsColorContouringColorShadeLowValueColor =
        theDrawParam->ColorContouringColorShadeLowValueColor();
    itsColorContouringColorShadeMidValueColor =
        theDrawParam->ColorContouringColorShadeMidValueColor();
    itsColorContouringColorShadeHighValueColor =
        theDrawParam->ColorContouringColorShadeHighValueColor();
    itsColorContouringColorShadeHigh2ValueColor =
        theDrawParam->ColorContouringColorShadeHigh2ValueColor();
    fUseWithIsoLineHatch1 = theDrawParam->UseWithIsoLineHatch1();
    fDrawIsoLineHatchWithBorders1 = theDrawParam->DrawIsoLineHatchWithBorders1();
    itsIsoLineHatchLowValue1 = theDrawParam->IsoLineHatchLowValue1();
    itsIsoLineHatchHighValue1 = theDrawParam->IsoLineHatchHighValue1();
    itsIsoLineHatchType1 = theDrawParam->IsoLineHatchType1();
    itsIsoLineHatchColor1 = theDrawParam->IsoLineHatchColor1();
    itsIsoLineHatchBorderColor1 = theDrawParam->IsoLineHatchBorderColor1();
    fUseWithIsoLineHatch2 = theDrawParam->UseWithIsoLineHatch2();
    fDrawIsoLineHatchWithBorders2 = theDrawParam->DrawIsoLineHatchWithBorders2();
    itsIsoLineHatchLowValue2 = theDrawParam->IsoLineHatchLowValue2();
    itsIsoLineHatchHighValue2 = theDrawParam->IsoLineHatchHighValue2();
    itsIsoLineHatchType2 = theDrawParam->IsoLineHatchType2();
    itsIsoLineHatchColor2 = theDrawParam->IsoLineHatchColor2();
    itsIsoLineLabelDigitCount = theDrawParam->IsoLineLabelDigitCount();
    //***********************************************
    //********** 'versio 2' parametreja *************
    //***********************************************

    //***********************************************
    //********** 'versio 3' parametreja *************
    //***********************************************
    itsContourLabelBoxFillColor = theDrawParam->itsContourLabelBoxFillColor;
    fUseContourGabWithCustomContours = theDrawParam->fUseContourGabWithCustomContours;
    itsContourGab = theDrawParam->itsContourGab;
    itsContourColor = theDrawParam->itsContourColor;
    itsContourTextColor = theDrawParam->itsContourTextColor;
    fUseContourFeathering = theDrawParam->fUseContourFeathering;
    fUseSimpleContourDefinitions = theDrawParam->fUseSimpleContourDefinitions;
    itsSimpleContourZeroValue = theDrawParam->itsSimpleContourZeroValue;
    itsSimpleContourLabelHeight = theDrawParam->itsSimpleContourLabelHeight;
    fShowSimpleContourLabelBox = theDrawParam->fShowSimpleContourLabelBox;
    itsSimpleContourWidth = theDrawParam->itsSimpleContourWidth;
    itsSimpleContourLineStyle = theDrawParam->itsSimpleContourLineStyle;
    itsSimpleIsoLineColorShadeHigh2Value = theDrawParam->itsSimpleIsoLineColorShadeHigh2Value;
    itsSimpleIsoLineColorShadeHigh2ValueColor =
        theDrawParam->itsSimpleIsoLineColorShadeHigh2ValueColor;
    itsSpecialContourValues = theDrawParam->itsSpecialContourValues;
    itsSpecialContourLabelHeight = theDrawParam->itsSpecialContourLabelHeight;
    itsSpecialContourWidth = theDrawParam->itsSpecialContourWidth;
    itsSpecialContourStyle = theDrawParam->itsSpecialContourStyle;
    itsSpecialContourColorIndexies = theDrawParam->itsSpecialContourColorIndexies;
    fUseCustomIsoLineing = theDrawParam->fUseCustomIsoLineing;
    itsContourLabelDigitCount = theDrawParam->itsContourLabelDigitCount;
    //***********************************************
    //********** 'versio 3' parametreja *************
    //***********************************************
    Alpha(theDrawParam->itsAlpha);
  }
  return;
}

//--------------------------------------------------------
// Init
//--------------------------------------------------------
bool NFmiDrawParam::Init(const std::string& theFilename)
{
  if (theFilename != std::string(""))
  {
    std::ifstream in(theFilename.c_str(), std::ios::binary);
    if (in)
    {
      in >> *this;
      if (in)
      {
        in.close();
        GeneralData::InitFileName(theFilename);
        GeneralData::ViewMacroDrawParam(false);
        GeneralData::BorrowedParam(false);
        return true;
      }
    }
  }
  return false;
}

//--------------------------------------------------------
// StoreData
//--------------------------------------------------------
bool NFmiDrawParam::StoreData(const std::string& theFilename)
{
  if (theFilename != std::string(""))
  {
    std::ofstream out(theFilename.c_str(), std::ios::binary);
    if (out)
    {
      out << *this;
      out.close();
      return true;
    }
  }
  return false;
}

//--------------------------------------------------------
// operator ==
//--------------------------------------------------------
bool NFmiDrawParam::operator==(const NFmiDrawParam& theDrawParam) const
{
  return (itsPriority == theDrawParam.itsPriority);
}

//--------------------------------------------------------
// operator <
//--------------------------------------------------------
bool NFmiDrawParam::operator<(const NFmiDrawParam& theDrawParam) const
{
  return (itsPriority < theDrawParam.itsPriority);
}

// NFmiMetTime stringiksi ja stringistä metTimeksi funktiot, että voidaan tallentaa fiksatut origin
// timet
// viewMakroihin.
static const unsigned long gMetTime2ViewMacroStringFormat = kYYYYMMDDHHMMSS;
static std::string MetTime2String(const NFmiMetTime& theTime)
{
  return static_cast<char*>(theTime.ToStr(gMetTime2ViewMacroStringFormat));
}
static NFmiMetTime String2MetTime(const std::string& theStr)
{
  NFmiMetTime tmpTime;
  tmpTime.FromStr(theStr, gMetTime2ViewMacroStringFormat);
  return tmpTime;
}

//--------------------------------------------------------
// Write
//--------------------------------------------------------
std::ostream& NFmiDrawParam::Write(std::ostream& file) const
{
  using namespace std;

  file << "Version ";
  file << itsFileVersionNumber << endl;
  file << "'ParameterAbbreviation'" << endl;  // selittävä teksti
  if (GeneralData::ViewMacroDrawParam())
  {  // jos viewmacro tapaus ja siinä oleva macroParam, sen drawParamin nimen lyhenteeseen
     // talletetaan suhteellinen polku
    // ('optimointia', näin minun ei vielä tarvitse muuttaa minkään macrosysteemien data tiedoston
    // rakennetta)
    std::string tmpStr(GeneralData::MacroParamRelativePath());
    tmpStr += tmpStr.empty() ? "" : "\\";
    tmpStr += GeneralData::ParameterAbbreviation();
    file << tmpStr << endl;
  }
  else
    file << GeneralData::ParameterAbbreviation() << endl;
  file << "'Priority'" << endl;  // selittävä teksti
  file << itsPriority << endl;
  file << "'ViewType'" << endl;  // selittävä teksti
  file << static_cast<int>(itsViewType) << endl;
  file << "'UseIsoLineGabWithCustomContours'" << endl;  // selittävä teksti
  file << static_cast<int>(fUseIsoLineGabWithCustomContours) << endl;
  file << "'IsoLineGab'" << endl;  // selittävä teksti
  file << itsIsoLineGab << endl;
  file << "'IsolineColor'" << endl;  // selittävä teksti
  file << itsIsolineColor << endl;
  file << "'IsolineTextColor'" << endl;  // selittävä teksti
  file << itsIsolineTextColor << endl;
  file << "'ModifyingStep'" << endl;  // selittävä teksti
  file << itsModifyingStep << endl;
  file << "'ModifyingUnit'" << endl;  // selittävä teksti
                                      //	file << fModifyingUnit << endl;
  file << true << endl;  // tässä on otettu pois modifyingUnit, mutta arvo pitää tallettaa että
                         // luku/kirjoitus operaatiot eivät mene rikki

  file << "'FrameColor'" << endl;  // selittävä teksti
  file << itsFrameColor << endl;
  file << "'FillColor'" << endl;  // selittävä teksti
  file << itsFillColor << endl;
  file << "'IsolineLabelBoxFillColor'" << endl;  // selittävä teksti
  file << itsIsolineLabelBoxFillColor << endl;

  file << "'itsAbsoluteMinValue'" << endl;  // selittävä testi
  file << itsAbsoluteMinValue << endl;
  file << "'itsAbsoluteMaxValue'" << endl;  // selittävä teksti
  file << itsAbsoluteMaxValue << endl;

  file << "'TimeSeriesScaleMin'" << endl;  // selittävä teksti
  file << itsTimeSeriesScaleMin << endl;
  file << "'TimeSeriesScaleMax'" << endl;  // selittävä teksti
  file << itsTimeSeriesScaleMax << endl;

  file << "'RelativeSize'" << endl;  // selittävä teksti
  file << itsRelativeSize;
  file << "'RelativePositionOffset'" << endl;  // selittävä teksti
  file << itsRelativePositionOffset;
  file << "'OnlyOneSymbolRelativeSize'" << endl;  // selittävä teksti
  file << itsOnlyOneSymbolRelativeSize;
  file << "'OnlyOneSymbolRelativePositionOffset'" << endl;  // selittävä teksti
  file << itsOnlyOneSymbolRelativePositionOffset;

  file << "'PossibleViewTypeCount'" << endl;  // selittävä teksti
  file << itsPossibleViewTypeCount << endl;
  file << "'PossibleViewTypeList'" << endl;  // selittävä teksti
  for (int ind = 0; ind < itsPossibleViewTypeCount; ind++)
    file << static_cast<int>(itsPossibleViewTypeList[ind]) << endl;

  file << "'TimeSerialModifyingLimit'" << endl;  // selittävä teksti
  file << itsTimeSerialModifyingLimit << endl;

  // ******************************************************************
  // StationDataViewType otetttiin käyttöön vasta v. 2007, kun halusin
  // että voidaan katsoa asemadataa myös gridattuna. Ongelma oli että
  // kyseiseen dataosaan on joihinkin drawparameihin tallettunut tiedostoon
  // jo 1:stä suurempia lukuja. Tämä on kikka viitonen että ei tarvitsisi
  // konvertoida nykyisiä drawparam (dpa) tiedostoja siten että niissä
  // olisi defaulttinä 1 (=teksti tyyppi). Eli talletettaessa lisätään
  // lukuun 100. Luettaessa vähennetään tuo 100. Jos luku tällöin
  // on pienempi kuin 1, annetaan arvoksi 1.
  file << "'StationDataViewType'" << endl;  // selittävä teksti
  file << static_cast<int>(itsStationDataViewType + 100) << endl;
  file << "'EditableParam'" << endl;  // selittävä teksti
  file << false << endl;     // tämä muuttuja poistettu, muttä jokin arvo laitettava tähän
  file << "'Unit'" << endl;  // selittävä teksti
  file << GeneralData::Unit() << endl;

  file << "'ShowNumbers'" << endl;  // selittävä teksti
  file << fShowNumbers << endl;

  file << "'ShowMasks'" << endl;  // selittävä teksti
  file << fShowMasks << endl;
  file << "'ShowColors'" << endl;  // selittävä teksti
  file << fShowColors << endl;
  file << "'ShowColoredNumbers'" << endl;  // selittävä teksti
  file << fShowColoredNumbers << endl;
  file << "'ZeroColorMean'" << endl;  // selittävä teksti
  file << fZeroColorMean << endl;

  if (itsFileVersionNumber >= 2.)  // tämä on vain esimerkki siitä mitä joskus tulee olemaan
  {
    //***********************************************
    //********** 'versio 2' parametreja *************
    //***********************************************
    file << itsStationSymbolColorShadeLowValue << endl;
    file << itsStationSymbolColorShadeMidValue << endl;
    file << itsStationSymbolColorShadeHighValue << endl;
    file << itsStationSymbolColorShadeLowValueColor << endl;
    file << itsStationSymbolColorShadeMidValueColor << endl;
    file << itsStationSymbolColorShadeHighValueColor << endl;
    file << itsStationSymbolColorShadeClassCount << endl;
    file << fUseSymbolsInTextMode << endl;
    file << itsUsedSymbolListIndex << endl;
    file << itsSymbolIndexingMapListIndex << endl;
    file << itsGridDataPresentationStyle << endl;
    file << fUseIsoLineFeathering << endl;
    file << fIsoLineLabelsOverLapping << endl;
    file << fShowColorLegend << endl;
    file << fUseSimpleIsoLineDefinitions << endl;
    file << fUseSeparatorLinesBetweenColorContourClasses << endl;
    file << itsSimpleIsoLineGap << endl;
    file << itsSimpleIsoLineZeroValue << endl;
    file << itsSimpleIsoLineLabelHeight << endl;
    file << fShowSimpleIsoLineLabelBox << endl;
    file << itsSimpleIsoLineWidth << endl;
    file << itsSimpleIsoLineLineStyle << endl;
    file << itsIsoLineSplineSmoothingFactor << endl;
    file << fUseSingleColorsWithSimpleIsoLines << endl;
    file << itsSimpleIsoLineColorShadeLowValue << endl;
    file << itsSimpleIsoLineColorShadeMidValue << endl;
    file << itsSimpleIsoLineColorShadeHighValue << endl;
    file << itsSimpleIsoLineColorShadeLowValueColor << endl;
    file << itsSimpleIsoLineColorShadeMidValueColor << endl;
    file << itsSimpleIsoLineColorShadeHighValueColor << endl;
    file << itsSimpleIsoLineColorShadeClassCount << endl;

    size_t i = 0;
    size_t size = itsSpecialIsoLineValues.size();
    file << size << endl;
    for (i = 0; i < size; i++)
      file << itsSpecialIsoLineValues[i] << " ";
    file << endl;

    size = itsSpecialIsoLineLabelHeight.size();
    file << size << endl;
    for (i = 0; i < size; i++)
      file << itsSpecialIsoLineLabelHeight[i] << " ";
    file << endl;

    size = itsSpecialIsoLineWidth.size();
    file << size << endl;
    for (i = 0; i < size; i++)
      file << itsSpecialIsoLineWidth[i] << " ";
    file << endl;

    size = itsSpecialIsoLineStyle.size();
    file << size << endl;
    for (i = 0; i < size; i++)
      file << itsSpecialIsoLineStyle[i] << " ";
    file << endl;

    size = itsSpecialIsoLineColorIndexies.size();
    file << size << endl;
    for (i = 0; i < size; i++)
      file << itsSpecialIsoLineColorIndexies[i] << " ";
    file << endl;

    size = itsSpecialIsoLineShowLabelBox.size();
    file << size << endl;
    for (i = 0; i < size; i++)
      file << itsSpecialIsoLineShowLabelBox[i] << " ";
    file << endl;

    file << fDrawOnlyOverMask << endl;
    file << fUseCustomColorContouring << endl;

    size = itsSpecialColorContouringValues.size();
    file << size << endl;
    for (i = 0; i < size; i++)
      file << itsSpecialColorContouringValues[i] << " ";
    file << endl;

    size = itsSpecialColorContouringColorIndexies.size();
    file << size << endl;
    for (i = 0; i < size; i++)
      file << itsSpecialColorContouringColorIndexies[i] << " ";
    file << endl;

    file << itsColorContouringColorShadeLowValue << endl;
    file << itsColorContouringColorShadeMidValue << endl;
    file << itsColorContouringColorShadeHighValue << endl;
    file << itsColorContouringColorShadeLowValueColor << endl;
    file << itsColorContouringColorShadeMidValueColor << endl;
    file << itsColorContouringColorShadeHighValueColor << endl;
    file << itsColorContouringColorShadeHigh2Value << endl;
    file << fUseWithIsoLineHatch1 << endl;
    file << fDrawIsoLineHatchWithBorders1 << endl;
    file << itsIsoLineHatchLowValue1 << endl;
    file << itsIsoLineHatchHighValue1 << endl;
    file << itsIsoLineHatchType1 << endl;
    file << itsIsoLineHatchColor1 << endl;
    file << itsIsoLineHatchBorderColor1 << endl;
    file << fUseWithIsoLineHatch2 << endl;
    file << fDrawIsoLineHatchWithBorders2 << endl;
    file << itsIsoLineHatchLowValue2 << endl;
    file << itsIsoLineHatchHighValue2 << endl;
    file << itsIsoLineHatchType2 << endl;
    file << itsIsoLineHatchColor2 << endl;
    file << itsColorContouringColorShadeHigh2ValueColor << endl;
    file << itsIsoLineLabelDigitCount << endl;
    //***********************************************
    //********** 'versio 2' parametreja *************
    //***********************************************
  }

  if (itsFileVersionNumber >= 3.)
  {
    //***********************************************
    //********** 'versio 3' parametreja *************
    //***********************************************
    file << "Version_3_stuff" << endl;
    file << itsContourLabelBoxFillColor << endl;
    file << fUseContourGabWithCustomContours << " " << itsContourGab << endl;
    file << itsContourColor << endl;
    file << itsContourTextColor << endl;
    file << fUseContourFeathering << " " << fUseSimpleContourDefinitions << " "
         << itsSimpleContourZeroValue << " " << itsSimpleContourLabelHeight << endl;
    file << fShowSimpleContourLabelBox << " " << itsSimpleContourWidth << " "
         << itsSimpleContourLineStyle << " " << itsSimpleIsoLineColorShadeHigh2Value << endl;
    file << itsSimpleIsoLineColorShadeHigh2ValueColor << endl;

    NFmiDataStoringHelpers::WriteContainer(itsSpecialContourValues, file, std::string(" "));
    file << endl;
    NFmiDataStoringHelpers::WriteContainer(itsSpecialContourLabelHeight, file, std::string(" "));
    file << endl;
    NFmiDataStoringHelpers::WriteContainer(itsSpecialContourWidth, file, std::string(" "));
    file << endl;
    NFmiDataStoringHelpers::WriteContainer(itsSpecialContourStyle, file, std::string(" "));
    file << endl;
    NFmiDataStoringHelpers::WriteContainer(itsSpecialContourColorIndexies, file, std::string(" "));
    file << endl;

    file << fUseCustomIsoLineing << " " << itsContourLabelDigitCount << endl;

    NFmiDataStoringHelpers::NFmiExtraDataStorage extraData;  // lopuksi vielä mahdollinen extra data
    // Kun tulee uusia muuttujia, tee tähän extradatan täyttöä, jotta se saadaan talteen tiedopstoon
    // siten että
    // edelliset versiot eivät mene solmuun vaikka on tullut uutta dataa.
    extraData.Add(itsAlpha);  // alpha on siis 1. uusista double-extra-parametreista
    extraData.Add(
        GeneralData::ModelRunIndex());  // modelRunIndex on 2. uusista double-extra-parametreista
    extraData.Add(GeneralData::TimeSerialModelRunCount());  // modelRunIndex on 3. uusista
                                                            // double-extra-parametreista
    extraData.Add(
        GeneralData::ModelRunDifferenceIndex());  // itsModelRunDifferenceIndex on 4. uusista
                                                  // double-extra-parametreista
    extraData.Add(
        static_cast<double>(GeneralData::DataComparisonProdId()));  // itsDataComparisonProdId on 5.
                                                                    // uusista
                                                                    // double-extra-parametreista
    extraData.Add(GeneralData::DataComparisonType());  // itsDataComparisonType on 6. uusista
                                                       // double-extra-parametreista

    extraData.Add(::MetTime2String(
        GeneralData::ModelOriginTime()));  // modelRunIndex on 1. uusista string-extra-parametreista

    file << "possible_extra_data" << std::endl;
    file << extraData;

    if (file.fail())
      throw std::runtime_error("NFmiDrawParam::Write failed");
    //***********************************************
    //********** 'versio 3' parametreja *************
    //***********************************************
  }
  return file;
}

// Kun muutin drawParamien luku/kirjoitukset binäärisiksi, tuli SmartMetiin seuraava bugi:
// Harmaassa param-boxissa tuli joillekin parametreille nimeksi pelkkä ?.
// Tämä johtui siitä että binääri luvussa luetaan teksti sellaisenään tiedostosta, eikä siinä
// skipata \r ja \n merkkejä.
// Nämä merkit ? -tekstin perässä aiheutti sen että parametri-näytössä ollut "?" tekstin tarkastu
// meni pieleen.
// Siksi luetusta parametrin nimi tekstistä pitää siivota lopusta poi mahdolliset rivinvaihto
// merkit.
static void FixBinaryReadParameterAbbreviation(std::string& paramNameInOut)
{
  // Trimmaa oikealta muutamia mahdollisia rivinvaihto merkkejä pois
  NFmiStringTools::TrimR(paramNameInOut, '\n');
  NFmiStringTools::TrimR(paramNameInOut, '\r');
  NFmiStringTools::TrimR(paramNameInOut, '\n');
}

//--------------------------------------------------------
// Read
//--------------------------------------------------------
std::istream& NFmiDrawParam::Read(std::istream& file)
{
  float istreamFileVersionNumber = GeneralData::InitFileVersionNumber();
  char temp[80];
  std::string tmpStr;
  int number;
  if (!file)
    return file;
  file >> temp;
  if (std::string(temp) == std::string("Version"))
  {
    file >> istreamFileVersionNumber;
    if (istreamFileVersionNumber > itsFileVersionNumber)
      throw std::runtime_error(
          "NFmiDrawParam::Read failed because version number in DrawParam was higher than program "
          "expects.");

    if (istreamFileVersionNumber >= 1.)  // tämä on vain esimerkki siitä mitä joskus tulee olemaan
    {
      if (!file)
        return file;
      file >> temp;                // luetaan nimike pois
      std::getline(file, tmpStr);  // luetaan ed. rivinvaihto pois jaloista
      std::getline(file, tmpStr);  // luetaan rivin loppuun, jos lyhenteessä spaceja mahdollisesti
      std::string::size_type pos = tmpStr.find_last_of('/');
      if (pos == std::string::npos)
        pos = tmpStr.find_last_of('\\');  // kokeillaan varmuuden vuoksi slasyä molempiin suuntiin

      if (pos != std::string::npos)
      {
        // jos löytyi kenoviiva lyhenteestä, laitetaan viemacrossa olevan macroparamin suhteellinen
        // polku talteen -- huom! kenoa ei oteta talteen
        GeneralData::MacroParamRelativePath(std::string(tmpStr.begin(), tmpStr.begin() + pos));
        GeneralData::ParameterAbbreviation(std::string(tmpStr.begin() + pos + 1, tmpStr.end()));
      }
      else
      {
        GeneralData::MacroParamRelativePath("");
        ::FixBinaryReadParameterAbbreviation(tmpStr);  // Binääri lukuun siirron takia pitää
                                                       // trimmata mahdollisia rivinvaihto merkkejä
                                                       // pois
        GeneralData::ParameterAbbreviation(tmpStr);
      }
      file >> temp;  // luetaan nimike pois
      file >> itsPriority;
      file >> temp;  // luetaan nimike pois
      file >> number;
      itsViewType = NFmiMetEditorTypes::View(number);

      file >> temp;  // luetaan nimike pois
      file >> number;
      fUseIsoLineGabWithCustomContours = number != 0;
      file >> temp;  // luetaan nimike pois
      file >> itsIsoLineGab;
      file >> temp;  // luetaan nimike pois
      file >> itsIsolineColor;
      file >> temp;  // luetaan nimike pois
      file >> itsIsolineTextColor;
      file >> temp;  // luetaan nimike pois
      file >> itsModifyingStep;
      file >> temp;  // luetaan nimike pois
                     //			file >> fModifyingUnit;
      file >> temp;  // luetaan legacy-koodi modifyingUnit pois

      file >> temp;  // luetaan nimike pois
      file >> itsFrameColor;
      file >> temp;  // luetaan nimike pois
      file >> itsFillColor;
      file >> temp;  // luetaan nimike pois
      file >> itsIsolineLabelBoxFillColor;

      file >> temp;  // luetaan nimike pois
      file >> itsAbsoluteMinValue;
      file >> temp;  // luetaan nimike pois
      file >> itsAbsoluteMaxValue;

      if (!file)
        return file;

      file >> temp;  // luetaan nimike pois
      file >> itsTimeSeriesScaleMin;
      file >> temp;  // luetaan nimike pois
      file >> itsTimeSeriesScaleMax;

      file >> temp;  // luetaan nimike pois
      file >> itsRelativeSize;
      file >> temp;  // luetaan nimike pois
      file >> itsRelativePositionOffset;
      file >> temp;  // luetaan nimike pois
      file >> itsOnlyOneSymbolRelativeSize;
      file >> temp;  // luetaan nimike pois
      file >> itsOnlyOneSymbolRelativePositionOffset;

      file >> temp;  // luetaan nimike pois
      file >> itsPossibleViewTypeCount;
      file >> temp;  // luetaan nimike pois
      if (!file)
        return file;
      for (int ind = 0; ind < itsPossibleViewTypeCount; ind++)
      {
        file >> number;
        itsPossibleViewTypeList[ind] = NFmiMetEditorTypes::View(number);
      }

      if (!file)
        return file;

      file >> temp;  // luetaan nimike pois
      file >> itsTimeSerialModifyingLimit;
      file >> temp;  // luetaan nimike pois

      // ******************************************************************
      // StationDataViewType otetttiin käyttöön vasta v. 2007, kun halusin
      // että voidaan katsoa asemadataa myös gridattuna. Ongelma oli että
      // kyseiseen dataosaan on joihinkin drawparameihin tallettunut tiedostoon
      // jo 1:stä suurempia lukuja. Tämä on kikka viitonen että ei tarvitsisi
      // konvertoida nykyisiä drawparam (dpa) tiedostoja siten että niissä
      // olisi defaulttinä 1 (=teksti tyyppi). Eli talletettaessa lisätään
      // lukuun 100. Luettaessa vähennetään tuo 100. Jos luku tällöin
      // on pienempi kuin 1, annetaan arvoksi 1.
      file >> number;
      number -= 100;
      if (number < 1)
        number = 1;
      itsStationDataViewType = NFmiMetEditorTypes::View(number);
      file >> temp;  // luetaan nimike pois
      bool tmpBool = false;
      file >> tmpBool;  // tämä on legacy koodia, pitää lukea bool arvo tässä

      file >> temp;  // luetaan nimike pois
      file >> temp;
      if (!file)
        return file;
      GeneralData::Unit(std::string(temp));

      file >> temp;  // luetaan nimike pois
      file >> fShowNumbers;
      file >> temp;  // luetaan nimike pois
      file >> fShowMasks;
      file >> temp;  // luetaan nimike pois
      file >> fShowColors;
      file >> temp;  // luetaan nimike pois
      file >> fShowColoredNumbers;
      file >> temp;  // luetaan nimike pois
      file >> fZeroColorMean;

      //***********************************************
      //********** 'versio 2' parametreja *************
      //***********************************************
      if (istreamFileVersionNumber >= 2.)  // tämä on vain esimerkki siitä mitä joskus tulee olemaan
      {
        if (!file)
          return file;
        file >> itsStationSymbolColorShadeLowValue;
        file >> itsStationSymbolColorShadeMidValue;
        file >> itsStationSymbolColorShadeHighValue;
        file >> itsStationSymbolColorShadeLowValueColor;
        file >> itsStationSymbolColorShadeMidValueColor;
        file >> itsStationSymbolColorShadeHighValueColor;
        file >> itsStationSymbolColorShadeClassCount;
        file >> fUseSymbolsInTextMode;
        file >> itsUsedSymbolListIndex;
        file >> itsSymbolIndexingMapListIndex;
        file >> itsGridDataPresentationStyle;
        file >> fUseIsoLineFeathering;
        file >> fIsoLineLabelsOverLapping;
        file >> fShowColorLegend;
        file >> fUseSimpleIsoLineDefinitions;
        file >> fUseSeparatorLinesBetweenColorContourClasses;
        file >> itsSimpleIsoLineGap;
        file >> itsSimpleIsoLineZeroValue;
        file >> itsSimpleIsoLineLabelHeight;
        file >> fShowSimpleIsoLineLabelBox;
        file >> itsSimpleIsoLineWidth;
        file >> itsSimpleIsoLineLineStyle;
        file >> itsIsoLineSplineSmoothingFactor;
        file >> fUseSingleColorsWithSimpleIsoLines;
        file >> itsSimpleIsoLineColorShadeLowValue;
        file >> itsSimpleIsoLineColorShadeMidValue;
        file >> itsSimpleIsoLineColorShadeHighValue;
        file >> itsSimpleIsoLineColorShadeLowValueColor;
        file >> itsSimpleIsoLineColorShadeMidValueColor;
        file >> itsSimpleIsoLineColorShadeHighValueColor;
        file >> itsSimpleIsoLineColorShadeClassCount;

        if (!file)
          return file;
        int i = 0;
        int size;
        file >> size;
        itsSpecialIsoLineValues.resize(size);
        for (i = 0; i < size; i++)
          file >> itsSpecialIsoLineValues[i];

        file >> size;
        itsSpecialIsoLineLabelHeight.resize(size);
        for (i = 0; i < size; i++)
          file >> itsSpecialIsoLineLabelHeight[i];

        file >> size;
        itsSpecialIsoLineWidth.resize(size);
        for (i = 0; i < size; i++)
          file >> itsSpecialIsoLineWidth[i];

        file >> size;
        itsSpecialIsoLineStyle.resize(size);
        for (i = 0; i < size; i++)
          file >> itsSpecialIsoLineStyle[i];

        file >> size;
        itsSpecialIsoLineColorIndexies.resize(size);
        for (i = 0; i < size; i++)
          file >> itsSpecialIsoLineColorIndexies[i];

        file >> size;
        itsSpecialIsoLineShowLabelBox.resize(size);
        for (i = 0; i < size; i++)
        {
          // Mika: g++ 2.95 ei salli suoraa sijoitusta
          bool foobar;
          file >> foobar;
          itsSpecialIsoLineShowLabelBox[i] = foobar;
        }

        if (!file)
          return file;
        file >> fDrawOnlyOverMask;
        file >> fUseCustomColorContouring;

        file >> size;
        if (!file)
          return file;
        itsSpecialColorContouringValues.resize(size);
        for (i = 0; i < size; i++)
          file >> itsSpecialColorContouringValues[i];

        file >> size;
        if (!file)
          return file;
        itsSpecialColorContouringColorIndexies.resize(size);
        for (i = 0; i < size; i++)
          file >> itsSpecialColorContouringColorIndexies[i];

        file >> itsColorContouringColorShadeLowValue;
        file >> itsColorContouringColorShadeMidValue;
        file >> itsColorContouringColorShadeHighValue;
        file >> itsColorContouringColorShadeLowValueColor;
        file >> itsColorContouringColorShadeMidValueColor;
        file >> itsColorContouringColorShadeHighValueColor;
        file >> itsColorContouringColorShadeHigh2Value;
        file >> fUseWithIsoLineHatch1;
        file >> fDrawIsoLineHatchWithBorders1;
        file >> itsIsoLineHatchLowValue1;
        file >> itsIsoLineHatchHighValue1;
        file >> itsIsoLineHatchType1;
        file >> itsIsoLineHatchColor1;
        file >> itsIsoLineHatchBorderColor1;
        file >> fUseWithIsoLineHatch2;
        file >> fDrawIsoLineHatchWithBorders2;
        file >> itsIsoLineHatchLowValue2;
        file >> itsIsoLineHatchHighValue2;
        file >> itsIsoLineHatchType2;
        file >> itsIsoLineHatchColor2;
        file >> itsColorContouringColorShadeHigh2ValueColor;
        file >> itsIsoLineLabelDigitCount;
        if (!file)
          return file;
        //***********************************************
        //********** 'versio 2' parametreja *************
        //***********************************************
      }

      if (istreamFileVersionNumber >= 3.)
      {
        //***********************************************
        //********** 'versio 3' parametreja *************
        //***********************************************
        file >> temp;  // luetaan 'Version_3_stuff' pois
        file >> itsContourLabelBoxFillColor;
        file >> fUseContourGabWithCustomContours >> itsContourGab;
        file >> itsContourColor;
        file >> itsContourTextColor;
        file >> fUseContourFeathering >> fUseSimpleContourDefinitions >>
            itsSimpleContourZeroValue >> itsSimpleContourLabelHeight;
        file >> fShowSimpleContourLabelBox >> itsSimpleContourWidth >> itsSimpleContourLineStyle >>
            itsSimpleIsoLineColorShadeHigh2Value;
        file >> itsSimpleIsoLineColorShadeHigh2ValueColor;

        NFmiDataStoringHelpers::ReadContainer(itsSpecialContourValues, file);
        NFmiDataStoringHelpers::ReadContainer(itsSpecialContourLabelHeight, file);
        NFmiDataStoringHelpers::ReadContainer(itsSpecialContourWidth, file);
        NFmiDataStoringHelpers::ReadContainer(itsSpecialContourStyle, file);
        NFmiDataStoringHelpers::ReadContainer(itsSpecialContourColorIndexies, file);

        file >> fUseCustomIsoLineing >> itsContourLabelDigitCount;

        if (file.fail())
          throw std::runtime_error("NFmiDrawParam::Write failed");

        file >> temp;  // luetaan 'possible_extra_data' pois
        NFmiDataStoringHelpers::NFmiExtraDataStorage
            extraData;  // lopuksi vielä mahdollinen extra data
        file >> extraData;
        // Tässä sitten otetaaan extradatasta talteen uudet muuttujat, mitä on mahdollisesti tullut
        // eli jos uusia muutujia tai arvoja, käsittele tässä.
        itsAlpha = 100.f;  // tämä on siis default arvo alphalle (täysin läpinäkyvä)
        if (extraData.itsDoubleValues.size() >= 1)
          Alpha(static_cast<float>(
              extraData
                  .itsDoubleValues[0]));  // laitetaan asetus-funktion läpi, jossa raja tarkistukset
        GeneralData::ModelRunIndex(0);    // 0 on default, eli ei ole käytössä
        if (extraData.itsDoubleValues.size() >= 2)
          ModelRunIndex(static_cast<int>(
              extraData
                  .itsDoubleValues[1]));  // laitetaan asetus-funktion läpi, jossa raja tarkistukset
        GeneralData::TimeSerialModelRunCount(0);
        if (extraData.itsDoubleValues.size() >= 3)
          TimeSerialModelRunCount(static_cast<int>(
              extraData
                  .itsDoubleValues[2]));  // laitetaan asetus-funktion läpi, jossa raja tarkistukset
        GeneralData::ModelRunDifferenceIndex(0);  // 0 on default, eli ei ole käytössä
        if (extraData.itsDoubleValues.size() >= 4)
          ModelRunDifferenceIndex(static_cast<int>(
              extraData
                  .itsDoubleValues[3]));  // laitetaan asetus-funktion läpi, jossa raja tarkistukset
        GeneralData::DataComparisonProdId(0);
        if (extraData.itsDoubleValues.size() >= 5)
          DataComparisonProdId(static_cast<unsigned long>(
              extraData
                  .itsDoubleValues[4]));  // laitetaan asetus-funktion läpi, jossa raja tarkistukset
        GeneralData::DataComparisonType(NFmiInfoData::kNoDataType);
        if (extraData.itsDoubleValues.size() >= 6)
          DataComparisonType(static_cast<NFmiInfoData::Type>(
              static_cast<int>(extraData.itsDoubleValues[5])));  // laitetaan asetus-funktion läpi,
                                                                 // jossa raja tarkistukset

        GeneralData::ModelOriginTime(
            NFmiMetTime::gMissingTime);  // tämä on oletus arvo eli ei ole käytössä
        if (extraData.itsStringValues.size() >= 1)
          GeneralData::ModelOriginTime(::String2MetTime(
              extraData
                  .itsStringValues[0]));  // laitetaan asetus-funktion läpi, jossa raja tarkistukset

        if (file.fail())
          throw std::runtime_error("NFmiDrawParam::Write failed");
        //***********************************************
        //********** 'versio 3' parametreja *************
        //***********************************************
      }
      else
      {  // tietyt muuttujat pitää alustaa jos versio 2.
        itsColorContouringColorShadeHigh2Value = itsColorContouringColorShadeHighValue;
        itsColorContouringColorShadeHigh2ValueColor = itsColorContouringColorShadeHighValueColor;

        itsContourLabelBoxFillColor = itsIsolineLabelBoxFillColor;
        fUseContourGabWithCustomContours = false;
        itsContourGab = itsIsoLineGab;
        itsContourColor = itsIsolineColor;
        itsContourTextColor = itsIsolineTextColor;
        fUseContourFeathering = fUseIsoLineFeathering;
        fUseSimpleContourDefinitions = fUseSimpleIsoLineDefinitions;
        itsSimpleContourZeroValue = itsSimpleIsoLineZeroValue;
        itsSimpleContourLabelHeight = itsSimpleIsoLineLabelHeight;
        fShowSimpleContourLabelBox = fShowSimpleIsoLineLabelBox;
        itsSimpleContourWidth = itsSimpleIsoLineWidth;
        itsSimpleContourLineStyle = itsSimpleIsoLineLineStyle;
        itsSimpleIsoLineColorShadeHigh2Value = itsSimpleIsoLineColorShadeHighValue;
        itsSimpleIsoLineColorShadeHigh2ValueColor = itsSimpleIsoLineColorShadeHighValueColor;

        itsSpecialContourValues = itsSpecialIsoLineValues;
        itsSpecialContourLabelHeight = itsSpecialIsoLineLabelHeight;
        itsSpecialContourWidth = itsSpecialIsoLineWidth;
        itsSpecialContourStyle = itsSpecialIsoLineStyle;
        itsSpecialContourColorIndexies = itsSpecialIsoLineColorIndexies;

        fUseCustomIsoLineing = fUseCustomColorContouring;
        itsContourLabelDigitCount = itsIsoLineLabelDigitCount;
        itsAlpha = 100.f;  // tämä on siis default arvo alphalle (täysin läpinäkyvä)
      }
    }
    GeneralData::InitFileVersionNumber(itsFileVersionNumber);  // lopuksi asetetaan versio numero
    // viimeisimmäksi, että tulevaisuudessa
    // talletus menee uudella versiolla
  }
  return file;
}

bool NFmiDrawParam::UseArchiveModelData(void) const
{
  if (IsModelRunDataType())
  {
    if (GeneralData::ModelOriginTime() != NFmiMetTime::gMissingTime)
      return true;
    if (GeneralData::ModelRunIndex() < 0)
      return true;
  }
  return false;
}

bool NFmiDrawParam::IsModelRunDataType(void) const
{
  return NFmiDrawParam::IsModelRunDataType(this->DataType());
}

bool NFmiDrawParam::IsModelRunDataType(NFmiInfoData::Type theDataType)
{
  if (theDataType == NFmiInfoData::kViewable || theDataType == NFmiInfoData::kHybridData ||
      theDataType == NFmiInfoData::kModelHelpData || theDataType == NFmiInfoData::kKepaData ||
      theDataType == NFmiInfoData::kTrajectoryHistoryData)
    return true;
  if (theDataType == NFmiInfoData::kClimatologyData)
    return true;
  return false;
}

bool NFmiDrawParam::DoDataComparison(void)
{
  if (GeneralData::DataComparisonProdId() != 0)
  {
    if (GeneralData::DataComparisonType() != NFmiInfoData::kNoDataType)
      return true;
  }
  return false;
}

bool NFmiDrawParam::IsMacroParamCase(NFmiInfoData::Type theDataType)
{
  if (theDataType == NFmiInfoData::kMacroParam ||
      theDataType == NFmiInfoData::kCrossSectionMacroParam ||
      theDataType == NFmiInfoData::kQ3MacroParam)
    return true;
  else
    return false;
}

bool NFmiDrawParam::IsMacroParamCase(bool justCheckDataType)
{
  if (justCheckDataType)
  {
    if (IsMacroParamCase(GeneralData::DataType()))
      return true;
  }
  else
  {
    if (GeneralData::ViewMacroDrawParam() == false && (IsMacroParamCase(GeneralData::DataType())) &&
        GeneralData::ParameterAbbreviation() != std::string("macroParam"))
      return true;
  }
  return false;
}
