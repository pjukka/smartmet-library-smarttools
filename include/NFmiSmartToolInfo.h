/*!
 *  \file NFmiSmartToolInfo.h 
 *  C++ Class Name : NFmiSmartToolInfo \par
 *  ---------------------------------------------------------
 *  Luokka pit‰‰ sis‰ll‰‰n SmartTool:ia ja SmartToolDialogia
 *  koskevia tietoja.
 * 
 */

#ifndef  NFMISMARTTOOLINFO_H
#define  NFMISMARTTOOLINFO_H

#include <string>

class NFmiSmartToolInfo
{

public:
	NFmiSmartToolInfo(void);
	~NFmiSmartToolInfo(void);
	bool Init(const std::string &theFileName);
	bool Store(const std::string &theFileName);

	bool LoadAsCurrentScript(const std::string &theFileName);
	bool SaveCurrentScript(void);
	bool SaveAsCurrentScript(const std::string &theFileName);
	bool LoadDBChecker(void);
	bool SaveDBChecker(void);

	const std::string& CurrentInfoFile(void) const{return itsCurrentInfoFile;}
	const std::string& CurrentScript(void) const{return itsCurrentScript;}
	const std::string& CurrentScriptFileName(void) const{return itsCurrentScriptFileName;}
	const std::string& LastLoadDirectory(void) const {return itsLastLoadDirectory;}
	const std::string& DBCheckerFileName(void) const {return itsDBCheckerFileName;}
	const std::string& DBCheckerText(void) const {return itsDBCheckerText;}
	bool MakeDBCheckAtSend(void) const {return fMakeDBCheckAtSend;}
	bool IsThereDBCheckScript(void) const {return fIsThereDBCheckScript;}
	bool HasCurrentScriptFileName(void) const {return fHasCurrentScriptFileName;}

	void CurrentInfoFile(const std::string& newValue) {itsCurrentInfoFile = newValue;}
	void CurrentScript(const std::string& newValue) {itsCurrentScript = newValue;}
	void CurrentScriptFileName(const std::string &newValue) {itsCurrentScriptFileName = newValue;}
	void LastLoadDirectory(const std::string& newValue) {itsLastLoadDirectory = newValue;}
	void DBCheckerFileName(const std::string& newValue) {itsDBCheckerFileName = newValue;}
	void DBCheckerText(const std::string& newValue) {itsDBCheckerText = newValue;}
	void MakeDBCheckAtSend(bool newValue) {fMakeDBCheckAtSend = newValue;}
	void IsThereDBCheckScript(bool newValue) {fIsThereDBCheckScript = newValue;}
	void HasCurrentScriptFileName(bool newValue) {fHasCurrentScriptFileName = newValue;}

	//@{ \name Kirjoitus- ja luku-operaatiot (luokka tallettaa vain pari polkua tiedostoon talteen)
	std::ostream& Write(std::ostream &file) const;
	std::istream& Read(std::istream &file); 
	//@}

private:
	std::string ExtractPath(const std::string &theFullFileName);
	bool LoadLastScript(void);
	bool ReadFileToString(const std::string &theFileName, std::string *theString);

	std::string itsCurrentInfoFile; //! T‰m‰n olion tiedoston nimi polkuineen. 
	std::string itsCurrentScript; //! Dialogissa oleva scripti. 
	std::string itsCurrentScriptFileName; //! (talleta init-fileen) Dialogissa olevan scriptin polku ja tiedostonnimi kokonaisuudessaan. 
	std::string itsLastLoadDirectory; //! (talleta init-fileen) Viimeisin hakemisto, mist‰ on ladattu/talletettu smarttool scripti. 
	std::string itsDBCheckerFileName; //! Tiedoston nimi (polkuineen), mist‰ ladataan DBChecker smarttool scripti. 
	std::string itsDBCheckerText; //! Itse DBChecker smarttool scripti. 
	bool fMakeDBCheckAtSend; //! (talleta init-fileen) Tehd‰‰nkˆ tarkistus scripti automaattisesti, kun dataa l‰hetet‰‰n tietokantaan.
	bool fIsThereDBCheckScript; //! Onko ylip‰‰t‰‰n ollenkaan oletus tiedostossa tallessa DBChecker scripti‰.
	bool fHasCurrentScriptFileName; //! Tehd‰‰nkˆ tarkistus scripti automaattisesti, kun dataa l‰hetet‰‰n tietokantaan.
};

//@{ \name Globaalit NFmiPoint-luokan uudelleenohjaus-operaatiot
inline std::ostream& operator<<(std::ostream& os, const NFmiSmartToolInfo& item){return item.Write(os);}
inline std::istream& operator>>(std::istream& is, NFmiSmartToolInfo& item){return item.Read(is);}
//@}

#endif // NFMISMARTTOOLINFO_H
