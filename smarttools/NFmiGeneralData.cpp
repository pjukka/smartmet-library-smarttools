#include "NFmiGeneralData.h"

NFmiGeneralData::NFmiGeneralData()
    : itsParameter(NFmiParam(kFmiLastParameter)),
      itsLevel(),
      itsParameterAbbreviation("?"),
      //********** 'version 2' parameters *************
      itsInitFileVersionNumber(1.),
      fEditedParam(false),
      itsUnit("?"),
      fActive(false),
      itsDataType(NFmiInfoData::kNoDataType),
      fViewMacroDrawParam(false),
      fBorrowedParam(false),
      //********** 'version 3' parameters *************
      itsModelOriginTime(NFmiMetTime::gMissingTime),
      itsModelRunIndex(0),
      itsModelOriginTimeCalculated(NFmiMetTime::gMissingTime),
      itsTimeSerialModelRunCount(0),
      itsModelRunDifferenceIndex(0),
      itsDataComparisonProdId(0),
      itsDataComparisonType(NFmiInfoData::kNoDataType)
{
}
NFmiGeneralData::NFmiGeneralData(const NFmiGeneralData& other)
    : itsParameter(other.Param()),
      itsLevel(other.Level()),
      itsParameterAbbreviation(other.ParameterAbbreviation()),
      itsInitFileName(other.InitFileName()),
      itsMacroParamRelativePath(other.MacroParamRelativePath()),
      itsInitFileVersionNumber(other.InitFileVersionNumber()),
      fEditedParam(other.IsParamEdited()),
      itsUnit(other.Unit()),
      fActive(other.IsActive()),
      itsDataType(other.DataType()),
      fViewMacroDrawParam(other.ViewMacroDrawParam()),
      fBorrowedParam(other.BorrowedParam()),
      itsModelOriginTime(other.ModelOriginTime()),
      itsModelRunIndex(other.ModelRunIndex()),
      itsModelOriginTimeCalculated(other.ModelOriginTimeCalculated()),
      itsTimeSerialModelRunCount(other.TimeSerialModelRunCount()),
      itsModelRunDifferenceIndex(other.itsModelRunDifferenceIndex),
      itsDataComparisonProdId(other.itsDataComparisonProdId),
      itsDataComparisonType(other.itsDataComparisonType)
{
}

NFmiGeneralData::~NFmiGeneralData() {}
const NFmiDataIdent& NFmiGeneralData::Param(void) const { return itsParameter; }
void NFmiGeneralData::Param(const NFmiDataIdent& theParameter) { itsParameter = theParameter; }
const NFmiLevel& NFmiGeneralData::Level(void) const { return itsLevel; }
const std::string& NFmiGeneralData::ParameterAbbreviation(void) const
{
  static std::string dummy;
  if (itsParameterAbbreviation != std::string("") && itsParameterAbbreviation != std::string("?"))
    return itsParameterAbbreviation;
  else
  {
    dummy = std::string(static_cast<char*>(NFmiGeneralData::Param().GetParamName()));
    return dummy;
  }
}
void NFmiGeneralData::ParameterAbbreviation(std::string theParameterAbbreviation)
{
  itsParameterAbbreviation = theParameterAbbreviation;
}
void NFmiGeneralData::Level(const NFmiLevel& theLevel) { itsLevel = theLevel; }
const std::string& NFmiGeneralData::InitFileName(void) const { return itsInitFileName; }
void NFmiGeneralData::InitFileName(std::string theFileName) { itsInitFileName = theFileName; }
const std::string& NFmiGeneralData::MacroParamRelativePath(void) const
{
  return itsMacroParamRelativePath;
}
void NFmiGeneralData::MacroParamRelativePath(const std::string& newValue)
{
  itsMacroParamRelativePath = newValue;
}

const float& NFmiGeneralData::InitFileVersionNumber(void) const { return itsInitFileVersionNumber; }
void NFmiGeneralData::InitFileVersionNumber(const float& theInitFileVersionNumber)
{
  itsInitFileVersionNumber = theInitFileVersionNumber;
}

void NFmiGeneralData::EditParam(bool newValue) { fEditedParam = newValue; }
bool NFmiGeneralData::IsParamEdited(void) const { return fEditedParam; }
void NFmiGeneralData::Unit(const std::string& theUnit) { itsUnit = theUnit; }
const std::string& NFmiGeneralData::Unit(void) const { return itsUnit; }
bool NFmiGeneralData::IsActive(void) const { return fActive; }
void NFmiGeneralData::Activate(const bool& newState) { fActive = newState; }
NFmiInfoData::Type NFmiGeneralData::DataType(void) const { return itsDataType; }
void NFmiGeneralData::DataType(const NFmiInfoData::Type& newValue) { itsDataType = newValue; }
bool NFmiGeneralData::ViewMacroDrawParam(void) const { return fViewMacroDrawParam; }
void NFmiGeneralData::ViewMacroDrawParam(const bool& newState) { fViewMacroDrawParam = newState; }
bool NFmiGeneralData::BorrowedParam(void) const { return fBorrowedParam; }
void NFmiGeneralData::BorrowedParam(const bool& newValue) { fBorrowedParam = newValue; }
const NFmiMetTime& NFmiGeneralData::ModelOriginTime(void) const { return itsModelOriginTime; }
void NFmiGeneralData::ModelOriginTime(const NFmiMetTime& newValue)
{
  itsModelOriginTime = newValue;
}
int NFmiGeneralData::ModelRunIndex(void) const { return itsModelRunIndex; }
void NFmiGeneralData::ModelRunIndex(const int& newValue) { itsModelRunIndex = newValue; }
const NFmiMetTime& NFmiGeneralData::ModelOriginTimeCalculated(void) const
{
  return itsModelOriginTimeCalculated;
}
void NFmiGeneralData::ModelOriginTimeCalculated(const NFmiMetTime& newValue)
{
  itsModelOriginTimeCalculated = newValue;
}

int NFmiGeneralData::TimeSerialModelRunCount(void) const { return itsTimeSerialModelRunCount; }
void NFmiGeneralData::TimeSerialModelRunCount(const int& newValue)
{
  itsTimeSerialModelRunCount = newValue;
  if (itsTimeSerialModelRunCount < 0) itsTimeSerialModelRunCount = 0;
}

int NFmiGeneralData::ModelRunDifferenceIndex(void) const { return itsModelRunDifferenceIndex; }
void NFmiGeneralData::ModelRunDifferenceIndex(const int& newValue)
{
  itsModelRunDifferenceIndex = newValue;
}

unsigned long NFmiGeneralData::DataComparisonProdId(void) const { return itsDataComparisonProdId; }
void NFmiGeneralData::DataComparisonProdId(const unsigned long& newValue)
{
  itsDataComparisonProdId = newValue;
}

NFmiInfoData::Type NFmiGeneralData::DataComparisonType(void) const { return itsDataComparisonType; }
void NFmiGeneralData::DataComparisonType(const NFmiInfoData::Type& newValue)
{
  itsDataComparisonType = newValue;
}
