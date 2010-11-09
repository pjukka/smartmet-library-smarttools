#pragma once
//**********************************************************
// C++ Class Name : NFmiSmartToolCalculationSection 
// ---------------------------------------------------------
//  Author         : pietarin 
//  Creation Date  : Thur - Jun 20, 2002 
// 
// Tämä luokka hoitaa calculationsectionin yksi laskurivi kerrallaan.
// esim. 
// T = T + 1
// P = P + 2
//**********************************************************

#include "NFmiDataMatrix.h"
#include "boost/shared_ptr.hpp"

class NFmiPoint;
class NFmiMetTime;
class NFmiSmartToolCalculation;
class NFmiFastQueryInfo;
class NFmiMacroParamValue;

class NFmiSmartToolCalculationSection 
{

public:
	void Calculate(const NFmiPoint &theLatlon, unsigned long theLocationIndex, const NFmiMetTime &theTime, int theTimeIndex, NFmiMacroParamValue &theMacroParamValue);
	void SetTime(const NFmiMetTime &theTime);
	boost::shared_ptr<NFmiFastQueryInfo> FirstVariableInfo(void);

	NFmiSmartToolCalculationSection(void);
	~NFmiSmartToolCalculationSection(void);

	void AddCalculations(boost::shared_ptr<NFmiSmartToolCalculation>  &value);
	checkedVector<boost::shared_ptr<NFmiSmartToolCalculation> >& GetCalculations(void){return itsCalculations;}

private:
	checkedVector<boost::shared_ptr<NFmiSmartToolCalculation> > itsCalculations; // omistaa+tuhoaa

};

