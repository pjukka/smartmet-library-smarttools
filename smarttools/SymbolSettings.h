#pragma once

#include "NFmiColor.h"
#include "NFmiMetEditorTypes.h"
#include <newbase/NFmiPoint.h>

class SymbolSettings
{
 public:
  SymbolSettings();
  SymbolSettings(const SymbolSettings& other);
  virtual ~SymbolSettings();

  /* If viewtype is isoline but the data is station data
     ther must be some other view type defined that drawing
     can be done. */
  NFmiMetEditorTypes::View StationDataViewType(void) const;
  void StationDataViewType(const NFmiMetEditorTypes::View& newValue);

  void FrameColor(const NFmiColor& theFrameColor);
  const NFmiColor& FrameColor(void) const;

  void FillColor(const NFmiColor& theFillColor);
  const NFmiColor& FillColor(void) const;

  /* Relative size of symbol data in a view compared to
     databox given for a station or a grid. */
  void RelativeSize(const NFmiPoint& theRelativeSize);
  const NFmiPoint& RelativeSize(void) const;

  void RelativePositionOffset(const NFmiPoint& theRelativePositionOffset);
  const NFmiPoint& RelativePositionOffset(void) const;

  void OnlyOneSymbolRelativeSize(const NFmiPoint& theOnlyOneSymbolRelativeSize);
  const NFmiPoint& OnlyOneSymbolRelativeSize(void) const;

  void OnlyOneSymbolRelativePositionOffset(const NFmiPoint& theOnlyOneSymbolRelativePositionOffset);
  const NFmiPoint& OnlyOneSymbolRelativePositionOffset(void) const;

  /* For station text */
  bool ShowNumbers(void) const;
  void ShowNumbers(const bool& theValue);

  /* For station text */
  bool ShowMasks(void) const;
  void ShowMasks(const bool& theValue);

  /* For station text */
  bool ShowColors(void) const;
  void ShowColors(const bool& theValue);

  /* For station text */
  bool ShowColoredNumbers(void) const;
  void ShowColoredNumbers(const bool& theValue);

  /* For station text */
  bool ZeroColorMean(void) const;
  void ZeroColorMean(const bool& theValue);

  /* Lower limit of a color scale. */
  float StationSymbolColorShadeLowValue(void) const;
  void StationSymbolColorShadeLowValue(const float& newValue);

  /* Mean value of color a scale. */
  float StationSymbolColorShadeMidValue(void) const;
  void StationSymbolColorShadeMidValue(const float& newValue);

  /* Upper limit of a color scale. */
  float StationSymbolColorShadeHighValue(void) const;
  void StationSymbolColorShadeHighValue(const float& newValue);

  const NFmiColor& StationSymbolColorShadeLowValueColor(void) const;
  void StationSymbolColorShadeLowValueColor(const NFmiColor& newValue);

  const NFmiColor& StationSymbolColorShadeMidValueColor(void) const;
  void StationSymbolColorShadeMidValueColor(const NFmiColor& newValue);

  const NFmiColor& StationSymbolColorShadeHighValueColor(void) const;
  void StationSymbolColorShadeHighValueColor(const NFmiColor& newValue);

  /* Number of color classes in a color scale. */
  int StationSymbolColorShadeClassCount(void) const;
  void StationSymbolColorShadeClassCount(const int& newValue);

  /* A symbol is used instead of test if value is true */
  bool UseSymbolsInTextMode(void) const;
  void UseSymbolsInTextMode(const bool& newValue);

  /* 0=nothing, 1=synopfont, 2=hessaa, ... */
  int UsedSymbolListIndex(void) const;
  void UsedSymbolListIndex(const int& newValue);

  /* Index reference to a symbol list that contains the wanted
     symbol values. */
  int SymbolIndexingMapListIndex(void) const;
  void SymbolIndexingMapListIndex(const int& newValue);

 private:
  SymbolSettings& operator=(const SymbolSettings& other) = delete;

  NFmiMetEditorTypes::View itsStationDataViewType;
  NFmiColor itsFrameColor;
  NFmiColor itsFillColor;

  NFmiPoint itsRelativeSize;
  NFmiPoint itsRelativePositionOffset;
  NFmiPoint itsOnlyOneSymbolRelativeSize;
  NFmiPoint itsOnlyOneSymbolRelativePositionOffset;

  bool fShowNumbers;
  bool fShowMasks;
  bool fShowColors;
  bool fShowColoredNumbers;
  bool fZeroColorMean;

  float itsStationSymbolColorShadeLowValue;
  float itsStationSymbolColorShadeMidValue;
  float itsStationSymbolColorShadeHighValue;
  NFmiColor itsStationSymbolColorShadeLowValueColor;
  NFmiColor itsStationSymbolColorShadeMidValueColor;
  NFmiColor itsStationSymbolColorShadeHighValueColor;

  int itsStationSymbolColorShadeClassCount;
  bool fUseSymbolsInTextMode;
  int itsUsedSymbolListIndex;
  int itsSymbolIndexingMapListIndex;
};
