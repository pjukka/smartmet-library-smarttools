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
//							vertailtaessa samantyyppistä dataa päällekkäin)
// 
//**********************************************************
#ifndef  NFMIDRAWPARAM_H
#define  NFMIDRAWPARAM_H

#include "NFmiGlobals.h"
#include "NFmiParameterName.h"
#include "NFmiDataIdent.h"
//#include "FmiNMeteditLibraryDefinitions.h"
#include "NFmiColor.h"
#include "NFmiPoint.h"
#include "NFmiMetEditorTypes.h"
#include "NFmiInfoData.h"

#include <vector>

class NFmiSmartInfo;
class NFmiDrawingEnvironment;
class NFmiString;
class NFmiDataIdent;
//class NFmiMetEditorCoordinatorMapOptions;

class NFmiDrawParam
{

public:
    NFmiDrawParam (void);
    NFmiDrawParam (NFmiSmartInfo* theInfo, const NFmiDataIdent& theParam, int thePriority, NFmiInfoData::Type theDataType = NFmiInfoData::kNoDataType);//, NFmiMetEditorCoordinatorMapOptions* theMetEditorCoordinatorMapOptions=0);
	NFmiDrawParam (NFmiSmartInfo* theInfo, const FmiParameterName& theParam, int thePriority);//, NFmiMetEditorCoordinatorMapOptions* theMetEditorCoordinatorMapOptions=0); // NFmiDrawParamFactory::CreateDrawParam käyttää tätä
	virtual  ~NFmiDrawParam (void);

	void Init(const NFmiDrawParam* theDrawParam);
	void HideParam(bool newValue){fHidden = newValue;};
	void EditParam(bool newValue){fEditedParam = newValue;};
	bool IsParamHidden(void) const {return fHidden;};
	bool IsParamEdited(void) const {return fEditedParam;};
	bool IsEditable(void) const {return fEditableParam;};
	void EditableParam(bool newValue){fEditableParam = newValue;};

	NFmiInfoData::Type DataType(void);
	// huom! tämä asettaa vain itsDataType-dataosan arvon, ei mahdollista itsInfon data tyyppiä!!!!!!
	void DataType(NFmiInfoData::Type newValue){itsDataType = newValue;};

	bool              Init (const NFmiString& theFilename = NFmiString());
	bool              StoreData (const NFmiString& theFilename = NFmiString());

// --------------- "set" ja "get" metodit -----------------
	const NFmiString&	 ParameterAbbreviation (void) const;
	void				 ParameterAbbreviation(NFmiString theParameterAbbreviation) { itsParameterAbbreviation = theParameterAbbreviation; }
	const NFmiDataIdent& Param (void) const { return itsParameter; }; 
	const NFmiDataIdent& EditParam (void) const; 
	void                 Param (const NFmiDataIdent& theParameter) { itsParameter = theParameter; }; 
	void                 Info (NFmiSmartInfo* theInfo){ itsInfo = theInfo; }; 
	NFmiSmartInfo*       Info (void){ return itsInfo; } ; 
	void				 Priority (int thePriority){ itsPriority = thePriority; }; 
	int					 Priority (void) const { return itsPriority; }; 
	void				 ViewType (const NFmiMetEditorTypes::View& theViewType) { itsViewType = theViewType; };
	NFmiMetEditorTypes::View ViewType (void) const {return itsViewType;};
	void				 FrameColor (const NFmiColor& theFrameColor) { itsFrameColor = theFrameColor; };
	const NFmiColor&	 FrameColor (void) const { return itsFrameColor; };
	void				 FillColor (const NFmiColor& theFillColor) { itsFillColor = theFillColor; };
	const NFmiColor&	 FillColor (void) const { return itsFillColor; };
	void				 NonModifiableColor (const NFmiColor& theNonModifiableColor) { itsNonModifiableColor = theNonModifiableColor; };
	const NFmiColor&	 NonModifiableColor (void) const { return itsNonModifiableColor; };
	void				 RelativeSize (const NFmiPoint& theRelativeSize) { itsRelativeSize = theRelativeSize; };
	const NFmiPoint&	 RelativeSize (void) const { return itsRelativeSize; };
	void				 RelativePositionOffset (const NFmiPoint& theRelativePositionOffset) { itsRelativePositionOffset = theRelativePositionOffset; };
	const NFmiPoint&	 RelativePositionOffset (void) const { return itsRelativePositionOffset; };
	void				 OnlyOneSymbolRelativeSize (const NFmiPoint& theOnlyOneSymbolRelativeSize) { itsOnlyOneSymbolRelativeSize = theOnlyOneSymbolRelativeSize; };
	const NFmiPoint&	 OnlyOneSymbolRelativeSize (void) const { return itsOnlyOneSymbolRelativeSize; };
	void				 OnlyOneSymbolRelativePositionOffset (const NFmiPoint& theOnlyOneSymbolRelativePositionOffset) { itsOnlyOneSymbolRelativePositionOffset = theOnlyOneSymbolRelativePositionOffset; };
	const NFmiPoint&	 OnlyOneSymbolRelativePositionOffset (void) const { return itsOnlyOneSymbolRelativePositionOffset; };
	void				 ShowStationMarker (const bool newState) { fShowStationMarker = newState; };
	bool			 ShowStationMarker (void) const { return fShowStationMarker; };
	void				 IsoLineGab (const double theIsoLineGab) { itsIsoLineGab = theIsoLineGab; };
	double				 IsoLineGab (void) const { return itsIsoLineGab; };
	void				 ModifyingStep (const double theModifyingStep) { itsModifyingStep = theModifyingStep; };
	double				 ModifyingStep (void) const { return itsModifyingStep; };
	void				 ModifyingUnit (bool theModifyingUnit) { fModifyingUnit = theModifyingUnit; }
	bool			 ModifyingUnit (void) const { return fModifyingUnit; }
	const NFmiMetEditorTypes::View* PossibleViewTypeList (void) const {return itsPossibleViewTypeList;}
	int					 PossibleViewTypeCount (void) const { return itsPossibleViewTypeCount; };
	const NFmiString&	 InitFileName(void) const { return itsInitFileName; }
	void				 InitFileName(NFmiString theFileName) { itsInitFileName = theFileName; }

