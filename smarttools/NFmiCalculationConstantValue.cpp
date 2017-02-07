//**********************************************************
// C++ Class Name : NFmiCalculationConstantValue
// ---------------------------------------------------
//  Author         : pietarin
//  Creation Date  : 9.11. 2010
//
//**********************************************************

#include "NFmiCalculationConstantValue.h"
#include "NFmiDrawParam.h"
#include <NFmiDataModifier.h>
#include <NFmiDataIterator.h>
#include <NFmiFastQueryInfo.h>

// ****************************************************************************
// *************** NFmiCalculationConstantValue *******************************
// ****************************************************************************

//--------------------------------------------------------
// Constructor/Destructor
//--------------------------------------------------------
NFmiCalculationConstantValue::NFmiCalculationConstantValue(double value) : itsValue(value) {}
NFmiCalculationConstantValue::~NFmiCalculationConstantValue() {}
NFmiCalculationConstantValue::NFmiCalculationConstantValue(
    const NFmiCalculationConstantValue &theOther)
    : NFmiAreaMaskImpl(theOther), itsValue(theOther.itsValue)
{
}

NFmiAreaMask *NFmiCalculationConstantValue::Clone(void) const
{
  return new NFmiCalculationConstantValue(*this);
}

//--------------------------------------------------------
// Value
//--------------------------------------------------------
double NFmiCalculationConstantValue::Value(const NFmiCalculationParams & /* theCalculationParams */,
                                           bool /* fUseTimeInterpolationAlways */)
{
  return itsValue;
}

// ****************************************************************************
// *************** NFmiCalculationConstantValue *******************************
// ****************************************************************************

// ****************************************************************************
// *************** NFmiCalculationSpecialCase *********************************
// ****************************************************************************

NFmiCalculationSpecialCase::NFmiCalculationSpecialCase(NFmiAreaMask::CalculationOperator theValue)
    : NFmiAreaMaskImpl()
{
  SetCalculationOperator(theValue);
}

NFmiCalculationSpecialCase::NFmiCalculationSpecialCase(const NFmiCalculationSpecialCase &theOther)
    : NFmiAreaMaskImpl(theOther)
{
}

NFmiAreaMask *NFmiCalculationSpecialCase::Clone(void) const
{
  return new NFmiCalculationSpecialCase(*this);
}
// ****************************************************************************
// *************** NFmiCalculationSpecialCase *********************************
// ****************************************************************************

// ****************************************************************************
// *************** NFmiCalculationRampFuction *********************************
// ****************************************************************************

NFmiCalculationRampFuction::NFmiCalculationRampFuction(
    const NFmiCalculationCondition &theOperation,
    Type theMaskType,
    NFmiInfoData::Type theDataType,
    boost::shared_ptr<NFmiFastQueryInfo> &theInfo,
    BinaryOperator thePostBinaryOperator)
    : NFmiInfoAreaMask(theOperation, theMaskType, theDataType, theInfo, thePostBinaryOperator)
{
}

NFmiCalculationRampFuction::~NFmiCalculationRampFuction(void) {}
NFmiCalculationRampFuction::NFmiCalculationRampFuction(const NFmiCalculationRampFuction &theOther)
    : NFmiInfoAreaMask(theOther)
{
}

NFmiAreaMask *NFmiCalculationRampFuction::Clone(void) const
{
  return new NFmiCalculationRampFuction(*this);
}

double NFmiCalculationRampFuction::Value(const NFmiCalculationParams &theCalculationParams,
                                         bool fUseTimeInterpolationAlways)
{
  double value = NFmiInfoAreaMask::Value(theCalculationParams, fUseTimeInterpolationAlways);
  return itsMaskCondition.MaskValue(value);
}
// ****************************************************************************
// *************** NFmiCalculationRampFuction *********************************
// ****************************************************************************

// ****************************************************************************
// ************ NFmiCalculationIntegrationFuction *****************************
// ****************************************************************************

