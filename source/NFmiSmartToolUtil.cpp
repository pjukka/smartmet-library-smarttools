// ======================================================================
/*!
 * \file NFmiSmartToolUtil.cpp
 * \brief Implementation of class NFmiSmartToolUtil
 */
// ======================================================================
#ifdef _MSC_VER
#pragma warning(disable : 4786) // poistaa n kpl VC++ kääntäjän varoitusta (liian pitkä nimi >255 merkkiä joka johtuu 'puretuista' STL-template nimistä)
#endif

#include "NFmiSmartToolUtil.h"
#include "NFmiInfoOrganizer.h"
#include "NFmiSmartInfo.h"
#include "NFmiSmartToolModifier.h"
#include "NFmiQueryData.h"
#include "NFmiStreamQueryData.h"

#ifndef UNIX
  #include <direct.h> // working directory juttuja varten
#else
  #include <stdexcept>
  #include <unistd.h>
#endif

NFmiQueryData* NFmiSmartToolUtil::ModifyData(const std::string &theMacroText, NFmiQueryData* theModifiedData, const std::vector<std::string> *theHelperDataFileNames)
{
	NFmiTimeDescriptor times(theModifiedData->Info()->TimeDescriptor());
	return ModifyData(theMacroText, theModifiedData, &times, theHelperDataFileNames);
}

NFmiQueryData* NFmiSmartToolUtil::ModifyData(const std::string &theMacroText, NFmiQueryData* theModifiedData, NFmiTimeDescriptor *theTimes, const std::vector<std::string> *theHelperDataFileNames)
{
	NFmiInfoOrganizer dataBase;
	if(!InitDataBase(&dataBase, theModifiedData, theHelperDataFileNames))
	{
		std::cerr << "Tietokannan alustus epäonnistui, ei jatketa." << std::endl;
		return 0;
	}

	NFmiSmartToolModifier smartToolModifier(&dataBase);

	try // ensin tulkitaan macro
	{
		smartToolModifier.InitSmartTool(theMacroText);
	}
	catch(NFmiSmartToolModifier::Exception exc)
	{
		std::cerr << exc.What() << std::endl;
		return 0;
	}

	try // suoritetaan macro sitten
	{
		std::vector<double> dummyFactors(theTimes->Size(), 0);
		smartToolModifier.ModifyData(theTimes, dummyFactors, false); // false = ei tehdä muokkauksia vain valituille pisteille vaan kaikille pisteille
	}
	catch(NFmiSmartToolModifier::Exception exc)
	{
		std::cerr << exc.What() << std::endl;
		return 0;
	}

	NFmiQueryData* data = 0;
	if(dataBase.EditedInfo() && dataBase.EditedInfo()->RefQueryData())
		data = dataBase.EditedInfo()->RefQueryData()->Clone();
	return data;
}

NFmiQueryData* NFmiSmartToolUtil::ModifyData(const std::string &theMacroText, NFmiQueryData* theModifiedData)
{
	NFmiTimeDescriptor times(theModifiedData->Info()->TimeDescriptor());
	return ModifyData(theMacroText, theModifiedData, &times);
}

NFmiQueryData* NFmiSmartToolUtil::ModifyData(const std::string &theMacroText, NFmiQueryData* theModifiedData, NFmiTimeDescriptor *theTimes)
{
	return ModifyData(theMacroText, theModifiedData, theTimes, 0); // 0=tyhjä apudata filename-lista
}

std::string NFmiSmartToolUtil::GetWorkingDirectory(void)
{
#ifndef UNIX
	static char path[_MAX_PATH];
	int curdrive = ::_getdrive();
	::_getdcwd(curdrive , path, _MAX_PATH );
	std::string workingDirectory(path);
	return workingDirectory;
#else
	static char path[4096];	// we assume 4096 is maximum buffer length
	if(!::getcwd(path,4096))
	  throw std::runtime_error("Error: Current path is too long (>4096)");
	return std::string(path);
#endif
}

bool NFmiSmartToolUtil::InitDataBase(NFmiInfoOrganizer *theDataBase, NFmiQueryData* theModifiedData, const std::vector<std::string> *theHelperDataFileNames)
{
	if(theDataBase)
	{
		theDataBase->WorkingDirectory(GetWorkingDirectory());
		theDataBase->Init();
		theDataBase->AddData(theModifiedData, "xxxfileName", NFmiInfoData::kEditable, 0); // 0=undolevel
		if(theHelperDataFileNames && theHelperDataFileNames->size())
			InitDataBaseHelperData(*theDataBase, *theHelperDataFileNames);
		return true;
	}
	return false;
}

bool NFmiSmartToolUtil::InitDataBaseHelperData(NFmiInfoOrganizer &theDataBase, const std::vector<std::string> &theHelperDataFileNames)
{
	NFmiStreamQueryData sQData;
	for(unsigned int i=0; i<theHelperDataFileNames.size(); i++)
	{
		if(sQData.ReadData(theHelperDataFileNames[i]))
		{
			NFmiInfoData::Type dataType = NFmiInfoData::kViewable;
			if(sQData.QueryData()->Info()->SizeTimes() == 1)
				dataType = NFmiInfoData::kStationary;
			theDataBase.AddData(sQData.QueryData(true), theHelperDataFileNames[i], dataType, 0); // 0=undolevel
		}
	}
	return true;
}
