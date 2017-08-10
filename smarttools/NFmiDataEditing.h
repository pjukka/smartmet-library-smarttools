#pragma once

class NFmiDataEditing
{
 public:
  NFmiDataEditing();
  NFmiDataEditing(const NFmiDataEditing& other);
  virtual ~NFmiDataEditing();

  double ModifyingStep(void) const;
  void ModifyingStep(const double theModifyingStep);

  /* Minimum and maximum value of change axel of timeseries view */
  double TimeSerialModifyingLimit(void) const;
  void TimeSerialModifyingLimit(const double& newValue);

  double AbsoluteMinValue(void) const;
  void AbsoluteMinValue(const double& theAbsoluteMinValue);

  double AbsoluteMaxValue(void) const;
  void AbsoluteMaxValue(const double& theAbsoluteMaxValue);

  /* Used in editor of timeseries. */
  double TimeSeriesScaleMin(void) const;
  void TimeSeriesScaleMin(const double& theValue);

  /* Used in editor of timeseries. */
  double TimeSeriesScaleMax(void) const;
  void TimeSeriesScaleMax(const double& theValue);

 private:
  NFmiDataEditing& operator=(const NFmiDataEditing& other) = delete;

  double itsModifyingStep;
  double itsTimeSerialModifyingLimit;

  double itsAbsoluteMinValue;
  double itsAbsoluteMaxValue;

  double itsTimeSeriesScaleMin;
  double itsTimeSeriesScaleMax;
};
