/*!
 *  \file NFmiSmartToolInfo.cpp \par
 *  Luokka pit‰‰ sis‰ll‰‰n SmartTool:ia ja SmartToolDialogia
 *  koskevia tietoja.
 */

#include "NFmiSmartToolInfo.h"
#include "NFmiFileString.h"
#include "NFmiFileSystem.h"
#include "NFmiSettings.h"
#include <fstream>
#include <iterator>
#include <algorithm>

using namespace std;

NFmiSmartToolInfo::NFmiSmartToolInfo(void)
:itsCurrentScript()
,itsScriptFileExtension()
,itsCurrentScriptName()
,itsLoadDirectory()
,itsDBCheckerFileName()
,itsDBCheckerText()
,fMakeDBCheckAtSend(false)
,fIsThereDBCheckScript(false)
{
}

NFmiSmartToolInfo::~NFmiSmartToolInfo(void)
{
}

// luetaan  asetukset nyky‰‰n NFmiSetting-luokasta
bool NFmiSmartToolInfo::Init(const std::string &theLoadDirectory)
{
	itsLoadDirectory = theLoadDirectory;
	if(LoadSettings())
	{
		return LoadScript(itsCurrentScriptName);
	}
	return false;
}

bool NFmiSmartToolInfo::LoadScript(const std::string &theScriptName)
{
	string fullFileName(GetFullScriptFileName(theScriptName));
	if(NFmiFileSystem::ReadFile2String(fullFileName, itsCurrentScript))
	{
		itsCurrentScriptName = theScriptName;
		return SaveSettings();
	}
	return false;
}

bool NFmiSmartToolInfo::SaveScript(const std::string &theScriptName)
{
	string fullFileName(GetFullScriptFileName(theScriptName));
	if(WriteScript2File(fullFileName, itsCurrentScript))
	{
		itsCurrentScriptName = theScriptName;
		return SaveSettings();
	}
	return false;
}

bool NFmiSmartToolInfo::LoadDBChecker(void)
{
	if(NFmiFileSystem::ReadFile2String(itsDBCheckerFileName, itsDBCheckerText))
	{
		fIsThereDBCheckScript = true;
		return true;
	}
	fIsThereDBCheckScript = false;
	return false;
}

bool NFmiSmartToolInfo::SaveDBChecker(void)
{
	return WriteScript2File(itsDBCheckerFileName, itsDBCheckerText);
}

bool NFmiSmartToolInfo::WriteScript2File(const std::string &theFileName, const std::string &theScript)
{
	if(!theFileName.empty())
	{
		ofstream out(theFileName.c_str());
		if(out)
		{
			out << theScript;
			return !out.fail();
		}
	}
	return false;
}

bool NFmiSmartToolInfo::LoadSettings(void)
{
	try
	{
		itsScriptFileExtension = NFmiSettings::Require<string>("MetEditor::SmartToolInfo::ScriptFileExtension");
		itsCurrentScriptName = NFmiSettings::Require<string>("MetEditor::SmartToolInfo::CurrentScriptName");
		fMakeDBCheckAtSend = NFmiSettings::Require<bool>("MetEditor::SmartToolInfo::MakeDBCheckAtSend");
		return true;
	}
	catch(exception & /* e */)
	{
		return false;
	}
}

bool NFmiSmartToolInfo::SaveSettings(void)
{
	try
	{
		NFmiSettings::Set("MetEditor::SmartToolInfo::ScriptFileExtension", itsScriptFileExtension);
		NFmiSettings::Set("MetEditor::SmartToolInfo::CurrentScriptName", itsCurrentScriptName);
		NFmiSettings::Set("MetEditor::SmartToolInfo::MakeDBCheckAtSend", fMakeDBCheckAtSend ? "true" : "false");
		return true;
	}
	catch(exception & /* e */)
	{
		return false;
	}
}

bool NFmiSmartToolInfo::ScriptExist(const std::string &theScriptName)
{
	vector<string> names = GetScriptNames();
	vector<string>::iterator it = std::find(names.begin(), names.end(), theScriptName);
	if(it != names.end())
		return true;
	return false;
}
std::vector<std::string> NFmiSmartToolInfo::GetScriptNames(void)
{
	string pattern(itsLoadDirectory);
	pattern += "*.";
	pattern += itsScriptFileExtension;
	list<string> tmpList = NFmiFileSystem::PatternFiles(pattern);

	if(tmpList.empty())
		return vector<string>();
	else
	{
		vector<string> names;
		list<string>::iterator it = tmpList.begin();
		for( ; it != tmpList.end(); ++it)
		{
			NFmiFileString fileString(*it);
			names.push_back(string(static_cast<char*>(fileString.Header())));
		}
		return names;
	}
}

std::string NFmiSmartToolInfo::GetFullScriptFileName(const std::string &theScriptName)
{
	string fullFileName(itsLoadDirectory);
	fullFileName += theScriptName;
	fullFileName += ".";
	fullFileName += itsScriptFileExtension;
	return fullFileName;
}

bool NFmiSmartToolInfo::RemoveScript(const std::string &theScriptName)
{
	string fullFileName(GetFullScriptFileName(theScriptName));
	return NFmiFileSystem::RemoveFile(fullFileName);
}