	double AbsoluteMinValue(void) const {return itsAbsoluteMinValue;}
	void AbsoluteMinValue(double theAbsoluteMinValue) {itsAbsoluteMinValue = theAbsoluteMinValue;}	
	double AbsoluteMaxValue(void) const {return itsAbsoluteMaxValue;}
	void AbsoluteMaxValue(double theAbsoluteMaxValue) {itsAbsoluteMaxValue = theAbsoluteMaxValue;}
	double TimeSeriesScaleMin(void) const {return itsTimeSeriesScaleMin;};
	double TimeSeriesScaleMax(void) const {return itsTimeSeriesScaleMax;};
	void TimeSeriesScaleMin(double theValue){itsTimeSeriesScaleMin = theValue;};
	void TimeSeriesScaleMax(double theValue){itsTimeSeriesScaleMax = theValue;};
	const NFmiColor& IsolineColor(void) const {return fUseSecondaryColors ? itsSecondaryIsolineColor : itsIsolineColor;};
	const NFmiColor& IsolineTextColor(void) const {return fUseSecondaryColors ? itsSecondaryIsolineTextColor : itsIsolineTextColor;};
	void IsolineColor(const NFmiColor& newColor){itsIsolineColor = newColor;};
	void IsolineTextColor(const NFmiColor& newColor){itsIsolineTextColor = newColor;};
	double TimeSerialModifyingLimit(void) const {return fModifyingUnit ? itsTimeSerialModifyingLimit : 100;};
	NFmiMetEditorTypes::View StationDataViewType(void) const {return itsStationDataViewType;};
	void TimeSerialModifyingLimit(double newValue){itsTimeSerialModifyingLimit = newValue;};
	void StationDataViewType(NFmiMetEditorTypes::View newValue){itsStationDataViewType = newValue;};


	void				 FileVersionNumber (const float theFileVersionNumber) { itsFileVersionNumber = theFileVersionNumber; };
	float				 FileVersionNumber (void) const { return itsFileVersionNumber; };

	void				 Unit (const NFmiString& theUnit) { itsUnit = theUnit; };
	const NFmiString&	 Unit (void) const { return itsUnit; };

