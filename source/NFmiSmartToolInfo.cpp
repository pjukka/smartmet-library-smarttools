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

using namespace std;

NFmiSmartToolInfo::NFmiSmartToolInfo(void)
:itsCurrentScript()
,itsCurrentScriptFileName()
,itsLastLoadDirectory()
,itsDBCheckerFileName()
,itsDBCheckerText()
,fMakeDBCheckAtSend(false)
,fIsThereDBCheckScript(false)
,fHasCurrentScriptFileName(false)
{
}

NFmiSmartToolInfo::~NFmiSmartToolInfo(void)
{
}

// luetaan  asetukset nyky‰‰n NFmiSetting-luokasta
bool NFmiSmartToolInfo::Init(void)
{
	if(LoadSettings())
	{
		return LoadAsCurrentScript(itsCurrentScriptFileName);
	}
	return false;
}

bool NFmiSmartToolInfo::LoadAsCurrentScript(const std::string &theFileName)
{
	if(::ReadFile2String(theFileName, itsCurrentScript))
	{
		fHasCurrentScriptFileName = true;
		itsCurrentScriptFileName = theFileName;
		itsLastLoadDirectory = ExtractPath(theFileName);
		return SaveSettings();
	}
	return false;
}

bool NFmiSmartToolInfo::SaveCurrentScript(void)
{
	if(fHasCurrentScriptFileName)
		return SaveAsCurrentScript(itsCurrentScriptFileName);
	else
		return false;
}

bool NFmiSmartToolInfo::SaveAsCurrentScript(const std::string &theFileName)
{
	if(WriteScript2File(theFileName, itsCurrentScript))
	{
		fHasCurrentScriptFileName = true;
		itsCurrentScriptFileName = theFileName;
		itsLastLoadDirectory = ExtractPath(theFileName);
		return SaveSettings();
	}
	return false;
}

bool NFmiSmartToolInfo::LoadDBChecker(void)
{
	if(::ReadFile2String(itsDBCheckerFileName, itsDBCheckerText))
	{
		fIsThereDBCheckScript = true;
		return true;
	}
	fIsThereDBCheckScript = false;
	return false;
}

bool NFmiSmartToolInfo::LoadLastScript(void)
{
	if(::ReadFile2String(itsCurrentScriptFileName, itsCurrentScript))
	{
		fHasCurrentScriptFileName = true;
		return true;
	}
	return false;
}

bool NFmiSmartToolInfo::SaveDBChecker(void)
{
	return WriteScript2File(itsDBCheckerFileName, itsDBCheckerText);
}

std::string NFmiSmartToolInfo::ExtractPath(const std::string &theFullFileName)
{
	NFmiFileString fileString(theFullFileName);
	NFmiString path(fileString.Device());
	path += kFmiDirectorySeparator;
	path += fileString.Path();
	path += kFmiDirectorySeparator;
	string path2(path);
	return path2;
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
		itsCurrentScriptFileName = NFmiSettings::Require<string>("MetEditor::SmartToolInfo::CurrentScriptFileName");
		itsLastLoadDirectory = NFmiSettings::Require<string>("MetEditor::SmartToolInfo::LastLoadDirectory");
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
		NFmiSettings::Set("MetEditor::SmartToolInfo::CurrentScriptFileName", itsCurrentScriptFileName);
		NFmiSettings::Set("MetEditor::SmartToolInfo::LastLoadDirectory", itsLastLoadDirectory);
		NFmiSettings::Set("MetEditor::SmartToolInfo::MakeDBCheckAtSend", fMakeDBCheckAtSend ? "true" : "false");
		return true;
	}
	catch(exception & /* e */)
	{
		return false;
	}
}
