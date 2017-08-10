#include "GeneralData.h"

GeneralData::GeneralData()
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
GeneralData::GeneralData(const GeneralData& other)
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

GeneralData::~GeneralData() {}
const NFmiDataIdent& GeneralData::Param(void) const { return itsParameter; }
void GeneralData::Param(const NFmiDataIdent& theParameter) { itsParameter = theParameter; }
const NFmiLevel& GeneralData::Level(void) const { return itsLevel; }
const std::string& GeneralData::ParameterAbbreviation(void) const
{
  static std::string dummy;
  if (itsParameterAbbreviation != std::string("") && itsParameterAbbreviation != std::string("?"))
    return itsParameterAbbreviation;
  else
  {
    dummy = std::string(static_cast<char*>(GeneralData::Param().GetParamName()));
    return dummy;
  }
}
void GeneralData::ParameterAbbreviation(std::string theParameterAbbreviation)
{
  itsParameterAbbreviation = theParameterAbbreviation;
}
void GeneralData::Level(const NFmiLevel& theLevel) { itsLevel = theLevel; }
const std::string& GeneralData::InitFileName(void) const { return itsInitFileName; }
void GeneralData::InitFileName(std::string theFileName) { itsInitFileName = theFileName; }
const std::string& GeneralData::MacroParamRelativePath(void) const
{
  return itsMacroParamRelativePath;
}
void GeneralData::MacroParamRelativePath(const std::string& newValue)
{
  itsMacroParamRelativePath = newValue;
}

const float& GeneralData::InitFileVersionNumber(void) const { return itsInitFileVersionNumber; }
void GeneralData::InitFileVersionNumber(const float& theInitFileVersionNumber)
{
  itsInitFileVersionNumber = theInitFileVersionNumber;
}

void GeneralData::EditParam(bool newValue) { fEditedParam = newValue; }
bool GeneralData::IsParamEdited(void) const { return fEditedParam; }
void GeneralData::Unit(const std::string& theUnit) { itsUnit = theUnit; }
const std::string& GeneralData::Unit(void) const { return itsUnit; }
bool GeneralData::IsActive(void) const { return fActive; }
void GeneralData::Activate(const bool& newState) { fActive = newState; }
NFmiInfoData::Type GeneralData::DataType(void) const { return itsDataType; }
void GeneralData::DataType(const NFmiInfoData::Type& newValue) { itsDataType = newValue; }
bool GeneralData::ViewMacroDrawParam(void) const { return fViewMacroDrawParam; }
void GeneralData::ViewMacroDrawParam(const bool& newState) { fViewMacroDrawParam = newState; }
bool GeneralData::BorrowedParam(void) const { return fBorrowedParam; }
void GeneralData::BorrowedParam(const bool& newValue) { fBorrowedParam = newValue; }
const NFmiMetTime& GeneralData::ModelOriginTime(void) const { return itsModelOriginTime; }
void GeneralData::ModelOriginTime(const NFmiMetTime& newValue) { itsModelOriginTime = newValue; }
int GeneralData::ModelRunIndex(void) const { return itsModelRunIndex; }
void GeneralData::ModelRunIndex(const int& newValue) { itsModelRunIndex = newValue; }
const NFmiMetTime& GeneralData::ModelOriginTimeCalculated(void) const
{
  return itsModelOriginTimeCalculated;
}
void GeneralData::ModelOriginTimeCalculated(const NFmiMetTime& newValue)
{
  itsModelOriginTimeCalculated = newValue;
}

int GeneralData::TimeSerialModelRunCount(void) const { return itsTimeSerialModelRunCount; }
void GeneralData::TimeSerialModelRunCount(const int& newValue)
{
  itsTimeSerialModelRunCount = newValue;
  if (itsTimeSerialModelRunCount < 0) itsTimeSerialModelRunCount = 0;
}

int GeneralData::ModelRunDifferenceIndex(void) const { return itsModelRunDifferenceIndex; }
void GeneralData::ModelRunDifferenceIndex(const int& newValue)
{
  itsModelRunDifferenceIndex = newValue;
}

unsigned long GeneralData::DataComparisonProdId(void) const { return itsDataComparisonProdId; }
void GeneralData::DataComparisonProdId(const unsigned long& newValue)
{
  itsDataComparisonProdId = newValue;
}

NFmiInfoData::Type GeneralData::DataComparisonType(void) const { return itsDataComparisonType; }
void GeneralData::DataComparisonType(const NFmiInfoData::Type& newValue)
{
  itsDataComparisonType = newValue;
}