NFmiCalculationIntegrationFuction::NFmiCalculationIntegrationFuction(
    boost::shared_ptr<NFmiDataIterator> &theDataIterator,
    boost::shared_ptr<NFmiDataModifier> &theDataModifier,
    Type theMaskType,
    NFmiInfoData::Type theDataType,
    boost::shared_ptr<NFmiFastQueryInfo> &theInfo)
    : NFmiInfoAreaMask(
          NFmiCalculationCondition(), theMaskType, theDataType, theInfo, NFmiAreaMask::kNoValue),
      itsDataModifier(theDataModifier),
      itsDataIterator(theDataIterator)
{
}

NFmiCalculationIntegrationFuction::~NFmiCalculationIntegrationFuction(void) {}
double NFmiCalculationIntegrationFuction::Value(const NFmiCalculationParams &theCalculationParams,
                                                bool /* fUseTimeInterpolationAlways */)
{
  // HUOM!!! T‰h‰n tuli pikaviritys:
  // asetan vain l‰himm‰n pisteen ja ajan kohdalleen.
  if (itsInfo->NearestPoint(theCalculationParams.itsLatlon) &&
      itsInfo->TimeToNearestStep(theCalculationParams.itsTime, kForward))
  {
    itsDataIterator->DoForEach(itsDataModifier.get());
    return itsDataModifier->CalculationResult();
  }
  return kFloatMissing;
}
// ****************************************************************************
// ************ NFmiCalculationIntegrationFuction *****************************
// ****************************************************************************

// ****************************************************************************
// ********** NFmiCalculationRampFuctionWithAreaMask **************************
// ****************************************************************************

// Ramppifunktioiden laskut AreaMask:ien avulla (mm. lat, lon ja elevationangle tapaukset).
double NFmiCalculationRampFuctionWithAreaMask::Value(
    const NFmiCalculationParams &theCalculationParams, bool fUseTimeInterpolationAlways)
{
  itsAreaMask->Time(theCalculationParams.itsTime);
  double value = itsAreaMask->Value(theCalculationParams, fUseTimeInterpolationAlways);
  return itsMaskCondition.MaskValue(value);
}

NFmiCalculationRampFuctionWithAreaMask::NFmiCalculationRampFuctionWithAreaMask(
    const NFmiCalculationCondition &theOperation,
    Type theMaskType,
    NFmiInfoData::Type theDataType,
    boost::shared_ptr<NFmiAreaMask> &theAreaMask,
    BinaryOperator thePostBinaryOperator)
    : NFmiAreaMaskImpl(theOperation, theMaskType, theDataType, thePostBinaryOperator),
      itsAreaMask(theAreaMask),
      fIsTimeIntepolationNeededInValue(false)
{
}

NFmiCalculationRampFuctionWithAreaMask::~NFmiCalculationRampFuctionWithAreaMask(void) {}
NFmiCalculationRampFuctionWithAreaMask::NFmiCalculationRampFuctionWithAreaMask(
    const NFmiCalculationRampFuctionWithAreaMask &theOther)
    : NFmiAreaMaskImpl(theOther),
      itsAreaMask(theOther.itsAreaMask ? theOther.itsAreaMask->Clone() : 0),
      fIsTimeIntepolationNeededInValue(theOther.fIsTimeIntepolationNeededInValue)
{
}

NFmiAreaMask *NFmiCalculationRampFuctionWithAreaMask::Clone(void) const
{
  return new NFmiCalculationRampFuctionWithAreaMask(*this);
}
// ****************************************************************************
// ********** NFmiCalculationRampFuctionWithAreaMask **************************
// ****************************************************************************

// ****************************************************************************
// **************** NFmiCalculationDeltaZValue ********************************
// ****************************************************************************
double NFmiCalculationDeltaZValue::itsHeightValue;

