//**********************************************************
// C++ Class Name : NFmiSmartToolCalculationSection 
// ---------------------------------------------------------
// Filetype: (HEADER)
// Filepath: G:/siirto/marko/oc/NFmiSmartToolCalculationSection.h 
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
// Tämä luokka hoitaa calculationsectionin yksi laskurivi kerrallaan.
// esim. 
// T = T + 1
// P = P + 2
//**********************************************************
#ifndef  NFMISMARTTOOLCALCULATIONSECTION_H
#define  NFMISMARTTOOLCALCULATIONSECTION_H

#include <vector>

class NFmiPoint;
class NFmiMetTime;
class NFmiSmartToolCalculation;
class NFmiSmartInfo;

class NFmiSmartToolCalculationSection 
{

public:
	void Calculate(const NFmiPoint &theLatlon, int theLocationIndex, const NFmiMetTime &theTime, int theTimeIndex);
	void Clear(void);
	void SetModificationFactors(std::vector<double> *theFactors);
	void SetTime(const NFmiMetTime &theTime);
	NFmiSmartInfo* FirstVariableInfo(void);

	NFmiSmartToolCalculationSection(void);
	~NFmiSmartToolCalculationSection(void);

	void AddCalculations(NFmiSmartToolCalculation* value);
	std::vector<NFmiSmartToolCalculation*>* GetCalculations(void){return &itsCalculations;}

private:
	std::vector<NFmiSmartToolCalculation*> itsCalculations; // omistaa+tuhoaa
	std::vector<double> *itsModificationFactors; // mahdolliset aikasarja muokkaus kertoimet (ei omista, ei tuhoa)

};
#endif
