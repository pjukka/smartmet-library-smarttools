//**********************************************************
// C++ Class Name : NFmiAreaMaskSectionInfo 
// ---------------------------------------------------------
// Filetype: (SOURCE)
// Filepath: G:/siirto/marko/oc/NFmiAreaMaskSectionInfo.cpp 
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

#include "NFmiAreaMaskSectionInfo.h"
#include "NFmiAreaMaskInfo.h"
#include <algorithm>
//--------------------------------------------------------
// Constructor/Destructor 
//--------------------------------------------------------
NFmiAreaMaskSectionInfo::NFmiAreaMaskSectionInfo ()
{
}
NFmiAreaMaskSectionInfo::~NFmiAreaMaskSectionInfo ()
{
	Clear();
}

NFmiAreaMaskInfo* NFmiAreaMaskSectionInfo::MaskInfo(int theIndex)
{
	if(theIndex >= 0 && theIndex < itsAreaMaskInfoVector.size())
		return itsAreaMaskInfoVector[theIndex];
	return 0;
}
void NFmiAreaMaskSectionInfo::Add(NFmiAreaMaskInfo* theMask)
{
	itsAreaMaskInfoVector.push_back(theMask);
}

void NFmiAreaMaskSectionInfo::Clear(void)
{
	std::for_each(itsAreaMaskInfoVector.begin(), itsAreaMaskInfoVector.end(), PointerDestroyer());
	itsAreaMaskInfoVector.clear();
}
