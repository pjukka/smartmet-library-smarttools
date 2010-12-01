
#include "NFmiOwnerInfo.h"
#include "NFmiQueryData.h"

NFmiOwnerInfo::NFmiOwnerInfo(NFmiQueryData *theOwnedData, NFmiInfoData::Type theDataType, 
							 const std::string &theDataFileName, const std::string &theDataFilePattern)
:NFmiFastQueryInfo(theOwnedData)
,itsDataType(theDataType)
,itsDataPtr(theOwnedData)
,itsDataFileName(theDataFileName)
,itsDataFilePattern(theDataFilePattern)
{
	if(theOwnedData == 0)
		throw std::runtime_error("Error in NFmiOwnerInfo konstructor, given queryData was NULL pointer.");
}

NFmiOwnerInfo::NFmiOwnerInfo(const NFmiOwnerInfo &theInfo)
:NFmiFastQueryInfo(theInfo)
,itsDataType(theInfo.itsDataType)
,itsDataPtr(theInfo.itsDataPtr)
,itsDataFileName(theInfo.itsDataFileName)
,itsDataFilePattern(theInfo.itsDataFilePattern)
{
}

NFmiOwnerInfo::~NFmiOwnerInfo(void)
{
}

NFmiOwnerInfo& NFmiOwnerInfo::operator=(const NFmiOwnerInfo &theInfo)
{
	NFmiFastQueryInfo::operator=(theInfo);
	itsDataType = theInfo.itsDataType;
	itsDataPtr = theInfo.itsDataPtr;
	itsDataFileName = theInfo.itsDataFileName;
	itsDataFilePattern = theInfo.itsDataFilePattern;

	return *this;
}

NFmiOwnerInfo* NFmiOwnerInfo::Clone(void) const
{
	NFmiQueryData *cloneData = itsDataPtr.get()->Clone();
	return new NFmiOwnerInfo(cloneData, itsDataType, itsDataFileName, itsDataFilePattern);
}