	bool ShowNumbers(void) const {return fShowNumbers;}
	void ShowNumbers(bool theValue) {fShowNumbers = theValue;}
	bool	ShowMasks(void) const {return fShowMasks;}
	void	ShowMasks(bool theValue) {fShowMasks = theValue;}
	bool	ShowColors(void) const {return fShowColors;}
	void	ShowColors(bool theValue) {fShowColors = theValue;}
	bool	ShowColoredNumbers(void) const {return fShowColoredNumbers;}
	void	ShowColoredNumbers(bool theValue) {fShowColoredNumbers = theValue;}
	bool	ZeroColorMean(void) const {return fZeroColorMean;}
	void	ZeroColorMean(bool theValue) {fZeroColorMean = theValue;}
	bool IsActive(void) const {return fActive;};
	void Activate(bool newState){fActive = newState;};
	bool UseSecondaryColors(void) const {return fUseSecondaryColors;};
	void UseSecondaryColors(bool newState){fUseSecondaryColors = newState;};
	bool ShowDifference(void) const {return fShowDifference;};
	void ShowDifference(bool newState){fShowDifference = newState;};
	bool ShowDifferenceToOriginalData(void) const {return fShowDifferenceToOriginalData;}
	void ShowDifferenceToOriginalData(bool newValue){fShowDifferenceToOriginalData = newValue;}
//	NFmiMetEditorCoordinatorMapOptions* MetEditorCoordinatorMapOptions(void){return itsMetEditorCoordinatorMapOptions;};
//	void MetEditorCoordinatorMapOptions(NFmiMetEditorCoordinatorMapOptions* theNewOptions);

//**************************************************************
//********** 'versio 2' parametrien asetusfunktiot *************
//**************************************************************
	float StationSymbolColorShadeLowValue(void) const {return itsStationSymbolColorShadeLowValue;}
	void StationSymbolColorShadeLowValue(float newValue){itsStationSymbolColorShadeLowValue = newValue;}
	float StationSymbolColorShadeMidValue(void) const {return itsStationSymbolColorShadeMidValue;}
	void StationSymbolColorShadeMidValue(float newValue){itsStationSymbolColorShadeMidValue = newValue;}
	float StationSymbolColorShadeHighValue(void) const {return itsStationSymbolColorShadeHighValue;}
	void StationSymbolColorShadeHighValue(float newValue){itsStationSymbolColorShadeHighValue = newValue;}
	const NFmiColor& StationSymbolColorShadeLowValueColor(void) const {return itsStationSymbolColorShadeLowValueColor;}
	void StationSymbolColorShadeLowValueColor(const NFmiColor& newValue){itsStationSymbolColorShadeLowValueColor = newValue;}
	const NFmiColor& StationSymbolColorShadeMidValueColor(void) const {return itsStationSymbolColorShadeMidValueColor;}
	void StationSymbolColorShadeMidValueColor(const NFmiColor& newValue){itsStationSymbolColorShadeMidValueColor = newValue;}
	const NFmiColor& StationSymbolColorShadeHighValueColor(void) const {return itsStationSymbolColorShadeHighValueColor;}
	void StationSymbolColorShadeHighValueColor(const NFmiColor& newValue){itsStationSymbolColorShadeHighValueColor = newValue;}
	int StationSymbolColorShadeClassCount(void) const {return itsStationSymbolColorShadeClassCount;}
	void StationSymbolColorShadeClassCount(int newValue){itsStationSymbolColorShadeClassCount = newValue;}
	bool UseSymbolsInTextMode(void) const {return fUseSymbolsInTextMode;}
	void UseSymbolsInTextMode(bool newValue){fUseSymbolsInTextMode = newValue;}
	int UsedSymbolListIndex(void) const {return itsUsedSymbolListIndex;}
	void UsedSymbolListIndex(int newValue){itsUsedSymbolListIndex = newValue;}
	int SymbolIndexingMapListIndex(void) const {return itsSymbolIndexingMapListIndex;}
	void SymbolIndexingMapListIndex(int newValue){itsSymbolIndexingMapListIndex = newValue;}

