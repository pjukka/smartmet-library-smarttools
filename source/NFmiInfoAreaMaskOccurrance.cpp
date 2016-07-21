
#include "NFmiInfoAreaMaskOccurrance.h"
#include "NFmiFastQueryInfo.h"
#include "NFmiDrawParam.h"
#include "NFmiProducerName.h"

// **********************************************************
// *****    NFmiInfoAreaMaskOccurrance  *********************
// **********************************************************

std::function<void(checkedVector<boost::shared_ptr<NFmiFastQueryInfo> > &, boost::shared_ptr<NFmiDrawParam> &, const boost::shared_ptr<NFmiArea> &)> NFmiInfoAreaMaskOccurrance::itsMultiSourceDataGetter; // Alustetaan tyhjäksi ensin

NFmiInfoAreaMaskOccurrance::~NFmiInfoAreaMaskOccurrance(void)
{}

NFmiInfoAreaMaskOccurrance::NFmiInfoAreaMaskOccurrance(const NFmiCalculationCondition & theOperation,
    Type theMaskType,
    NFmiInfoData::Type theDataType,
    const boost::shared_ptr<NFmiFastQueryInfo> &theInfo,
    NFmiAreaMask::FunctionType thePrimaryFunc,
    NFmiAreaMask::FunctionType theSecondaryFunc,
    int theArgumentCount)
    :NFmiInfoAreaMaskProbFunc(theOperation, theMaskType, theDataType, theInfo, thePrimaryFunc, theSecondaryFunc, theArgumentCount)
    , fStationLocationsStoredInData(false)
    , fUseMultiSourceData(false)
{
    fStationLocationsStoredInData = theInfo->HasLatlonInfoInData();
    fUseMultiSourceData = IsKnownMultiSourceData();
}

NFmiInfoAreaMaskOccurrance::NFmiInfoAreaMaskOccurrance(const NFmiInfoAreaMaskOccurrance &theOther)
    : NFmiInfoAreaMaskProbFunc(theOther)
    , fStationLocationsStoredInData(theOther.fStationLocationsStoredInData)
    , fUseMultiSourceData(theOther.fUseMultiSourceData)
{
}

NFmiAreaMask* NFmiInfoAreaMaskOccurrance::Clone(void) const
{
    return new NFmiInfoAreaMaskOccurrance(*this);
}

void NFmiInfoAreaMaskOccurrance::SetMultiSourceDataGetterCallback(const std::function<void(checkedVector<boost::shared_ptr<NFmiFastQueryInfo> > &, boost::shared_ptr<NFmiDrawParam> &, const boost::shared_ptr<NFmiArea> &)> &theCallbackFunction)
{
    NFmiInfoAreaMaskOccurrance::itsMultiSourceDataGetter = theCallbackFunction;
}

// Nyt synop ja salama datat ovat tälläisiä. Tämä on yritys tehdä vähän optimointia muutenkin jo pirun raskaaseen koodiin.
// HUOM! Tämä on riippuvainen NFmiEditMapGeneralDataDoc::MakeDrawedInfoVectorForMapView -metodin erikoistapauksista.
bool NFmiInfoAreaMaskOccurrance::IsKnownMultiSourceData()
{
    if(itsInfo)
    {
        if(itsInfo->DataType() == NFmiInfoData::kFlashData)
            return true;
        // HUOM! kaikkien synop datojen käyttö on aivan liian hidasta, käytetään vain primääri synop dataa laskuissa.
        //if(itsInfo->Producer()->GetIdent() == kFmiSYNOP)
        //    return true;
    }
    return false;
}

