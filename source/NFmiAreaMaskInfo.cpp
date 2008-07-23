//**********************************************************
// C++ Class Name : NFmiAreaMaskInfo 
// ---------------------------------------------------------
// Filetype: (SOURCE)
// Filepath: G:/siirto/marko/oc/NFmiAreaMaskInfo.cpp 
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
#ifdef WIN32
#pragma warning(disable : 4786) // poistaa n kpl VC++ k‰‰nt‰j‰n varoitusta
#endif

#include "NFmiAreaMaskInfo.h"
#include "NFmiLevel.h"
//--------------------------------------------------------
// Constructor/Destructor 
//--------------------------------------------------------
NFmiAreaMaskInfo::NFmiAreaMaskInfo( const NFmiMetTime *origin_time_ )
:itsOriginTime(0)
,itsDataIdent()
,fUseDefaultProducer(true)
,itsMaskCondition()
,itsOperationType(NFmiAreaMask::NoType)
,itsCalculationOperator(NFmiAreaMask::NotOperation)
,itsBinaryOperator(NFmiAreaMask::kNoValue)
,itsDataType(NFmiInfoData::kNoDataType)
,itsLevel(0)
,itsMaskText()
,itsFunctionType(NFmiAreaMask::NotFunction)
,itsOffsetPoint1()
,itsOffsetPoint2()
,itsMathFunctionType(NFmiAreaMask::NotMathFunction)
,itsIntegrationFunctionType(0)
,itsSoundingParameter(kSoundingParNone)
{
    if (origin_time_) {
        itsOriginTimeStore= *origin_time_;   // local copy
        itsOriginTime= &itsOriginTimeStore;
    }
}

NFmiAreaMaskInfo::~NFmiAreaMaskInfo(void)
{
	delete itsLevel;
}

void NFmiAreaMaskInfo::SetLevel(NFmiLevel *theLevel)
{
	delete itsLevel;
	itsLevel = theLevel ? new NFmiLevel(*theLevel) : 0;
}