NFmiCalculationDeltaZValue::NFmiCalculationDeltaZValue(void) : NFmiAreaMaskImpl() {}
NFmiCalculationDeltaZValue::NFmiCalculationDeltaZValue(const NFmiCalculationDeltaZValue &theOther)
    : NFmiAreaMaskImpl(theOther)
{
}

NFmiAreaMask *NFmiCalculationDeltaZValue::Clone(void) const
{
  return new NFmiCalculationDeltaZValue(*this);
}
// ****************************************************************************
// **************** NFmiCalculationDeltaZValue ********************************
// ****************************************************************************

#ifdef FMI_SUPPORT_STATION_DATA_SMARTTOOL

#include "NFmiStationView.h"

// ****************************************************************************
// ****************** NFmiStation2GridMask ************************************
// ****************************************************************************

NFmiStation2GridMask::NFmiStation2GridMask(Type theMaskType,
                                           NFmiInfoData::Type theDataType,
                                           boost::shared_ptr<NFmiFastQueryInfo> &theInfo)
    : NFmiInfoAreaMask(
          NFmiCalculationCondition(), theMaskType, theDataType, theInfo, NFmiAreaMask::kNoValue),
      itsGriddedStationData(new DataCache()),
      itsCurrentGriddedStationData(0),
      itsLastCalculatedTime(NFmiMetTime::gMissingTime),
      itsAreaPtr(),
      itsDoc(0),
      itsStation2GridSize(1, 1),
      itsCacheMutex(new MutexType())
{
}

NFmiStation2GridMask::~NFmiStation2GridMask(void) {}
NFmiStation2GridMask::NFmiStation2GridMask(const NFmiStation2GridMask &theOther)
    : NFmiInfoAreaMask(theOther),
      itsGriddedStationData(theOther.itsGriddedStationData),
      itsCurrentGriddedStationData(0)  // t‰m‰ laitetaan aina 0:ksi
      ,
      itsLastCalculatedTime(theOther.itsLastCalculatedTime),
      itsAreaPtr(theOther.itsAreaPtr.get() ? theOther.itsAreaPtr.get()->Clone() : 0),
      itsDoc(theOther.itsDoc),
      itsStation2GridSize(theOther.itsStation2GridSize),
      itsCacheMutex(theOther.itsCacheMutex)
{
}

NFmiAreaMask *NFmiStation2GridMask::Clone(void) const { return new NFmiStation2GridMask(*this); }
double NFmiStation2GridMask::Value(const NFmiCalculationParams &theCalculationParams,
                                   bool /* fUseTimeInterpolationAlways */)
{
  DoGriddingCheck(theCalculationParams);
  if (itsCurrentGriddedStationData)
    return itsCurrentGriddedStationData->GetValue(theCalculationParams.itsLocationIndex,
                                                  kFloatMissing);
  else
    return kFloatMissing;
}

void NFmiStation2GridMask::SetGriddingHelpers(NFmiArea *theArea,
                                              NFmiEditMapGeneralDataDoc *theDoc,
                                              const NFmiPoint &theStation2GridSize)
{
  itsAreaPtr.reset(theArea->Clone());
  itsDoc = theDoc;
  itsStation2GridSize = theStation2GridSize;
}

