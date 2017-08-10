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

#include "CustomColorContour.h"
#include "DataEditing.h"
#include "GeneralData.h"
#include "GeneralVisualization.h"
#include "HatchSettings.h"
#include "SimpleColorContour.h"
#include "SymbolSettings.h"

#include "NFmiColor.h"
#include "NFmiMetEditorTypes.h"

#include <boost/shared_ptr.hpp>
#include <newbase/NFmiDataIdent.h>
#include <newbase/NFmiDataMatrix.h>  // täältä tulee myös checkedVector
#include <newbase/NFmiInfoData.h>
#include <newbase/NFmiLevel.h>
#include <newbase/NFmiMetTime.h>
#include <newbase/NFmiParameterName.h>
#include <newbase/NFmiPoint.h>

class NFmiDrawingEnvironment;

class NFmiDrawParam : public GeneralData,
                      public GeneralVisualization,
                      public SimpleColorContour,
                      public CustomColorContour,
                      public SymbolSettings,
                      public DataEditing,
                      public HatchSettings
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
  //	void				 ModifyingUnit (bool theModifyingUnit) { fModifyingUnit =
  // theModifyingUnit; }
  //	bool			 ModifyingUnit (void) const { return fModifyingUnit; }
  const NFmiMetEditorTypes::View* PossibleViewTypeList(void) const
  {
    return itsPossibleViewTypeList;
  }
  int PossibleViewTypeCount(void) const { return itsPossibleViewTypeCount; };
  void ContourTextColor(const NFmiColor& newColor) { itsContourTextColor = newColor; };
  const NFmiColor& ContourTextColor(void) const { return itsContourTextColor; };
  //	double TimeSerialModifyingLimit(void) const {return fModifyingUnit ?
  // itsTimeSerialModifyingLimit : 100;};
  void FileVersionNumber(const float theFileVersionNumber)
  {
    itsFileVersionNumber = theFileVersionNumber;
  };
  float FileVersionNumber(void) const { return itsFileVersionNumber; };
  bool UseSecondaryColors(void) const { return fUseSecondaryColors; };
  void UseSecondaryColors(bool newState) { fUseSecondaryColors = newState; };
  //**************************************************************
  //********** 'versio 2' parametrien asetusfunktiot *************
  //**************************************************************
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
  int ContourLabelDigitCount(void) const { return itsContourLabelDigitCount; }
  void ContourLabelDigitCount(int newValue)
  {
    itsContourLabelDigitCount = newValue;
    if (itsContourLabelDigitCount > 10) itsContourLabelDigitCount = 10;
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

  //	bool fModifyingUnit;	//(= 0, jos yksikkö on %, = 1, jos yksikkö on sama kuin itsUnit)
  NFmiColor itsContourTextColor;  // **** Versio 3 parametri ****
  bool fUseSecondaryColors;

  //   Lista mahdollisista näyttötyypeistä kyseiselle
  //   parametrille.
  NFmiMetEditorTypes::View itsPossibleViewTypeList[5];
  int itsPossibleViewTypeCount;

  //***********************************************
  //********** 'versio 2' parametreja *************
  //***********************************************

  bool fUseContourFeathering;      // **** Versio 3 parametri ****
  bool fIsoLineLabelsOverLapping;  // voivatko isoviiva labelit mennä päällekkäin vai ei?
  bool fShowColorLegend;  // piirretäänko rivin viimeiseen ruutuun color contouringin väri selitys
                          // laatikko?
  float itsSimpleIsoLineZeroValue;    // tämän arvon kautta isoviivat joutuvat menemään
  float itsSimpleContourZeroValue;    // **** Versio 3 parametri ****
  float itsSimpleContourLabelHeight;  // **** Versio 3 parametri ****
  bool fShowSimpleContourLabelBox;    // **** Versio 3 parametri ****

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
inline std::istream& operator>>(std::istream& is, NFmiDrawParam& item) { return item.Read(is); }
//@}
