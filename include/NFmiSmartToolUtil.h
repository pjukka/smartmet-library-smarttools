// ======================================================================
/*!
 * \file NFmiSmartToolUtil.h
 * \brief Interface of class NFmiSmartToolUtil
 */
// ======================================================================
/*!
 * \class NFmiSmartToolUtil
 *
 * T‰m‰ luokka tekee halutut smarttool-operaatiot halutuille datoille.
 * Toiminta samanlainen kuin NFmiQueryDataUtil-luokalla, mutta muutokset 
 * tehd‰‰n erilaisilla scripteill‰. Muokkaus datoihin tapahtuu luokan static
 * funktioilla.
 */
// ======================================================================
#ifndef  NFMISMARTTOOLUTIL_H
#define  NFMISMARTTOOLUTIL_H

#include <string>
#include <vector>

class NFmiQueryData;
class NFmiTimeDescriptor;
class NFmiInfoOrganizer;

class NFmiSmartToolUtil 
{
public:

	static NFmiQueryData* ModifyData(const std::string &theMacroText, NFmiQueryData* theModifiedData);
	static NFmiQueryData* ModifyData(const std::string &theMacroText, NFmiQueryData* theModifiedData, NFmiTimeDescriptor *theTimes);
	static NFmiQueryData* ModifyData(const std::string &theMacroText, NFmiQueryData* theModifiedData, const std::vector<std::string> *theHelperDataFileNames);
	static NFmiQueryData* ModifyData(const std::string &theMacroText, NFmiQueryData* theModifiedData, NFmiTimeDescriptor *theTimes, const std::vector<std::string> *theHelperDataFileNames);

private:
	static bool InitDataBase(NFmiInfoOrganizer *theDataBase, NFmiQueryData* theModifiedData, const std::vector<std::string> *theHelperDataFileNames);
	static bool InitDataBaseHelperData(NFmiInfoOrganizer &theDataBase, const std::vector<std::string> &theHelperDataFileNames);
	static std::string GetWorkingDirectory(void);
};
#endif // NFMISMARTTOOLUTIL_H
