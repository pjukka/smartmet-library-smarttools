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
	static char path[_MAX_PATH];
	int curdrive = ::_getdrive();
	::_getdcwd(curdrive , path, _MAX_PATH );
	std::string workingDirectory(path);
	return workingDirectory;
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
	for(int i=0; i<theHelperDataFileNames.size(); i++)
	{
		if(sQData.ReadData(theHelperDataFileNames[i]))
			theDataBase.AddData(sQData.QueryData(true), theHelperDataFileNames[i], NFmiInfoData::kViewable, 0); // 0=undolevel
	}
	return true;
}