#include "SymbolSettings.h"

SymbolSettings::SymbolSettings()
    : itsStationDataViewType(NFmiMetEditorTypes::kFmiTextView),
      itsFrameColor(NFmiColor(0., 0., 0.)),
      itsFillColor(NFmiColor(0., 0., 0.)),
      itsRelativeSize(NFmiPoint(1, 1)),
      itsRelativePositionOffset(NFmiPoint(0, 0)),
      itsOnlyOneSymbolRelativeSize(NFmiPoint(1, 1)),
      itsOnlyOneSymbolRelativePositionOffset(NFmiPoint(0, 0)),
      fShowNumbers(true),
      fShowMasks(false),
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
      itsSymbolIndexingMapListIndex(-1)
{
}

SymbolSettings::SymbolSettings(const SymbolSettings& other)
    : itsStationDataViewType(other.StationDataViewType()),
      itsFrameColor(other.FrameColor()),
      itsFillColor(other.FillColor()),
      itsRelativeSize(other.RelativeSize()),
      itsRelativePositionOffset(other.RelativePositionOffset()),
      itsOnlyOneSymbolRelativeSize(other.OnlyOneSymbolRelativeSize()),
      itsOnlyOneSymbolRelativePositionOffset(other.OnlyOneSymbolRelativePositionOffset()),
      fShowNumbers(other.ShowNumbers()),
      fShowMasks(other.ShowMasks()),
      fShowColors(other.ShowColors()),
      fShowColoredNumbers(other.ShowColoredNumbers()),
      fZeroColorMean(other.ZeroColorMean()),
      itsStationSymbolColorShadeLowValue(other.StationSymbolColorShadeLowValue()),
      itsStationSymbolColorShadeMidValue(other.StationSymbolColorShadeMidValue()),
      itsStationSymbolColorShadeHighValue(other.StationSymbolColorShadeHighValue()),
      itsStationSymbolColorShadeLowValueColor(other.StationSymbolColorShadeLowValueColor()),
      itsStationSymbolColorShadeMidValueColor(other.StationSymbolColorShadeMidValueColor()),
      itsStationSymbolColorShadeHighValueColor(other.StationSymbolColorShadeHighValueColor()),
      itsStationSymbolColorShadeClassCount(other.StationSymbolColorShadeClassCount()),
      fUseSymbolsInTextMode(other.UseSymbolsInTextMode()),
      itsUsedSymbolListIndex(other.UsedSymbolListIndex()),
      itsSymbolIndexingMapListIndex(other.SymbolIndexingMapListIndex())
{
}

SymbolSettings::~SymbolSettings()
{
}

NFmiMetEditorTypes::View SymbolSettings::StationDataViewType(void) const
{
  return itsStationDataViewType;
}
void SymbolSettings::StationDataViewType(const NFmiMetEditorTypes::View& newValue)
{
  itsStationDataViewType = newValue;
}

void SymbolSettings::FrameColor(const NFmiColor& theFrameColor)
{
  itsFrameColor = theFrameColor;
}
const NFmiColor& SymbolSettings::FrameColor(void) const
{
  return itsFrameColor;
}

void SymbolSettings::FillColor(const NFmiColor& theFillColor)
{
  itsFillColor = theFillColor;
}
const NFmiColor& SymbolSettings::FillColor(void) const
{
  return itsFillColor;
}

void SymbolSettings::RelativeSize(const NFmiPoint& theRelativeSize)
{
  itsRelativeSize = theRelativeSize;
}
const NFmiPoint& SymbolSettings::RelativeSize(void) const
{
  return itsRelativeSize;
}

void SymbolSettings::RelativePositionOffset(const NFmiPoint& theRelativePositionOffset)
{
  itsRelativePositionOffset = theRelativePositionOffset;
}
const NFmiPoint& SymbolSettings::RelativePositionOffset(void) const
{
  return itsRelativePositionOffset;
}

void SymbolSettings::OnlyOneSymbolRelativeSize(const NFmiPoint& theOnlyOneSymbolRelativeSize)
{
  itsOnlyOneSymbolRelativeSize = theOnlyOneSymbolRelativeSize;
}
const NFmiPoint& SymbolSettings::OnlyOneSymbolRelativeSize(void) const
{
  return itsOnlyOneSymbolRelativeSize;
}

