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
//							vertailtaessa samantyyppistä dataa päällekkäin)
//
//**********************************************************
#include "NFmiDrawParam.h"

float NFmiDrawParam::itsFileVersionNumber=2.0;

//--------------------------------------------------------
// NFmiDrawParam(void)
//--------------------------------------------------------
NFmiDrawParam::NFmiDrawParam()
: itsParameter(NFmiParam(kFmiLastParameter))
, itsLevel()
, itsParameterAbbreviation("?")
, itsPriority(1)
, itsInitFileName("")
, itsViewType							 (NFmiMetEditorTypes::kFmiIsoLineView)
, itsStationDataViewType				 (NFmiMetEditorTypes::kFmiTextView)
, itsFrameColor						     (NFmiColor(0.,0.,0.)) // musta
, itsFillColor						     (NFmiColor(0.,0.,0.)) // musta
, itsIsolineLabelBoxFillColor		     (NFmiColor(1.,1.,0.)) // keltainen
, itsRelativeSize                        (NFmiPoint(1,1))
, itsRelativePositionOffset              (NFmiPoint(0,0))
, itsOnlyOneSymbolRelativeSize           (NFmiPoint(1,1))
, itsOnlyOneSymbolRelativePositionOffset (NFmiPoint(0,0))
, fShowStationMarker(false)
, itsIsoLineGab						     (1.)
, itsModifyingStep                       (1.)
, fModifyingUnit						 (true)
, itsTimeSerialModifyingLimit(10)
, itsIsolineColor						(NFmiColor(0.,0.,0.))
, itsIsolineTextColor					(NFmiColor(0.,0.,0.))
, itsSecondaryIsolineColor(0.6f,0.6f,0.6f)		// tehdään secondary väreistä aina harmaita
, itsSecondaryIsolineTextColor(0.6f,0.6f,0.6f)	// tehdään secondary väreistä aina harmaita
, fUseSecondaryColors(false)
, itsAbsoluteMinValue(-1000000000)
, itsAbsoluteMaxValue(1000000000)
, itsTimeSeriesScaleMin(0)
, itsTimeSeriesScaleMax(100)
, itsPossibleViewTypeCount(2)
, fShowNumbers(true)
, fShowMasks(false) // tämä on turha
, fShowColors(false)
, fShowColoredNumbers(false)
, fZeroColorMean(false)
//***********************************************
//********** 'versio 2' parametreja *************
//***********************************************
, itsStationSymbolColorShadeLowValue(0)
, itsStationSymbolColorShadeMidValue(50)
, itsStationSymbolColorShadeHighValue(100)
//, itsStationSymbolColorShadeLowValueColor(0,0,0)
, itsStationSymbolColorShadeLowValueColor(0,0,1)
//, itsStationSymbolColorShadeMidValueColor(0.4f,0.4f,0.4f)
, itsStationSymbolColorShadeMidValueColor(0,1,0)
//, itsStationSymbolColorShadeHighValueColor(1,1,1)
, itsStationSymbolColorShadeHighValueColor(0,1,0)
, itsStationSymbolColorShadeClassCount(9)
, fUseSymbolsInTextMode(false)
, itsUsedSymbolListIndex(0)
, itsSymbolIndexingMapListIndex(-1)
, itsGridDataPresentationStyle(2)
, fUseIsoLineFeathering(false)
, fIsoLineLabelsOverLapping(true)
, fShowColorLegend(false)
, fUseSimpleIsoLineDefinitions(true)
, fUseSeparatorLinesBetweenColorContourClasses(true)
, itsSimpleIsoLineGap(1)
, itsSimpleIsoLineZeroValue(0)
, itsSimpleIsoLineLabelHeight(4)
, fShowSimpleIsoLineLabelBox(false)
, itsSimpleIsoLineWidth(0.2f)
, itsSimpleIsoLineLineStyle(0)
, itsIsoLineSplineSmoothingFactor(6)
, fUseSingleColorsWithSimpleIsoLines(true)
, itsSimpleIsoLineColorShadeLowValue(0)
, itsSimpleIsoLineColorShadeMidValue(50)
, itsSimpleIsoLineColorShadeHighValue(100)
, itsSimpleIsoLineColorShadeLowValueColor(0,0,1)
, itsSimpleIsoLineColorShadeMidValueColor(0,1,0)
, itsSimpleIsoLineColorShadeHighValueColor(0,1,0)
, itsSimpleIsoLineColorShadeClassCount(9)
, itsSpecialIsoLineValues()
, itsSpecialIsoLineLabelHeight()
, itsSpecialIsoLineWidth()
, itsSpecialIsoLineStyle()
, itsSpecialIsoLineColorIndexies()
, itsSpecialIsoLineShowLabelBox()
, fUseDefaultRegioning(false)
, fUseCustomColorContouring(false)
, itsSpecialColorContouringValues()
, itsSpecialColorContouringColorIndexies()
, itsColorContouringColorShadeLowValue(0)
, itsColorContouringColorShadeMidValue(50)
, itsColorContouringColorShadeHighValue(100)
, itsColorContouringColorShadeLowValueColor(0,0,1)
, itsColorContouringColorShadeMidValueColor(0,1,0)
, itsColorContouringColorShadeHighValueColor(0,1,0)
, itsColorContouringColorShadeClassCount(9)
, fUseWithIsoLineHatch1(false)
, fDrawIsoLineHatchWithBorders1(false)
, itsIsoLineHatchLowValue1(0)
, itsIsoLineHatchHighValue1(10)
, itsIsoLineHatchType1(1)
, itsIsoLineHatchColor1(0,0,0)
, itsIsoLineHatchBorderColor1(0,0,0)
, fUseWithIsoLineHatch2(false)
, fDrawIsoLineHatchWithBorders2(false)
, itsIsoLineHatchLowValue2(50)
, itsIsoLineHatchHighValue2(60)
, itsIsoLineHatchType2(2)
, itsIsoLineHatchColor2(0.5f,0.5f,0.5f)
, itsIsoLineHatchBorderColor2(0.5f,0.5f,0.5f)
// protected osa
, itsIsoLineLabelDigitCount(0)
, itsInitFileVersionNumber(itsFileVersionNumber)
, fHidden(false)
, fEditedParam(false)
, fEditableParam(true)
//, itsInfo(0)
, itsUnit								("?")
, fActive(false)
, fShowDifference(false)
, fShowDifferenceToOriginalData(false)
//, itsMetEditorCoordinatorMapOptions(0)
, itsDataType(NFmiInfoData::kNoDataType)

