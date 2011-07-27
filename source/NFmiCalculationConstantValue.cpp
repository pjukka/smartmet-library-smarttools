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



NFmiCalculationRampFuction::NFmiCalculationRampFuction(const NFmiCalculationCondition& theOperation, Type theMaskType, NFmiInfoData::Type theDataType, boost::shared_ptr<NFmiFastQueryInfo> &theInfo, BinaryOperator thePostBinaryOperator)
:NFmiInfoAreaMask(theOperation, theMaskType, theDataType, theInfo, thePostBinaryOperator)
{
}

NFmiCalculationRampFuction::~NFmiCalculationRampFuction(void)
{}

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

double NFmiCalculationDeltaZValue::itsHeightValue;

NFmiCalculationDeltaZValue::NFmiCalculationDeltaZValue(void)
:NFmiAreaMaskImpl()
{
}


#ifdef FMI_SUPPORT_STATION_DATA_SMARTTOOL

#include "NFmiStationView.h"


NFmiStation2GridMask::NFmiStation2GridMask(Type theMaskType, NFmiInfoData::Type theDataType, boost::shared_ptr<NFmiFastQueryInfo> &theInfo)
:NFmiInfoAreaMask(NFmiCalculationCondition(), theMaskType, theDataType, theInfo, NFmiAreaMask::kNoValue)
,itsLastCalculatedTime(NFmiMetTime::gMissingTime)
,itsGriddedStationData()
,itsArea(0)
,itsDoc(0)
,itsStation2GridSize(1,1)
{
}

NFmiStation2GridMask::~NFmiStation2GridMask(void)
{
}

double NFmiStation2GridMask::Value(const NFmiCalculationParams &theCalculationParams, bool fUseTimeInterpolationAlways)
{
	DoGriddingCheck(theCalculationParams);
	return itsGriddedStationData.GetValue(theCalculationParams.itsLocationIndex, kFloatMissing);
}

void NFmiStation2GridMask::SetGriddingHelpers(NFmiArea *theArea, NFmiEditMapGeneralDataDoc *theDoc, const NFmiPoint &theStation2GridSize)
{
	itsArea = theArea;
	itsDoc = theDoc;
	itsStation2GridSize = theStation2GridSize;
}

void NFmiStation2GridMask::DoGriddingCheck(const NFmiCalculationParams &theCalculationParams)
{
	if(itsLastCalculatedTime != theCalculationParams.itsTime)
	{ // alustetaan hila
		itsLastCalculatedTime = theCalculationParams.itsTime;
		if(itsDoc && itsArea)
		{
			boost::shared_ptr<NFmiDrawParam> drawParam(new NFmiDrawParam(itsDataIdent, itsLevel, 0, itsDataType));
			itsGriddedStationData.Resize(static_cast<NFmiDataMatrix<float>::size_type>(itsStation2GridSize.X()), static_cast<NFmiDataMatrix<float>::size_type>(itsStation2GridSize.Y()), kFloatMissing);
			NFmiStationView::GridStationData(itsDoc, itsArea, drawParam, itsGriddedStationData, theCalculationParams.itsTime);
		}
	}
}


#endif // FMI_SUPPORT_STATION_DATA_SMARTTOOL
