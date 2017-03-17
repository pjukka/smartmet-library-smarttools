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

//--------------------------------------------------------
// NFmiDrawParam(void)
//--------------------------------------------------------
NFmiDrawParam::NFmiDrawParam()
    : GeneralData(),
      GeneralVisualization(),
      SimpleColorContour(),
      CustomIsoline(),
      itsPriority(1),
      itsStationDataViewType(NFmiMetEditorTypes::kFmiTextView),
      itsFrameColor(NFmiColor(0., 0., 0.))  // musta
      ,
      itsFillColor(NFmiColor(0., 0., 0.))  // musta
      ,
      itsRelativeSize(NFmiPoint(1, 1)),
      itsRelativePositionOffset(NFmiPoint(0, 0)),
      itsOnlyOneSymbolRelativeSize(NFmiPoint(1, 1)),
      itsOnlyOneSymbolRelativePositionOffset(NFmiPoint(0, 0)),
      fUseIsoLineGabWithCustomContours(false),
      fUseContourGabWithCustomContours(false),
      itsModifyingStep(1.)
      //, fModifyingUnit						 (true)
      ,
      itsTimeSerialModifyingLimit(10),
      itsContourTextColor(NFmiColor(0., 0., 0.)),
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
      fUseContourFeathering(false),
      fIsoLineLabelsOverLapping(true),
      fShowColorLegend(false),
      itsSimpleIsoLineZeroValue(0),
      itsSimpleContourZeroValue(0),
      itsSimpleContourLabelHeight(4),
      fShowSimpleContourLabelBox(false),
      itsSpecialColorContouringValues(),
      itsSpecialColorContouringColorIndexies(),
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
      itsContourLabelDigitCount(0)
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
      GeneralVisualization(),
      SimpleColorContour(),
      CustomIsoline(),
      itsPriority(thePriority),
      itsStationDataViewType(NFmiMetEditorTypes::kFmiTextView),
      itsFrameColor(NFmiColor(0., 0., 0.))  // musta
      ,
      itsFillColor(NFmiColor(0., 0., 0.))  // musta
      ,
      itsRelativeSize(NFmiPoint(1, 1)),
      itsRelativePositionOffset(NFmiPoint(0, 0)),
      itsOnlyOneSymbolRelativeSize(NFmiPoint(1, 1)),
      itsOnlyOneSymbolRelativePositionOffset(NFmiPoint(0, 0)),
      fUseIsoLineGabWithCustomContours(false),
      fUseContourGabWithCustomContours(false),
      itsModifyingStep(1.)
      //, fModifyingUnit						 (true)
      ,
      itsTimeSerialModifyingLimit(10),
      itsContourTextColor(NFmiColor(0., 0., 0.)),
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
      fUseContourFeathering(false),
      fIsoLineLabelsOverLapping(true),
      fShowColorLegend(false),
      itsSimpleIsoLineZeroValue(0),
      itsSimpleContourZeroValue(0),
      itsSimpleContourLabelHeight(4),
      fShowSimpleContourLabelBox(false),
      itsSpecialColorContouringValues(),
      itsSpecialColorContouringColorIndexies(),
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
      itsContourLabelDigitCount(0)
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
      GeneralVisualization(other),
      SimpleColorContour(other),
      CustomIsoline(other),
      itsPriority(other.itsPriority),
      itsStationDataViewType(other.itsStationDataViewType),
      itsFrameColor(other.itsFrameColor),
      itsFillColor(other.itsFillColor),
      itsRelativeSize(other.itsRelativeSize),
      itsRelativePositionOffset(other.itsRelativePositionOffset),
      itsOnlyOneSymbolRelativeSize(other.itsOnlyOneSymbolRelativeSize),
      itsOnlyOneSymbolRelativePositionOffset(other.itsOnlyOneSymbolRelativePositionOffset),
      fUseIsoLineGabWithCustomContours(other.fUseIsoLineGabWithCustomContours),
      fUseContourGabWithCustomContours(other.fUseContourGabWithCustomContours),
      itsModifyingStep(other.itsModifyingStep)
      //, fModifyingUnit(other.fModifyingUnit)
      ,
      itsTimeSerialModifyingLimit(other.itsTimeSerialModifyingLimit),
      itsContourTextColor(other.itsContourTextColor),
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
      fUseContourFeathering(other.fUseContourFeathering),
      fIsoLineLabelsOverLapping(other.fIsoLineLabelsOverLapping),
      fShowColorLegend(other.fShowColorLegend),
      itsSimpleIsoLineZeroValue(other.itsSimpleIsoLineZeroValue),
      itsSimpleContourZeroValue(other.itsSimpleContourZeroValue),
      itsSimpleContourLabelHeight(other.itsSimpleContourLabelHeight),
      fShowSimpleContourLabelBox(other.fShowSimpleContourLabelBox),
      itsSpecialColorContouringValues(other.itsSpecialColorContouringValues),
      itsSpecialColorContouringColorIndexies(other.itsSpecialColorContouringColorIndexies),
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
      itsContourLabelDigitCount(other.itsContourLabelDigitCount)