void NFmiStation2GridMask::DoGriddingCheck(const NFmiCalculationParams &theCalculationParams)
{
  if (itsLastCalculatedTime != theCalculationParams.itsTime)
  {
    WriteLock lock(*itsCacheMutex);  // t‰st‰ saa edet‰ vain yksi s‰ie kerrallaan, eli joku joka
                                     // ehtii, laskee cache-matriisin ja asettaa sen
                                     // itsCurrentGriddedStationData:n arvoksi, muut sitten vain
                                     // k‰ytt‰v‰t sit‰

    // katsotaanko lˆytyykˆ valmiiksi laskettua hilaa halutulle ajalle
    DataCache::iterator it = itsGriddedStationData->find(theCalculationParams.itsTime);
    if (it != itsGriddedStationData->end())
      itsCurrentGriddedStationData = &((*it).second);
    else
    {
      // lasketaan halutun ajan hila
      if (itsDoc && itsAreaPtr.get())
      {
        boost::shared_ptr<NFmiDrawParam> drawParam(
            new NFmiDrawParam(itsDataIdent, itsLevel, 0, itsDataType));
        NFmiDataMatrix<float> griddedData(
            static_cast<NFmiDataMatrix<float>::size_type>(itsStation2GridSize.X()),
            static_cast<NFmiDataMatrix<float>::size_type>(itsStation2GridSize.Y()),
            kFloatMissing);
        NFmiStationView::GridStationData(
            itsDoc, itsAreaPtr, drawParam, griddedData, theCalculationParams.itsTime);
        std::pair<DataCache::iterator, bool> insertResult = itsGriddedStationData->insert(
            std::make_pair(theCalculationParams.itsTime, griddedData));
        if (insertResult.second)
          itsCurrentGriddedStationData = &((*insertResult.first).second);
        else
          itsCurrentGriddedStationData = 0;
      }
    }
    itsLastCalculatedTime = theCalculationParams.itsTime;
  }
}
// ****************************************************************************
// ****************** NFmiStation2GridMask ************************************
// ****************************************************************************

// ****************************************************************************
// *************** NFmiNearestObsValue2GridMask *******************************
// ****************************************************************************

NFmiNearestObsValue2GridMask::NFmiNearestObsValue2GridMask(
    Type theMaskType,
    NFmiInfoData::Type theDataType,
    boost::shared_ptr<NFmiFastQueryInfo> &theInfo,
    NFmiAreaMask::FunctionType thePrimaryFunc,
    NFmiAreaMask::FunctionType theSecondaryFunc,
    int theArgumentCount)
    : NFmiInfoAreaMask(
          NFmiCalculationCondition(), theMaskType, theDataType, theInfo, NFmiAreaMask::kNoValue),
      itsNearestObsValuesData(new DataCache()),
      itsCurrentNearestObsValuesData(0),
      itsLastCalculatedTime(NFmiMetTime::gMissingTime),
      itsPrimaryFunc(thePrimaryFunc),
      itsSecondaryFunc(theSecondaryFunc),
      itsAreaPtr(),
      itsDoc(0),
      itsResultGridSize(1, 1),
      itsCacheMutex(new MutexType())
{
  itsFunctionArgumentCount = theArgumentCount;
}

NFmiNearestObsValue2GridMask::~NFmiNearestObsValue2GridMask(void) {}
NFmiNearestObsValue2GridMask::NFmiNearestObsValue2GridMask(
    const NFmiNearestObsValue2GridMask &theOther)
    : NFmiInfoAreaMask(theOther),
      itsNearestObsValuesData(theOther.itsNearestObsValuesData),
      itsCurrentNearestObsValuesData(0)  // t‰m‰ laitetaan aina 0:ksi
      ,
      itsLastCalculatedTime(theOther.itsLastCalculatedTime),
      itsPrimaryFunc(theOther.itsPrimaryFunc),
      itsSecondaryFunc(theOther.itsSecondaryFunc),
      itsAreaPtr(theOther.itsAreaPtr.get() ? theOther.itsAreaPtr.get()->Clone() : 0),
      itsDoc(theOther.itsDoc),
      itsResultGridSize(theOther.itsResultGridSize),
      itsCacheMutex(theOther.itsCacheMutex)
{
}

NFmiAreaMask *NFmiNearestObsValue2GridMask::Clone(void) const
{
  return new NFmiNearestObsValue2GridMask(*this);
}

double NFmiNearestObsValue2GridMask::Value(const NFmiCalculationParams &theCalculationParams,
                                           bool /* fUseTimeInterpolationAlways */)
{
  DoNearestValueGriddingCheck(theCalculationParams);
  if (itsCurrentNearestObsValuesData)
    return itsCurrentNearestObsValuesData->GetValue(theCalculationParams.itsLocationIndex,
                                                    kFloatMissing);
  else
    return kFloatMissing;
}

