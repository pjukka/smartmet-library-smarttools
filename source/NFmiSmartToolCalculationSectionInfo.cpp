//**********************************************************
// C++ Class Name : NFmiSmartToolCalculationSectionInfo 
// ---------------------------------------------------------
// Filetype: (SOURCE)
// Filepath: G:/siirto/marko/oc/NFmiSmartToolCalculationSectionInfo.cpp 
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

#include "NFmiSmartToolCalculationSectionInfo.h"
#include "NFmiAreaMaskInfo.h"
#include "NFmiSmartToolCalculationInfo.h"
#include <algorithm>
//--------------------------------------------------------
// Constructor/Destructor 
//--------------------------------------------------------
NFmiSmartToolCalculationSectionInfo::NFmiSmartToolCalculationSectionInfo(void)
{
}
NFmiSmartToolCalculationSectionInfo::~NFmiSmartToolCalculationSectionInfo(void)
{
	Clear();
}

void NFmiSmartToolCalculationSectionInfo::Clear(void)
{
	std::for_each(itsSmartToolCalculationInfoVector.begin(), itsSmartToolCalculationInfoVector.end(), PointerDestroyer());
	itsSmartToolCalculationInfoVector.clear();
}

void NFmiSmartToolCalculationSectionInfo::AddCalculationInfo(NFmiSmartToolCalculationInfo* value)
{
	if(value)
		itsSmartToolCalculationInfoVector.push_back(value);
}

// Lis‰t‰‰n set:iin kaikki parametrit, joita t‰ss‰ sectioniossa voidaan muokata.
// talteen otetaan vain identti, koska muu ei kiinnosta (ainakaan nyt)
void NFmiSmartToolCalculationSectionInfo::AddModifiedParams(std::set<int> &theModifiedParams)
{
	std::vector<NFmiSmartToolCalculationInfo*>::size_type ssize = itsSmartToolCalculationInfoVector.size();
	std::vector<NFmiSmartToolCalculationInfo*>::size_type i = 0;
	for( ;i<ssize; i++)
	{
		theModifiedParams.insert(itsSmartToolCalculationInfoVector[i]->GetResultDataInfo()->GetDataIdent().GetParamIdent());
	}
}
