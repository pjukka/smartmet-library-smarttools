#include "NFmiDataEditing.h"

NFmiDataEditing::NFmiDataEditing()
    : itsModifyingStep(1.),
      itsTimeSerialModifyingLimit(10),
      itsAbsoluteMinValue(-1000000000),
      itsAbsoluteMaxValue(1000000000),
      itsTimeSeriesScaleMin(0),
      itsTimeSeriesScaleMax(100)
{
}

NFmiDataEditing::NFmiDataEditing(const NFmiDataEditing& other)
    : itsModifyingStep(other.ModifyingStep()),
      itsTimeSerialModifyingLimit(other.TimeSerialModifyingLimit()),
      itsAbsoluteMinValue(other.AbsoluteMinValue()),
      itsAbsoluteMaxValue(other.AbsoluteMaxValue()),
      itsTimeSeriesScaleMin(other.TimeSeriesScaleMin()),
      itsTimeSeriesScaleMax(other.TimeSeriesScaleMax())
{
}

NFmiDataEditing::~NFmiDataEditing() {}

double NFmiDataEditing::ModifyingStep(void) const { return itsModifyingStep; }
void NFmiDataEditing::ModifyingStep(const double theModifyingStep)
{
  itsModifyingStep = theModifyingStep;
}

double NFmiDataEditing::TimeSerialModifyingLimit(void) const { return itsTimeSerialModifyingLimit; }
void NFmiDataEditing::TimeSerialModifyingLimit(const double& newValue)
{
  itsTimeSerialModifyingLimit = newValue;
}

double NFmiDataEditing::AbsoluteMinValue(void) const { return itsAbsoluteMinValue; }
void NFmiDataEditing::AbsoluteMinValue(const double& theAbsoluteMinValue)
{
  itsAbsoluteMinValue = theAbsoluteMinValue;
}

double NFmiDataEditing::AbsoluteMaxValue(void) const { return itsAbsoluteMaxValue; }
void NFmiDataEditing::AbsoluteMaxValue(const double& theAbsoluteMaxValue)
{
  itsAbsoluteMaxValue = theAbsoluteMaxValue;
}

double NFmiDataEditing::TimeSeriesScaleMin(void) const { return itsTimeSeriesScaleMin; }
void NFmiDataEditing::TimeSeriesScaleMin(const double& theValue)
{
  itsTimeSeriesScaleMin = theValue;
}

double NFmiDataEditing::TimeSeriesScaleMax(void) const { return itsTimeSeriesScaleMax; }
void NFmiDataEditing::TimeSeriesScaleMax(const double& theValue)
{
  itsTimeSeriesScaleMax = theValue;
}
