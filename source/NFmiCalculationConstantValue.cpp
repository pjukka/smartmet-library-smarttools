//**********************************************************
// C++ Class Name : NFmiCalculationConstantValue 
// ---------------------------------------------------------
// Filetype: (SOURCE)
// Filepath: G:/siirto/marko/oc/NFmiCalculationConstantValue.cpp 
// 
// 
// GDPro Properties 
// ---------------------------------------------------
//  - GD Symbol Type    : CLD_Class 
//  - GD Method         : UML ( 4.0 ) 
//  - GD System Name    : aSmartTools 
//  - GD View Type      : Class Diagram 
//  - GD View Name      : smarttools 1 
// ---------------------------------------------------  
//  Author         : pietarin 
//  Creation Date  : Thur - Jun 20, 2002 
// 
//  Change Log     : 
// 
//**********************************************************
#include "NFmiCalculationConstantValue.h"
#include "NFmiDataModifier.h"
#include "NFmiDataIterator.h"
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
double NFmiCalculationConstantValue::Value(const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex, bool fUseTimeInterpolationAlways)
{
	return itsValue;
}

NFmiCalculationSpecialCase::NFmiCalculationSpecialCase(NFmiAreaMask::CalculationOperator theValue)
:NFmiAreaMaskImpl()
{
	SetCalculationOperator(theValue);
}



NFmiCalculationRampFuction::NFmiCalculationRampFuction(const NFmiCalculationCondition& theOperation, Type theMaskType, NFmiInfoData::Type theDataType, NFmiQueryInfo* theInfo, bool ownsInfo, BinaryOperator thePostBinaryOperator)
:NFmiInfoAreaMask(theOperation, theMaskType, theDataType, theInfo, ownsInfo, thePostBinaryOperator)
{
}

NFmiCalculationRampFuction::~NFmiCalculationRampFuction(void)
{}

double NFmiCalculationRampFuction::Value(const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex, bool fUseTimeInterpolationAlways)
{
	double value = NFmiInfoAreaMask::Value(theLatlon, theTime, theTimeIndex, fUseTimeInterpolationAlways);
	return itsMaskCondition.MaskValue(value);
}


NFmiCalculationIntegrationFuction::NFmiCalculationIntegrationFuction(NFmiDataIterator *theDataIterator, NFmiDataModifier *theDataModifier, Type theMaskType, NFmiInfoData::Type theDataType, NFmiQueryInfo* theInfo, bool ownsInfo, bool destroySmartInfoData)
:NFmiInfoAreaMask(NFmiCalculationCondition(), theMaskType, theDataType, theInfo, ownsInfo, NFmiAreaMask::kNoValue, destroySmartInfoData)
,itsDataModifier(theDataModifier)
,itsDataIterator(theDataIterator)
{
}

NFmiCalculationIntegrationFuction::~NFmiCalculationIntegrationFuction(void)
{
	delete itsDataModifier;
	delete itsDataIterator;
}

double NFmiCalculationIntegrationFuction::Value(const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex, bool fUseTimeInterpolationAlways)
{
	// HUOM!!! Tähän tuli pikaviritys:
	// asetan vain lähimmän pisteen ja ajan kohdalleen.
	if(itsInfo->NearestPoint(theLatlon) && itsInfo->TimeToNearestStep(theTime, kForward))
	{
		itsDataIterator->DoForEach(itsDataModifier);
		return itsDataModifier->CalculationResult();
	}
	return kFloatMissing;
}

// Ramppifunktioiden laskut AreaMask:ien avulla (mm. lat, lon ja elevationangle tapaukset).
double NFmiCalculationRampFuctionWithAreaMask::Value(const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex, bool fUseTimeInterpolationAlways)
{
	itsAreaMask->Time(theTime);
	double value = itsAreaMask->Value(theLatlon, theTime, theTimeIndex, fUseTimeInterpolationAlways);
	return itsMaskCondition.MaskValue(value);
}

NFmiCalculationRampFuctionWithAreaMask::NFmiCalculationRampFuctionWithAreaMask(const NFmiCalculationCondition & theOperation,
																			   Type theMaskType,
																			   NFmiInfoData::Type theDataType,
																			   NFmiAreaMask * theAreaMask,
																			   bool ownsAreaMask,
																			   BinaryOperator thePostBinaryOperator)
:NFmiAreaMaskImpl(theOperation, theMaskType, theDataType, thePostBinaryOperator)
,itsAreaMask(theAreaMask)
,fOwnsAreaMask(ownsAreaMask)
,fIsTimeIntepolationNeededInValue(false)
{
}

NFmiCalculationRampFuctionWithAreaMask::~NFmiCalculationRampFuctionWithAreaMask(void)
{
	if(itsAreaMask && fOwnsAreaMask)
	{
		delete itsAreaMask;
	}
}

