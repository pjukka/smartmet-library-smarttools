#include "NFmiSymbolSettings.h"

NFmiSymbolSettings::NFmiSymbolSettings()
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

NFmiSymbolSettings::NFmiSymbolSettings(const NFmiSymbolSettings& other)
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

NFmiSymbolSettings::~NFmiSymbolSettings() {}

NFmiMetEditorTypes::View NFmiSymbolSettings::StationDataViewType(void) const
{
  return itsStationDataViewType;
}
void NFmiSymbolSettings::StationDataViewType(const NFmiMetEditorTypes::View& newValue)
{
  itsStationDataViewType = newValue;
}

void NFmiSymbolSettings::FrameColor(const NFmiColor& theFrameColor)
{
  itsFrameColor = theFrameColor;
}
const NFmiColor& NFmiSymbolSettings::FrameColor(void) const { return itsFrameColor; }

void NFmiSymbolSettings::FillColor(const NFmiColor& theFillColor) { itsFillColor = theFillColor; }
const NFmiColor& NFmiSymbolSettings::FillColor(void) const { return itsFillColor; }

void NFmiSymbolSettings::RelativeSize(const NFmiPoint& theRelativeSize)
{
  itsRelativeSize = theRelativeSize;
}
const NFmiPoint& NFmiSymbolSettings::RelativeSize(void) const { return itsRelativeSize; }

void NFmiSymbolSettings::RelativePositionOffset(const NFmiPoint& theRelativePositionOffset)
{
  itsRelativePositionOffset = theRelativePositionOffset;
}
const NFmiPoint& NFmiSymbolSettings::RelativePositionOffset(void) const
{
  return itsRelativePositionOffset;
}

void NFmiSymbolSettings::OnlyOneSymbolRelativeSize(const NFmiPoint& theOnlyOneSymbolRelativeSize)
{
  itsOnlyOneSymbolRelativeSize = theOnlyOneSymbolRelativeSize;
}
const NFmiPoint& NFmiSymbolSettings::OnlyOneSymbolRelativeSize(void) const
{
  return itsOnlyOneSymbolRelativeSize;
}

void NFmiSymbolSettings::OnlyOneSymbolRelativePositionOffset(
    const NFmiPoint& theOnlyOneSymbolRelativePositionOffset)
{
  itsOnlyOneSymbolRelativePositionOffset = theOnlyOneSymbolRelativePositionOffset;
}
const NFmiPoint& NFmiSymbolSettings::OnlyOneSymbolRelativePositionOffset(void) const
{
  return itsOnlyOneSymbolRelativePositionOffset;
}

bool NFmiSymbolSettings::ShowNumbers(void) const { return fShowNumbers; }
void NFmiSymbolSettings::ShowNumbers(const bool& theValue) { fShowNumbers = theValue; }
bool NFmiSymbolSettings::ShowMasks(void) const { return fShowMasks; }
void NFmiSymbolSettings::ShowMasks(const bool& theValue) { fShowMasks = theValue; }
bool NFmiSymbolSettings::ShowColors(void) const { return fShowColors; }
void NFmiSymbolSettings::ShowColors(const bool& theValue) { fShowColors = theValue; }
bool NFmiSymbolSettings::ShowColoredNumbers(void) const { return fShowColoredNumbers; }
void NFmiSymbolSettings::ShowColoredNumbers(const bool& theValue)
{
  fShowColoredNumbers = theValue;
}
bool NFmiSymbolSettings::ZeroColorMean(void) const { return fZeroColorMean; }
void NFmiSymbolSettings::ZeroColorMean(const bool& theValue) { fZeroColorMean = theValue; }

float NFmiSymbolSettings::StationSymbolColorShadeLowValue(void) const
{
  return itsStationSymbolColorShadeLowValue;
}
void NFmiSymbolSettings::StationSymbolColorShadeLowValue(const float& newValue)
{
  itsStationSymbolColorShadeLowValue = newValue;
}
float NFmiSymbolSettings::StationSymbolColorShadeMidValue(void) const
{
  return itsStationSymbolColorShadeMidValue;
}
void NFmiSymbolSettings::StationSymbolColorShadeMidValue(const float& newValue)
{
  itsStationSymbolColorShadeMidValue = newValue;
}
float NFmiSymbolSettings::StationSymbolColorShadeHighValue(void) const
{
  return itsStationSymbolColorShadeHighValue;
}
void NFmiSymbolSettings::StationSymbolColorShadeHighValue(const float& newValue)
{
  itsStationSymbolColorShadeHighValue = newValue;
}
const NFmiColor& NFmiSymbolSettings::StationSymbolColorShadeLowValueColor(void) const
{
  return itsStationSymbolColorShadeLowValueColor;
}
void NFmiSymbolSettings::StationSymbolColorShadeLowValueColor(const NFmiColor& newValue)
{
  itsStationSymbolColorShadeLowValueColor = newValue;
}
const NFmiColor& NFmiSymbolSettings::StationSymbolColorShadeMidValueColor(void) const
{
  return itsStationSymbolColorShadeMidValueColor;
}
void NFmiSymbolSettings::StationSymbolColorShadeMidValueColor(const NFmiColor& newValue)
{
  itsStationSymbolColorShadeMidValueColor = newValue;
}
const NFmiColor& NFmiSymbolSettings::StationSymbolColorShadeHighValueColor(void) const
{
  return itsStationSymbolColorShadeHighValueColor;
}
void NFmiSymbolSettings::StationSymbolColorShadeHighValueColor(const NFmiColor& newValue)
{
  itsStationSymbolColorShadeHighValueColor = newValue;
}
int NFmiSymbolSettings::StationSymbolColorShadeClassCount(void) const
{
  return itsStationSymbolColorShadeClassCount;
}
void NFmiSymbolSettings::StationSymbolColorShadeClassCount(const int& newValue)
{
  itsStationSymbolColorShadeClassCount = newValue;
}
bool NFmiSymbolSettings::UseSymbolsInTextMode(void) const { return fUseSymbolsInTextMode; }
void NFmiSymbolSettings::UseSymbolsInTextMode(const bool& newValue)
{
  fUseSymbolsInTextMode = newValue;
}
int NFmiSymbolSettings::UsedSymbolListIndex(void) const { return itsUsedSymbolListIndex; }
void NFmiSymbolSettings::UsedSymbolListIndex(const int& newValue)
{
  itsUsedSymbolListIndex = newValue;
}
int NFmiSymbolSettings::SymbolIndexingMapListIndex(void) const
{
  return itsSymbolIndexingMapListIndex;
}
void NFmiSymbolSettings::SymbolIndexingMapListIndex(const int& newValue)
{
  itsSymbolIndexingMapListIndex = newValue;
}
