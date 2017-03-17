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
#include "SimpleColorContour.h"
#include "CustomColorContour.h"

#include "NFmiColor.h"
#include "NFmiMetEditorTypes.h"

#include <newbase/NFmiParameterName.h>
#include <newbase/NFmiPoint.h>
#include <newbase/NFmiDataMatrix.h>  // täältä tulee myös checkedVector
#include <boost/shared_ptr.hpp>

class NFmiDrawingEnvironment;

class NFmiDrawParam : public GeneralData,
                      public GeneralVisualization,
                      public SimpleColorContour,
                      public CustomColorContour
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
  float SimpleIsoLineZeroValue(void) const { return itsSimpleIsoLineZeroValue; }
  void SimpleIsoLineZeroValue(float newValue)
  {
    itsSimpleIsoLineZeroValue = newValue;
    itsSimpleContourZeroValue =
        newValue;  // **** Versio 3 parametri asetetaan toistaiseksi myös näin ****
  }

  float SimpleContourZeroValue(void) const { return itsSimpleContourZeroValue; }
  void SimpleContourZeroValue(float newValue) { itsSimpleContourZeroValue = newValue; }
  float SimpleContourLabelHeight(void) const { return itsSimpleContourLabelHeight; }
  void SimpleContourLabelHeight(float newValue) { itsSimpleContourLabelHeight = newValue; }
  bool ShowSimpleContourLabelBox(void) const { return fShowSimpleContourLabelBox; }
  void ShowSimpleContourLabelBox(bool newValue) { fShowSimpleContourLabelBox = newValue; }
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

  //   Minkä kokoinen näyttöön piirrettävä 'symbolidata'
  //   on suhteessa
  //   annettuun asemalle/hilalle varattuun 'datalaatikkoon'.
  //   (oletusarvo)
  NFmiPoint itsRelativeSize;  // nämä ovat asemadata symboli kokoja
  NFmiPoint itsRelativePositionOffset;
  NFmiPoint itsOnlyOneSymbolRelativeSize;
  NFmiPoint itsOnlyOneSymbolRelativePositionOffset;
  double itsModifyingStep;
  //	bool fModifyingUnit;	//(= 0, jos yksikkö on %, = 1, jos yksikkö on sama kuin itsUnit)
  double itsTimeSerialModifyingLimit;  // aikasarjanäytön muutos akselin minimi ja maksimi arvo
  NFmiColor itsContourTextColor;       // **** Versio 3 parametri ****
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
  float itsSimpleIsoLineZeroValue;    // tämän arvon kautta isoviivat joutuvat menemään
  float itsSimpleContourZeroValue;    // **** Versio 3 parametri ****
  float itsSimpleContourLabelHeight;  // **** Versio 3 parametri ****
  bool fShowSimpleContourLabelBox;    // **** Versio 3 parametri ****

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

