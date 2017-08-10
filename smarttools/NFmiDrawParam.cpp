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
#include "NFmiColorSpaces.h"
#include "NFmiDataStoringHelpers.h"

#include <fstream>

float NFmiDrawParam::itsFileVersionNumber = 3.0;

//--------------------------------------------------------
// NFmiDrawParam(void)
//--------------------------------------------------------
NFmiDrawParam::NFmiDrawParam()
    : NFmiGeneralData(),
      NFmiGeneralVisualization(),
      NFmiSimpleColorContour(),
      NFmiCustomColorContour(),
      NFmiSymbolSettings(),
      NFmiDataEditing(),
      NFmiHatchSettings(),
      itsPriority(1),
      //, fModifyingUnit						 (true)
      itsContourTextColor(NFmiColor(0., 0., 0.)),
      itsPossibleViewTypeCount(2),
      fUseContourFeathering(false),
      fIsoLineLabelsOverLapping(true),
      fShowColorLegend(false),
      itsSimpleIsoLineZeroValue(0),
      itsSimpleContourZeroValue(0),
      itsSimpleContourLabelHeight(4),
      fShowSimpleContourLabelBox(false),
      // protected osa
      itsContourLabelDigitCount(0)
{
  NFmiGeneralData::InitFileVersionNumber(itsFileVersionNumber);
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
    : NFmiGeneralData(),
      NFmiGeneralVisualization(),
      NFmiSimpleColorContour(),
      NFmiCustomColorContour(),
      NFmiSymbolSettings(),
      NFmiDataEditing(),
      NFmiHatchSettings(),
      itsPriority(thePriority),
      //, fModifyingUnit						 (true)
      itsContourTextColor(NFmiColor(0., 0., 0.)),
      itsPossibleViewTypeCount(2),
      fUseContourFeathering(false),
      fIsoLineLabelsOverLapping(true),
      fShowColorLegend(false),
      itsSimpleIsoLineZeroValue(0),
      itsSimpleContourZeroValue(0),
      itsSimpleContourLabelHeight(4),
      fShowSimpleContourLabelBox(false),
      itsContourLabelDigitCount(0)
{
  NFmiGeneralData::Param(theParam);
  NFmiGeneralData::Level(theLevel);
  NFmiGeneralData::InitFileVersionNumber(itsFileVersionNumber);
  NFmiGeneralData::DataType(theDataType);

  itsPossibleViewTypeList[0] = NFmiMetEditorTypes::kFmiTextView;
  itsPossibleViewTypeList[1] = NFmiMetEditorTypes::kFmiIsoLineView;
}

NFmiDrawParam::NFmiDrawParam(const NFmiDrawParam& other)
    : NFmiGeneralData(other),
      NFmiGeneralVisualization(other),
      NFmiSimpleColorContour(other),
      NFmiCustomColorContour(other),
      NFmiSymbolSettings(other),
      NFmiDataEditing(other),
      NFmiHatchSettings(other),
      itsPriority(other.itsPriority),
      //, fModifyingUnit(other.fModifyingUnit)
      itsContourTextColor(other.itsContourTextColor),
      itsPossibleViewTypeCount(other.itsPossibleViewTypeCount),

      //***********************************************
      //********** 'versio 2' parametreja *************
      //***********************************************

      fUseContourFeathering(other.fUseContourFeathering),
      fIsoLineLabelsOverLapping(other.fIsoLineLabelsOverLapping),
      fShowColorLegend(other.fShowColorLegend),
      itsSimpleIsoLineZeroValue(other.itsSimpleIsoLineZeroValue),
      itsSimpleContourZeroValue(other.itsSimpleContourZeroValue),
      itsSimpleContourLabelHeight(other.itsSimpleContourLabelHeight),
      fShowSimpleContourLabelBox(other.fShowSimpleContourLabelBox),
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
NFmiDrawParam::~NFmiDrawParam(void) {}

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
      NFmiGeneralData::InitFileName(theDrawParam->InitFileName());
      // HUOM! NFmiGeneralData::MacroParamRelativePath -dataosaa ei saa initialisoida, koska sitä
      // käytetään vain
      // viewmakrojen yhteydessä
      NFmiGeneralData::ParameterAbbreviation(theDrawParam->ParameterAbbreviation());
      NFmiGeneralData::ViewMacroDrawParam(theDrawParam->ViewMacroDrawParam());
      NFmiGeneralData::Param(theDrawParam->Param());
      NFmiGeneralData::Level(const_cast<NFmiDrawParam*>(theDrawParam)->Level());
      NFmiGeneralData::DataType(theDrawParam->DataType());
      NFmiGeneralData::ModelOriginTime(theDrawParam->ModelOriginTime());
      NFmiGeneralData::ModelRunIndex(theDrawParam->ModelRunIndex());
      NFmiGeneralData::TimeSerialModelRunCount(theDrawParam->TimeSerialModelRunCount());
      NFmiGeneralData::ModelRunDifferenceIndex(theDrawParam->ModelRunDifferenceIndex());
      NFmiGeneralData::DataComparisonProdId(theDrawParam->DataComparisonProdId());
      NFmiGeneralData::DataComparisonType(theDrawParam->DataComparisonType());
    }
    itsPriority = theDrawParam->Priority();

    NFmiGeneralVisualization::ViewType(theDrawParam->ViewType());
    NFmiSymbolSettings::StationDataViewType(theDrawParam->StationDataViewType());

    NFmiSymbolSettings::FrameColor(NFmiColor(theDrawParam->FrameColor()));
    NFmiSymbolSettings::FillColor(NFmiColor(theDrawParam->FillColor()));
    NFmiSimpleIsoline::IsolineLabelBoxFillColor(NFmiColor(theDrawParam->IsolineLabelBoxFillColor()));

    NFmiSymbolSettings::RelativeSize(NFmiPoint(theDrawParam->RelativeSize()));
    NFmiSymbolSettings::RelativePositionOffset(NFmiPoint(theDrawParam->RelativePositionOffset()));
    NFmiSymbolSettings::OnlyOneSymbolRelativeSize(NFmiPoint(theDrawParam->OnlyOneSymbolRelativeSize()));
    NFmiSymbolSettings::OnlyOneSymbolRelativePositionOffset(
        NFmiPoint(theDrawParam->OnlyOneSymbolRelativePositionOffset()));

    NFmiCustomColorContour::UseIsoLineGabWithCustomContours(
        theDrawParam->UseIsoLineGabWithCustomContours());
    NFmiCustomColorContour::UseContourGabWithCustomContours(
        theDrawParam->UseContourGabWithCustomContours());
    NFmiSimpleIsoline::IsoLineGab(theDrawParam->IsoLineGab());
    NFmiDataEditing::ModifyingStep(theDrawParam->ModifyingStep());
    //		fModifyingUnit =  theDrawParam->ModifyingUnit();
    NFmiDataEditing::TimeSerialModifyingLimit(theDrawParam->TimeSerialModifyingLimit());

    NFmiSimpleIsoline::IsolineColor(theDrawParam->NFmiSimpleIsoline::IsolineColor());
    NFmiSimpleIsoline::IsolineTextColor(theDrawParam->IsolineTextColor());

    // ei tarvitse toistaiseksi alustaa sekundaarisia värejä
    //	itsSecondaryIsolineColor = theDrawParam->IsolineSecondaryColor();
    //	itsSecondaryIsolineTextColor = theDrawParam->IsolineSecondaryTextColor();
    // ei tarvitse toistaiseksi alustaa sekundaarisia värejä

    NFmiDataEditing::AbsoluteMinValue(theDrawParam->AbsoluteMinValue());
    NFmiDataEditing::AbsoluteMaxValue(theDrawParam->AbsoluteMaxValue());

    NFmiDataEditing::TimeSeriesScaleMin(theDrawParam->TimeSeriesScaleMin());
    NFmiDataEditing::TimeSeriesScaleMax(theDrawParam->TimeSeriesScaleMax());

    itsPossibleViewTypeCount = theDrawParam->PossibleViewTypeCount();

    const NFmiMetEditorTypes::View* possibleViewTypeList = theDrawParam->PossibleViewTypeList();
    for (int typeNumber = 0; typeNumber < itsPossibleViewTypeCount; typeNumber++)
    {
      itsPossibleViewTypeList[typeNumber] = possibleViewTypeList[typeNumber];
    }

    NFmiGeneralVisualization::HideParam(theDrawParam->IsParamHidden());

    NFmiGeneralData::Unit(theDrawParam->Unit());

    NFmiSymbolSettings::ShowNumbers(theDrawParam->ShowNumbers());
    NFmiSymbolSettings::ShowMasks(theDrawParam->ShowMasks());
    NFmiSymbolSettings::ShowColors(theDrawParam->ShowColors());
    NFmiSymbolSettings::ShowColoredNumbers(theDrawParam->ShowColoredNumbers());
    NFmiSymbolSettings::ZeroColorMean(theDrawParam->ZeroColorMean());
    NFmiGeneralVisualization::ShowDifference(theDrawParam->ShowDifference());
    NFmiGeneralVisualization::ShowDifferenceToOriginalData(
        theDrawParam->ShowDifferenceToOriginalData());

    //***********************************************
    //********** 'versio 2' parametreja *************
    //***********************************************
    NFmiSymbolSettings::StationSymbolColorShadeLowValue(
        theDrawParam->StationSymbolColorShadeLowValue());
    NFmiSymbolSettings::StationSymbolColorShadeMidValue(
        theDrawParam->StationSymbolColorShadeMidValue());
    NFmiSymbolSettings::StationSymbolColorShadeHighValue(
        theDrawParam->StationSymbolColorShadeHighValue());
    NFmiSymbolSettings::StationSymbolColorShadeLowValueColor(
        theDrawParam->StationSymbolColorShadeLowValueColor());
    NFmiSymbolSettings::StationSymbolColorShadeMidValueColor(
        theDrawParam->StationSymbolColorShadeMidValueColor());
    NFmiSymbolSettings::StationSymbolColorShadeHighValueColor(
        theDrawParam->StationSymbolColorShadeHighValueColor());
    NFmiSymbolSettings::StationSymbolColorShadeClassCount(
        theDrawParam->StationSymbolColorShadeClassCount());
    NFmiSymbolSettings::UseSymbolsInTextMode(theDrawParam->UseSymbolsInTextMode());
    NFmiSymbolSettings::UsedSymbolListIndex(theDrawParam->UsedSymbolListIndex());
    NFmiSymbolSettings::SymbolIndexingMapListIndex(theDrawParam->SymbolIndexingMapListIndex());
    NFmiGeneralVisualization::GridDataPresentationStyle(theDrawParam->GridDataPresentationStyle());
    NFmiGeneralVisualization::UseIsoLineFeathering(theDrawParam->UseIsoLineFeathering());
    fIsoLineLabelsOverLapping = theDrawParam->IsoLineLabelsOverLapping();
    fShowColorLegend = theDrawParam->ShowColorLegend();
    NFmiGeneralVisualization::UseSimpleIsoLineDefinitions(theDrawParam->UseSimpleIsoLineDefinitions());
    NFmiGeneralVisualization::UseSeparatorLinesBetweenColorContourClasses(
        theDrawParam->UseSeparatorLinesBetweenColorContourClasses());
    NFmiSimpleIsoline::SimpleIsoLineGap(theDrawParam->SimpleIsoLineGap());
    itsSimpleIsoLineZeroValue = theDrawParam->SimpleIsoLineZeroValue();
    NFmiSimpleIsoline::SimpleIsoLineLabelHeight(theDrawParam->SimpleIsoLineLabelHeight());
    NFmiSimpleIsoline::ShowSimpleIsoLineLabelBox(theDrawParam->ShowSimpleIsoLineLabelBox());
    NFmiSimpleIsoline::SimpleIsoLineWidth(theDrawParam->NFmiSimpleIsoline::SimpleIsoLineWidth());
    NFmiSimpleIsoline::SimpleIsoLineLineStyle(theDrawParam->NFmiSimpleIsoline::SimpleIsoLineLineStyle());
    NFmiGeneralVisualization::IsoLineSplineSmoothingFactor(
        theDrawParam->IsoLineSplineSmoothingFactor());
    NFmiSimpleIsoline::UseSingleColorsWithSimpleIsoLines(
        theDrawParam->UseSingleColorsWithSimpleIsoLines());
    NFmiSimpleIsoline::SimpleIsoLineColorShadeLowValue(theDrawParam->SimpleIsoLineColorShadeLowValue());
    NFmiSimpleIsoline::SimpleIsoLineColorShadeMidValue(theDrawParam->SimpleIsoLineColorShadeMidValue());
    NFmiSimpleIsoline::SimpleIsoLineColorShadeHighValue(
        theDrawParam->SimpleIsoLineColorShadeHighValue());
    NFmiSimpleIsoline::SimpleIsoLineColorShadeLowValueColor(
        theDrawParam->SimpleIsoLineColorShadeLowValueColor());
    NFmiSimpleIsoline::SimpleIsoLineColorShadeMidValueColor(
        theDrawParam->SimpleIsoLineColorShadeMidValueColor());
    NFmiSimpleIsoline::SimpleIsoLineColorShadeHighValueColor(
        theDrawParam->SimpleIsoLineColorShadeHighValueColor());
    NFmiSimpleIsoline::SimpleIsoLineColorShadeClassCount(
        theDrawParam->SimpleIsoLineColorShadeClassCount());
    NFmiCustomIsoline::SetSpecialIsoLineValues(theDrawParam->SpecialIsoLineValues());
    NFmiCustomIsoline::SetSpecialIsoLineLabelHeight(theDrawParam->SpecialIsoLineLabelHeight());
    NFmiCustomIsoline::SetSpecialIsoLineWidth(theDrawParam->SpecialIsoLineWidth());
    NFmiCustomIsoline::SetSpecialIsoLineStyle(theDrawParam->SpecialIsoLineStyle());
    NFmiCustomIsoline::SetSpecialIsoLineColorIndexies(theDrawParam->SpecialIsoLineColorIndexies());
    NFmiCustomIsoline::SpecialIsoLineShowLabelBox(theDrawParam->SpecialIsoLineShowLabelBox());
    NFmiGeneralVisualization::DrawOnlyOverMask(theDrawParam->DrawOnlyOverMask());
    NFmiGeneralVisualization::UseCustomColorContouring(theDrawParam->UseCustomColorContouring());
    NFmiCustomColorContour::SetSpecialColorContouringValues(
        theDrawParam->SpecialColorContouringValues());
    NFmiCustomColorContour::SpecialColorContouringColorIndexies(
        theDrawParam->SpecialColorContouringColorIndexies());
    NFmiSimpleColorContour::ColorContouringColorShadeLowValue(
        theDrawParam->ColorContouringColorShadeLowValue());
    NFmiSimpleColorContour::ColorContouringColorShadeMidValue(
        theDrawParam->ColorContouringColorShadeMidValue());
    NFmiSimpleColorContour::ColorContouringColorShadeHighValue(
        theDrawParam->ColorContouringColorShadeHighValue());
    NFmiSimpleColorContour::ColorContouringColorShadeHigh2Value(
        theDrawParam->ColorContouringColorShadeHigh2Value());
    NFmiSimpleColorContour::ColorContouringColorShadeLowValueColor(
        theDrawParam->ColorContouringColorShadeLowValueColor());
    NFmiSimpleColorContour::ColorContouringColorShadeMidValueColor(
        theDrawParam->ColorContouringColorShadeMidValueColor());
    NFmiSimpleColorContour::ColorContouringColorShadeHighValueColor(
        theDrawParam->ColorContouringColorShadeHighValueColor());
    NFmiSimpleColorContour::ColorContouringColorShadeHigh2ValueColor(
        theDrawParam->ColorContouringColorShadeHigh2ValueColor());
    NFmiHatchSettings::UseWithIsoLineHatch1(theDrawParam->UseWithIsoLineHatch1());
    NFmiHatchSettings::DrawIsoLineHatchWithBorders1(theDrawParam->DrawIsoLineHatchWithBorders1());
    NFmiHatchSettings::IsoLineHatchLowValue1(theDrawParam->IsoLineHatchLowValue1());
    NFmiHatchSettings::IsoLineHatchHighValue1(theDrawParam->IsoLineHatchHighValue1());
    NFmiHatchSettings::IsoLineHatchType1(theDrawParam->IsoLineHatchType1());
    NFmiHatchSettings::IsoLineHatchColor1(theDrawParam->IsoLineHatchColor1());
    NFmiHatchSettings::IsoLineHatchBorderColor1(theDrawParam->IsoLineHatchBorderColor1());
    NFmiHatchSettings::UseWithIsoLineHatch2(theDrawParam->UseWithIsoLineHatch2());
    NFmiHatchSettings::DrawIsoLineHatchWithBorders2(theDrawParam->DrawIsoLineHatchWithBorders2());
    NFmiHatchSettings::IsoLineHatchLowValue2(theDrawParam->IsoLineHatchLowValue2());
    NFmiHatchSettings::IsoLineHatchHighValue2(theDrawParam->IsoLineHatchHighValue2());
    NFmiHatchSettings::IsoLineHatchType2(theDrawParam->IsoLineHatchType2());
    NFmiHatchSettings::IsoLineHatchColor2(theDrawParam->IsoLineHatchColor2());
    NFmiGeneralVisualization::IsoLineLabelDigitCount(theDrawParam->IsoLineLabelDigitCount());
    //***********************************************
    //********** 'versio 2' parametreja *************
    //***********************************************

    //***********************************************
    //********** 'versio 3' parametreja *************
    //***********************************************
    NFmiSimpleIsoline::ContourLabelBoxFillColor(theDrawParam->ContourLabelBoxFillColor());
    NFmiCustomColorContour::UseContourGabWithCustomContours(
        theDrawParam->UseContourGabWithCustomContours());
    NFmiSimpleColorContour::ContourGab(theDrawParam->ContourGab());
    NFmiSimpleColorContour::ContourColor(theDrawParam->ContourColor());
    itsContourTextColor = theDrawParam->itsContourTextColor;
    fUseContourFeathering = theDrawParam->fUseContourFeathering;
    NFmiGeneralVisualization::UseSimpleContourDefinitions(theDrawParam->UseSimpleContourDefinitions());
    itsSimpleContourZeroValue = theDrawParam->itsSimpleContourZeroValue;
    itsSimpleContourLabelHeight = theDrawParam->itsSimpleContourLabelHeight;
    NFmiSimpleColorContour::SimpleContourWidth(theDrawParam->SimpleContourWidth());
    NFmiSimpleColorContour::SimpleContourLineStyle(theDrawParam->SimpleContourLineStyle());
    NFmiSimpleIsoline::SimpleIsoLineColorShadeHigh2Value(
        theDrawParam->SimpleIsoLineColorShadeHigh2Value());
    NFmiSimpleIsoline::SimpleIsoLineColorShadeHigh2ValueColor(
        theDrawParam->SimpleIsoLineColorShadeHigh2ValueColor());
    NFmiCustomIsoline::SetSpecialContourValues(theDrawParam->SpecialContourValues());
    NFmiCustomIsoline::SetSpecialContourLabelHeight(theDrawParam->SpecialContourLabelHeight());
    NFmiCustomIsoline::SetSpecialcontourWidth(theDrawParam->SpecialcontourWidth());
    NFmiCustomIsoline::SetSpecialContourStyle(theDrawParam->SpecialContourStyle());
    NFmiCustomIsoline::SetSpecialContourColorIndexies(theDrawParam->SpecialContourColorIndexies());
    NFmiGeneralVisualization::UseCustomIsoLineing(theDrawParam->UseCustomIsoLineing());
    itsContourLabelDigitCount = theDrawParam->itsContourLabelDigitCount;
    //***********************************************
    //********** 'versio 3' parametreja *************
    //***********************************************
    NFmiGeneralVisualization::Alpha(theDrawParam->Alpha());
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
        NFmiGeneralData::InitFileName(theFilename);
        NFmiGeneralData::ViewMacroDrawParam(false);
        NFmiGeneralData::BorrowedParam(false);
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
  if (NFmiGeneralData::ViewMacroDrawParam())
  {  // jos viewmacro tapaus ja siinä oleva macroParam, sen drawParamin nimen lyhenteeseen
     // talletetaan suhteellinen polku
    // ('optimointia', näin minun ei vielä tarvitse muuttaa minkään macrosysteemien data tiedoston
    // rakennetta)
    std::string tmpStr(NFmiGeneralData::MacroParamRelativePath());
    tmpStr += tmpStr.empty() ? "" : "\\";
    tmpStr += NFmiGeneralData::ParameterAbbreviation();
    file << tmpStr << endl;
  }
  else
    file << NFmiGeneralData::ParameterAbbreviation() << endl;
  file << "'Priority'" << endl;  // selittävä teksti
  file << itsPriority << endl;
  file << "'ViewType'" << endl;  // selittävä teksti
  file << static_cast<int>(NFmiGeneralVisualization::ViewType()) << endl;
  file << "'UseIsoLineGabWithCustomContours'" << endl;  // selittävä teksti
  file << static_cast<int>(NFmiCustomColorContour::UseIsoLineGabWithCustomContours()) << endl;
  file << "'IsoLineGab'" << endl;  // selittävä teksti
  file << NFmiSimpleIsoline::IsoLineGab() << endl;
  file << "'IsolineColor'" << endl;  // selittävä teksti
  file << NFmiSimpleIsoline::IsolineColor() << endl;
  file << "'IsolineTextColor'" << endl;  // selittävä teksti
  file << NFmiSimpleIsoline::IsolineTextColor() << endl;
  file << "'ModifyingStep'" << endl;  // selittävä teksti
  file << NFmiDataEditing::ModifyingStep() << endl;
  file << "'ModifyingUnit'" << endl;  // selittävä teksti
                                      //	file << fModifyingUnit << endl;
  file << true << endl;  // tässä on otettu pois modifyingUnit, mutta arvo pitää tallettaa että
                         // luku/kirjoitus operaatiot eivät mene rikki

  file << "'FrameColor'" << endl;  // selittävä teksti
  file << NFmiSymbolSettings::FrameColor() << endl;
  file << "'FillColor'" << endl;  // selittävä teksti
  file << NFmiSymbolSettings::FillColor() << endl;
  file << "'IsolineLabelBoxFillColor'" << endl;  // selittävä teksti
  file << NFmiSimpleIsoline::IsolineLabelBoxFillColor() << endl;

  file << "'itsAbsoluteMinValue'" << endl;  // selittävä testi
  file << NFmiDataEditing::AbsoluteMinValue() << endl;
  file << "'itsAbsoluteMaxValue'" << endl;  // selittävä teksti
  file << NFmiDataEditing::AbsoluteMaxValue() << endl;

  file << "'TimeSeriesScaleMin'" << endl;  // selittävä teksti
  file << NFmiDataEditing::TimeSeriesScaleMin() << endl;
  file << "'TimeSeriesScaleMax'" << endl;  // selittävä teksti
  file << NFmiDataEditing::TimeSeriesScaleMax() << endl;

  file << "'RelativeSize'" << endl;  // selittävä teksti
  file << NFmiSymbolSettings::RelativeSize();
  file << "'RelativePositionOffset'" << endl;  // selittävä teksti
  file << NFmiSymbolSettings::RelativePositionOffset();
  file << "'OnlyOneSymbolRelativeSize'" << endl;  // selittävä teksti
  file << NFmiSymbolSettings::OnlyOneSymbolRelativeSize();
  file << "'OnlyOneSymbolRelativePositionOffset'" << endl;  // selittävä teksti
  file << NFmiSymbolSettings::OnlyOneSymbolRelativePositionOffset();

  file << "'PossibleViewTypeCount'" << endl;  // selittävä teksti
  file << itsPossibleViewTypeCount << endl;
  file << "'PossibleViewTypeList'" << endl;  // selittävä teksti
  for (int ind = 0; ind < itsPossibleViewTypeCount; ind++)
    file << static_cast<int>(itsPossibleViewTypeList[ind]) << endl;

  file << "'TimeSerialModifyingLimit'" << endl;  // selittävä teksti
  file << NFmiDataEditing::TimeSerialModifyingLimit() << endl;

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
  file << static_cast<int>(NFmiSymbolSettings::StationDataViewType() + 100) << endl;
  file << "'EditableParam'" << endl;  // selittävä teksti
  file << false << endl;     // tämä muuttuja poistettu, muttä jokin arvo laitettava tähän
  file << "'Unit'" << endl;  // selittävä teksti
  file << NFmiGeneralData::Unit() << endl;

  file << "'ShowNumbers'" << endl;  // selittävä teksti
  file << NFmiSymbolSettings::ShowNumbers() << endl;

  file << "'ShowMasks'" << endl;  // selittävä teksti
  file << NFmiSymbolSettings::ShowMasks() << endl;
  file << "'ShowColors'" << endl;  // selittävä teksti
  file << NFmiSymbolSettings::ShowColors() << endl;
  file << "'ShowColoredNumbers'" << endl;  // selittävä teksti
  file << NFmiSymbolSettings::ShowColoredNumbers() << endl;
  file << "'ZeroColorMean'" << endl;  // selittävä teksti
  file << NFmiSymbolSettings::ZeroColorMean() << endl;

  if (itsFileVersionNumber >= 2.)  // tämä on vain esimerkki siitä mitä joskus tulee olemaan
  {
    //***********************************************
    //********** 'versio 2' parametreja *************
    //***********************************************
    file << NFmiSymbolSettings::StationSymbolColorShadeLowValue() << endl;
    file << NFmiSymbolSettings::StationSymbolColorShadeMidValue() << endl;
    file << NFmiSymbolSettings::StationSymbolColorShadeHighValue() << endl;
    file << NFmiSymbolSettings::StationSymbolColorShadeLowValueColor() << endl;
    file << NFmiSymbolSettings::StationSymbolColorShadeMidValueColor() << endl;
    file << NFmiSymbolSettings::StationSymbolColorShadeHighValueColor() << endl;
    file << NFmiSymbolSettings::StationSymbolColorShadeClassCount() << endl;
    file << NFmiSymbolSettings::UseSymbolsInTextMode() << endl;
    file << NFmiSymbolSettings::UsedSymbolListIndex() << endl;
    file << NFmiSymbolSettings::SymbolIndexingMapListIndex() << endl;
    file << NFmiGeneralVisualization::GridDataPresentationStyle() << endl;
    file << NFmiGeneralVisualization::UseIsoLineFeathering() << endl;
    file << fIsoLineLabelsOverLapping << endl;
    file << fShowColorLegend << endl;
    file << NFmiGeneralVisualization::UseSimpleIsoLineDefinitions() << endl;
    file << NFmiGeneralVisualization::UseSeparatorLinesBetweenColorContourClasses() << endl;
    file << NFmiSimpleIsoline::SimpleIsoLineGap() << endl;
    file << itsSimpleIsoLineZeroValue << endl;
    file << NFmiSimpleIsoline::SimpleIsoLineLabelHeight() << endl;
    file << NFmiSimpleIsoline::ShowSimpleIsoLineLabelBox() << endl;
    file << NFmiSimpleIsoline::SimpleIsoLineWidth() << endl;
    file << NFmiSimpleIsoline::SimpleIsoLineLineStyle() << endl;
    file << NFmiGeneralVisualization::IsoLineSplineSmoothingFactor() << endl;
    file << NFmiSimpleIsoline::UseSingleColorsWithSimpleIsoLines() << endl;
    file << NFmiSimpleIsoline::SimpleIsoLineColorShadeLowValue() << endl;
    file << NFmiSimpleIsoline::SimpleIsoLineColorShadeMidValue() << endl;
    file << NFmiSimpleIsoline::SimpleIsoLineColorShadeHighValue() << endl;
    file << NFmiSimpleIsoline::SimpleIsoLineColorShadeLowValueColor() << endl;
    file << NFmiSimpleIsoline::SimpleIsoLineColorShadeMidValueColor() << endl;
    file << NFmiSimpleIsoline::SimpleIsoLineColorShadeHighValueColor() << endl;
    file << NFmiSimpleIsoline::SimpleIsoLineColorShadeClassCount() << endl;

    size_t i = 0;
    size_t size = NFmiCustomIsoline::SpecialIsoLineValues().size();
    file << size << endl;
    for (i = 0; i < size; i++)
      file << NFmiCustomIsoline::SpecialIsoLineValues()[i] << " ";
    file << endl;

    size = NFmiCustomIsoline::SpecialIsoLineLabelHeight().size();
    file << size << endl;
    for (i = 0; i < size; i++)
      file << NFmiCustomIsoline::SpecialIsoLineLabelHeight()[i] << " ";
    file << endl;

    size = NFmiCustomIsoline::SpecialIsoLineWidth().size();
    file << size << endl;
    for (i = 0; i < size; i++)
      file << NFmiCustomIsoline::SpecialIsoLineWidth()[i] << " ";
    file << endl;

    size = NFmiCustomIsoline::SpecialIsoLineStyle().size();
    file << size << endl;
    for (i = 0; i < size; i++)
      file << NFmiCustomIsoline::SpecialIsoLineStyle()[i] << " ";
    file << endl;

    size = NFmiCustomIsoline::SpecialIsoLineColorIndexies().size();
    file << size << endl;
    for (i = 0; i < size; i++)
      file << NFmiCustomIsoline::SpecialIsoLineColorIndexies()[i] << " ";
    file << endl;

    size = NFmiCustomIsoline::SpecialIsoLineShowLabelBox().size();
    file << size << endl;
    for (i = 0; i < size; i++)
      file << NFmiCustomIsoline::SpecialIsoLineShowLabelBox()[i] << " ";
    file << endl;

    file << NFmiGeneralVisualization::DrawOnlyOverMask() << endl;
    file << NFmiGeneralVisualization::UseCustomColorContouring() << endl;

    size = NFmiCustomColorContour::SpecialColorContouringValues().size();
    file << size << endl;
    for (i = 0; i < size; i++)
      file << NFmiCustomColorContour::SpecialColorContouringValues()[i] << " ";
    file << endl;

    size = NFmiCustomColorContour::SpecialColorContouringColorIndexies().size();
    file << size << endl;
    for (i = 0; i < size; i++)
      file << NFmiCustomColorContour::SpecialColorContouringColorIndexies()[i] << " ";
    file << endl;

    file << NFmiSimpleColorContour::ColorContouringColorShadeLowValue() << endl;
    file << NFmiSimpleColorContour::ColorContouringColorShadeMidValue() << endl;
    file << NFmiSimpleColorContour::ColorContouringColorShadeHighValue() << endl;
    file << NFmiSimpleColorContour::ColorContouringColorShadeLowValueColor() << endl;
    file << NFmiSimpleColorContour::ColorContouringColorShadeMidValueColor() << endl;
    file << NFmiSimpleColorContour::ColorContouringColorShadeHighValueColor() << endl;
    file << NFmiSimpleColorContour::ColorContouringColorShadeHigh2Value() << endl;
    file << NFmiHatchSettings::UseWithIsoLineHatch1() << endl;
    file << NFmiHatchSettings::DrawIsoLineHatchWithBorders1() << endl;
    file << NFmiHatchSettings::IsoLineHatchLowValue1() << endl;
    file << NFmiHatchSettings::IsoLineHatchHighValue1() << endl;
    file << NFmiHatchSettings::IsoLineHatchType1() << endl;
    file << NFmiHatchSettings::IsoLineHatchColor1() << endl;
    file << NFmiHatchSettings::IsoLineHatchBorderColor1() << endl;
    file << NFmiHatchSettings::UseWithIsoLineHatch2() << endl;
    file << NFmiHatchSettings::DrawIsoLineHatchWithBorders2() << endl;
    file << NFmiHatchSettings::IsoLineHatchLowValue2() << endl;
    file << NFmiHatchSettings::IsoLineHatchHighValue2() << endl;
    file << NFmiHatchSettings::IsoLineHatchType2() << endl;
    file << NFmiHatchSettings::IsoLineHatchColor2() << endl;
    file << NFmiSimpleColorContour::ColorContouringColorShadeHigh2ValueColor() << endl;
    file << NFmiGeneralVisualization::IsoLineLabelDigitCount() << endl;
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
    file << NFmiSimpleIsoline::ContourLabelBoxFillColor() << endl;
    file << NFmiCustomColorContour::UseContourGabWithCustomContours() << " "
         << NFmiSimpleColorContour::ContourGab() << endl;
    file << NFmiSimpleColorContour::ContourColor() << endl;
    file << itsContourTextColor << endl;
    file << fUseContourFeathering << " " << NFmiGeneralVisualization::UseSimpleContourDefinitions()
         << " " << itsSimpleContourZeroValue << " " << itsSimpleContourLabelHeight << endl;
    file << fShowSimpleContourLabelBox << " " << NFmiSimpleColorContour::SimpleContourWidth() << " "
         << NFmiSimpleColorContour::SimpleContourLineStyle() << " "
         << NFmiSimpleIsoline::SimpleIsoLineColorShadeHigh2Value() << endl;
    file << NFmiSimpleIsoline::SimpleIsoLineColorShadeHigh2ValueColor() << endl;

    NFmiDataStoringHelpers::WriteContainer(
        NFmiCustomIsoline::SpecialContourValues(), file, std::string(" "));
    file << endl;
    NFmiDataStoringHelpers::WriteContainer(
        NFmiCustomIsoline::SpecialContourLabelHeight(), file, std::string(" "));
    file << endl;
    NFmiDataStoringHelpers::WriteContainer(
        NFmiCustomIsoline::SpecialcontourWidth(), file, std::string(" "));
    file << endl;
    NFmiDataStoringHelpers::WriteContainer(
        NFmiCustomIsoline::SpecialContourStyle(), file, std::string(" "));
    file << endl;
    NFmiDataStoringHelpers::WriteContainer(
        NFmiCustomIsoline::SpecialContourColorIndexies(), file, std::string(" "));
    file << endl;

    file << NFmiGeneralVisualization::UseCustomIsoLineing() << " " << itsContourLabelDigitCount << endl;

    NFmiDataStoringHelpers::NFmiExtraDataStorage extraData;  // lopuksi vielä mahdollinen extra data
    // Kun tulee uusia muuttujia, tee tähän extradatan täyttöä, jotta se saadaan talteen tiedopstoon
    // siten että
    // edelliset versiot eivät mene solmuun vaikka on tullut uutta dataa.
    extraData.Add(
        NFmiGeneralVisualization::Alpha());  // alpha on siis 1. uusista double-extra-parametreista
    extraData.Add(
        NFmiGeneralData::ModelRunIndex());  // modelRunIndex on 2. uusista double-extra-parametreista
    extraData.Add(NFmiGeneralData::TimeSerialModelRunCount());  // modelRunIndex on 3. uusista
                                                            // double-extra-parametreista
    extraData.Add(
        NFmiGeneralData::ModelRunDifferenceIndex());  // itsModelRunDifferenceIndex on 4. uusista
                                                  // double-extra-parametreista
    extraData.Add(
        static_cast<double>(NFmiGeneralData::DataComparisonProdId()));  // itsDataComparisonProdId on 5.
                                                                    // uusista
                                                                    // double-extra-parametreista
    extraData.Add(NFmiGeneralData::DataComparisonType());  // itsDataComparisonType on 6. uusista
                                                       // double-extra-parametreista

    extraData.Add(::MetTime2String(
        NFmiGeneralData::ModelOriginTime()));  // modelRunIndex on 1. uusista string-extra-parametreista

    file << "possible_extra_data" << std::endl;
    file << extraData;

    if (file.fail()) throw std::runtime_error("NFmiDrawParam::Write failed");
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
  float istreamFileVersionNumber = NFmiGeneralData::InitFileVersionNumber();
  char temp[80];
  std::string tmpStr;
  int number;
  bool tmpBool = false;
  float tmpFloat = 0.0;
  float tmpFloat2 = 0.0;
  double tmpDouble = 0.0;
  NFmiColor tmpColor;
  NFmiPoint tmpPoint;
  if (!file) return file;
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
      if (!file) return file;
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
        NFmiGeneralData::MacroParamRelativePath(std::string(tmpStr.begin(), tmpStr.begin() + pos));
        NFmiGeneralData::ParameterAbbreviation(std::string(tmpStr.begin() + pos + 1, tmpStr.end()));
      }
      else
      {
        NFmiGeneralData::MacroParamRelativePath("");
        ::FixBinaryReadParameterAbbreviation(tmpStr);  // Binääri lukuun siirron takia pitää
                                                       // trimmata mahdollisia rivinvaihto merkkejä
                                                       // pois
        NFmiGeneralData::ParameterAbbreviation(tmpStr);
      }
      file >> temp;  // luetaan nimike pois
      file >> itsPriority;
      file >> temp;  // luetaan nimike pois
      file >> number;
      NFmiGeneralVisualization::ViewType(NFmiMetEditorTypes::View(number));

      file >> temp;  // luetaan nimike pois
      file >> number;
      NFmiCustomColorContour::UseIsoLineGabWithCustomContours(number != 0);
      file >> temp;  // luetaan nimike pois
      file >> tmpDouble;
      NFmiSimpleIsoline::IsoLineGab(tmpDouble);
      file >> temp;  // luetaan nimike pois
      file >> tmpColor;
      NFmiSimpleIsoline::IsolineColor(tmpColor);
      file >> temp;  // luetaan nimike pois
      file >> tmpColor;
      NFmiSimpleIsoline::IsolineTextColor(tmpColor);
      file >> temp;  // luetaan nimike pois
      file >> tmpDouble;
      NFmiDataEditing::ModifyingStep(tmpDouble);
      file >> temp;  // luetaan nimike pois
                     //			file >> fModifyingUnit;
      file >> temp;  // luetaan legacy-koodi modifyingUnit pois

      file >> temp;  // luetaan nimike pois
      file >> tmpColor;
      NFmiSymbolSettings::FrameColor(tmpColor);
      file >> temp;  // luetaan nimike pois
      file >> tmpColor;
      NFmiSymbolSettings::FillColor(tmpColor);
      file >> temp;  // luetaan nimike pois
      file >> tmpColor;
      NFmiSimpleIsoline::IsolineLabelBoxFillColor(tmpColor);

      file >> temp;  // luetaan nimike pois
      file >> tmpDouble;
      NFmiDataEditing::AbsoluteMinValue(tmpDouble);
      file >> temp;  // luetaan nimike pois
      file >> tmpDouble;
      NFmiDataEditing::AbsoluteMaxValue(tmpDouble);

      if (!file) return file;

      file >> temp;  // luetaan nimike pois
      file >> tmpDouble;
      NFmiDataEditing::TimeSeriesScaleMin(tmpDouble);
      file >> temp;  // luetaan nimike pois
      file >> tmpDouble;
      NFmiDataEditing::TimeSeriesScaleMax(tmpDouble);

      file >> temp;  // luetaan nimike pois
      file >> tmpPoint;
      NFmiSymbolSettings::RelativeSize(tmpPoint);
      file >> temp;  // luetaan nimike pois
      file >> tmpPoint;
      NFmiSymbolSettings::RelativePositionOffset(tmpPoint);
      file >> temp;  // luetaan nimike pois
      file >> tmpPoint;
      NFmiSymbolSettings::OnlyOneSymbolRelativeSize(tmpPoint);
      file >> temp;  // luetaan nimike pois
      file >> tmpPoint;
      NFmiSymbolSettings::OnlyOneSymbolRelativePositionOffset(tmpPoint);

      file >> temp;  // luetaan nimike pois
      file >> itsPossibleViewTypeCount;
      file >> temp;  // luetaan nimike pois
      if (!file) return file;
      for (int ind = 0; ind < itsPossibleViewTypeCount; ind++)
      {
        file >> number;
        itsPossibleViewTypeList[ind] = NFmiMetEditorTypes::View(number);
      }

      if (!file) return file;

      file >> temp;  // luetaan nimike pois
      file >> tmpDouble;
      NFmiDataEditing::TimeSerialModifyingLimit();
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
      if (number < 1) number = 1;
      NFmiSymbolSettings::StationDataViewType(NFmiMetEditorTypes::View(number));
      file >> temp;     // luetaan nimike pois
      file >> tmpBool;  // tämä on legacy koodia, pitää lukea bool arvo tässä

      file >> temp;  // luetaan nimike pois
      file >> temp;
      if (!file) return file;
      NFmiGeneralData::Unit(std::string(temp));

      file >> temp;  // luetaan nimike pois
      file >> tmpBool;
      NFmiSymbolSettings::ShowNumbers(tmpBool);
      file >> temp;  // luetaan nimike pois
      file >> tmpBool;
      NFmiSymbolSettings::ShowMasks(tmpBool);
      file >> temp;  // luetaan nimike pois
      file >> tmpBool;
      NFmiSymbolSettings::ShowColors(tmpBool);
      file >> temp;  // luetaan nimike pois
      file >> tmpBool;
      NFmiSymbolSettings::ShowColoredNumbers(tmpBool);
      file >> temp;  // luetaan nimike pois
      file >> tmpBool;
      NFmiSymbolSettings::ZeroColorMean(tmpBool);

      //***********************************************
      //********** 'versio 2' parametreja *************
      //***********************************************
      if (istreamFileVersionNumber >= 2.)  // tämä on vain esimerkki siitä mitä joskus tulee olemaan
      {
        if (!file) return file;

        file >> tmpFloat;
        NFmiSymbolSettings::StationSymbolColorShadeLowValue(tmpFloat);
        file >> tmpFloat;
        NFmiSymbolSettings::StationSymbolColorShadeMidValue(tmpFloat);
        file >> tmpFloat;
        NFmiSymbolSettings::StationSymbolColorShadeHighValue(tmpFloat);
        file >> tmpColor;
        NFmiSymbolSettings::StationSymbolColorShadeLowValueColor(tmpColor);
        file >> tmpColor;
        NFmiSymbolSettings::StationSymbolColorShadeMidValueColor(tmpColor);
        file >> tmpColor;
        NFmiSymbolSettings::StationSymbolColorShadeHighValueColor(tmpColor);
        file >> number;
        NFmiSymbolSettings::StationSymbolColorShadeClassCount(number);
        file >> tmpBool;
        NFmiSymbolSettings::UseSymbolsInTextMode(tmpBool);
        file >> number;
        NFmiSymbolSettings::UsedSymbolListIndex(number);
        file >> number;
        NFmiSymbolSettings::SymbolIndexingMapListIndex(number);
        file >> number;
        NFmiGeneralVisualization::GridDataPresentationStyle(number);
        file >> tmpBool;
        NFmiGeneralVisualization::UseIsoLineFeathering(tmpBool);
        file >> fIsoLineLabelsOverLapping;
        file >> fShowColorLegend;
        file >> tmpBool;
        NFmiGeneralVisualization::UseSimpleIsoLineDefinitions(tmpBool);
        file >> tmpBool;
        NFmiGeneralVisualization::UseSeparatorLinesBetweenColorContourClasses(tmpBool);
        file >> tmpFloat;
        NFmiSimpleIsoline::SimpleIsoLineGap(tmpFloat);
        file >> itsSimpleIsoLineZeroValue;
        file >> tmpFloat;
        NFmiSimpleIsoline::SimpleIsoLineLabelHeight(tmpFloat);
        file >> tmpBool;
        NFmiSimpleIsoline::ShowSimpleIsoLineLabelBox(tmpBool);
        file >> tmpFloat;
        NFmiSimpleIsoline::SimpleIsoLineWidth(tmpFloat);
        file >> number;
        NFmiSimpleIsoline::SimpleIsoLineLineStyle(number);
        file >> tmpFloat;
        NFmiGeneralVisualization::IsoLineSplineSmoothingFactor(tmpFloat);
        file >> tmpBool;
        NFmiSimpleIsoline::UseSingleColorsWithSimpleIsoLines(tmpBool);
        file >> tmpFloat;
        NFmiSimpleIsoline::SimpleIsoLineColorShadeLowValue(tmpFloat);
        file >> tmpFloat;
        NFmiSimpleIsoline::SimpleIsoLineColorShadeMidValue(tmpFloat);
        file >> tmpFloat;
        NFmiSimpleIsoline::SimpleIsoLineColorShadeHighValue(tmpFloat);
        file >> tmpColor;
        NFmiSimpleIsoline::SimpleIsoLineColorShadeLowValueColor(tmpColor);
        file >> tmpColor;
        NFmiSimpleIsoline::SimpleIsoLineColorShadeMidValueColor(tmpColor);
        file >> tmpColor;
        NFmiSimpleIsoline::SimpleIsoLineColorShadeHighValueColor(tmpColor);
        file >> number;
        NFmiSimpleIsoline::SimpleIsoLineColorShadeClassCount(number);

        if (!file) return file;

        int i = 0;
        int size;
        file >> size;
        checkedVector<float> tmpFloatVector(size);
        for (i = 0; i < size; i++)
          file >> tmpFloatVector[i];
        NFmiCustomIsoline::SetSpecialIsoLineValues(tmpFloatVector);

        file >> size;
        tmpFloatVector.resize(size);
        for (i = 0; i < size; i++)
          file >> tmpFloatVector[i];
        NFmiCustomIsoline::SetSpecialIsoLineLabelHeight(tmpFloatVector);

        file >> size;
        tmpFloatVector.resize(size);
        for (i = 0; i < size; i++)
          file >> tmpFloatVector[i];
        NFmiCustomIsoline::SetSpecialIsoLineWidth(tmpFloatVector);

        file >> size;
        checkedVector<int> tmpIntVector(size);
        for (i = 0; i < size; i++)
          file >> tmpIntVector[i];
        NFmiCustomIsoline::SetSpecialIsoLineStyle(tmpIntVector);

        file >> size;
        tmpIntVector.resize(size);
        for (i = 0; i < size; i++)
          file >> tmpIntVector[i];
        NFmiCustomIsoline::SetSpecialIsoLineColorIndexies(tmpIntVector);

        file >> size;
        checkedVector<bool> tmpBoolVector(size);
        for (i = 0; i < size; i++)
        {
          // Mika: g++ 2.95 ei salli suoraa sijoitusta
          bool foobar;
          file >> foobar;
          tmpBoolVector[i] = foobar;
        }
        NFmiCustomIsoline::SpecialIsoLineShowLabelBox(tmpBoolVector);

        if (!file) return file;
        file >> tmpBool;
        NFmiGeneralVisualization::DrawOnlyOverMask(tmpBool);
        file >> tmpBool;
        NFmiGeneralVisualization::UseCustomColorContouring(tmpBool);

        file >> size;
        if (!file) return file;
        tmpFloatVector.resize(size);
        for (i = 0; i < size; i++)
          file >> tmpFloatVector[i];
        NFmiCustomColorContour::SetSpecialColorContouringValues(tmpFloatVector);

        file >> size;
        if (!file) return file;
        tmpIntVector.resize(size);
        for (i = 0; i < size; i++)
          file >> tmpIntVector[i];
        NFmiCustomColorContour::SpecialColorContouringColorIndexies(tmpIntVector);

        file >> tmpFloat;
        NFmiSimpleColorContour::ColorContouringColorShadeLowValue(tmpFloat);
        file >> tmpFloat;
        NFmiSimpleColorContour::ColorContouringColorShadeMidValue(tmpFloat);
        file >> tmpFloat;
        NFmiSimpleColorContour::ColorContouringColorShadeHighValue(tmpFloat);
        file >> tmpColor;
        NFmiSimpleColorContour::ColorContouringColorShadeLowValueColor(tmpColor);
        file >> tmpColor;
        NFmiSimpleColorContour::ColorContouringColorShadeMidValueColor(tmpColor);
        file >> tmpColor;
        NFmiSimpleColorContour::ColorContouringColorShadeHighValueColor(tmpColor);
        file >> tmpFloat;
        NFmiSimpleColorContour::ColorContouringColorShadeHigh2Value(tmpFloat);
        file >> tmpBool;
        NFmiHatchSettings::UseWithIsoLineHatch1(tmpBool);
        file >> tmpBool;
        NFmiHatchSettings::DrawIsoLineHatchWithBorders1(tmpBool);
        file >> tmpFloat;
        NFmiHatchSettings::IsoLineHatchLowValue1(tmpFloat);
        file >> tmpFloat;
        NFmiHatchSettings::IsoLineHatchHighValue1(tmpFloat);
        file >> number;
        NFmiHatchSettings::IsoLineHatchType1(number);
        file >> tmpColor;
        NFmiHatchSettings::IsoLineHatchColor1(tmpColor);
        file >> tmpColor;
        NFmiHatchSettings::IsoLineHatchBorderColor1(tmpColor);
        file >> tmpBool;
        NFmiHatchSettings::UseWithIsoLineHatch2(tmpBool);
        file >> tmpBool;
        NFmiHatchSettings::DrawIsoLineHatchWithBorders2(tmpBool);
        file >> tmpFloat;
        NFmiHatchSettings::IsoLineHatchLowValue2(tmpFloat);
        file >> tmpFloat;
        NFmiHatchSettings::IsoLineHatchHighValue2(tmpFloat);
        file >> number;
        NFmiHatchSettings::IsoLineHatchType2(number);
        file >> tmpColor;
        NFmiHatchSettings::IsoLineHatchColor2(tmpColor);
        file >> tmpColor;
        NFmiSimpleColorContour::ColorContouringColorShadeHigh2ValueColor(tmpColor);
        file >> number;
        NFmiGeneralVisualization::IsoLineLabelDigitCount(number);
        if (!file) return file;
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
        NFmiSimpleIsoline::ContourLabelBoxFillColor(tmpColor);
        file >> tmpBool >> tmpDouble;
        NFmiCustomColorContour::UseContourGabWithCustomContours(tmpBool);
        NFmiSimpleColorContour::ContourGab(tmpDouble);
        file >> tmpColor;
        NFmiSimpleColorContour::ContourColor(tmpColor);
        file >> itsContourTextColor;
        file >> fUseContourFeathering >> tmpBool >> itsSimpleContourZeroValue >>
            itsSimpleContourLabelHeight;
        NFmiGeneralVisualization::UseSimpleContourDefinitions(tmpBool);
        file >> fShowSimpleContourLabelBox >> tmpFloat >> number >> tmpFloat2;
        NFmiSimpleColorContour::SimpleContourWidth(tmpFloat);
        NFmiSimpleColorContour::SimpleContourLineStyle(number);
        NFmiSimpleIsoline::SimpleIsoLineColorShadeHigh2Value(tmpFloat2);
        file >> tmpColor;
        NFmiSimpleIsoline::SimpleIsoLineColorShadeHigh2ValueColor(tmpColor);

        checkedVector<float> tmpFloatVector;
        NFmiDataStoringHelpers::ReadContainer(tmpFloatVector, file);
        NFmiCustomIsoline::SetSpecialContourValues(tmpFloatVector);

        NFmiDataStoringHelpers::ReadContainer(tmpFloatVector, file);
        NFmiCustomIsoline::SetSpecialContourLabelHeight(tmpFloatVector);

        NFmiDataStoringHelpers::ReadContainer(tmpFloatVector, file);
        NFmiCustomIsoline::SetSpecialcontourWidth(tmpFloatVector);

        checkedVector<int> tmpIntVector;
        NFmiDataStoringHelpers::ReadContainer(tmpIntVector, file);
        NFmiCustomIsoline::SetSpecialContourStyle(tmpIntVector);

        NFmiDataStoringHelpers::ReadContainer(tmpIntVector, file);
        NFmiCustomIsoline::SetSpecialContourColorIndexies(tmpIntVector);

        file >> tmpBool >> itsContourLabelDigitCount;
        NFmiGeneralVisualization::UseCustomIsoLineing(tmpBool);

        if (file.fail()) throw std::runtime_error("NFmiDrawParam::Write failed");

        file >> temp;  // luetaan 'possible_extra_data' pois
        NFmiDataStoringHelpers::NFmiExtraDataStorage
            extraData;  // lopuksi vielä mahdollinen extra data
        file >> extraData;
        // Tässä sitten otetaaan extradatasta talteen uudet muuttujat, mitä on mahdollisesti tullut
        // eli jos uusia muutujia tai arvoja, käsittele tässä.
        NFmiGeneralVisualization::Alpha(
            100.f);  // tämä on siis default arvo alphalle (täysin läpinäkyvä)
        if (extraData.itsDoubleValues.size() >= 1)
          Alpha(static_cast<float>(
              extraData
                  .itsDoubleValues[0]));  // laitetaan asetus-funktion läpi, jossa raja tarkistukset
        NFmiGeneralData::ModelRunIndex(0);    // 0 on default, eli ei ole käytössä
        if (extraData.itsDoubleValues.size() >= 2)
          ModelRunIndex(static_cast<int>(
              extraData
                  .itsDoubleValues[1]));  // laitetaan asetus-funktion läpi, jossa raja tarkistukset
        NFmiGeneralData::TimeSerialModelRunCount(0);
        if (extraData.itsDoubleValues.size() >= 3)
          TimeSerialModelRunCount(static_cast<int>(
              extraData
                  .itsDoubleValues[2]));  // laitetaan asetus-funktion läpi, jossa raja tarkistukset
        NFmiGeneralData::ModelRunDifferenceIndex(0);  // 0 on default, eli ei ole käytössä
        if (extraData.itsDoubleValues.size() >= 4)
          ModelRunDifferenceIndex(static_cast<int>(
              extraData
                  .itsDoubleValues[3]));  // laitetaan asetus-funktion läpi, jossa raja tarkistukset
        NFmiGeneralData::DataComparisonProdId(0);
        if (extraData.itsDoubleValues.size() >= 5)
          DataComparisonProdId(static_cast<unsigned long>(
              extraData
                  .itsDoubleValues[4]));  // laitetaan asetus-funktion läpi, jossa raja tarkistukset
        NFmiGeneralData::DataComparisonType(NFmiInfoData::kNoDataType);
        if (extraData.itsDoubleValues.size() >= 6)
          DataComparisonType(static_cast<NFmiInfoData::Type>(
              static_cast<int>(extraData.itsDoubleValues[5])));  // laitetaan asetus-funktion läpi,
                                                                 // jossa raja tarkistukset

        NFmiGeneralData::ModelOriginTime(
            NFmiMetTime::gMissingTime);  // tämä on oletus arvo eli ei ole käytössä
        if (extraData.itsStringValues.size() >= 1)
          NFmiGeneralData::ModelOriginTime(::String2MetTime(
              extraData
                  .itsStringValues[0]));  // laitetaan asetus-funktion läpi, jossa raja tarkistukset

        if (file.fail()) throw std::runtime_error("NFmiDrawParam::Write failed");
        //***********************************************
        //********** 'versio 3' parametreja *************
        //***********************************************
      }
      else
      {  // tietyt muuttujat pitää alustaa jos versio 2.
        NFmiSimpleColorContour::ColorContouringColorShadeHigh2Value(
            NFmiSimpleColorContour::ColorContouringColorShadeHighValue());
        NFmiSimpleColorContour::ColorContouringColorShadeHigh2ValueColor(
            NFmiSimpleColorContour::ColorContouringColorShadeHighValueColor());

        NFmiSimpleIsoline::ContourLabelBoxFillColor(NFmiSimpleIsoline::IsolineLabelBoxFillColor());
        NFmiCustomColorContour::UseContourGabWithCustomContours(false);
        NFmiSimpleColorContour::ContourGab(NFmiSimpleIsoline::IsoLineGab());
        NFmiSimpleColorContour::ContourColor(NFmiSimpleIsoline::IsolineColor());
        itsContourTextColor = NFmiSimpleIsoline::IsolineTextColor();
        fUseContourFeathering = NFmiGeneralVisualization::UseIsoLineFeathering();
        NFmiGeneralVisualization::UseSimpleContourDefinitions(
            NFmiGeneralVisualization::UseSimpleIsoLineDefinitions());
        itsSimpleContourZeroValue = itsSimpleIsoLineZeroValue;
        itsSimpleContourLabelHeight = NFmiSimpleIsoline::SimpleIsoLineLabelHeight();
        fShowSimpleContourLabelBox = NFmiSimpleIsoline::ShowSimpleIsoLineLabelBox();
        NFmiSimpleColorContour::SimpleContourWidth(NFmiSimpleIsoline::SimpleIsoLineWidth());
        NFmiSimpleColorContour::SimpleContourLineStyle(NFmiSimpleIsoline::SimpleIsoLineLineStyle());
        NFmiSimpleIsoline::SimpleIsoLineColorShadeHigh2Value(
            NFmiSimpleIsoline::SimpleIsoLineColorShadeHighValue());
        NFmiSimpleIsoline::SimpleIsoLineColorShadeHigh2ValueColor(
            NFmiSimpleIsoline::SimpleIsoLineColorShadeHighValueColor());

        NFmiCustomIsoline::SetSpecialContourValues(NFmiCustomIsoline::SpecialIsoLineValues());
        NFmiCustomIsoline::SetSpecialContourLabelHeight(NFmiCustomIsoline::SpecialIsoLineLabelHeight());
        NFmiCustomIsoline::SetSpecialcontourWidth(NFmiCustomIsoline::SpecialIsoLineWidth());
        NFmiCustomIsoline::SetSpecialContourStyle(NFmiCustomIsoline::SpecialIsoLineStyle());
        NFmiCustomIsoline::SetSpecialContourColorIndexies(NFmiCustomIsoline::SpecialIsoLineColorIndexies());

        NFmiGeneralVisualization::UseCustomIsoLineing(NFmiGeneralVisualization::UseCustomColorContouring());
        itsContourLabelDigitCount = NFmiGeneralVisualization::IsoLineLabelDigitCount();
        NFmiGeneralVisualization::Alpha(
            100.f);  // tämä on siis default arvo alphalle (täysin läpinäkyvä)
      }
    }
    NFmiGeneralData::InitFileVersionNumber(itsFileVersionNumber);  // lopuksi asetetaan versio numero
    // viimeisimmäksi, että tulevaisuudessa
    // talletus menee uudella versiolla
  }
  return file;
}

bool NFmiDrawParam::UseArchiveModelData(void) const
{
  if (IsModelRunDataType())
  {
    if (NFmiGeneralData::ModelOriginTime() != NFmiMetTime::gMissingTime) return true;
    if (NFmiGeneralData::ModelRunIndex() < 0) return true;
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
  if (theDataType == NFmiInfoData::kClimatologyData) return true;
  return false;
}

bool NFmiDrawParam::DoDataComparison(void)
{
  if (NFmiGeneralData::DataComparisonProdId() != 0)
  {
    if (NFmiGeneralData::DataComparisonType() != NFmiInfoData::kNoDataType) return true;
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
    if (IsMacroParamCase(NFmiGeneralData::DataType())) return true;
  }
  else
  {
    if (NFmiGeneralData::ViewMacroDrawParam() == false && (IsMacroParamCase(NFmiGeneralData::DataType())) &&
        NFmiGeneralData::ParameterAbbreviation() != std::string("macroParam"))
      return true;
  }
  return false;
}
