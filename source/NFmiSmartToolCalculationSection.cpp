//**********************************************************
// C++ Class Name : NFmiSmartToolCalculationSection 
// ---------------------------------------------------------
// Filetype: (SOURCE)
// Filepath: G:/siirto/marko/oc/NFmiSmartToolCalculationSection.cpp 
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

#include "NFmiSmartToolCalculationSection.h"
#include "NFmiAreaMaskInfo.h"
#include "NFmiSmartToolCalculation.h"
#include <algorithm>
//--------------------------------------------------------
// Constructor/Destructor 
//--------------------------------------------------------
NFmiSmartToolCalculationSection::NFmiSmartToolCalculationSection(void)
{
}
NFmiSmartToolCalculationSection::~NFmiSmartToolCalculationSection(void)
{
	Clear();
}
//--------------------------------------------------------
// Calculate 
//--------------------------------------------------------
void NFmiSmartToolCalculationSection::Calculate(const NFmiPoint &theLatlon, int theLocationIndex, const NFmiMetTime &theTime, int theTimeIndex)
{
	int size = itsCalculations.size();
	for(int i=0; i<size; i++)
		itsCalculations[i]->Calculate(theLatlon, theLocationIndex, theTime, theTimeIndex);
}

void NFmiSmartToolCalculationSection::AddCalculations(NFmiSmartToolCalculation* value)
{
	if(value)
		itsCalculations.push_back(value);
}

void NFmiSmartToolCalculationSection::Clear(void)
{
	std::for_each(itsCalculations.begin(), itsCalculations.end(), PointerDestroyer());
	itsCalculations.clear();
}

NFmiSmartInfo* NFmiSmartToolCalculationSection::FirstVariableInfo(void)
{
	NFmiSmartInfo *info = 0;
	if(itsCalculations.size())
		info = itsCalculations[0]->GetResultInfo();
	return info;
}

// globaali asetus luokka for_each-funktioon
struct TimeSetter
{
	TimeSetter(const NFmiMetTime &theTime):itsTime(theTime){}
	void operator()(NFmiSmartToolCalculation* theCalculation){theCalculation->SetTime(itsTime);}

	const NFmiMetTime &itsTime;
};

void NFmiSmartToolCalculationSection::SetTime(const NFmiMetTime &theTime)
{
	std::for_each(itsCalculations.begin(), itsCalculations.end(), TimeSetter(theTime));
}

// globaali asetus luokka for_each-funktioon
struct FactorSetter
{
	FactorSetter(std::vector<double> *theFactors):itsFactors(theFactors){}
	void operator()(NFmiSmartToolCalculation* theCalculation){theCalculation->SetModificationFactors(itsFactors);}

	std::vector<double> *itsFactors;
};

void NFmiSmartToolCalculationSection::SetModificationFactors(std::vector<double> *theFactors) 
{
	itsModificationFactors = theFactors;
	std::for_each(itsCalculations.begin(), itsCalculations.end(), FactorSetter(theFactors));
}
