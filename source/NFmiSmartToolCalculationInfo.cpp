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
,fAllowMissingValueAssignment(false)
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

/*! tarkistaa onko lause muotoa:
 * par = MISS
 * vain t‰ll‰inen lauseke sallii puuttuvan arvon sijoituksen dataan skripteill‰.
 * Pit‰‰ tehd‰ speciaali asetus systeemi, sill‰ muuten tulee ongelmia sijotusten kanssa,
 * koska muuten puuttuvia arvoja tulee sijoitetuksi aina kun jotain dataa puuttuu, 
 * tai se ei kata koko aluetta tai aika-skaalaa.
 */
void NFmiSmartToolCalculationInfo::CheckIfAllowMissingValueAssignment(void)
{
	fAllowMissingValueAssignment = false;
	int size = itsCalculationOperandInfoVector.size();
	if(size == 1)
	{
		if(itsCalculationOperandInfoVector[0]->GetOperationType() == NFmiAreaMask::Constant)
		{
			if(itsCalculationOperandInfoVector[0]->GetMaskCondition().LowerLimit() == kFloatMissing)
				fAllowMissingValueAssignment = true;
		}
	}
}