	int GridDataPresentationStyle(void) const {return itsGridDataPresentationStyle;}
	void GridDataPresentationStyle(int newValue){itsGridDataPresentationStyle = newValue;}
	bool UseIsoLineFeathering(void) const {return fUseIsoLineFeathering;}
	void UseIsoLineFeathering(bool newValue){fUseIsoLineFeathering = newValue;}
	bool IsoLineLabelsOverLapping(void) const {return fIsoLineLabelsOverLapping;}
	void IsoLineLabelsOverLapping(bool newValue){fIsoLineLabelsOverLapping = newValue;}
	bool ShowColorLegend(void) const {return fShowColorLegend;}
	void ShowColorLegend(bool newValue){fShowColorLegend = newValue;}
	bool UseSimpleIsoLineDefinitions(void) const {return fUseSimpleIsoLineDefinitions;}
	void UseSimpleIsoLineDefinitions(bool newValue){fUseSimpleIsoLineDefinitions = newValue;}
	bool UseSeparatorLinesBetweenColorContourClasses(void) const {return fUseSeparatorLinesBetweenColorContourClasses;}
	void UseSeparatorLinesBetweenColorContourClasses(bool newValue){fUseSeparatorLinesBetweenColorContourClasses = newValue;}
	float SimpleIsoLineGap(void) const {return itsSimpleIsoLineGap;}
	void SimpleIsoLineGap(float newValue){itsSimpleIsoLineGap = newValue;}
	float SimpleIsoLineZeroValue(void) const {return itsSimpleIsoLineZeroValue;}
	void SimpleIsoLineZeroValue(float newValue){itsSimpleIsoLineZeroValue = newValue;}
	float SimpleIsoLineLabelHeight(void) const {return itsSimpleIsoLineLabelHeight;}
	void SimpleIsoLineLabelHeight(float newValue){itsSimpleIsoLineLabelHeight = newValue;}
	bool ShowSimpleIsoLineLabelBox(void) const {return fShowSimpleIsoLineLabelBox;}
	void ShowSimpleIsoLineLabelBox(bool newValue){fShowSimpleIsoLineLabelBox = newValue;}
	float SimpleIsoLineWidth(void) const {return itsSimpleIsoLineWidth;}
	void SimpleIsoLineWidth(float newValue){itsSimpleIsoLineWidth = newValue;}
	int SimpleIsoLineLineStyle(void) const {return itsSimpleIsoLineLineStyle;}
	void SimpleIsoLineLineStyle(int newValue){itsSimpleIsoLineLineStyle = newValue;}
	float IsoLineSplineSmoothingFactor(void) const {return itsIsoLineSplineSmoothingFactor;}
	void IsoLineSplineSmoothingFactor(float newValue){itsIsoLineSplineSmoothingFactor = newValue;}
	bool UseSingleColorsWithSimpleIsoLines(void) const {return fUseSingleColorsWithSimpleIsoLines;}
	void UseSingleColorsWithSimpleIsoLines(bool newValue){fUseSingleColorsWithSimpleIsoLines = newValue;}
	float SimpleIsoLineColorShadeLowValue(void) const {return itsSimpleIsoLineColorShadeLowValue;}
	void SimpleIsoLineColorShadeLowValue(float newValue){itsSimpleIsoLineColorShadeLowValue = newValue;}
	float SimpleIsoLineColorShadeMidValue(void) const {return itsSimpleIsoLineColorShadeMidValue;}
	void SimpleIsoLineColorShadeMidValue(float newValue){itsSimpleIsoLineColorShadeMidValue = newValue;}
	float SimpleIsoLineColorShadeHighValue(void) const {return itsSimpleIsoLineColorShadeHighValue;}
	void SimpleIsoLineColorShadeHighValue(float newValue){itsSimpleIsoLineColorShadeHighValue = newValue;}
	const NFmiColor& SimpleIsoLineColorShadeLowValueColor(void) const {return itsSimpleIsoLineColorShadeLowValueColor;}
	void SimpleIsoLineColorShadeLowValueColor(const NFmiColor& newValue){itsSimpleIsoLineColorShadeLowValueColor = newValue;}
	const NFmiColor& SimpleIsoLineColorShadeMidValueColor(void) const {return itsSimpleIsoLineColorShadeMidValueColor;}
	void SimpleIsoLineColorShadeMidValueColor(const NFmiColor& newValue){itsSimpleIsoLineColorShadeMidValueColor = newValue;}
	const NFmiColor& SimpleIsoLineColorShadeHighValueColor(void) const {return itsSimpleIsoLineColorShadeHighValueColor;}
	void SimpleIsoLineColorShadeHighValueColor(const NFmiColor& newValue){itsSimpleIsoLineColorShadeHighValueColor = newValue;}
	int SimpleIsoLineColorShadeClassCount(void) const {return itsSimpleIsoLineColorShadeClassCount;}
	void SimpleIsoLineColorShadeClassCount(int newValue){itsSimpleIsoLineColorShadeClassCount = newValue;}
	const std::vector<float>& SpecialIsoLineValues(void) const {return itsSpecialIsoLineValues;}
	void SpecialIsoLineValues(std::vector<float>& newValue){itsSpecialIsoLineValues = newValue;}
	const std::vector<float>& SpecialIsoLineLabelHeight(void) const {return itsSpecialIsoLineLabelHeight;}
	void SpecialIsoLineLabelHeight(std::vector<float>& newValue){itsSpecialIsoLineLabelHeight = newValue;}
	const std::vector<float>& SpecialIsoLineWidth(void) const {return itsSpecialIsoLineWidth;}
	void SpecialIsoLineWidth(std::vector<float>& newValue){itsSpecialIsoLineWidth = newValue;}
	const std::vector<int>& SpecialIsoLineStyle(void) const {return itsSpecialIsoLineStyle;}
	void SpecialIsoLineStyle(std::vector<int>& newValue){itsSpecialIsoLineStyle = newValue;}
	const std::vector<int>& SpecialIsoLineColorIndexies(void) const {return itsSpecialIsoLineColorIndexies;}
	void SpecialIsoLineColorIndexies(std::vector<int>& newValue){itsSpecialIsoLineColorIndexies = newValue;}
	const std::vector<bool>& SpecialIsoLineShowLabelBox(void) const {return itsSpecialIsoLineShowLabelBox;}
	void SpecialIsoLineShowLabelBox(std::vector<bool>& newValue){itsSpecialIsoLineShowLabelBox = newValue;}
	bool UseDefaultRegioning(void) const {return fUseDefaultRegioning;}
	void UseDefaultRegioning(bool newValue){fUseDefaultRegioning = newValue;}
	bool UseCustomColorContouring(void) const {return fUseCustomColorContouring;}
	void UseCustomColorContouring(bool newValue){fUseCustomColorContouring = newValue;}
	const std::vector<float>& SpecialColorContouringValues(void) const {return itsSpecialColorContouringValues;}
	void SpecialColorContouringValues(std::vector<float>& newValue){itsSpecialColorContouringValues = newValue;}
	const std::vector<int>& SpecialColorContouringColorIndexies(void) const {return itsSpecialColorContouringColorIndexies;}
	void SpecialColorContouringColorIndexies(std::vector<int>& newValue){itsSpecialColorContouringColorIndexies = newValue;}
	float ColorContouringColorShadeLowValue(void) const {return itsColorContouringColorShadeLowValue;}
	void ColorContouringColorShadeLowValue(float newValue){itsColorContouringColorShadeLowValue = newValue;}
	float ColorContouringColorShadeMidValue(void) const {return itsColorContouringColorShadeMidValue;}
	void ColorContouringColorShadeMidValue(float newValue){itsColorContouringColorShadeMidValue = newValue;}
	float ColorContouringColorShadeHighValue(void) const {return itsColorContouringColorShadeHighValue;}
	void ColorContouringColorShadeHighValue(float newValue){itsColorContouringColorShadeHighValue = newValue;}
	const NFmiColor& ColorContouringColorShadeLowValueColor(void) const {return itsColorContouringColorShadeLowValueColor;}
	void ColorContouringColorShadeLowValueColor(const NFmiColor& newValue){itsColorContouringColorShadeLowValueColor = newValue;}
	const NFmiColor& ColorContouringColorShadeMidValueColor(void) const {return itsColorContouringColorShadeMidValueColor;}
	void ColorContouringColorShadeMidValueColor(const NFmiColor& newValue){itsColorContouringColorShadeMidValueColor = newValue;}
	const NFmiColor& ColorContouringColorShadeHighValueColor(void) const {return itsColorContouringColorShadeHighValueColor;}
	void ColorContouringColorShadeHighValueColor(const NFmiColor& newValue){itsColorContouringColorShadeHighValueColor = newValue;}
	int ColorContouringColorShadeClassCount(void) const {return itsColorContouringColorShadeClassCount;}
	void ColorContouringColorShadeClassCount(int newValue){itsColorContouringColorShadeClassCount = newValue;}
	bool UseWithIsoLineHatch1(void) const {return fUseWithIsoLineHatch1;}
	void UseWithIsoLineHatch1(bool newValue){fUseWithIsoLineHatch1 = newValue;}
	bool DrawIsoLineHatchWithBorders1(void) const {return fDrawIsoLineHatchWithBorders1;}
	void DrawIsoLineHatchWithBorders1(bool newValue){fDrawIsoLineHatchWithBorders1 = newValue;}
	float IsoLineHatchLowValue1(void) const {return itsIsoLineHatchLowValue1;}
	void IsoLineHatchLowValue1(float newValue){itsIsoLineHatchLowValue1 = newValue;}
	float IsoLineHatchHighValue1(void) const {return itsIsoLineHatchHighValue1;}
	void IsoLineHatchHighValue1(float newValue){itsIsoLineHatchHighValue1 = newValue;}
	int IsoLineHatchType1(void) const {return itsIsoLineHatchType1;}
	void IsoLineHatchType1(int newValue){itsIsoLineHatchType1 = newValue;}
	const NFmiColor& IsoLineHatchColor1(void) const {return itsIsoLineHatchColor1;}
	void IsoLineHatchColor1(const NFmiColor& newValue){itsIsoLineHatchColor1 = newValue;}
	const NFmiColor& IsoLineHatchBorderColor1(void) const {return itsIsoLineHatchBorderColor1;}
	void IsoLineHatchBorderColor1(const NFmiColor& newValue){itsIsoLineHatchBorderColor1 = newValue;}