void NFmiNearestObsValue2GridMask::SetGriddingHelpers(NFmiArea *theArea,
                                                      NFmiEditMapGeneralDataDoc *theDoc,
                                                      const NFmiPoint &theResultGridSize)
{
  itsAreaPtr.reset(theArea->Clone());
  itsDoc = theDoc;
  itsResultGridSize = theResultGridSize;
}

void NFmiNearestObsValue2GridMask::SetArguments(std::vector<float> &theArgumentVector)
{
  // jokaiselle pisteelle ja ajanhetkelle annetaan eri argumentit t‰ss‰
  itsArgumentVector = theArgumentVector;
  if (static_cast<int>(itsArgumentVector.size()) !=
      itsFunctionArgumentCount - 1)  // -1 tarkoittaa ett‰ funktion 1. argumentti tulee suoraan
                                     // itsIfo:sta, eli sit‰ ei anneta argumentti-listassa
    throw std::runtime_error(
        "Internal SmartMet error: Probability function was given invalid number of arguments, "
        "cannot calculate the macro.");
}

static NFmiDataMatrix<float> CalcNearestValueMatrix(
    const NFmiLevel &theLevel,
    const NFmiCalculationParams &theCalculationParams,
    const NFmiPoint &theResultGridSize,
    checkedVector<boost::shared_ptr<NFmiFastQueryInfo> > &theInfoVector,
    boost::shared_ptr<NFmiArea> &theAreaPtr,
    float theTimePeekInHours)
{
  // Luodaan tulos matriisi t‰ytettyn‰ puuttuvilla arvoilla
  NFmiDataMatrix<float> nearestValueMatrix(
      static_cast<NFmiDataMatrix<float>::size_type>(theResultGridSize.X()),
      static_cast<NFmiDataMatrix<float>::size_type>(theResultGridSize.Y()),
      kFloatMissing);
  if (theAreaPtr && theInfoVector.size())
  {
    const double kMissingDistanceValue =
        9999999999.;  // et‰isyys matriisi alustetaan t‰ll‰ suurella luvulla, jolloin aina riitt‰‰
                      // yksi < -vertailu, kun etsit‰‰n l‰hint‰ asemaa
    NFmiDataMatrix<double> distanceMatrix(
        nearestValueMatrix.NX(), nearestValueMatrix.NY(), kMissingDistanceValue);
    NFmiGrid grid(theAreaPtr.get(),
                  static_cast<unsigned long>(nearestValueMatrix.NX()),
                  static_cast<unsigned long>(nearestValueMatrix.NY()));
    for (auto infoIter : theInfoVector)
    {
      // data ei saa olla hiladataa, eik‰ ns. laivadataa (lokaatio muuttuu ajan myˆt‰ ja lat/lon
      // arvot ovat erillisi‰ parametreja)
      if (!infoIter->IsGrid() && !NFmiStationView::IsInfoShipTypeData(*infoIter))
      {
        NFmiMetTime wantedTime(theCalculationParams.itsTime);
        if (theTimePeekInHours)
        {  // jos halutaan kurkata ajassa eteen/taakse, tehd‰‰n ajan asetus t‰ss‰
          long changeByMinutesValue = FmiRound(theTimePeekInHours * 60);
          wantedTime.ChangeByMinutes(changeByMinutesValue);
        }
        if (infoIter->Time(wantedTime))
        {
          for (infoIter->ResetLocation(); infoIter->NextLocation();)
          {
            const NFmiLocation *location = infoIter->Location();
            const NFmiPoint &latlon(location->GetLocation());
            if (theAreaPtr->IsInside(latlon))  // aseman pit‰‰ olla kartta-alueen sis‰ll‰
            {
              // Katsotaan mit‰ tuloshilan hilapistett‰ l‰hinn‰ t‰m‰ asema oli.
              if (grid.NearestLatLon(latlon.X(), latlon.Y()))
              {
                double distance = infoIter->Location()->Distance(grid.LatLon());
                if (distance < distanceMatrix[grid.CurrentX()][grid.CurrentY()])
                {
                  distanceMatrix[grid.CurrentX()][grid.CurrentY()] = distance;
                  NFmiStationView::SetSoundingDataLevel(
                      theLevel, *infoIter);  // T‰m‰ tehd‰‰n vain luotaus datalle: t‰m‰ level pit‰‰
                                             // asettaa joka pisteelle erikseen, koska vakio
                                             // painepinnat eiv‰t ole kaikille luotaus parametreille
                                             // samoilla leveleill‰
                  nearestValueMatrix[grid.CurrentX()][grid.CurrentY()] =
                      infoIter->FloatValue();  // ei ole v‰li‰ onko l‰himm‰n aseman arvo puuttuva
                                               // vai ei, se halutaan aina tulokseen!!!
                }
              }
            }
          }
        }
      }
    }
  }
  return nearestValueMatrix;
}

