//**********************************************************
// C++ Class Name : NFmiCalculationChangeFactorArray 
// ---------------------------------------------------------
// Filetype: (HEADER)
// Filepath: G:/siirto/marko/oc/NFmiCalculationChangeFactorArray.h 
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
#ifndef  NFMICALCULATIONCHANGEFACTORARRAY_H
#define  NFMICALCULATIONCHANGEFACTORARRAY_H

#include "NFmiAreaMaskImpl.h"
#include <vector>

// Tämä luokka antaa laskuihin mukaan ajan mukana muuttuvat kertoimet. Aikasarja-editorista huom!
class NFmiCalculationChangeFactorArray : public NFmiAreaMaskImpl
{

public:
	double Value(const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex);

	NFmiCalculationChangeFactorArray(void);
	~NFmiCalculationChangeFactorArray(void);

	void SetChangeFactors(const std::vector<double>& value);
	const std::vector<double>& GetChangeFactors (void) const {return itsChangeFactors;}

private:
	std::vector<double> itsChangeFactors;

};
#endif
