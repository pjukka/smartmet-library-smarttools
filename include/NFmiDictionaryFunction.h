
/*!  NFmiDictionaryFunction.h
 * T‰m‰ on Editorin k‰ytt‰m‰ sana kirja funktio. 
 * Kieli versiot stringeihin tulevat t‰‰lt‰.
 */

#ifndef NFMIDICTIONARYFUNCTION_H
#define NFMIDICTIONARYFUNCTION_H

#include "NFmiSettings.h"

// HUOM! T‰m‰ on kopio NFmiEditMapGeneralDataDoc-luokan metodista, kun en voinut antaa t‰nne dokumenttia
inline std::string GetDictionaryString(const char *theMagicWord)
{
	const std::string baseWords = "MetEditor::Dictionary::";

	std::string finalMagicWord(baseWords);
	finalMagicWord += theMagicWord;
	return NFmiSettings::Optional<std::string>(finalMagicWord.c_str(), std::string("XXXXX"));
}

#endif
