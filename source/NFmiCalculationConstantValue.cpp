//**********************************************************
// C++ Class Name : NFmiCalculationConstantValue 
// ---------------------------------------------------  
//  Author         : pietarin 
//  Creation Date  : 9.11. 2010 
// 
//**********************************************************
#include "NFmiCalculationConstantValue.h"
#include "NFmiDataModifier.h"
#include "NFmiDataIterator.h"
#include "NFmiFastQueryInfo.h"
#include "NFmiDrawParam.h"

//--------------------------------------------------------
// Constructor/Destructor 
//--------------------------------------------------------
NFmiCalculationConstantValue::NFmiCalculationConstantValue(double value)
:itsValue(value)
{
}
NFmiCalculationConstantValue::~NFmiCalculationConstantValue()
{
}

NFmiCalculationConstantValue::NFmiCalculationConstantValue(const NFmiCalculationConstantValue &theOther)
:NFmiAreaMaskImpl(theOther)
,itsValue(theOther.itsValue)
{
}

NFmiAreaMask* NFmiCalculationConstantValue::Clone(void) const
{
	return new NFmiCalculationConstantValue(*this);
}

//--------------------------------------------------------
// Value 
//--------------------------------------------------------
double NFmiCalculationConstantValue::Value(const NFmiCalculationParams &theCalculationParams, bool fUseTimeInterpolationAlways)
{
	return itsValue;
}

NFmiCalculationSpecialCase::NFmiCalculationSpecialCase(NFmiAreaMask::CalculationOperator theValue)
:NFmiAreaMaskImpl()
{
	SetCalculationOperator(theValue);
}

NFmiCalculationSpecialCase::NFmiCalculationSpecialCase(const NFmiCalculationSpecialCase &theOther)
:NFmiAreaMaskImpl(theOther)
{
}

NFmiAreaMask* NFmiCalculationSpecialCase::Clone(void) const
{
	return new NFmiCalculationSpecialCase(*this);
}


NFmiCalculationRampFuction::NFmiCalculationRampFuction(const NFmiCalculationCondition& theOperation, Type theMaskType, NFmiInfoData::Type theDataType, boost::shared_ptr<NFmiFastQueryInfo> &theInfo, BinaryOperator thePostBinaryOperator)
:NFmiInfoAreaMask(theOperation, theMaskType, theDataType, theInfo, thePostBinaryOperator)
{
}

NFmiCalculationRampFuction::~NFmiCalculationRampFuction(void)
{}

NFmiCalculationRampFuction::NFmiCalculationRampFuction(const NFmiCalculationRampFuction &theOther)
:NFmiInfoAreaMask(theOther)
{
}

NFmiAreaMask* NFmiCalculationRampFuction::Clone(void) const
{
	return new NFmiCalculationRampFuction(*this);
}

double NFmiCalculationRampFuction::Value(const NFmiCalculationParams &theCalculationParams, bool fUseTimeInterpolationAlways)
{
	double value = NFmiInfoAreaMask::Value(theCalculationParams, fUseTimeInterpolationAlways);
	return itsMaskCondition.MaskValue(value);
}


NFmiCalculationIntegrationFuction::NFmiCalculationIntegrationFuction(boost::shared_ptr<NFmiDataIterator> &theDataIterator, boost::shared_ptr<NFmiDataModifier> &theDataModifier, Type theMaskType, NFmiInfoData::Type theDataType, boost::shared_ptr<NFmiFastQueryInfo> &theInfo)
:NFmiInfoAreaMask(NFmiCalculationCondition(), theMaskType, theDataType, theInfo, NFmiAreaMask::kNoValue)
,itsDataModifier(theDataModifier)
,itsDataIterator(theDataIterator)
{
}

NFmiCalculationIntegrationFuction::~NFmiCalculationIntegrationFuction(void)
{
}

double NFmiCalculationIntegrationFuction::Value(const NFmiCalculationParams &theCalculationParams, bool fUseTimeInterpolationAlways)
{
	// HUOM!!! Tähän tuli pikaviritys:
	// asetan vain lähimmän pisteen ja ajan kohdalleen.
	if(itsInfo->NearestPoint(theCalculationParams.itsLatlon) && itsInfo->TimeToNearestStep(theCalculationParams.itsTime, kForward))
	{
		itsDataIterator->DoForEach(itsDataModifier.get());
		return itsDataModifier->CalculationResult();
	}
	return kFloatMissing;
}

// Ramppifunktioiden laskut AreaMask:ien avulla (mm. lat, lon ja elevationangle tapaukset).
double NFmiCalculationRampFuctionWithAreaMask::Value(const NFmiCalculationParams &theCalculationParams, bool fUseTimeInterpolationAlways)
{
	itsAreaMask->Time(theCalculationParams.itsTime);
	double value = itsAreaMask->Value(theCalculationParams, fUseTimeInterpolationAlways);
	return itsMaskCondition.MaskValue(value);
}

NFmiCalculationRampFuctionWithAreaMask::NFmiCalculationRampFuctionWithAreaMask(const NFmiCalculationCondition & theOperation,
																			   Type theMaskType,
																			   NFmiInfoData::Type theDataType,
																			   boost::shared_ptr<NFmiAreaMask> &theAreaMask,
																			   BinaryOperator thePostBinaryOperator)
