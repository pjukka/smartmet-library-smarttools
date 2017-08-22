#pragma once

#include <string>

#include <newbase/NFmiDataIdent.h>
#include <newbase/NFmiInfoData.h>
#include <newbase/NFmiLevel.h>
#include <newbase/NFmiMetTime.h>

class NFmiGeneralData
{
 public:
  explicit NFmiGeneralData();
  explicit NFmiGeneralData(const NFmiGeneralData& other);
  virtual ~NFmiGeneralData();

  NFmiGeneralData& operator=(NFmiGeneralData& other) = default;

  const NFmiDataIdent& Param(void) const;
  void Param(const NFmiDataIdent& theParameter);

  const NFmiLevel& Level(void) const;
  void Level(const NFmiLevel& theLevel);

  const std::string& ParameterAbbreviation(void) const;
  void ParameterAbbreviation(std::string theParameterAbbreviation);

  // File name for reading and writing data of the class instance.
  const std::string& InitFileName(void) const;
  void InitFileName(std::string theFileName);

  /* When there is macroParameters in a subfolder with viewMacro to be
     stored MacroParamRelativePath is read from here to be stored with
     ParameterAbbreviation. The path is also stored here when viewMacro
     is read from a file. With the path it is possible to contruct the
     correct relative path to folder where macroParam is stored. A path
     should not start or end for slash or backslash (e.g. "euroMakrot"
     or "euroMakrot\analyysi") */
  const std::string& MacroParamRelativePath(void) const;
  void MacroParamRelativePath(const std::string& newValue);

  const float& InitFileVersionNumber(void) const;
  void InitFileVersionNumber(const float& theInitFileVersionNumber);

  /* Multiple parameters are not allowed to be in editing mode at same
     time. Only one allowed. */
  void EditParam(bool newValue);
  bool IsParamEdited(void) const;

  void Unit(const std::string& theUnit);
  const std::string& Unit(void) const;

  /* Is the parameter the current active parameter. Only one parameter
     is active in each view at once. */
  bool IsActive(void) const;
  void Activate(const bool& newState);

  /* What kind of data the class instance contains. By default data type
     is kNoDataType */
  NFmiInfoData::Type DataType(void) const;
  void DataType(const NFmiInfoData::Type& newValue);

  /* Classify the content as a DrawParam from viewMacro. If this is
     a DrawParam from viewmacro then some things are handled
     differently when modifying options. Default value is false.
     This is not stored in file! */
  bool ViewMacroDrawParam(void) const;
  void ViewMacroDrawParam(const bool& newState);

  /* If the class instance is picked from an other DrawParamList value
     is true. Defaul value is false */
  bool BorrowedParam(void) const;
  void BorrowedParam(const bool& newValue);

  /* There is two way to refer to archived data: ModelOriginTime and
     ModelRunIndex. ModelRunIndex tells which forecast run is wanted
     from the newest one. Typically ModelOriginTime is checked first
     and after that is it relative. If ModelOriginTime is set a dataset
     is searched from achived data. Default value is gMissinTime. The
     feature is not in use in that case. */
  const NFmiMetTime& ModelOriginTime(void) const;
  void ModelOriginTime(const NFmiMetTime& newValue);

  /* Archived data index -1 is previous forecast run, -2 before previous,
     -3 and so on from the current one. If the value is non negative
     feature is not in use. The feature can be used to track the latest
     forecast data if there is multiple forecast models in use. */
  int ModelRunIndex(void) const;
  void ModelRunIndex(const int& newValue);

  /* Origin time of relative modelrun which is used e.g. in tooltips. */
  const NFmiMetTime& ModelOriginTimeCalculated(void) const;
  void ModelOriginTimeCalculated(const NFmiMetTime& newValue);

  /* How many forecast runs are shown for the model in a time series.
     Default value is 0 which means that only latest run is shown out. */
  int TimeSerialModelRunCount(void) const;
  void TimeSerialModelRunCount(const int& newValue);

  /* Compare the current modelrun to one of the previous runs.
     Non negative value means that there is not ongoing comparison and
     negative value indicates the modelrun to which the current one
     is compared. */
  int ModelRunDifferenceIndex(void) const;
  void ModelRunDifferenceIndex(const int& newValue);

  /* Model data can be compared to an analysis data or an observation
     data. The data to be compared has only one value for an observation
     time. If value is 0 comparision is not in use. Otherwise a producer
     id of the analysi or observation data to be compared to is used. */
  unsigned long DataComparisonProdId(void) const;
  void DataComparisonProdId(const unsigned long& newValue);

  /* Datatype of the data to be compared to.
     E.g. kAnalyzeData or kObservations */
  NFmiInfoData::Type DataComparisonType(void) const;
  void DataComparisonType(const NFmiInfoData::Type& newValue);

 private:
  NFmiDataIdent itsParameter;
  NFmiLevel itsLevel;
  std::string itsParameterAbbreviation;
  std::string itsInitFileName;
  std::string itsMacroParamRelativePath;
  float itsInitFileVersionNumber;
  bool fEditedParam;
  std::string itsUnit;
  bool fActive;
  NFmiInfoData::Type itsDataType;
  bool fViewMacroDrawParam;
  bool fBorrowedParam;
  NFmiMetTime itsModelOriginTime;
  int itsModelRunIndex;
  NFmiMetTime itsModelOriginTimeCalculated;
  int itsTimeSerialModelRunCount;
  int itsModelRunDifferenceIndex;
  unsigned long itsDataComparisonProdId;
  NFmiInfoData::Type itsDataComparisonType;
};