#include "NFmiEditMapGeneralDataDoc.h"

void NFmiNearestObsValue2GridMask::DoNearestValueGriddingCheck(
    const NFmiCalculationParams &theCalculationParams)
{
  if (itsLastCalculatedTime != theCalculationParams.itsTime)
  {
    WriteLock lock(*itsCacheMutex);  // t‰st‰ saa edet‰ vain yksi s‰ie kerrallaan, eli joku joka
                                     // ehtii, laskee cache-matriisin ja asettaa sen
                                     // itsCurrentGriddedStationData:n arvoksi, muut sitten vain
                                     // k‰ytt‰v‰t sit‰

    // katsotaanko lˆytyykˆ valmiiksi laskettua hilaa halutulle ajalle
    DataCache::iterator it = itsNearestObsValuesData->find(theCalculationParams.itsTime);
    if (it != itsNearestObsValuesData->end())
      itsCurrentNearestObsValuesData = &((*it).second);
    else
    {
      // lasketaan halutun ajan hila
      if (itsDoc && itsAreaPtr.get())
      {
        // otetaan argumenttina annettu arvo aika hypp‰ykseen
        float timePeekInHours = itsArgumentVector[0];  // kuinka paljon kurkataan ajassa
                                                       // eteen/taakse kun arvoa haetaan t‰h‰n
                                                       // ajanhetkeen

        boost::shared_ptr<NFmiDrawParam> drawParam(
            new NFmiDrawParam(itsDataIdent, itsLevel, 0, itsDataType));
        checkedVector<boost::shared_ptr<NFmiFastQueryInfo> > infoVector;  // t‰h‰n haetaan
                                                                          // tarvittavat datat
                                                                          // (synopin tapauksessa
                                                                          // mahdollisesti lista)
        itsDoc->MakeDrawedInfoVectorForMapView(infoVector, drawParam, itsAreaPtr);
        NFmiDataMatrix<float> nearestValueData = ::CalcNearestValueMatrix(itsLevel,
                                                                          theCalculationParams,
                                                                          itsResultGridSize,
                                                                          infoVector,
                                                                          itsAreaPtr,
                                                                          timePeekInHours);
        std::pair<DataCache::iterator, bool> insertResult = itsNearestObsValuesData->insert(
            std::make_pair(theCalculationParams.itsTime, nearestValueData));
        if (insertResult.second)
          itsCurrentNearestObsValuesData = &((*insertResult.first).second);
        else
          itsCurrentNearestObsValuesData = 0;
      }
    }
    itsLastCalculatedTime = theCalculationParams.itsTime;
  }
}
// ****************************************************************************
// *************** NFmiNearestObsValue2GridMask *******************************
// ****************************************************************************

#endif  // FMI_SUPPORT_STATION_DATA_SMARTTOOL
