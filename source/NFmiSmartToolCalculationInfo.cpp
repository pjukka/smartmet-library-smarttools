//**********************************************************
// C++ Class Name : NFmiSmartToolCalculationInfo 
// ---------------------------------------------------------
// Filetype: (SOURCE)
// Filepath: G:/siirto/marko/oc/NFmiSmartToolCalculationInfo.cpp 
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

#include "NFmiSmartToolCalculationInfo.h"
#include "NFmiAreaMaskInfo.h"
#include <algorithm>
//--------------------------------------------------------
// Constructor/Destructor 
//--------------------------------------------------------
NFmiSmartToolCalculationInfo::NFmiSmartToolCalculationInfo(void)
:itsResultDataInfo(0)
,itsCalculationOperandInfoVector()
{
}

NFmiSmartToolCalculationInfo::~NFmiSmartToolCalculationInfo(void)
{
	Clear();
}

void NFmiSmartToolCalculationInfo::AddCalculationInfo(NFmiAreaMaskInfo* theAreaMaskInfo)
{
	if(theAreaMaskInfo)
		itsCalculationOperandInfoVector.push_back(theAreaMaskInfo);
}

void NFmiSmartToolCalculationInfo::Clear(void)
{
	delete itsResultDataInfo;
	itsResultDataInfo = 0;
	std::for_each(itsCalculationOperandInfoVector.begin(), itsCalculationOperandInfoVector.end(), PointerDestroyer());
	itsCalculationOperandInfoVector.clear();
}
