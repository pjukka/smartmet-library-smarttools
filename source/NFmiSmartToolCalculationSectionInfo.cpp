//**********************************************************
// C++ Class Name : NFmiSmartToolCalculationSectionInfo
// ---------------------------------------------------------
//  Author         : pietarin
//  Creation Date  : 8.11. 2010
//**********************************************************
#ifdef _MSC_VER
#pragma warning(disable : 4786)  // poistaa n kpl VC++ k‰‰nt‰j‰n varoitusta
#endif

#include "NFmiSmartToolCalculationSectionInfo.h"
#include "NFmiAreaMaskInfo.h"
#include "NFmiSmartToolCalculationInfo.h"
#include <algorithm>
//--------------------------------------------------------
// Constructor/Destructor
//--------------------------------------------------------
NFmiSmartToolCalculationSectionInfo::NFmiSmartToolCalculationSectionInfo(void) {}
NFmiSmartToolCalculationSectionInfo::~NFmiSmartToolCalculationSectionInfo(void) {}
void NFmiSmartToolCalculationSectionInfo::AddCalculationInfo(
    boost::shared_ptr<NFmiSmartToolCalculationInfo> &value)
{
  if (value) itsSmartToolCalculationInfoVector.push_back(value);
}

// Lis‰t‰‰n set:iin kaikki parametrit, joita t‰ss‰ sectioniossa voidaan muokata.
// talteen otetaan vain identti, koska muu ei kiinnosta (ainakaan nyt)
void NFmiSmartToolCalculationSectionInfo::AddModifiedParams(std::set<int> &theModifiedParams)
{
  checkedVector<boost::shared_ptr<NFmiSmartToolCalculationInfo> >::size_type ssize =
      itsSmartToolCalculationInfoVector.size();
  for (size_t i = 0; i < ssize; i++)
  {
    theModifiedParams.insert(
        itsSmartToolCalculationInfoVector[i]->GetResultDataInfo()->GetDataIdent().GetParamIdent());
  }
}
