//**********************************************************
// C++ Class Name : NFmiSmartToolCalculationSectionInfo 
// ---------------------------------------------------------
// Filetype: (HEADER)
// Filepath: G:/siirto/marko/oc/NFmiSmartToolCalculationSectionInfo.h 
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
// Sis‰lt‰‰ joukon smarttool laskuja, jotka kuuluvat yhteen blokkiin. Esim.
// 
// T = T +1
// P = P * 0.99
// 
// Yksi rivi on aina yksi lasku ja laskussa pit‰‰ olla sijoitus johonkin parametriin (=).
//**********************************************************
#ifndef  NFMISMARTTOOLCALCULATIONSECTIONINFO_H
#define  NFMISMARTTOOLCALCULATIONSECTIONINFO_H

#include "NFmiDataMatrix.h"
#include <set>

class NFmiSmartToolCalculationInfo;

class NFmiSmartToolCalculationSectionInfo 
{

public:

	NFmiSmartToolCalculationSectionInfo(void);
	~NFmiSmartToolCalculationSectionInfo(void);

	void Clear(void);
	void AddCalculationInfo(NFmiSmartToolCalculationInfo* value);
	checkedVector<NFmiSmartToolCalculationInfo*>* GetCalculationInfos(void){return &itsSmartToolCalculationInfoVector;}
	void AddModifiedParams(std::set<int> &theModifiedParams);

private:
	checkedVector<NFmiSmartToolCalculationInfo*> itsSmartToolCalculationInfoVector;

};
#endif
