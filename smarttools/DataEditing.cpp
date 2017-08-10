#include "DataEditing.h"

DataEditing::DataEditing()
    : itsModifyingStep(1.),
      itsTimeSerialModifyingLimit(10),
      itsAbsoluteMinValue(-1000000000),
      itsAbsoluteMaxValue(1000000000),
      itsTimeSeriesScaleMin(0),
      itsTimeSeriesScaleMax(100)
{
}

DataEditing::DataEditing(const DataEditing& other)
    : itsModifyingStep(other.ModifyingStep()),
      itsTimeSerialModifyingLimit(other.TimeSerialModifyingLimit()),
      itsAbsoluteMinValue(other.AbsoluteMinValue()),
      itsAbsoluteMaxValue(other.AbsoluteMaxValue()),
      itsTimeSeriesScaleMin(other.TimeSeriesScaleMin()),
      itsTimeSeriesScaleMax(other.TimeSeriesScaleMax())
{
}

DataEditing::~DataEditing() {}

double DataEditing::ModifyingStep(void) const { return itsModifyingStep; }
void DataEditing::ModifyingStep(const double theModifyingStep)
{
  itsModifyingStep = theModifyingStep;
}

double DataEditing::TimeSerialModifyingLimit(void) const { return itsTimeSerialModifyingLimit; }
void DataEditing::TimeSerialModifyingLimit(const double& newValue)
{
  itsTimeSerialModifyingLimit = newValue;
}

double DataEditing::AbsoluteMinValue(void) const { return itsAbsoluteMinValue; }
void DataEditing::AbsoluteMinValue(const double& theAbsoluteMinValue)
{
  itsAbsoluteMinValue = theAbsoluteMinValue;
}

double DataEditing::AbsoluteMaxValue(void) const { return itsAbsoluteMaxValue; }
void DataEditing::AbsoluteMaxValue(const double& theAbsoluteMaxValue)
{
  itsAbsoluteMaxValue = theAbsoluteMaxValue;
}

double DataEditing::TimeSeriesScaleMin(void) const { return itsTimeSeriesScaleMin; }
void DataEditing::TimeSeriesScaleMin(const double& theValue) { itsTimeSeriesScaleMin = theValue; }

double DataEditing::TimeSeriesScaleMax(void) const { return itsTimeSeriesScaleMax; }
void DataEditing::TimeSeriesScaleMax(const double& theValue) { itsTimeSeriesScaleMax = theValue; }