//***********************************************
//********** 'versio 2' parametreja *************
//***********************************************
, fViewMacroDrawParam(false)
, fBorrowedParam(false)
{
	itsPossibleViewTypeList[0] = NFmiMetEditorTypes::kFmiTextView;
	itsPossibleViewTypeList[1] = NFmiMetEditorTypes::kFmiIsoLineView;
}

//-------------------------------------------------------
// NFmiDrawParam
//-------------------------------------------------------
NFmiDrawParam::NFmiDrawParam(const NFmiDataIdent& theParam
							,const NFmiLevel &theLevel
							,int thePriority
							,NFmiInfoData::Type theDataType)
: itsParameter(theParam)
, itsLevel(theLevel)
, itsParameterAbbreviation("?")
, itsPriority(thePriority)
, itsInitFileName("")
, itsViewType							 (NFmiMetEditorTypes::kFmiIsoLineView)
, itsStationDataViewType				 (NFmiMetEditorTypes::kFmiTextView)
, itsFrameColor						     (NFmiColor(0.,0.,0.)) // musta
, itsFillColor						     (NFmiColor(0.,0.,0.)) // musta
, itsIsolineLabelBoxFillColor		     (NFmiColor(1.,1.,0.)) // keltainen
, itsRelativeSize                        (NFmiPoint(1,1))
, itsRelativePositionOffset              (NFmiPoint(0,0))
, itsOnlyOneSymbolRelativeSize           (NFmiPoint(1,1))
, itsOnlyOneSymbolRelativePositionOffset (NFmiPoint(0,0))
, fShowStationMarker(false)
, itsIsoLineGab						     (1.)
, itsModifyingStep                       (1.)
, fModifyingUnit						 (true)
, itsTimeSerialModifyingLimit(10)
, itsIsolineColor						(NFmiColor(0.,0.,0.))
, itsIsolineTextColor					(NFmiColor(0.,0.,0.))
, itsSecondaryIsolineColor(0.6f,0.6f,0.6f)		// tehdään secondary väreistä aina harmaita
, itsSecondaryIsolineTextColor(0.6f,0.6f,0.6f)	// tehdään secondary väreistä aina harmaita
, fUseSecondaryColors(false)
, itsAbsoluteMinValue(-1000000000)
, itsAbsoluteMaxValue(1000000000)
, itsTimeSeriesScaleMin(0)
, itsTimeSeriesScaleMax(100)
, itsPossibleViewTypeCount(2)
, fShowNumbers(true)
, fShowMasks(false) // tämä on turha
, fShowColors(false)
, fShowColoredNumbers(false)
, fZeroColorMean(false)
//***********************************************
//********** 'versio 2' parametreja *************
//***********************************************
, itsStationSymbolColorShadeLowValue(0)
, itsStationSymbolColorShadeMidValue(50)
, itsStationSymbolColorShadeHighValue(100)
, itsStationSymbolColorShadeLowValueColor(0,0,1)
, itsStationSymbolColorShadeMidValueColor(0,1,0)
, itsStationSymbolColorShadeHighValueColor(0,1,0)
, itsStationSymbolColorShadeClassCount(9)
, fUseSymbolsInTextMode(false)
, itsUsedSymbolListIndex(0)
, itsSymbolIndexingMapListIndex(-1)
, itsGridDataPresentationStyle(2)
, fUseIsoLineFeathering(false)
, fIsoLineLabelsOverLapping(true)
, fShowColorLegend(false)
, fUseSimpleIsoLineDefinitions(true)
, fUseSeparatorLinesBetweenColorContourClasses(true)
, itsSimpleIsoLineGap(1)
, itsSimpleIsoLineZeroValue(0)
, itsSimpleIsoLineLabelHeight(4)
, fShowSimpleIsoLineLabelBox(false)
, itsSimpleIsoLineWidth(0.2f)
, itsSimpleIsoLineLineStyle(0)
, itsIsoLineSplineSmoothingFactor(6)
, fUseSingleColorsWithSimpleIsoLines(true)
, itsSimpleIsoLineColorShadeLowValue(0)
, itsSimpleIsoLineColorShadeMidValue(50)
, itsSimpleIsoLineColorShadeHighValue(100)
, itsSimpleIsoLineColorShadeLowValueColor(0,0,1)
, itsSimpleIsoLineColorShadeMidValueColor(0,1,0)
, itsSimpleIsoLineColorShadeHighValueColor(0,1,0)
, itsSimpleIsoLineColorShadeClassCount(9)
, itsSpecialIsoLineValues()
, itsSpecialIsoLineLabelHeight()
, itsSpecialIsoLineWidth()
, itsSpecialIsoLineStyle()
, itsSpecialIsoLineColorIndexies()
, itsSpecialIsoLineShowLabelBox()
, fUseDefaultRegioning(false)
, fUseCustomColorContouring(false)
, itsSpecialColorContouringValues()
, itsSpecialColorContouringColorIndexies()
, itsColorContouringColorShadeLowValue(0)
, itsColorContouringColorShadeMidValue(50)
, itsColorContouringColorShadeHighValue(100)
, itsColorContouringColorShadeLowValueColor(0,0,1)
, itsColorContouringColorShadeMidValueColor(0,1,0)
, itsColorContouringColorShadeHighValueColor(0,1,0)
, itsColorContouringColorShadeClassCount(9)
, fUseWithIsoLineHatch1(false)
, fDrawIsoLineHatchWithBorders1(false)
, itsIsoLineHatchLowValue1(0)
, itsIsoLineHatchHighValue1(10)
, itsIsoLineHatchType1(1)
, itsIsoLineHatchColor1(0,0,0)
, itsIsoLineHatchBorderColor1(0,0,0)
, fUseWithIsoLineHatch2(false)
, fDrawIsoLineHatchWithBorders2(false)
, itsIsoLineHatchLowValue2(50)
, itsIsoLineHatchHighValue2(60)
, itsIsoLineHatchType2(2)
, itsIsoLineHatchColor2(0.5f,0.5f,0.5f)
, itsIsoLineHatchBorderColor2(0.5f,0.5f,0.5f)
, itsIsoLineLabelDigitCount(0)
//***********************************************
//********** 'versio 2' parametreja *************
//***********************************************
, itsInitFileVersionNumber(itsFileVersionNumber)
, fHidden(false)
, fEditedParam(false)
, fEditableParam(true)
, itsUnit								("?")
, fActive(false)
, fShowDifference(false)
, fShowDifferenceToOriginalData(false)
//, itsMetEditorCoordinatorMapOptions(theMetEditorCoordinatorMapOptions ? new NFmiMetEditorCoordinatorMapOptions(*theMetEditorCoordinatorMapOptions) : 0)
, itsDataType(theDataType)
, fViewMacroDrawParam(false)
, fBorrowedParam(false)
{
	itsPossibleViewTypeList[0] = NFmiMetEditorTypes::kFmiTextView;
	itsPossibleViewTypeList[1] = NFmiMetEditorTypes::kFmiIsoLineView;
}