	bool UseWithIsoLineHatch2(void) const {return fUseWithIsoLineHatch2;}
	void UseWithIsoLineHatch2(bool newValue){fUseWithIsoLineHatch2 = newValue;}
	bool DrawIsoLineHatchWithBorders2(void) const {return fDrawIsoLineHatchWithBorders2;}
	void DrawIsoLineHatchWithBorders2(bool newValue){fDrawIsoLineHatchWithBorders2 = newValue;}
	float IsoLineHatchLowValue2(void) const {return itsIsoLineHatchLowValue2;}
	void IsoLineHatchLowValue2(float newValue){itsIsoLineHatchLowValue2 = newValue;}
	float IsoLineHatchHighValue2(void) const {return itsIsoLineHatchHighValue2;}
	void IsoLineHatchHighValue2(float newValue){itsIsoLineHatchHighValue2 = newValue;}
	int IsoLineHatchType2(void) const {return itsIsoLineHatchType2;}
	void IsoLineHatchType2(int newValue){itsIsoLineHatchType2 = newValue;}
	const NFmiColor& IsoLineHatchColor2(void) const {return itsIsoLineHatchColor2;}
	void IsoLineHatchColor2(const NFmiColor& newValue){itsIsoLineHatchColor2 = newValue;}
	const NFmiColor& IsoLineHatchBorderColor2(void) const {return itsIsoLineHatchBorderColor2;}
	void IsoLineHatchBorderColor2(const NFmiColor& newValue){itsIsoLineHatchBorderColor2 = newValue;}
	int IsoLineLabelDigitCount(void) const {return itsIsoLineLabelDigitCount;}
	void IsoLineLabelDigitCount(int newValue){itsIsoLineLabelDigitCount = newValue; if(itsIsoLineLabelDigitCount > 10)itsIsoLineLabelDigitCount = 10;}
//**************************************************************
//********** 'versio 2' parametrien asetusfunktiot *************
//**************************************************************