//***********************************************
//********** 'versio 2' parametreja *************
//***********************************************
{
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

    GeneralVisualization::ViewType(theDrawParam->ViewType());
    itsStationDataViewType = theDrawParam->StationDataViewType();

    itsFrameColor = NFmiColor(theDrawParam->FrameColor());
    itsFillColor = NFmiColor(theDrawParam->FillColor());
    SimpleIsoline::IsolineLabelBoxFillColor(NFmiColor(theDrawParam->IsolineLabelBoxFillColor()));

    itsRelativeSize = NFmiPoint(theDrawParam->RelativeSize());
    itsRelativePositionOffset = NFmiPoint(theDrawParam->RelativePositionOffset());
    itsOnlyOneSymbolRelativeSize = NFmiPoint(theDrawParam->OnlyOneSymbolRelativeSize());
    itsOnlyOneSymbolRelativePositionOffset =
        NFmiPoint(theDrawParam->OnlyOneSymbolRelativePositionOffset());

    fUseIsoLineGabWithCustomContours = theDrawParam->UseIsoLineGabWithCustomContours();
    fUseContourGabWithCustomContours = theDrawParam->UseContourGabWithCustomContours();
    SimpleIsoline::IsoLineGab(theDrawParam->IsoLineGab());
    itsModifyingStep = theDrawParam->ModifyingStep();
    //		fModifyingUnit =  theDrawParam->ModifyingUnit();
    itsTimeSerialModifyingLimit = theDrawParam->TimeSerialModifyingLimit();

    SimpleIsoline::IsolineColor(theDrawParam->SimpleIsoline::IsolineColor());
    SimpleIsoline::IsolineTextColor(theDrawParam->IsolineTextColor());

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

    GeneralVisualization::HideParam(theDrawParam->IsParamHidden());

    GeneralData::Unit(theDrawParam->Unit());

    fShowNumbers = theDrawParam->ShowNumbers();
    fShowMasks = theDrawParam->ShowMasks();
    fShowColors = theDrawParam->ShowColors();
    fShowColoredNumbers = theDrawParam->ShowColoredNumbers();
    fZeroColorMean = theDrawParam->ZeroColorMean();
    GeneralVisualization::ShowDifference(theDrawParam->ShowDifference());
    GeneralVisualization::ShowDifferenceToOriginalData(
        theDrawParam->ShowDifferenceToOriginalData());

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
    GeneralVisualization::GridDataPresentationStyle(theDrawParam->GridDataPresentationStyle());
    GeneralVisualization::UseIsoLineFeathering(theDrawParam->UseIsoLineFeathering());
    fIsoLineLabelsOverLapping = theDrawParam->IsoLineLabelsOverLapping();
    fShowColorLegend = theDrawParam->ShowColorLegend();
    GeneralVisualization::UseSimpleIsoLineDefinitions(theDrawParam->UseSimpleIsoLineDefinitions());
    GeneralVisualization::UseSeparatorLinesBetweenColorContourClasses(
        theDrawParam->UseSeparatorLinesBetweenColorContourClasses());
    SimpleIsoline::SimpleIsoLineGap(theDrawParam->SimpleIsoLineGap());
    itsSimpleIsoLineZeroValue = theDrawParam->SimpleIsoLineZeroValue();
    SimpleIsoline::SimpleIsoLineLabelHeight(theDrawParam->SimpleIsoLineLabelHeight());
    SimpleIsoline::ShowSimpleIsoLineLabelBox(theDrawParam->ShowSimpleIsoLineLabelBox());
    SimpleIsoline::SimpleIsoLineWidth(theDrawParam->SimpleIsoline::SimpleIsoLineWidth());
    SimpleIsoline::SimpleIsoLineLineStyle(theDrawParam->SimpleIsoline::SimpleIsoLineLineStyle());
    GeneralVisualization::IsoLineSplineSmoothingFactor(
        theDrawParam->IsoLineSplineSmoothingFactor());
    SimpleIsoline::UseSingleColorsWithSimpleIsoLines(
        theDrawParam->UseSingleColorsWithSimpleIsoLines());
    SimpleIsoline::SimpleIsoLineColorShadeLowValue(theDrawParam->SimpleIsoLineColorShadeLowValue());
    SimpleIsoline::SimpleIsoLineColorShadeMidValue(theDrawParam->SimpleIsoLineColorShadeMidValue());
    SimpleIsoline::SimpleIsoLineColorShadeHighValue(
        theDrawParam->SimpleIsoLineColorShadeHighValue());
    SimpleIsoline::SimpleIsoLineColorShadeLowValueColor(
        theDrawParam->SimpleIsoLineColorShadeLowValueColor());
    SimpleIsoline::SimpleIsoLineColorShadeMidValueColor(
        theDrawParam->SimpleIsoLineColorShadeMidValueColor());
    SimpleIsoline::SimpleIsoLineColorShadeHighValueColor(
        theDrawParam->SimpleIsoLineColorShadeHighValueColor());
    SimpleIsoline::SimpleIsoLineColorShadeClassCount(
        theDrawParam->SimpleIsoLineColorShadeClassCount());
    CustomIsoline::SetSpecialIsoLineValues(theDrawParam->SpecialIsoLineValues());
    CustomIsoline::SetSpecialIsoLineLabelHeight(theDrawParam->SpecialIsoLineLabelHeight());
    CustomIsoline::SetSpecialIsoLineWidth(theDrawParam->SpecialIsoLineWidth());
    CustomIsoline::SetSpecialIsoLineStyle(theDrawParam->SpecialIsoLineStyle());
    CustomIsoline::SetSpecialIsoLineColorIndexies(theDrawParam->SpecialIsoLineColorIndexies());
    CustomIsoline::SpecialIsoLineShowLabelBox(theDrawParam->SpecialIsoLineShowLabelBox());
    GeneralVisualization::DrawOnlyOverMask(theDrawParam->DrawOnlyOverMask());
    GeneralVisualization::UseCustomColorContouring(theDrawParam->UseCustomColorContouring());
    itsSpecialColorContouringValues = theDrawParam->SpecialColorContouringValues();
    itsSpecialColorContouringColorIndexies = theDrawParam->SpecialColorContouringColorIndexies();
    SimpleColorContour::ColorContouringColorShadeLowValue(
        theDrawParam->ColorContouringColorShadeLowValue());
    SimpleColorContour::ColorContouringColorShadeMidValue(
        theDrawParam->ColorContouringColorShadeMidValue());
    SimpleColorContour::ColorContouringColorShadeHighValue(
        theDrawParam->ColorContouringColorShadeHighValue());
    SimpleColorContour::ColorContouringColorShadeHigh2Value(
        theDrawParam->ColorContouringColorShadeHigh2Value());
    SimpleColorContour::ColorContouringColorShadeLowValueColor(
        theDrawParam->ColorContouringColorShadeLowValueColor());
    SimpleColorContour::ColorContouringColorShadeMidValueColor(
        theDrawParam->ColorContouringColorShadeMidValueColor());
    SimpleColorContour::ColorContouringColorShadeHighValueColor(
        theDrawParam->ColorContouringColorShadeHighValueColor());
    SimpleColorContour::ColorContouringColorShadeHigh2ValueColor(
        theDrawParam->ColorContouringColorShadeHigh2ValueColor());
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
    GeneralVisualization::IsoLineLabelDigitCount(theDrawParam->IsoLineLabelDigitCount());
    //***********************************************
    //********** 'versio 2' parametreja *************
    //***********************************************

    //***********************************************
    //********** 'versio 3' parametreja *************
    //***********************************************
    SimpleIsoline::ContourLabelBoxFillColor(theDrawParam->ContourLabelBoxFillColor());
    fUseContourGabWithCustomContours = theDrawParam->fUseContourGabWithCustomContours;
    SimpleColorContour::ContourGab(theDrawParam->ContourGab());
    SimpleColorContour::ContourColor(theDrawParam->ContourColor());
    itsContourTextColor = theDrawParam->itsContourTextColor;
    fUseContourFeathering = theDrawParam->fUseContourFeathering;
    GeneralVisualization::UseSimpleContourDefinitions(theDrawParam->UseSimpleContourDefinitions());
    itsSimpleContourZeroValue = theDrawParam->itsSimpleContourZeroValue;
    itsSimpleContourLabelHeight = theDrawParam->itsSimpleContourLabelHeight;
    SimpleColorContour::SimpleContourWidth(theDrawParam->SimpleContourWidth());
    SimpleColorContour::SimpleContourLineStyle(theDrawParam->SimpleContourLineStyle());
    SimpleIsoline::SimpleIsoLineColorShadeHigh2Value(
        theDrawParam->SimpleIsoLineColorShadeHigh2Value());
    SimpleIsoline::SimpleIsoLineColorShadeHigh2ValueColor(
        theDrawParam->SimpleIsoLineColorShadeHigh2ValueColor());
    CustomIsoline::SetSpecialContourValues(theDrawParam->SpecialContourValues());
    CustomIsoline::SetSpecialContourLabelHeight(theDrawParam->SpecialContourLabelHeight());
    CustomIsoline::SetSpecialcontourWidth(theDrawParam->SpecialcontourWidth());
    CustomIsoline::SetSpecialContourStyle(theDrawParam->SpecialContourStyle());
    CustomIsoline::SetSpecialContourColorIndexies(theDrawParam->SpecialContourColorIndexies());
    GeneralVisualization::UseCustomIsoLineing(theDrawParam->UseCustomIsoLineing());
    itsContourLabelDigitCount = theDrawParam->itsContourLabelDigitCount;
    //***********************************************
    //********** 'versio 3' parametreja *************
    //***********************************************
    GeneralVisualization::Alpha(theDrawParam->Alpha());
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
    std::cerr << "Cannot open the file " << theFilename << "\n";
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
  file << static_cast<int>(GeneralVisualization::ViewType()) << endl;
  file << "'UseIsoLineGabWithCustomContours'" << endl;  // selittävä teksti
  file << static_cast<int>(fUseIsoLineGabWithCustomContours) << endl;
  file << "'IsoLineGab'" << endl;  // selittävä teksti
  file << SimpleIsoline::IsoLineGab() << endl;
  file << "'IsolineColor'" << endl;  // selittävä teksti
  file << SimpleIsoline::IsolineColor() << endl;
  file << "'IsolineTextColor'" << endl;  // selittävä teksti
  file << SimpleIsoline::IsolineTextColor() << endl;
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
  file << SimpleIsoline::IsolineLabelBoxFillColor() << endl;

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
    file << GeneralVisualization::GridDataPresentationStyle() << endl;
    file << GeneralVisualization::UseIsoLineFeathering() << endl;
    file << fIsoLineLabelsOverLapping << endl;
    file << fShowColorLegend << endl;
    file << GeneralVisualization::UseSimpleIsoLineDefinitions() << endl;
    file << GeneralVisualization::UseSeparatorLinesBetweenColorContourClasses() << endl;
    file << SimpleIsoline::SimpleIsoLineGap() << endl;
    file << itsSimpleIsoLineZeroValue << endl;
    file << SimpleIsoline::SimpleIsoLineLabelHeight() << endl;
    file << SimpleIsoline::ShowSimpleIsoLineLabelBox() << endl;
    file << SimpleIsoline::SimpleIsoLineWidth() << endl;
    file << SimpleIsoline::SimpleIsoLineLineStyle() << endl;
    file << GeneralVisualization::IsoLineSplineSmoothingFactor() << endl;
    file << SimpleIsoline::UseSingleColorsWithSimpleIsoLines() << endl;
    file << SimpleIsoline::SimpleIsoLineColorShadeLowValue() << endl;
    file << SimpleIsoline::SimpleIsoLineColorShadeMidValue() << endl;
    file << SimpleIsoline::SimpleIsoLineColorShadeHighValue() << endl;
    file << SimpleIsoline::SimpleIsoLineColorShadeLowValueColor() << endl;
    file << SimpleIsoline::SimpleIsoLineColorShadeMidValueColor() << endl;
    file << SimpleIsoline::SimpleIsoLineColorShadeHighValueColor() << endl;
    file << SimpleIsoline::SimpleIsoLineColorShadeClassCount() << endl;

    size_t i = 0;
    size_t size = CustomIsoline::SpecialIsoLineValues().size();
    file << size << endl;
    for (i = 0; i < size; i++)
      file << CustomIsoline::SpecialIsoLineValues()[i] << " ";
    file << endl;

    size = CustomIsoline::SpecialIsoLineLabelHeight().size();
    file << size << endl;
    for (i = 0; i < size; i++)
      file << CustomIsoline::SpecialIsoLineLabelHeight()[i] << " ";
    file << endl;

    size = CustomIsoline::SpecialIsoLineWidth().size();
    file << size << endl;
    for (i = 0; i < size; i++)
      file << CustomIsoline::SpecialIsoLineWidth()[i] << " ";
    file << endl;

    size = CustomIsoline::SpecialIsoLineStyle().size();
    file << size << endl;
    for (i = 0; i < size; i++)
      file << CustomIsoline::SpecialIsoLineStyle()[i] << " ";
    file << endl;

    size = CustomIsoline::SpecialIsoLineColorIndexies().size();
    file << size << endl;
    for (i = 0; i < size; i++)
      file << CustomIsoline::SpecialIsoLineColorIndexies()[i] << " ";
    file << endl;

    size = CustomIsoline::SpecialIsoLineShowLabelBox().size();
    file << size << endl;
    for (i = 0; i < size; i++)
      file << CustomIsoline::SpecialIsoLineShowLabelBox()[i] << " ";
    file << endl;

    file << GeneralVisualization::DrawOnlyOverMask() << endl;
    file << GeneralVisualization::UseCustomColorContouring() << endl;

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

    file << SimpleColorContour::ColorContouringColorShadeLowValue() << endl;
    file << SimpleColorContour::ColorContouringColorShadeMidValue() << endl;
    file << SimpleColorContour::ColorContouringColorShadeHighValue() << endl;
    file << SimpleColorContour::ColorContouringColorShadeLowValueColor() << endl;
    file << SimpleColorContour::ColorContouringColorShadeMidValueColor() << endl;
    file << SimpleColorContour::ColorContouringColorShadeHighValueColor() << endl;
    file << SimpleColorContour::ColorContouringColorShadeHigh2Value() << endl;
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
    file << SimpleColorContour::ColorContouringColorShadeHigh2ValueColor() << endl;
    file << GeneralVisualization::IsoLineLabelDigitCount() << endl;
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
    file << SimpleIsoline::ContourLabelBoxFillColor() << endl;
    file << fUseContourGabWithCustomContours << " " << SimpleColorContour::ContourGab() << endl;
    file << SimpleColorContour::ContourColor() << endl;
    file << itsContourTextColor << endl;
    file << fUseContourFeathering << " " << GeneralVisualization::UseSimpleContourDefinitions()
         << " " << itsSimpleContourZeroValue << " " << itsSimpleContourLabelHeight << endl;
    file << fShowSimpleContourLabelBox << " " << SimpleColorContour::SimpleContourWidth() << " "
         << SimpleColorContour::SimpleContourLineStyle() << " "
         << SimpleIsoline::SimpleIsoLineColorShadeHigh2Value() << endl;
    file << SimpleIsoline::SimpleIsoLineColorShadeHigh2ValueColor() << endl;

    NFmiDataStoringHelpers::WriteContainer(
        CustomIsoline::SpecialContourValues(), file, std::string(" "));
    file << endl;
    NFmiDataStoringHelpers::WriteContainer(
        CustomIsoline::SpecialContourLabelHeight(), file, std::string(" "));
    file << endl;
    NFmiDataStoringHelpers::WriteContainer(
        CustomIsoline::SpecialcontourWidth(), file, std::string(" "));
    file << endl;
    NFmiDataStoringHelpers::WriteContainer(
        CustomIsoline::SpecialContourStyle(), file, std::string(" "));
    file << endl;
    NFmiDataStoringHelpers::WriteContainer(
        CustomIsoline::SpecialContourColorIndexies(), file, std::string(" "));
    file << endl;

    file << GeneralVisualization::UseCustomIsoLineing() << " " << itsContourLabelDigitCount << endl;

    NFmiDataStoringHelpers::NFmiExtraDataStorage extraData;  // lopuksi vielä mahdollinen extra data
    // Kun tulee uusia muuttujia, tee tähän extradatan täyttöä, jotta se saadaan talteen tiedopstoon
    // siten että
    // edelliset versiot eivät mene solmuun vaikka on tullut uutta dataa.
    extraData.Add(
        GeneralVisualization::Alpha());  // alpha on siis 1. uusista double-extra-parametreista
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
  bool tmpBool = false;
  float tmpFloat = 0.0;
  float tmpFloat2 = 0.0;
  double tmpDouble = 0.0;
  NFmiColor tmpColor;
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
      GeneralVisualization::ViewType(NFmiMetEditorTypes::View(number));

      file >> temp;  // luetaan nimike pois
      file >> number;
      fUseIsoLineGabWithCustomContours = number != 0;
      file >> temp;  // luetaan nimike pois
      file >> tmpDouble;
      SimpleIsoline::IsoLineGab(tmpDouble);
      file >> temp;  // luetaan nimike pois
      file >> tmpColor;
      SimpleIsoline::IsolineColor(tmpColor);
      file >> temp;  // luetaan nimike pois
      file >> tmpColor;
      SimpleIsoline::IsolineTextColor(tmpColor);
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
      file >> tmpColor;
      SimpleIsoline::IsolineLabelBoxFillColor(tmpColor);

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
      file >> temp;     // luetaan nimike pois
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
        file >> number;
        GeneralVisualization::GridDataPresentationStyle(number);
        file >> tmpBool;
        GeneralVisualization::UseIsoLineFeathering(tmpBool);
        file >> fIsoLineLabelsOverLapping;
        file >> fShowColorLegend;
        file >> tmpBool;
        GeneralVisualization::UseSimpleIsoLineDefinitions(tmpBool);
        file >> tmpBool;
        GeneralVisualization::UseSeparatorLinesBetweenColorContourClasses(tmpBool);
        file >> tmpFloat;
        SimpleIsoline::SimpleIsoLineGap(tmpFloat);
        file >> itsSimpleIsoLineZeroValue;
        file >> tmpFloat;
        SimpleIsoline::SimpleIsoLineLabelHeight(tmpFloat);
        file >> tmpBool;
        SimpleIsoline::ShowSimpleIsoLineLabelBox(tmpBool);
        file >> tmpFloat;
        SimpleIsoline::SimpleIsoLineWidth(tmpFloat);
        file >> number;
        SimpleIsoline::SimpleIsoLineLineStyle(number);
        file >> tmpFloat;
        GeneralVisualization::IsoLineSplineSmoothingFactor(tmpFloat);
        file >> tmpBool;
        SimpleIsoline::UseSingleColorsWithSimpleIsoLines(tmpBool);
        file >> tmpFloat;
        SimpleIsoline::SimpleIsoLineColorShadeLowValue(tmpFloat);
        file >> tmpFloat;
        SimpleIsoline::SimpleIsoLineColorShadeMidValue(tmpFloat);
        file >> tmpFloat;
        SimpleIsoline::SimpleIsoLineColorShadeHighValue(tmpFloat);
        file >> tmpColor;
        SimpleIsoline::SimpleIsoLineColorShadeLowValueColor(tmpColor);
        file >> tmpColor;
        SimpleIsoline::SimpleIsoLineColorShadeMidValueColor(tmpColor);
        file >> tmpColor;
        SimpleIsoline::SimpleIsoLineColorShadeHighValueColor(tmpColor);
        file >> number;
        SimpleIsoline::SimpleIsoLineColorShadeClassCount(number);

        if (!file)
          return file;
        int i = 0;
        int size;
        file >> size;
        checkedVector<float> tmpFloatVector(size);
        for (i = 0; i < size; i++)
          file >> tmpFloatVector[i];
        CustomIsoline::SetSpecialIsoLineValues(tmpFloatVector);

        file >> size;
        tmpFloatVector.resize(size);
        for (i = 0; i < size; i++)
          file >> tmpFloatVector[i];
        CustomIsoline::SetSpecialIsoLineLabelHeight(tmpFloatVector);

        file >> size;
        tmpFloatVector.resize(size);
        for (i = 0; i < size; i++)
          file >> tmpFloatVector[i];
        CustomIsoline::SetSpecialIsoLineWidth(tmpFloatVector);

        file >> size;
        checkedVector<int> tmpIntVector(size);
        for (i = 0; i < size; i++)
          file >> tmpIntVector[i];
        CustomIsoline::SetSpecialIsoLineStyle(tmpIntVector);

        file >> size;
        tmpIntVector.resize(size);
        for (i = 0; i < size; i++)
          file >> tmpIntVector[i];
        CustomIsoline::SetSpecialIsoLineColorIndexies(tmpIntVector);

        file >> size;
        checkedVector<bool> tmpBoolVector(size);
        for (i = 0; i < size; i++)
        {
          // Mika: g++ 2.95 ei salli suoraa sijoitusta
          bool foobar;
          file >> foobar;
          tmpBoolVector[i] = foobar;
        }
        CustomIsoline::SpecialIsoLineShowLabelBox(tmpBoolVector);

        if (!file)
          return file;
        file >> tmpBool;
        GeneralVisualization::DrawOnlyOverMask(tmpBool);
        file >> tmpBool;
        GeneralVisualization::UseCustomColorContouring(tmpBool);

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

        file >> tmpFloat;
        SimpleColorContour::ColorContouringColorShadeLowValue(tmpFloat);
        file >> tmpFloat;
        SimpleColorContour::ColorContouringColorShadeMidValue(tmpFloat);
        file >> tmpFloat;
        SimpleColorContour::ColorContouringColorShadeHighValue(tmpFloat);
        file >> tmpColor;
        SimpleColorContour::ColorContouringColorShadeLowValueColor(tmpColor);
        file >> tmpColor;
        SimpleColorContour::ColorContouringColorShadeMidValueColor(tmpColor);
        file >> tmpColor;
        SimpleColorContour::ColorContouringColorShadeHighValueColor(tmpColor);
        file >> tmpFloat;
        SimpleColorContour::ColorContouringColorShadeHigh2Value(tmpFloat);
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
        file >> tmpColor;
        SimpleColorContour::ColorContouringColorShadeHigh2ValueColor(tmpColor);
        file >> number;
        GeneralVisualization::IsoLineLabelDigitCount(number);
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
        file >> tmpColor;
        SimpleIsoline::ContourLabelBoxFillColor(tmpColor);
        file >> fUseContourGabWithCustomContours >> tmpDouble;
        SimpleColorContour::ContourGab(tmpDouble);
        file >> tmpColor;
        SimpleColorContour::ContourColor(tmpColor);
        file >> itsContourTextColor;
        bool tmpBool;
        file >> fUseContourFeathering >> tmpBool >> itsSimpleContourZeroValue >>
            itsSimpleContourLabelHeight;
        GeneralVisualization::UseSimpleContourDefinitions(tmpBool);
        file >> fShowSimpleContourLabelBox >> tmpFloat >> number >> tmpFloat2;
        SimpleColorContour::SimpleContourWidth(tmpFloat);
        SimpleColorContour::SimpleContourLineStyle(number);
        SimpleIsoline::SimpleIsoLineColorShadeHigh2Value(tmpFloat2);
        file >> tmpColor;
        SimpleIsoline::SimpleIsoLineColorShadeHigh2ValueColor(tmpColor);

        checkedVector<float> tmpFloatVector;
        NFmiDataStoringHelpers::ReadContainer(tmpFloatVector, file);
        CustomIsoline::SetSpecialContourValues(tmpFloatVector);

        NFmiDataStoringHelpers::ReadContainer(tmpFloatVector, file);
        CustomIsoline::SetSpecialContourLabelHeight(tmpFloatVector);

        NFmiDataStoringHelpers::ReadContainer(tmpFloatVector, file);
        CustomIsoline::SetSpecialcontourWidth(tmpFloatVector);

        checkedVector<int> tmpIntVector;
        NFmiDataStoringHelpers::ReadContainer(tmpIntVector, file);
        CustomIsoline::SetSpecialContourStyle(tmpIntVector);

        NFmiDataStoringHelpers::ReadContainer(tmpIntVector, file);
        CustomIsoline::SetSpecialContourColorIndexies(tmpIntVector);

        file >> tmpBool >> itsContourLabelDigitCount;
        GeneralVisualization::UseCustomIsoLineing(tmpBool);

        if (file.fail())
          throw std::runtime_error("NFmiDrawParam::Write failed");

        file >> temp;  // luetaan 'possible_extra_data' pois
        NFmiDataStoringHelpers::NFmiExtraDataStorage
            extraData;  // lopuksi vielä mahdollinen extra data
        file >> extraData;
        // Tässä sitten otetaaan extradatasta talteen uudet muuttujat, mitä on mahdollisesti tullut
        // eli jos uusia muutujia tai arvoja, käsittele tässä.
        GeneralVisualization::Alpha(
            100.f);  // tämä on siis default arvo alphalle (täysin läpinäkyvä)
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
        SimpleColorContour::ColorContouringColorShadeHigh2Value(
            SimpleColorContour::ColorContouringColorShadeHighValue());
        SimpleColorContour::ColorContouringColorShadeHigh2ValueColor(
            SimpleColorContour::ColorContouringColorShadeHighValueColor());

        SimpleIsoline::ContourLabelBoxFillColor(SimpleIsoline::IsolineLabelBoxFillColor());
        fUseContourGabWithCustomContours = false;
        SimpleColorContour::ContourGab(SimpleIsoline::IsoLineGab());
        SimpleColorContour::ContourColor(SimpleIsoline::IsolineColor());
        itsContourTextColor = SimpleIsoline::IsolineTextColor();
        fUseContourFeathering = GeneralVisualization::UseIsoLineFeathering();
        GeneralVisualization::UseSimpleContourDefinitions(
            GeneralVisualization::UseSimpleIsoLineDefinitions());
        itsSimpleContourZeroValue = itsSimpleIsoLineZeroValue;
        itsSimpleContourLabelHeight = SimpleIsoline::SimpleIsoLineLabelHeight();
        fShowSimpleContourLabelBox = SimpleIsoline::ShowSimpleIsoLineLabelBox();
        SimpleColorContour::SimpleContourWidth(SimpleIsoline::SimpleIsoLineWidth());
        SimpleColorContour::SimpleContourLineStyle(SimpleIsoline::SimpleIsoLineLineStyle());
        SimpleIsoline::SimpleIsoLineColorShadeHigh2Value(
            SimpleIsoline::SimpleIsoLineColorShadeHighValue());
        SimpleIsoline::SimpleIsoLineColorShadeHigh2ValueColor(
            SimpleIsoline::SimpleIsoLineColorShadeHighValueColor());

        CustomIsoline::SetSpecialContourValues(CustomIsoline::SpecialIsoLineValues());
        CustomIsoline::SetSpecialContourLabelHeight(CustomIsoline::SpecialIsoLineLabelHeight());
        CustomIsoline::SetSpecialcontourWidth(CustomIsoline::SpecialIsoLineWidth());
        CustomIsoline::SetSpecialContourStyle(CustomIsoline::SpecialIsoLineStyle());
        CustomIsoline::SetSpecialContourColorIndexies(CustomIsoline::SpecialIsoLineColorIndexies());

        GeneralVisualization::UseCustomIsoLineing(GeneralVisualization::UseCustomColorContouring());
        itsContourLabelDigitCount = GeneralVisualization::IsoLineLabelDigitCount();
        GeneralVisualization::Alpha(
            100.f);  // tämä on siis default arvo alphalle (täysin läpinäkyvä)
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
