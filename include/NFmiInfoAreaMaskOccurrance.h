#pragma once

#include "NFmiInfoAreaMask.h"

class NFmiDrawParam;

class _FMI_DLL NFmiInfoAreaMaskOccurrance : public NFmiInfoAreaMaskProbFunc
{
public:
    ~NFmiInfoAreaMaskOccurrance(void);
    NFmiInfoAreaMaskOccurrance(const NFmiCalculationCondition & theOperation,
        Type theMaskType,
        NFmiInfoData::Type theDataType,
        const boost::shared_ptr<NFmiFastQueryInfo> &theInfo,
        NFmiAreaMask::FunctionType thePrimaryFunc,
        NFmiAreaMask::FunctionType theSecondaryFunc,
        int theArgumentCount);
    NFmiInfoAreaMaskOccurrance(const NFmiInfoAreaMaskOccurrance &theOther);
    NFmiAreaMask* Clone(void) const;

    static void SetMultiSourceDataGetterCallback(const std::function<void(checkedVector<boost::shared_ptr<NFmiFastQueryInfo> > &, boost::shared_ptr<NFmiDrawParam> &, const boost::shared_ptr<NFmiArea> &)> &theCallbackFunction);

    // tätä kaytetaan smarttool-modifierin yhteydessä
    double Value(const NFmiCalculationParams &theCalculationParams, bool fUseTimeInterpolationAlways);

protected:
    bool IsGridData() const;
    bool IsKnownMultiSourceData(); // nyt synop ja salama datat ovat tälläisiä
    void DoCalculations(boost::shared_ptr<NFmiFastQueryInfo> &theInfo, const NFmiCalculationParams &theCalculationParams, const NFmiLocation &theLocation, int &theOccurranceCountInOut);

    bool fStationLocationsStoredInData; // Esim. salama ja SHIP datoissa kunkin ajan ja paikan latlon sijainti on tallessa Longitude ja Latitude parametreissa
    bool fUseMultiSourceData;

    static std::function<void(checkedVector<boost::shared_ptr<NFmiFastQueryInfo> > &, boost::shared_ptr<NFmiDrawParam> &, const boost::shared_ptr<NFmiArea> &)> itsMultiSourceDataGetter;

private:
    NFmiInfoAreaMaskProbFunc & operator=(const NFmiInfoAreaMaskProbFunc & theMask);
};