NFmiDrawParam::NFmiDrawParam(const NFmiDrawParam& other)
: itsParameter(other.itsParameter)
, itsLevel(other.itsLevel)
, itsParameterAbbreviation(other.itsParameterAbbreviation)
, itsPriority(other.itsPriority)
, itsInitFileName(other.itsInitFileName)
, itsViewType(other.itsViewType)
, itsStationDataViewType(other.itsStationDataViewType)
, itsFrameColor(other.itsFrameColor)
, itsFillColor(other.itsFillColor)
, itsIsolineLabelBoxFillColor(other.itsIsolineLabelBoxFillColor)
, itsRelativeSize(other.itsRelativeSize)
, itsRelativePositionOffset(other.itsRelativePositionOffset)
, itsOnlyOneSymbolRelativeSize(other.itsOnlyOneSymbolRelativeSize)
, itsOnlyOneSymbolRelativePositionOffset(other.itsOnlyOneSymbolRelativePositionOffset)
, fShowStationMarker(other.fShowStationMarker)
, itsIsoLineGab(other.itsIsoLineGab)
, itsModifyingStep(other.itsModifyingStep)
, fModifyingUnit(other.fModifyingUnit)
, itsTimeSerialModifyingLimit(other.itsTimeSerialModifyingLimit)
, itsIsolineColor(other.itsIsolineColor)
, itsIsolineTextColor(other.itsIsolineTextColor)
, itsSecondaryIsolineColor(other.itsSecondaryIsolineColor)
, itsSecondaryIsolineTextColor(other.itsSecondaryIsolineTextColor)
, fUseSecondaryColors(other.fUseSecondaryColors)
, itsAbsoluteMinValue(other.itsAbsoluteMinValue)
, itsAbsoluteMaxValue(other.itsAbsoluteMaxValue)
, itsTimeSeriesScaleMin(other.itsTimeSeriesScaleMin)
, itsTimeSeriesScaleMax(other.itsTimeSeriesScaleMax)
, itsPossibleViewTypeCount(other.itsPossibleViewTypeCount)
, fShowNumbers(other.fShowNumbers)
, fShowMasks(other.fShowMasks)
, fShowColors(other.fShowColors)
, fShowColoredNumbers(other.fShowColoredNumbers)
, fZeroColorMean(other.fZeroColorMean)
//***********************************************
//********** 'versio 2' parametreja *************
//***********************************************
, itsStationSymbolColorShadeLowValue(other.itsStationSymbolColorShadeLowValue)
, itsStationSymbolColorShadeMidValue(other.itsStationSymbolColorShadeMidValue)
, itsStationSymbolColorShadeHighValue(other.itsStationSymbolColorShadeHighValue)
, itsStationSymbolColorShadeLowValueColor(other.itsStationSymbolColorShadeLowValueColor)
, itsStationSymbolColorShadeMidValueColor(other.itsStationSymbolColorShadeMidValueColor)
, itsStationSymbolColorShadeHighValueColor(other.itsStationSymbolColorShadeHighValueColor)
, itsStationSymbolColorShadeClassCount(other.itsStationSymbolColorShadeClassCount)
, fUseSymbolsInTextMode(other.fUseSymbolsInTextMode)
, itsUsedSymbolListIndex(other.itsUsedSymbolListIndex)
, itsSymbolIndexingMapListIndex(other.itsSymbolIndexingMapListIndex)
, itsGridDataPresentationStyle(other.itsGridDataPresentationStyle)
, fUseIsoLineFeathering(other.fUseIsoLineFeathering)
, fIsoLineLabelsOverLapping(other.fIsoLineLabelsOverLapping)
, fShowColorLegend(other.fShowColorLegend)
, fUseSimpleIsoLineDefinitions(other.fUseSimpleIsoLineDefinitions)
, fUseSeparatorLinesBetweenColorContourClasses(other.fUseSeparatorLinesBetweenColorContourClasses)
, itsSimpleIsoLineGap(other.itsSimpleIsoLineGap)
, itsSimpleIsoLineZeroValue(other.itsSimpleIsoLineZeroValue)
, itsSimpleIsoLineLabelHeight(other.itsSimpleIsoLineLabelHeight)
, fShowSimpleIsoLineLabelBox(other.fShowSimpleIsoLineLabelBox)
, itsSimpleIsoLineWidth(other.itsSimpleIsoLineWidth)
, itsSimpleIsoLineLineStyle(other.itsSimpleIsoLineLineStyle)
, itsIsoLineSplineSmoothingFactor(other.itsIsoLineSplineSmoothingFactor)
, fUseSingleColorsWithSimpleIsoLines(other.fUseSingleColorsWithSimpleIsoLines)
, itsSimpleIsoLineColorShadeLowValue(other.itsSimpleIsoLineColorShadeLowValue)
, itsSimpleIsoLineColorShadeMidValue(other.itsSimpleIsoLineColorShadeMidValue)
, itsSimpleIsoLineColorShadeHighValue(other.itsSimpleIsoLineColorShadeHighValue)
, itsSimpleIsoLineColorShadeLowValueColor(other.itsSimpleIsoLineColorShadeLowValueColor)
, itsSimpleIsoLineColorShadeMidValueColor(other.itsSimpleIsoLineColorShadeMidValueColor)
, itsSimpleIsoLineColorShadeHighValueColor(other.itsSimpleIsoLineColorShadeHighValueColor)
, itsSimpleIsoLineColorShadeClassCount(other.itsSimpleIsoLineColorShadeClassCount)
, itsSpecialIsoLineValues(other.itsSpecialIsoLineValues)
, itsSpecialIsoLineLabelHeight(other.itsSpecialIsoLineLabelHeight)
, itsSpecialIsoLineWidth(other.itsSpecialIsoLineWidth)
, itsSpecialIsoLineStyle(other.itsSpecialIsoLineStyle)
, itsSpecialIsoLineColorIndexies(other.itsSpecialIsoLineColorIndexies)
, itsSpecialIsoLineShowLabelBox(other.itsSpecialIsoLineShowLabelBox)
, fUseDefaultRegioning(other.fUseDefaultRegioning)
, fUseCustomColorContouring(other.fUseCustomColorContouring)
, itsSpecialColorContouringValues(other.itsSpecialColorContouringValues)
, itsSpecialColorContouringColorIndexies(other.itsSpecialColorContouringColorIndexies)
, itsColorContouringColorShadeLowValue(other.itsColorContouringColorShadeLowValue)
, itsColorContouringColorShadeMidValue(other.itsColorContouringColorShadeMidValue)
, itsColorContouringColorShadeHighValue(other.itsColorContouringColorShadeHighValue)
, itsColorContouringColorShadeLowValueColor(other.itsColorContouringColorShadeLowValueColor)
, itsColorContouringColorShadeMidValueColor(other.itsColorContouringColorShadeMidValueColor)
, itsColorContouringColorShadeHighValueColor(other.itsColorContouringColorShadeHighValueColor)
, itsColorContouringColorShadeClassCount(other.itsColorContouringColorShadeClassCount)
, fUseWithIsoLineHatch1(other.fUseWithIsoLineHatch1)
, fDrawIsoLineHatchWithBorders1(other.fDrawIsoLineHatchWithBorders1)
, itsIsoLineHatchLowValue1(other.itsIsoLineHatchLowValue1)
, itsIsoLineHatchHighValue1(other.itsIsoLineHatchHighValue1)
, itsIsoLineHatchType1(other.itsIsoLineHatchType1)
, itsIsoLineHatchColor1(other.itsIsoLineHatchColor1)
, itsIsoLineHatchBorderColor1(other.itsIsoLineHatchBorderColor1)
, fUseWithIsoLineHatch2(other.fUseWithIsoLineHatch2)
, fDrawIsoLineHatchWithBorders2(other.fDrawIsoLineHatchWithBorders2)
, itsIsoLineHatchLowValue2(other.itsIsoLineHatchLowValue2)
, itsIsoLineHatchHighValue2(other.itsIsoLineHatchHighValue2)
, itsIsoLineHatchType2(other.itsIsoLineHatchType2)
, itsIsoLineHatchColor2(other.itsIsoLineHatchColor2)
, itsIsoLineHatchBorderColor2(other.itsIsoLineHatchBorderColor2)
, itsIsoLineLabelDigitCount(other.itsIsoLineLabelDigitCount)
//***********************************************
//********** 'versio 2' parametreja *************
//***********************************************
, itsInitFileVersionNumber(other.itsInitFileVersionNumber)
, fHidden(other.fHidden)
, fEditedParam(other.fEditedParam)
, fEditableParam(other.fEditableParam)
, itsUnit(other.itsUnit)
, fActive(other.fActive)
, fShowDifference(other.fShowDifference)
, fShowDifferenceToOriginalData(other.fShowDifferenceToOriginalData)
//, itsMetEditorCoordinatorMapOptions(theMetEditorCoordinatorMapOptions ? new NFmiMetEditorCoordinatorMapOptions(*theMetEditorCoordinatorMapOptions) : 0)
, itsDataType(other.itsDataType)
, fViewMacroDrawParam(other.fViewMacroDrawParam)
, fBorrowedParam(other.fBorrowedParam)
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
void NFmiDrawParam::Init(const NFmiDrawParam* theDrawParam, bool fInitOnlyDrawingOptions)
{
	if(theDrawParam)
	{
		if(fInitOnlyDrawingOptions == false)
		{
			itsInitFileName = theDrawParam->InitFileName();
			// HUOM! itsMacroParamRelativePath-dataosaa ei saa initialisoida, koska sitä käytetään vain viewmakrojen yhteydessä
			itsParameterAbbreviation = theDrawParam->ParameterAbbreviation();
			fEditableParam = theDrawParam->IsEditable();
			fViewMacroDrawParam = theDrawParam->ViewMacroDrawParam();
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
		itsOnlyOneSymbolRelativePositionOffset = NFmiPoint(theDrawParam->OnlyOneSymbolRelativePositionOffset());

		itsIsoLineGab = theDrawParam->IsoLineGab();
		itsModifyingStep =  theDrawParam->ModifyingStep();
		fModifyingUnit =  theDrawParam->ModifyingUnit();
		itsTimeSerialModifyingLimit = theDrawParam->TimeSerialModifyingLimit();

		itsIsolineColor =  theDrawParam->IsolineColor();
		itsIsolineTextColor =  theDrawParam->IsolineTextColor();

	// ei tarvitse toistaiseksi alustaa sekundaarisia värejä
	//	itsSecondaryIsolineColor = theDrawParam->IsolineSecondaryColor();
	//	itsSecondaryIsolineTextColor = theDrawParam->IsolineSecondaryTextColor();
	// ei tarvitse toistaiseksi alustaa sekundaarisia värejä

		itsAbsoluteMinValue = theDrawParam->AbsoluteMinValue();
		itsAbsoluteMaxValue =  theDrawParam->AbsoluteMaxValue();

		itsTimeSeriesScaleMin =  theDrawParam->TimeSeriesScaleMin();
		itsTimeSeriesScaleMax =  theDrawParam->TimeSeriesScaleMax();

		itsPossibleViewTypeCount = theDrawParam->PossibleViewTypeCount();

		const NFmiMetEditorTypes::View* possibleViewTypeList = theDrawParam->PossibleViewTypeList();
		for (int typeNumber = 0; typeNumber < itsPossibleViewTypeCount; typeNumber++)
		{
			itsPossibleViewTypeList[typeNumber] = possibleViewTypeList[typeNumber];
		}

		fHidden = theDrawParam->IsParamHidden();

		itsUnit = theDrawParam->Unit();

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
		itsStationSymbolColorShadeHighValueColor = theDrawParam->StationSymbolColorShadeHighValueColor();
		itsStationSymbolColorShadeClassCount = theDrawParam->StationSymbolColorShadeClassCount();
		fUseSymbolsInTextMode = theDrawParam->UseSymbolsInTextMode();
		itsUsedSymbolListIndex = theDrawParam->UsedSymbolListIndex();
		itsSymbolIndexingMapListIndex = theDrawParam->SymbolIndexingMapListIndex();
		itsGridDataPresentationStyle = theDrawParam->GridDataPresentationStyle();
		fUseIsoLineFeathering = theDrawParam->UseIsoLineFeathering();
		fIsoLineLabelsOverLapping = theDrawParam->IsoLineLabelsOverLapping();
		fShowColorLegend = theDrawParam->ShowColorLegend();
		fUseSimpleIsoLineDefinitions = theDrawParam->UseSimpleIsoLineDefinitions();
		fUseSeparatorLinesBetweenColorContourClasses = theDrawParam->UseSeparatorLinesBetweenColorContourClasses();
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
		itsSimpleIsoLineColorShadeHighValueColor = theDrawParam->SimpleIsoLineColorShadeHighValueColor();
		itsSimpleIsoLineColorShadeClassCount = theDrawParam->SimpleIsoLineColorShadeClassCount();
		itsSpecialIsoLineValues = theDrawParam->SpecialIsoLineValues();
		itsSpecialIsoLineLabelHeight = theDrawParam->SpecialIsoLineLabelHeight();
		itsSpecialIsoLineWidth = theDrawParam->SpecialIsoLineWidth();
		itsSpecialIsoLineStyle = theDrawParam->SpecialIsoLineStyle();
		itsSpecialIsoLineColorIndexies = theDrawParam->SpecialIsoLineColorIndexies();
		itsSpecialIsoLineShowLabelBox = theDrawParam->SpecialIsoLineShowLabelBox();
		fUseDefaultRegioning = theDrawParam->UseDefaultRegioning();
		fUseCustomColorContouring = theDrawParam->UseCustomColorContouring();
		itsSpecialColorContouringValues = theDrawParam->SpecialColorContouringValues();
		itsSpecialColorContouringColorIndexies = theDrawParam->SpecialColorContouringColorIndexies();
		itsColorContouringColorShadeLowValue = theDrawParam->ColorContouringColorShadeLowValue();
		itsColorContouringColorShadeMidValue = theDrawParam->ColorContouringColorShadeMidValue();
		itsColorContouringColorShadeHighValue = theDrawParam->ColorContouringColorShadeHighValue();
		itsColorContouringColorShadeLowValueColor = theDrawParam->ColorContouringColorShadeLowValueColor();
		itsColorContouringColorShadeMidValueColor = theDrawParam->ColorContouringColorShadeMidValueColor();
		itsColorContouringColorShadeHighValueColor = theDrawParam->ColorContouringColorShadeHighValueColor();
		itsColorContouringColorShadeClassCount = theDrawParam->ColorContouringColorShadeClassCount();
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
		itsIsoLineHatchBorderColor2 = theDrawParam->IsoLineHatchBorderColor2();
		itsIsoLineLabelDigitCount = theDrawParam->IsoLineLabelDigitCount();
	//***********************************************
	//********** 'versio 2' parametreja *************
	//***********************************************

	}
	return;
}

//--------------------------------------------------------
// Init
//--------------------------------------------------------
bool NFmiDrawParam::Init (const std::string& theFilename)
{
	if(theFilename != std::string(""))
	{
		std::ifstream in(theFilename.c_str(), std::ios::in);
		if(in)
		{
			in >> *this;
			in.close();
			itsInitFileName = theFilename;
			fViewMacroDrawParam = false;
			fBorrowedParam = false;
			return true;
		}
	}
	return false;
}

//--------------------------------------------------------
// StoreData
//--------------------------------------------------------
bool NFmiDrawParam::StoreData (const std::string& theFilename)
{
	if(theFilename != std::string(""))
	{
		std::ofstream out(theFilename.c_str());
		if(out)
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
bool NFmiDrawParam::operator< (const NFmiDrawParam& theDrawParam) const
{
	return (itsPriority < theDrawParam.itsPriority);
}

//--------------------------------------------------------
// Write
//--------------------------------------------------------
std::ostream& NFmiDrawParam::Write (std::ostream &file) const
{
	using namespace std;

	file << "Version ";
	file << itsFileVersionNumber << endl;
	file << "'ParameterAbbreviation'" << endl;  // selittävä teksti
	if(fViewMacroDrawParam)
	{ // jos viewmacro tapaus ja siinä oleva macroParam, sen drawParamin nimen lyhenteeseen talletetaan suhteellinen polku
	  // ('optimointia', näin minun ei vielä tarvitse muuttaa minkään macrosysteemien data tiedoston rakennetta)
		std::string tmpStr(itsMacroParamRelativePath);
		tmpStr += tmpStr.empty() ? "" : "\\";
		tmpStr += itsParameterAbbreviation;
		file << tmpStr << endl;
	}
	else
		file << itsParameterAbbreviation << endl;
	file << "'Priority'" << endl;  // selittävä teksti
	file << itsPriority << endl;
	file << "'ViewType'" << endl;   // selittävä teksti
	file << static_cast<int>(itsViewType) << endl;
	file << "'ShowStationMarker'" << endl;   // selittävä teksti
	file << static_cast<int>(fShowStationMarker) << endl;
	file << "'IsoLineGab'" << endl;   // selittävä teksti
	file << itsIsoLineGab << endl;
	file << "'IsolineColor'" << endl;   // selittävä teksti
	file << itsIsolineColor << endl;
	file << "'IsolineTextColor'" << endl;   // selittävä teksti
	file << itsIsolineTextColor	<< endl;
	file << "'ModifyingStep'" << endl;   // selittävä teksti
	file << itsModifyingStep << endl;
	file << "'ModifyingUnit'" << endl;   // selittävä teksti
	file << fModifyingUnit << endl;

	file << "'FrameColor'" << endl;   // selittävä teksti
	file << itsFrameColor << endl;
	file << "'FillColor'" << endl;   // selittävä teksti
	file << itsFillColor << endl;
	file << "'IsolineLabelBoxFillColor'" << endl;   // selittävä teksti
	file << itsIsolineLabelBoxFillColor << endl;

	file << "'itsAbsoluteMinValue'" << endl;	// selittävä testi
	file << itsAbsoluteMinValue << endl;
	file << "'itsAbsoluteMaxValue'" << endl;		// selittävä teksti
	file << itsAbsoluteMaxValue << endl;

	file << "'TimeSeriesScaleMin'" << endl;   // selittävä teksti
	file << itsTimeSeriesScaleMin << endl;
	file << "'TimeSeriesScaleMax'" << endl;   // selittävä teksti
	file << itsTimeSeriesScaleMax << endl;

	file << "'RelativeSize'" << endl;   // selittävä teksti
	file << itsRelativeSize;
	file << "'RelativePositionOffset'" << endl;   // selittävä teksti
	file << itsRelativePositionOffset;
	file << "'OnlyOneSymbolRelativeSize'" << endl;   // selittävä teksti
	file << itsOnlyOneSymbolRelativeSize;
	file << "'OnlyOneSymbolRelativePositionOffset'" << endl;   // selittävä teksti
	file << itsOnlyOneSymbolRelativePositionOffset;

	file << "'PossibleViewTypeCount'" << endl;   // selittävä teksti
	file << itsPossibleViewTypeCount << endl;
	file << "'PossibleViewTypeList'" << endl;   // selittävä teksti
	for(int ind = 0; ind < itsPossibleViewTypeCount;ind++)
		file << static_cast<int>(itsPossibleViewTypeList[ind]) << endl;

	file << "'TimeSerialModifyingLimit'" << endl;   // selittävä teksti
	file << itsTimeSerialModifyingLimit << endl;
	file << "'StationDataViewType'" << endl;   // selittävä teksti
	file << static_cast<int>(itsStationDataViewType) << endl;
	file << "'EditableParam'" << endl;   // selittävä teksti
	file << fEditableParam << endl;
	file << "'Unit'" << endl;   // selittävä teksti
	file << itsUnit << endl;

	file << "'ShowNumbers'" << endl;   // selittävä teksti
	file << fShowNumbers << endl;

	file << "'ShowMasks'" << endl;   // selittävä teksti
	file << fShowMasks << endl;
	file << "'ShowColors'" << endl;   // selittävä teksti
	file << fShowColors << endl;
	file << "'ShowColoredNumbers'" << endl;   // selittävä teksti
	file << fShowColoredNumbers << endl;
	file << "'ZeroColorMean'" << endl;   // selittävä teksti
	file << fZeroColorMean << endl;

	if(itsFileVersionNumber >= 2.) // tämä on vain esimerkki siitä mitä joskus tulee olemaan
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

		int i = 0;
		int size = itsSpecialIsoLineValues.size();
		file << size << endl;
		for(i=0; i<size;i++)
			file << itsSpecialIsoLineValues[i] << " ";
		file << endl;

		size = itsSpecialIsoLineLabelHeight.size();
		file << size << endl;
		for(i=0; i<size;i++)
			file << itsSpecialIsoLineLabelHeight[i] << " ";
		file << endl;

		size = itsSpecialIsoLineWidth.size();
		file << size << endl;
		for(i=0; i<size;i++)
			file << itsSpecialIsoLineWidth[i] << " ";
		file << endl;

		size = itsSpecialIsoLineStyle.size();
		file << size << endl;
		for(i=0; i<size;i++)
			file << itsSpecialIsoLineStyle[i] << " ";
		file << endl;

		size = itsSpecialIsoLineColorIndexies.size();
		file << size << endl;
		for(i=0; i<size;i++)
			file << itsSpecialIsoLineColorIndexies[i] << " ";
		file << endl;

		size = itsSpecialIsoLineShowLabelBox.size();
		file << size << endl;
		for(i=0; i<size;i++)
			file << itsSpecialIsoLineShowLabelBox[i] << " ";
		file << endl;

		file << fUseDefaultRegioning << endl;
		file << fUseCustomColorContouring << endl;

		size = itsSpecialColorContouringValues.size();
		file << size << endl;
		for(i=0; i<size;i++)
			file << itsSpecialColorContouringValues[i] << " ";
		file << endl;

		size = itsSpecialColorContouringColorIndexies.size();
		file << size << endl;
		for(i=0; i<size;i++)
			file << itsSpecialColorContouringColorIndexies[i] << " ";
		file << endl;


		file << itsColorContouringColorShadeLowValue << endl;
		file << itsColorContouringColorShadeMidValue << endl;
		file << itsColorContouringColorShadeHighValue << endl;
		file << itsColorContouringColorShadeLowValueColor << endl;
		file << itsColorContouringColorShadeMidValueColor << endl;
		file << itsColorContouringColorShadeHighValueColor << endl;
		file << itsColorContouringColorShadeClassCount << endl;
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
		file << itsIsoLineHatchBorderColor2 << endl;
		file << itsIsoLineLabelDigitCount << endl;
	//***********************************************
	//********** 'versio 2' parametreja *************
	//***********************************************
	}
	return file;
}

//--------------------------------------------------------
// Read
//--------------------------------------------------------
std::istream & NFmiDrawParam::Read (std::istream &file)
{
	char temp[80];
	std::string tmpStr;
	int number;
	if(!file)
		return file;
	file >> temp;
	if(std::string(temp) == std::string("Version"))
	{
		file >> itsInitFileVersionNumber;
		if(itsInitFileVersionNumber >= 1.) // tämä on vain esimerkki siitä mitä joskus tulee olemaan
		{
			if(!file)
				return file;
			file >> temp; // luetaan nimike pois
			std::getline(file, tmpStr); // luetaan ed. rivinvaihto pois jaloista
			std::getline(file, tmpStr); // luetaan rivin loppuun, jos lyhenteessä spaceja mahdollisesti
			std::string::size_type pos = tmpStr.find_last_of('/');
			if(pos == std::string::npos)
				pos = tmpStr.find_last_of('\\'); // kokeillaan varmuuden vuoksi slasyä molempiin suuntiin

			if(pos != std::string::npos)
			{ // jos löytyi kenoviiva lyhenteestä, laitetaan viemacrossa olevan macroparamin suhteellinen polku talteen
				itsMacroParamRelativePath = std::string(tmpStr.begin(), tmpStr.begin()+pos); // huom! kenoa ei oteta talteen
				itsParameterAbbreviation = std::string(tmpStr.begin()+pos+1, tmpStr.end());
			}
			else
			{
				itsMacroParamRelativePath = "";
				itsParameterAbbreviation = tmpStr;
			}
			file >> temp; // luetaan nimike pois
			file >> itsPriority;
			file >> temp; // luetaan nimike pois
			file >> number;
			itsViewType = NFmiMetEditorTypes::View(number);

			file >> temp; // luetaan nimike pois
			file >> number;
			fShowStationMarker = number != 0;
			file >> temp; // luetaan nimike pois
			file >> itsIsoLineGab;
			file >> temp; // luetaan nimike pois
			file >> itsIsolineColor;
			file >> temp; // luetaan nimike pois
			file >> itsIsolineTextColor;
			file >> temp; // luetaan nimike pois
			file >> itsModifyingStep;
			file >> temp; // luetaan nimike pois
			file >> fModifyingUnit;

			file >> temp; // luetaan nimike pois
			file >> itsFrameColor;
			file >> temp; // luetaan nimike pois
			file >> itsFillColor;
			file >> temp; // luetaan nimike pois
			file >> itsIsolineLabelBoxFillColor;

			file >> temp; // luetaan nimike pois
			file >> itsAbsoluteMinValue;
			file >> temp; // luetaan nimike pois
			file >> itsAbsoluteMaxValue;

			if(!file)
				return file;

			file >> temp; // luetaan nimike pois
			file >> itsTimeSeriesScaleMin;
			file >> temp; // luetaan nimike pois
			file >> itsTimeSeriesScaleMax;

			file >> temp; // luetaan nimike pois
			file >> itsRelativeSize;
			file >> temp; // luetaan nimike pois
			file >> itsRelativePositionOffset;
			file >> temp; // luetaan nimike pois
			file >> itsOnlyOneSymbolRelativeSize;
			file >> temp; // luetaan nimike pois
			file >> itsOnlyOneSymbolRelativePositionOffset;

			file >> temp; // luetaan nimike pois
			file >> itsPossibleViewTypeCount;
			file >> temp; // luetaan nimike pois
			if(!file)
				return file;
			for(int ind = 0; ind < itsPossibleViewTypeCount;ind++)
			{
				file >> number;
				itsPossibleViewTypeList[ind] = NFmiMetEditorTypes::View(number);
			}

			if(!file)
				return file;

			file >> temp; // luetaan nimike pois
			file >> itsTimeSerialModifyingLimit;
			file >> temp; // luetaan nimike pois
			file >> number;
			itsStationDataViewType = NFmiMetEditorTypes::View(number);
			file >> temp; // luetaan nimike pois
			file >> fEditableParam;

			file >> temp; // luetaan nimike pois
			file >> temp;
			if(!file)
				return file;
			itsUnit = std::string(temp);


			file >> temp; // luetaan nimike pois
			file >> fShowNumbers;
			file >> temp; // luetaan nimike pois
			file >> fShowMasks;
			file >> temp; // luetaan nimike pois
			file >> fShowColors;
			file >> temp; // luetaan nimike pois
			file >> fShowColoredNumbers;
			file >> temp; // luetaan nimike pois
			file >> fZeroColorMean;

//***********************************************
//********** 'versio 2' parametreja *************
//***********************************************
			if(itsInitFileVersionNumber >= 2.) // tämä on vain esimerkki siitä mitä joskus tulee olemaan
			{
				if(!file)
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

				if(!file)
					return file;
				int i = 0;
				int size;
				file >> size;
				itsSpecialIsoLineValues.resize(size);
				for(i=0; i < size; i++)
					file >> itsSpecialIsoLineValues[i];

				file >> size;
				itsSpecialIsoLineLabelHeight.resize(size);
				for(i=0; i < size; i++)
					file >> itsSpecialIsoLineLabelHeight[i];

				file >> size;
				itsSpecialIsoLineWidth.resize(size);
				for(i=0; i < size; i++)
					file >> itsSpecialIsoLineWidth[i];

				file >> size;
				itsSpecialIsoLineStyle.resize(size);
				for(i=0; i < size; i++)
					file >> itsSpecialIsoLineStyle[i];

				file >> size;
				itsSpecialIsoLineColorIndexies.resize(size);
				for(i=0; i < size; i++)
					file >> itsSpecialIsoLineColorIndexies[i];

				file >> size;
				itsSpecialIsoLineShowLabelBox.resize(size);
				for(i=0; i < size; i++)
				  {
				    // Mika: g++ 2.95 ei salli suoraa sijoitusta
				    bool temp;
				    file >> temp;
				    itsSpecialIsoLineShowLabelBox[i] = temp;
				  }

				if(!file)
					return file;
				file >> fUseDefaultRegioning;
				file >> fUseCustomColorContouring;

				file >> size;
				if(!file)
					return file;
				itsSpecialColorContouringValues.resize(size);
				for(i=0; i < size; i++)
					file >> itsSpecialColorContouringValues[i];

				file >> size;
				if(!file)
					return file;
				itsSpecialColorContouringColorIndexies.resize(size);
				for(i=0; i < size; i++)
					file >> itsSpecialColorContouringColorIndexies[i];

				file >> itsColorContouringColorShadeLowValue;
				file >> itsColorContouringColorShadeMidValue;
				file >> itsColorContouringColorShadeHighValue;
				file >> itsColorContouringColorShadeLowValueColor;
				file >> itsColorContouringColorShadeMidValueColor;
				file >> itsColorContouringColorShadeHighValueColor;
				file >> itsColorContouringColorShadeClassCount;
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
				file >> itsIsoLineHatchBorderColor2;
				file >> itsIsoLineLabelDigitCount;
				if(!file)
					return file;
	//***********************************************
	//********** 'versio 2' parametreja *************
	//***********************************************
			}
		}
		itsInitFileVersionNumber = itsFileVersionNumber; // lopuksi asetetaan versio numero viimeisimmäksi, että tulevaisuudessa talletus menee uudella versiolla
	}
    return file;
}

const std::string& NFmiDrawParam::ParameterAbbreviation(void) const
{
	static std::string dummy;
	if(itsParameterAbbreviation != std::string("") && itsParameterAbbreviation != std::string("?"))
		return itsParameterAbbreviation;
	else
	{
		dummy = std::string(static_cast<char*>(itsParameter.GetParamName()));
		return dummy;
	}
}


NFmiInfoData::Type NFmiDrawParam::DataType(void)
{
	return itsDataType;
}

