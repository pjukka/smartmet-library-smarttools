//**********************************************************
// C++ Class Name : NFmiCalculationChangeFactorArray 
// ---------------------------------------------------------
// Filetype: (SOURCE)
// Filepath: G:/siirto/marko/oc/NFmiCalculationChangeFactorArray.cpp 
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
#include "NFmiCalculationChangeFactorArray.h"
//--------------------------------------------------------
// Constructor/Destructor 
//--------------------------------------------------------
NFmiCalculationChangeFactorArray::NFmiCalculationChangeFactorArray(void)
:NFmiAreaMaskImpl()
{
}
	
NFmiCalculationChangeFactorArray::~NFmiCalculationChangeFactorArray(void)
{
}

void NFmiCalculationChangeFactorArray::SetChangeFactors(const std::vector<double>& value)
{
	itsChangeFactors = value;
}

//--------------------------------------------------------
// Value 
//--------------------------------------------------------
double NFmiCalculationChangeFactorArray::Value(const NFmiPoint &theLatlon, const NFmiMetTime &theTime, int theTimeIndex)
{
	return itsChangeFactors[theTimeIndex];
}
