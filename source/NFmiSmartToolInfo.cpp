/*!
 *  \file NFmiSmartToolInfo.cpp \par
 *  Luokka pit‰‰ sis‰ll‰‰n SmartTool:ia ja SmartToolDialogia
 *  koskevia tietoja.
 */

#include "NFmiSmartToolInfo.h"
#include "NFmiFileString.h"
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

bool NFmiSmartToolInfo::Init(const std::string &theFileName)
{
	ifstream in(theFileName.c_str());
	if(in)
	{
		in >> *this;
//		in.close(); // ei tarvitse sulkea, koska in sulkeutuu automaattisesti poistuttaessa scoopista.
		if(!in.fail())
		{
			itsCurrentInfoFile = theFileName;
			LoadLastScript();
			return true;
		}
	}
	return false;
}

bool NFmiSmartToolInfo::Store(const std::string &theFileName)
{
	ofstream out(theFileName.c_str());
	if(out)
	{
		out << *this;
//		out.close(); // ei tarvitse sulkea, koska out sulkeutuu automaattisesti poistuttaessa scoopista.
		return !out.fail();
	}
	return false;
}

bool NFmiSmartToolInfo::LoadAsCurrentScript(const std::string &theFileName)
{
	if(ReadFileToString(theFileName, &itsCurrentScript))
	{
		fHasCurrentScriptFileName = true;
		itsCurrentScriptFileName = theFileName;
		itsLastLoadDirectory = ExtractPath(theFileName);
		return Store(itsCurrentInfoFile);
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
	ofstream out(theFileName.c_str());
	if(out)
	{
		out << itsCurrentScript;
//		out.close(); // ei tarvitse sulkea, koska out sulkeutuu automaattisesti poistuttaessa scoopista.
		if(!out.fail())
		{
			fHasCurrentScriptFileName = true;
			itsCurrentScriptFileName = theFileName;
			itsLastLoadDirectory = ExtractPath(theFileName);
			return Store(itsCurrentInfoFile);
		}
	}
	return false;
}

bool NFmiSmartToolInfo::LoadDBChecker(void)
{
	if(ReadFileToString(itsDBCheckerFileName, &itsDBCheckerText))
	{
		fIsThereDBCheckScript = true;
		return true;
	}
	fIsThereDBCheckScript = false;
	return false;
}

bool NFmiSmartToolInfo::LoadLastScript(void)
{
	if(ReadFileToString(itsCurrentScriptFileName, &itsCurrentScript))
	{
		fHasCurrentScriptFileName = true;
		return true;
	}
	return false;
}

bool NFmiSmartToolInfo::ReadFileToString(const std::string &theFileName, std::string *theString)
{
	if(theFileName != "")
	{
		ifstream in(theFileName.c_str());
		if(in)
		{
			in.unsetf(std::ios::skipws);
			istream_iterator<char> it(in);
			istream_iterator<char> endFile;
			*theString = "";
			for(; it != endFile; ++it)
				*theString += *it;
	//		in.close(); // ei tarvitse sulkea, koska in sulkeutuu automaattisesti poistuttaessa scoopista.
			return true;
		}
	}
	return false;
}

bool NFmiSmartToolInfo::SaveDBChecker(void)
{
	ofstream out(itsDBCheckerFileName.c_str());
	if(out)
	{
		out << itsDBCheckerText;
//		out.close(); // ei tarvitse sulkea, koska out sulkeutuu automaattisesti poistuttaessa scoopista.
		return !out.fail();
	}
	return false;
}

std::ostream& NFmiSmartToolInfo::Write(std::ostream &file) const
{
	file << itsCurrentScriptFileName << endl;
	file << itsLastLoadDirectory << endl;
	file << fMakeDBCheckAtSend << endl;
	return file;
}

/*!
 *  Oletus, ett‰ luettavat jutut ovat aina omilla riveill‰‰n.
 *  T‰m‰ sen takia, ett‰ polussa tai tiedoston nimess‰ voi olla spaceja.
 */
std::istream& NFmiSmartToolInfo::Read(std::istream &file)
{
	string buffer;
	buffer.resize(512);
	file.getline(&buffer[0], buffer.size());
	itsCurrentScriptFileName = buffer;

	file.getline(&buffer[0], buffer.size());
	itsLastLoadDirectory = buffer;

	file >> fMakeDBCheckAtSend;
	return file;
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