:NFmiAreaMaskImpl(theOperation, theMaskType, theDataType, thePostBinaryOperator)
,itsAreaMask(theAreaMask)
,fIsTimeIntepolationNeededInValue(false)
{
}

NFmiCalculationRampFuctionWithAreaMask::~NFmiCalculationRampFuctionWithAreaMask(void)
{
}

NFmiCalculationRampFuctionWithAreaMask::NFmiCalculationRampFuctionWithAreaMask(const NFmiCalculationRampFuctionWithAreaMask &theOther)
:NFmiAreaMaskImpl(theOther)
,itsAreaMask(theOther.itsAreaMask ? theOther.itsAreaMask->Clone() : 0)
,fIsTimeIntepolationNeededInValue(theOther.fIsTimeIntepolationNeededInValue)
{
}

NFmiAreaMask* NFmiCalculationRampFuctionWithAreaMask::Clone(void) const
{
	return new NFmiCalculationRampFuctionWithAreaMask(*this);
}

double NFmiCalculationDeltaZValue::itsHeightValue;

NFmiCalculationDeltaZValue::NFmiCalculationDeltaZValue(void)
:NFmiAreaMaskImpl()
{
}

NFmiCalculationDeltaZValue::NFmiCalculationDeltaZValue(const NFmiCalculationDeltaZValue &theOther)
:NFmiAreaMaskImpl(theOther)
{
}

NFmiAreaMask* NFmiCalculationDeltaZValue::Clone(void) const
{
	return new NFmiCalculationDeltaZValue(*this);
}

#ifdef FMI_SUPPORT_STATION_DATA_SMARTTOOL

#include "NFmiStationView.h"


NFmiStation2GridMask::NFmiStation2GridMask(Type theMaskType, NFmiInfoData::Type theDataType, boost::shared_ptr<NFmiFastQueryInfo> &theInfo)
:NFmiInfoAreaMask(NFmiCalculationCondition(), theMaskType, theDataType, theInfo, NFmiAreaMask::kNoValue)
,itsGriddedStationData()
,itsCurrentGriddedStationData(0)
,itsLastCalculatedTime(NFmiMetTime::gMissingTime)
,itsAreaPtr()
,itsDoc(0)
,itsStation2GridSize(1,1)
{
}

NFmiStation2GridMask::~NFmiStation2GridMask(void)
{
}

NFmiStation2GridMask::NFmiStation2GridMask(const NFmiStation2GridMask &theOther)
:NFmiInfoAreaMask(theOther)
,itsGriddedStationData(theOther.itsGriddedStationData)
,itsCurrentGriddedStationData(0) // tämä laitetaan aina 0:ksi
,itsLastCalculatedTime(theOther.itsLastCalculatedTime)
,itsAreaPtr(theOther.itsAreaPtr.get() ? theOther.itsAreaPtr.get()->Clone() : 0)
,itsDoc(theOther.itsDoc)
,itsStation2GridSize(theOther.itsStation2GridSize)
{
}

NFmiAreaMask* NFmiStation2GridMask::Clone(void) const
{
}

double NFmiStation2GridMask::Value(const NFmiCalculationParams &theCalculationParams, bool fUseTimeInterpolationAlways)
{
	DoGriddingCheck(theCalculationParams);
	if(itsCurrentGriddedStationData)
		return itsCurrentGriddedStationData->GetValue(theCalculationParams.itsLocationIndex, kFloatMissing);
	else
		return kFloatMissing;
}

void NFmiStation2GridMask::SetGriddingHelpers(NFmiArea *theArea, NFmiEditMapGeneralDataDoc *theDoc, const NFmiPoint &theStation2GridSize)
{
	itsAreaPtr.reset(theArea->Clone());
	itsDoc = theDoc;
	itsStation2GridSize = theStation2GridSize;
}

void NFmiStation2GridMask::DoGriddingCheck(const NFmiCalculationParams &theCalculationParams)
{
	if(itsLastCalculatedTime != theCalculationParams.itsTime)
	{ 
		// katsotaanko löytyykö valmiiksi laskettua hilaa halutulle ajalle
		DataCache::iterator it = itsGriddedStationData.find(theCalculationParams.itsTime);
		if(it != itsGriddedStationData.end())
			itsCurrentGriddedStationData = &((*it).second);
		else
		{
			// lasketaan halutun ajan hila
			if(itsDoc && itsAreaPtr.get())
			{
				boost::shared_ptr<NFmiDrawParam> drawParam(new NFmiDrawParam(itsDataIdent, itsLevel, 0, itsDataType));
				NFmiDataMatrix<float> griddedData(static_cast<NFmiDataMatrix<float>::size_type>(itsStation2GridSize.X()), static_cast<NFmiDataMatrix<float>::size_type>(itsStation2GridSize.Y()), kFloatMissing);
				NFmiStationView::GridStationData(itsDoc, itsAreaPtr.get(), drawParam, griddedData, theCalculationParams.itsTime);
				std::pair<DataCache::iterator, bool> insertResult = itsGriddedStationData.insert(std::make_pair(theCalculationParams.itsTime, griddedData));
				if(insertResult.second)
					itsCurrentGriddedStationData = &((*insertResult.first).second);
				else
					itsCurrentGriddedStationData = 0;
			}
		}
		itsLastCalculatedTime = theCalculationParams.itsTime;
	}
}


#endif // FMI_SUPPORT_STATION_DATA_SMARTTOOL
