// ======================================================================
/*!
 * C++ Class Name : NFmiSmartToolCalculationSection
 * ---------------------------------------------------------
 *  Author         : pietarin
 *  Creation Date  : 8.11. 2010
 */
// ======================================================================

#ifdef _MSC_VER
#pragma warning(disable : 4786) // poistaa n kpl VC++ k��nt�j�n varoitusta
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
}
//--------------------------------------------------------
// Calculate
//--------------------------------------------------------
void NFmiSmartToolCalculationSection::Calculate(const NFmiPoint &theLatlon, unsigned long theLocationIndex, const NFmiMetTime &theTime, int theTimeIndex, NFmiMacroParamValue &theMacroParamValue)
{
	for(size_t i=0; i < itsCalculations.size(); i++)
		itsCalculations[i]->Calculate(theLatlon, theLocationIndex, theTime, theTimeIndex, theMacroParamValue);
}

void NFmiSmartToolCalculationSection::AddCalculations(const boost::shared_ptr<NFmiSmartToolCalculation> &value)
{
	if(value)
		itsCalculations.push_back(value);
}

boost::shared_ptr<NFmiFastQueryInfo> NFmiSmartToolCalculationSection::FirstVariableInfo(void)
{
	boost::shared_ptr<NFmiFastQueryInfo> info;
	if(itsCalculations.size())
		info = itsCalculations[0]->GetResultInfo();
	return info;
}

// globaali asetus luokka for_each-funktioon
struct TimeSetter
{
	TimeSetter(const NFmiMetTime &theTime):itsTime(theTime){}
	void operator()(boost::shared_ptr<NFmiSmartToolCalculation> &theCalculation){theCalculation->SetTime(itsTime);}

	const NFmiMetTime &itsTime;
};

void NFmiSmartToolCalculationSection::SetTime(const NFmiMetTime &theTime)
{
	std::for_each(itsCalculations.begin(), itsCalculations.end(), TimeSetter(theTime));
}
