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
#include "NFmiDataMatrix.h"

class NFmiQueryData;
class NFmiTimeDescriptor;
class NFmiInfoOrganizer;

class NFmiSmartToolUtil 
{
public:

	static NFmiQueryData* ModifyData(const std::string &theMacroText, NFmiQueryData* theModifiedData, bool createDrawParamFileIfNotExist);
	static NFmiQueryData* ModifyData(const std::string &theMacroText, NFmiQueryData* theModifiedData, NFmiTimeDescriptor *theTimes, bool createDrawParamFileIfNotExist);
	static NFmiQueryData* ModifyData(const std::string &theMacroText, NFmiQueryData* theModifiedData, const checkedVector<std::string> *theHelperDataFileNames, bool createDrawParamFileIfNotExist, bool goThroughLevels = false);
	static NFmiQueryData* ModifyData(const std::string &theMacroText, NFmiQueryData* theModifiedData, NFmiTimeDescriptor *theTimes, const checkedVector<std::string> *theHelperDataFileNames, bool createDrawParamFileIfNotExist, bool goThroughLevels = false);

private:
	static bool InitDataBase(NFmiInfoOrganizer *theDataBase, NFmiQueryData* theModifiedData, const checkedVector<std::string> *theHelperDataFileNames, bool createDrawParamFileIfNotExist);
	static bool InitDataBaseHelperData(NFmiInfoOrganizer &theDataBase, const checkedVector<std::string> &theHelperDataFileNames);
	static std::string GetWorkingDirectory(void);
};
#endif // NFMISMARTTOOLUTIL_H
