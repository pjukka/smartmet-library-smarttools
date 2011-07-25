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