	bool ViewMacroDrawParam(void) const {return fViewMacroDrawParam;}
	void ViewMacroDrawParam(bool newState) {fViewMacroDrawParam = newState;}

// ---------------------- operators ------------------------
	bool operator == (const NFmiDrawParam & theDrawParam) const;
	bool operator < (const NFmiDrawParam & theDrawParam) const;
	virtual std::ostream & Write (std::ostream &file) const;
	virtual std::istream & Read (std::istream &file);


protected:

    NFmiDataIdent itsParameter;
	NFmiString itsParameterAbbreviation;
    int itsPriority;

//  Parametrin oletus näyttötyyppi (symboli, 
//  isoviiva, teksti...)
	NFmiMetEditorTypes::View itsViewType;
	NFmiMetEditorTypes::View itsStationDataViewType; // jos viewtype on isoviiva, mutta data on asema dataa, pitää olla varalla joku näyttötyyppi että voidaan piirtää tällöin
	NFmiColor itsFrameColor;
	NFmiColor itsFillColor;

//   Jos jonkin paikan arvo on ei-maskattu, eli 
//   sitä ei voi muuttaa koska
//   parametrimaskaus ei salli, väritetään data 
//   tällä värillä (oletusarvo).
	NFmiColor itsNonModifiableColor;
//   Minkä kokoinen näyttöön piirrettävä 'symbolidata' 
//   on suhteessa
//   annettuun asemalle/hilalle varattuun 'datalaatikkoon'. 
//   (oletusarvo)
	NFmiPoint itsRelativeSize;	// nämä ovat asemadata symboli kokoja
	NFmiPoint itsRelativePositionOffset;
	NFmiPoint itsOnlyOneSymbolRelativeSize;
	NFmiPoint itsOnlyOneSymbolRelativePositionOffset;
	bool fShowStationMarker;
	double itsIsoLineGab;
	double itsModifyingStep;
	bool fModifyingUnit;	//(= 0, jos yksikkö on %, = 1, jos yksikkö on sama kuin itsUnit)
	double itsTimeSerialModifyingLimit; // aikasarjanäytön muutos akselin minimi ja maksimi arvo
	NFmiColor itsIsolineColor;
	NFmiColor itsIsolineTextColor;
	NFmiColor itsSecondaryIsolineColor;
	NFmiColor itsSecondaryIsolineTextColor;
	bool fUseSecondaryColors;

	double itsAbsoluteMinValue;
	double itsAbsoluteMaxValue;

	double itsTimeSeriesScaleMin; // käytetään aikasarjaeditorissa
	double itsTimeSeriesScaleMax; // käytetään aikasarjaeditorissa

//   Lista mahdollisista näyttötyypeistä kyseiselle 
//   parametrille.
	NFmiMetEditorTypes::View itsPossibleViewTypeList[5];
	int itsPossibleViewTypeCount;

