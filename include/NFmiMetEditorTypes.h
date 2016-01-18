// ======================================================================
/*!
 * \file NFmiMetEditorTypes.h
 * \brief NFmiInfoData namespace declarations \par
 *
 * T‰h‰n on ker‰tty m‰‰rityksi‰, mit‰ tarvitaan mm. erilaisten maskien
 * ja datojen m‰‰rityksiss‰. Olen ker‰nnyt ne t‰h‰n, koska Metkun editorin
 * luokkakirjastojen siivous vaatii tietyn koodin siirt‰mist‰ newbase:en.
 */
// ======================================================================

#ifndef NFMIMETEDITORTYPES_H
#define NFMIMETEDITORTYPES_H

namespace NFmiMetEditorTypes
{
typedef enum
{
  kFmiTextView = 1,
  kFmiIsoLineView = 2,
  kFmiColorContourView = 3,
  kFmiColorContourIsoLineView = 4,
  kFmiQuickColorContourView = 5,
  kFmiSymbolView = 6,
  kFmiIndexedTextView = 7,
  kFmiFontTextView = 8,
  kFmiBitmapView = 9,
  kFmiParamsDefaultView = 10,
  kFmiPrecipFormSymbolView = 11,
  kFmiSynopWeatherSymbolView = 12,
  kFmiRawMirriFontSymbolView = 13
} View;

typedef enum
{
  kFmiNoMask = 1,
  kFmiSelectionMask = 4,
  kFmiDisplayedMask = 16
} Mask;

enum FmiUsedSmartMetTool
{
  kFmiNoToolSelected = 0,          // default tyhj‰ arvo
  kFmiDataModificationTool = 1,    // muokkausdialogi
  kFmiBrush = 2,                   // pensseli
  kFmiTimeSerialModification = 3,  // aikasarja editointi
  kFmiSmarttool = 4,               // smarttool kieli
  kFmiHarmonization = 5,           // harmonisaattorit
  kFmiDataLoading = 6              // harmonisaattorit
};

}  // end of namespace NFmiMetEditorTypes

#endif  // NFMIMETEDITORTYPES_H