void SymbolSettings::OnlyOneSymbolRelativePositionOffset(
    const NFmiPoint& theOnlyOneSymbolRelativePositionOffset)
{
  itsOnlyOneSymbolRelativePositionOffset = theOnlyOneSymbolRelativePositionOffset;
}
const NFmiPoint& SymbolSettings::OnlyOneSymbolRelativePositionOffset(void) const
{
  return itsOnlyOneSymbolRelativePositionOffset;
}

bool SymbolSettings::ShowNumbers(void) const
{
  return fShowNumbers;
}
void SymbolSettings::ShowNumbers(const bool& theValue)
{
  fShowNumbers = theValue;
}
bool SymbolSettings::ShowMasks(void) const
{
  return fShowMasks;
}
void SymbolSettings::ShowMasks(const bool& theValue)
{
  fShowMasks = theValue;
}
bool SymbolSettings::ShowColors(void) const
{
  return fShowColors;
}
void SymbolSettings::ShowColors(const bool& theValue)
{
  fShowColors = theValue;
}
bool SymbolSettings::ShowColoredNumbers(void) const
{
  return fShowColoredNumbers;
}
void SymbolSettings::ShowColoredNumbers(const bool& theValue)
{
  fShowColoredNumbers = theValue;
}
bool SymbolSettings::ZeroColorMean(void) const
{
  return fZeroColorMean;
}
void SymbolSettings::ZeroColorMean(const bool& theValue)
{
  fZeroColorMean = theValue;
}

float SymbolSettings::StationSymbolColorShadeLowValue(void) const
{
  return itsStationSymbolColorShadeLowValue;
}
void SymbolSettings::StationSymbolColorShadeLowValue(const float& newValue)
{
  itsStationSymbolColorShadeLowValue = newValue;
}
float SymbolSettings::StationSymbolColorShadeMidValue(void) const
{
  return itsStationSymbolColorShadeMidValue;
}
void SymbolSettings::StationSymbolColorShadeMidValue(const float& newValue)
{
  itsStationSymbolColorShadeMidValue = newValue;
}
float SymbolSettings::StationSymbolColorShadeHighValue(void) const
{
  return itsStationSymbolColorShadeHighValue;
}
void SymbolSettings::StationSymbolColorShadeHighValue(const float& newValue)
{
  itsStationSymbolColorShadeHighValue = newValue;
}
const NFmiColor& SymbolSettings::StationSymbolColorShadeLowValueColor(void) const
{
  return itsStationSymbolColorShadeLowValueColor;
}
void SymbolSettings::StationSymbolColorShadeLowValueColor(const NFmiColor& newValue)
{
  itsStationSymbolColorShadeLowValueColor = newValue;
}
const NFmiColor& SymbolSettings::StationSymbolColorShadeMidValueColor(void) const
{
  return itsStationSymbolColorShadeMidValueColor;
}
void SymbolSettings::StationSymbolColorShadeMidValueColor(const NFmiColor& newValue)
{
  itsStationSymbolColorShadeMidValueColor = newValue;
}
const NFmiColor& SymbolSettings::StationSymbolColorShadeHighValueColor(void) const
{
  return itsStationSymbolColorShadeHighValueColor;
}
void SymbolSettings::StationSymbolColorShadeHighValueColor(const NFmiColor& newValue)
{
  itsStationSymbolColorShadeHighValueColor = newValue;
}
int SymbolSettings::StationSymbolColorShadeClassCount(void) const
{
  return itsStationSymbolColorShadeClassCount;
}
void SymbolSettings::StationSymbolColorShadeClassCount(const int& newValue)
{
  itsStationSymbolColorShadeClassCount = newValue;
}
bool SymbolSettings::UseSymbolsInTextMode(void) const
{
  return fUseSymbolsInTextMode;
}
void SymbolSettings::UseSymbolsInTextMode(const bool& newValue)
{
  fUseSymbolsInTextMode = newValue;
}
int SymbolSettings::UsedSymbolListIndex(void) const
{
  return itsUsedSymbolListIndex;
}
void SymbolSettings::UsedSymbolListIndex(const int& newValue)
{
  itsUsedSymbolListIndex = newValue;
}
int SymbolSettings::SymbolIndexingMapListIndex(void) const
{
  return itsSymbolIndexingMapListIndex;
}
void SymbolSettings::SymbolIndexingMapListIndex(const int& newValue)
{
  itsSymbolIndexingMapListIndex = newValue;
}