	// Tekstinäyttö:
	bool	fShowNumbers;
	bool	fShowMasks; // tämä on turha!!!!!
	bool	fShowColors; // asema tekstiä varten
	bool	fShowColoredNumbers; // asema tekstiä varten
	bool	fZeroColorMean; // asema tekstiä varten

//***********************************************
//********** 'versio 2' parametreja *************
//***********************************************
	float itsStationSymbolColorShadeLowValue; //väri skaalaus alkaa tästä arvosta
	float itsStationSymbolColorShadeMidValue; //väri skaalauksen keskiarvo
	float itsStationSymbolColorShadeHighValue; //väri skaalaus loppuu tähän arvoon
	NFmiColor itsStationSymbolColorShadeLowValueColor;
	NFmiColor itsStationSymbolColorShadeMidValueColor;
	NFmiColor itsStationSymbolColorShadeHighValueColor;
	int itsStationSymbolColorShadeClassCount; // kuinka monta väri luokkaa tehdään skaalaukseen
	bool fUseSymbolsInTextMode; // käytetäänkö tekstiä vai mapataanko arvoja kohden jokin symboli ruudulle?
	int itsUsedSymbolListIndex; // 0=ei mitään, 1=synopfont, 2=hessaa, ...
	int itsSymbolIndexingMapListIndex; // indeksi johonkin symbolilistaan, jossa on mapattu arvot haluttuihin symboleihin

	int itsGridDataPresentationStyle; // 1=asema muodossa, 2=isoviiva, 3=color contouring, 4= contour+isoviiva, 5=quick color contouring
	bool fUseIsoLineFeathering; // isoviivojen harvennus optio
	bool fIsoLineLabelsOverLapping; // voivatko isoviiva labelit mennä päällekkäin vai ei?
	bool fShowColorLegend; // piirretäänko rivin viimeiseen ruutuun color contouringin väri selitys laatikko?
	bool fUseSimpleIsoLineDefinitions; // otetaanko isoviivamääritykset simppelillä tavalla, vai kaikki arvot spesiaali tapauksina määritettyinä
	bool fUseSeparatorLinesBetweenColorContourClasses; // piirrä viivat arvo/väri luokkien välille
	float itsSimpleIsoLineGap;
	float itsSimpleIsoLineZeroValue; // tämän arvon kautta isoviivat joutuvat menemään
	float itsSimpleIsoLineLabelHeight; // relatiivinen vai mm? (0=ei näytetä ollenkaan)
	bool fShowSimpleIsoLineLabelBox; // ei vielä muita attribuutteja isoviiva labelille (tämä tarkoittaa lukua ympäroivää laatikkoa)
	float itsSimpleIsoLineWidth; // relatiivinen vai mm?
	int itsSimpleIsoLineLineStyle; // 1=yht. viiva, 2=pisteviiva jne.
	float itsIsoLineSplineSmoothingFactor; // 0-10, 0=ei pehmennystä, 10=maksimi pyöritys
	bool fUseSingleColorsWithSimpleIsoLines; // true=sama väri kaikille isoviivoille, false=tehdään väriskaala
// isoviivan väriskaalaus asetukset
	float itsSimpleIsoLineColorShadeLowValue; //väri skaalaus alkaa tästä arvosta
	float itsSimpleIsoLineColorShadeMidValue; //väri skaalauksen keskiarvo
	float itsSimpleIsoLineColorShadeHighValue; //väri skaalaus loppuu tähän arvoon
	NFmiColor itsSimpleIsoLineColorShadeLowValueColor;
	NFmiColor itsSimpleIsoLineColorShadeMidValueColor;
	NFmiColor itsSimpleIsoLineColorShadeHighValueColor;
	int itsSimpleIsoLineColorShadeClassCount; // kuinka monta väri luokkaa tehdään skaalaukseen
// speciaali isoviiva asetukset (otetaan käyttöön, jos fUseSimpleIsoLineDefinitions=false)
	std::vector<float> itsSpecialIsoLineValues; // tähän laitetaan kaikki arvot, johon halutaan isoviiva
	std::vector<float> itsSpecialIsoLineLabelHeight; // isoviivalabeleiden korkeudet (0=ei labelia)
	std::vector<float> itsSpecialIsoLineWidth; // viivan paksuudet
	std::vector<int> itsSpecialIsoLineStyle; // viiva tyylit
	std::vector<int> itsSpecialIsoLineColorIndexies; // eri viivojen väri indeksit (pitää tehdä näyttö taulukko käyttäjälle)
	std::vector<bool> itsSpecialIsoLineShowLabelBox; // eri viivojen väri indeksit (pitää tehdä näyttö taulukko käyttäjälle)
// colorcontouring ja quick contouring asetukset
	bool fUseDefaultRegioning; // jos true, data piirretään vain defaultregionin (maan) päälle
	bool fUseCustomColorContouring; // true ja saa määritellä luokka rajat ja värit
	std::vector<float> itsSpecialColorContouringValues; // tähän laitetaan kaikki arvot, johon halutaan color contour luokka rajoiksi
	std::vector<int> itsSpecialColorContouringColorIndexies; // eri viivojen väri indeksit (pitää tehdä näyttö taulukko käyttäjälle)
	float itsColorContouringColorShadeLowValue; //väri skaalaus alkaa tästä arvosta
	float itsColorContouringColorShadeMidValue; //väri skaalauksen keskiarvo
	float itsColorContouringColorShadeHighValue; //väri skaalaus loppuu tähän arvoon
	NFmiColor itsColorContouringColorShadeLowValueColor;
	NFmiColor itsColorContouringColorShadeMidValueColor;
	NFmiColor itsColorContouringColorShadeHighValueColor;
	int itsColorContouringColorShadeClassCount; // kuinka monta väri luokkaa tehdään skaalaukseen
// isoviivojen kanssa voi käyttää myös hatchättyjä alueita (2 kpl)
	bool fUseWithIsoLineHatch1;
	bool fDrawIsoLineHatchWithBorders1;
	float itsIsoLineHatchLowValue1; // hatch alueen ala-arvo
	float itsIsoLineHatchHighValue1; // hatch alueen yläarvo
	int itsIsoLineHatchType1; // hatch tyyppi 1=vinoviiva oikealle, 2=vinoviiva vasemmalle jne.
	NFmiColor itsIsoLineHatchColor1;
	NFmiColor itsIsoLineHatchBorderColor1;
	bool fUseWithIsoLineHatch2;
	bool fDrawIsoLineHatchWithBorders2;
	float itsIsoLineHatchLowValue2; // hatch alueen ala-arvo
	float itsIsoLineHatchHighValue2; // hatch alueen yläarvo
	int itsIsoLineHatchType2; // hatch tyyppi 1=vinoviiva oikealle, 2=vinoviiva vasemmalle jne.
	NFmiColor itsIsoLineHatchColor2;
	NFmiColor itsIsoLineHatchBorderColor2;
	int itsIsoLineLabelDigitCount; // isoviiva labelin näytettävien digitaalien lukumäärä 
//***********************************************
//********** 'versio 2' parametreja *************
//***********************************************

private:

