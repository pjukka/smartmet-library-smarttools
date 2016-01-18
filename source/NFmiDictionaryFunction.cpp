
/*!
 * T‰m‰ on Editorin k‰ytt‰m‰ sana kirja funktio.
 * Kieli versiot stringeihin tulevat t‰‰lt‰.
 */

#include "NFmiDictionaryFunction.h"

// HUOM! T‰m‰ on kopio NFmiEditMapGeneralDataDoc-luokan metodista, kun en voinut antaa t‰nne
// dokumenttia
std::string GetDictionaryString(const char *theMagicWord)
{
  const std::string baseWords = "MetEditor::Dictionary::";

  std::string usedMagicWord(theMagicWord);
  NFmiStringTools::ReplaceAll(usedMagicWord, "\t", "\\t");  // pit‰‰ muuttaa mahdollinen
                                                            // tabulaattori merkkisarjaksi, miss‰ on
                                                            // kenoviiva ja t per‰kk‰in, koska
                                                            // tabulaattori on siten sanakirjassa
  std::string finalMagicWord(baseWords);
  finalMagicWord += usedMagicWord;
  std::string dictionarySentense =
      NFmiSettings::Optional<std::string>(finalMagicWord.c_str(), std::string(theMagicWord));
  NFmiStringTools::ReplaceAll(
      dictionarySentense,
      "\\t",
      "\t");  // pit‰‰ muuttaa mahdolliset "\\t"-merkkijonot takaisin tabulaattoreiksi
  return dictionarySentense;
}
