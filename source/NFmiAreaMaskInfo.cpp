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
NFmiAreaMaskInfo::NFmiAreaMaskInfo()
:itsDataIdent()
,fUseDefaultProducer(true)
,itsMaskCondition()
,itsOperationType(NFmiAreaMask::NoType)
,itsCalculationOperator(NFmiAreaMask::NotOperation)
,itsBinaryOperator(NFmiAreaMask::kNoValue)
,itsDataType(kFmiDataTypeNoDataType)
,itsLevel(0)
{
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