	static float itsFileVersionNumber;
	float itsInitFileVersionNumber;

//   Tähän talletetaan tiedoston nimi, mistä luetaan/mihin 
//   kirjoitetaan 
//   kyseisen luokan tiedot.
	NFmiString itsInitFileName;

	bool fHidden;			// näyttö voidaan piiloittaa tämän mukaisesti
	bool fEditedParam;	// vain yksi parametreista voidaan editoida yhtä aikaa
	bool fEditableParam;	// onko parametri suoraan editoitavissa ollenkaan? (esim. HESSAA tai tuulivektori eivät ole)

	NFmiSmartInfo* itsInfo;
	NFmiString itsUnit;			
	bool fActive;			// onko kyseinen parametri näytön aktiivinen parametri (jokaisella näyttörivillä aina yksi aktivoitunut parametri)
	bool fShowDifference;	// näytetäänkö kartalla parametrin arvo, vai erotus edelliseen aikaan (ei ole vielä talletettu tiedostoon)
	bool fShowDifferenceToOriginalData;

	NFmiInfoData::Type itsDataType; // lisäsin tämän, kun laitoin editoriin satelliitti kuvien katselun mahdollisuuden (satel-datalla ei ole infoa)
	bool fViewMacroDrawParam; // is this DrawParam from viewmacro, if it is, then some things are handled 
							  // differently when modifying options, default value is false
							  // This is not stored in file!

//	NFmiMetEditorCoordinatorMapOptions* itsMetEditorCoordinatorMapOptions; // tätä käytetään koordinaatio tarkasteluissa
};
//@{ \name Globaalit NFmiDrawParam-luokan uudelleenohjaus-operaatiot
inline std::ostream& operator<<(std::ostream& os, const NFmiDrawParam& item){return item.Write(os);}
inline std::istream& operator>>(std::istream& is, NFmiDrawParam& item){return item.Read(is);}
//@}

#endif