// tätä kaytetaan smarttool-modifierin yhteydessä
double NFmiInfoAreaMaskOccurrance::Value(const NFmiCalculationParams &theCalculationParams, bool fUseTimeInterpolationAlways)
{
    // Nämä occurance tetit tehdään erilailla riippuen onko kyse hila vai asema datasta
    if(IsGridData())
    {
        NFmiInfoAreaMaskProbFunc::Value(theCalculationParams, fUseTimeInterpolationAlways);
        return itsConditionFullfilledGridPointCount; // Tähän on laskettu hiladatan tapauksessa emoluokan Value -metodissa halutun ehdon täyttymiset (tai puuttuva arvo)
    }
    else
    {
        // Asemadatalle tehdään omat laskut. Tätä ei ole toteutettu emoluokan laskuissa ollenkaan, koska en oikein nähnyt hyödylliseksi/oikeelliseksi laskea asemadatalle todennäköisyyksiä.
        // Mutta esiintymislukumäärä on ihan ok laskea (mm. salamadatasta salamoiden esiintymiset jne.)

        InitializeFromArguments();
        NFmiLocation location(theCalculationParams.itsLatlon);
        int occurranceCount = 0;

        if(fUseMultiSourceData && itsMultiSourceDataGetter)
        {
            boost::shared_ptr<NFmiArea> dummyAreaPtr; // tällä ei tehdä mitään haussa
            boost::shared_ptr<NFmiDrawParam> drawParam(new NFmiDrawParam(itsInfo->Param(), *itsInfo->Level(), 0, itsInfo->DataType()));
            checkedVector<boost::shared_ptr<NFmiFastQueryInfo>> infos;
            itsMultiSourceDataGetter(infos, drawParam, dummyAreaPtr);
            for(size_t i = 0; i < infos.size(); i++)
                DoCalculations(infos[i], theCalculationParams, location, occurranceCount);
        }
        else
        {
            DoCalculations(itsInfo, theCalculationParams, location, occurranceCount);
        }

        return occurranceCount;
    }
}

void NFmiInfoAreaMaskOccurrance::DoCalculations(boost::shared_ptr<NFmiFastQueryInfo> &theInfo, const NFmiCalculationParams &theCalculationParams, const NFmiLocation &theLocation, int &theOccurranceCountInOut)
{
    fStationLocationsStoredInData = theInfo->HasLatlonInfoInData(); // Eri multi source datoille voi olla erilaiset lokaatio jutut (tosessa synop datassa vakio maaasema, toisessa liikkuva laiva)
    // Lasketaan aikaloopitus rajat
    unsigned long origTimeIndex = theInfo->TimeIndex(); // Otetaan aikaindeksi talteen, jotta se voidaan lopuksi palauttaa takaisin
    unsigned long startTimeIndex = origTimeIndex;
    unsigned long endTimeIndex = origTimeIndex;
    bool doSpecialCalculationDummy = false;

    NFmiMetTime interpolationTimeDummy = NFmiInfoAreaMaskProbFunc::CalcTimeLoopLimits(theInfo, theCalculationParams, itsStartTimeOffsetInHours, itsEndTimeOffsetInHours, &startTimeIndex, &endTimeIndex, &doSpecialCalculationDummy, false);

    for(unsigned long timeIndex = startTimeIndex; timeIndex <= endTimeIndex; timeIndex++)
    {
        theInfo->TimeIndex(timeIndex); // Käydään läpi aikajakso datan originaali aikaresoluutiossa

        for(theInfo->ResetLocation(); theInfo->NextLocation(); ) // Käydään läpi kaikki asemat (miten käydään läpi eri synop datat????)
        {
            // Tarkastetaan jokainen piste erikseen, onko se halutun säteisen ympyrän sisällä
            double distanceInKM = theLocation.Distance(fStationLocationsStoredInData ? theInfo->GetLatlonFromData() : theInfo->LatLon()) * 0.001;
            if(distanceInKM > itsSearchRangeInKM)
                continue; // kyseinen piste oli ympyrän ulkopuolella

            float value = theInfo->FloatValue();
            if(value != kFloatMissing)
            {
                if(CheckProbabilityCondition(value))
                    theOccurranceCountInOut++;
            }
        }
        if(startTimeIndex == gMissingIndex || endTimeIndex == gMissingIndex || startTimeIndex > endTimeIndex)
            break; // time-looppi voi mennä gMissingIndex => gMissingIndex, jolloin ++-operaatio veisi luvun takaisin 0:aan, siksi tämä ehto ja loopin breikki
    }
    theInfo->TimeIndex(origTimeIndex);
}

bool NFmiInfoAreaMaskOccurrance::IsGridData() const
{
    if(itsInfo && itsInfo->Grid())
        return true;
    else
        return false;
}

// **********************************************************
// *****    NFmiInfoAreaMaskOccurrance  *********************
// **********************************************************
