//© Ilmatieteenlaitos/Marko.
//Original 20.6.2006
//
// Luokka pit‰‰ huolta Editoriin konffatuista malli tuottajista.
// Voidaan konffata esim. 10 tuottajaa. Mutta editorin eri valikot ja systeemit
// itse p‰‰tt‰v‰t kuinka montaa ensimm‰ist‰ voidaan k‰ytt‰‰. Eli Esim. Luotausn‰ytˆn
// tuottaja valinta radio buttoneihin saa mukaan vain kolme mallia (pit‰‰ kai tehd‰
// joskus dropdown lista). Mutta esim. erilaisiin dynaamisiin parametrin valinta
// popup-valikoihin voidaan laittaa useampia tuottajia.
//---------------------------------------------------------- NFmiProducerSystem.h

#ifndef __NFMIPRODUCERSYSTEM_H__
#define __NFMIPRODUCERSYSTEM_H__

#include "NFmiProducer.h"
#include "NFmiProducerName.h"
#include "NFmiInfoData.h"
#include <string>
#include <vector>

class NFmiProducer;
class NFmiLevel;
class NFmiMetTime;

class NFmiProducerInfo
{
public:
	NFmiProducerInfo(void):itsName(),itsShortName(),itsUltraShortName(),itsProducerId(kFmiNoProducer),itsDescription(),fHasRealVerticalData(false){}

	const std::string& Name(void) const {return itsName;}
	void Name(const std::string &newValue) {itsName = newValue;}
	const std::string& ShortName(void) const {return itsShortName;}
	void ShortName(const std::string &newValue) {itsShortName = newValue;}

	const std::string& UltraShortName(void) const {return itsUltraShortName;}
	void UltraShortName(const std::string &newValue) {itsUltraShortName = newValue;}

	unsigned long ProducerId(void) {return itsProducerId;}
	void ProducerId(unsigned long newValue) {itsProducerId = newValue;}
	const std::string& Description(void) const {return itsDescription;}
	void Description(const std::string &newValue) {itsDescription = newValue;}
	NFmiProducer GetProducer(void);
	bool HasRealVerticalData(void) const {return fHasRealVerticalData;}
	void HasRealVerticalData(bool newValue) {fHasRealVerticalData = newValue;}
private:
	std::string itsName; // Pitempi nimi esim. Hirlam tai Ecmwf (voidaan k‰ytt‰‰ esim. popup-valikoissa, miss‰ on tilaa)
	std::string itsShortName; // Lyhyempi nimi esim. Hir tai Ec (k‰ytet‰‰n mm. jossain pikavalinnoissa nimen‰ ja smarttool-kielen tuottaja nimen‰ skripteiss‰)
	std::string itsUltraShortName; // Lyhyempi nimi esim. Hir tai Ec (k‰ytet‰‰n mm. jossain pikavalinnoissa nimen‰ ja smarttool-kielen tuottaja nimen‰ skripteiss‰)
	unsigned long itsProducerId;
	std::string itsDescription;
	bool fHasRealVerticalData; // esim. UK,DWDjaUSa datoissa ei ole nyt oikeasti vertikaalidataa, vaikka siin‰ on painepintoja, n‰ist‰ malleista ei haluta
							   // tehd‰ luotauksia, poikkileikkausia tai trajektori laskuja, joten n‰m‰ mallit saavat arvon false.
};

class NFmiProducerSystem
{
public:
	static const unsigned int gHelpEditorDataProdId = 996; // pistin t‰m‰n tuottaja id:n tanne, koska en viitsi laittaa t‰t‰
												// metkun editorin sis‰ist‰ help-data tuottaja id juttua
												// newbasen ProducerName listaan. T‰t‰ k‰ytet‰‰n ainakin
												// smarttool-kielen alustamisessa ja dokumentin HelpDataSystemin alustuksessa

	NFmiProducerSystem(void):itsProducers(){}

	void Add(const NFmiProducerInfo &newValue){itsProducers.push_back(newValue);}
	void Clear(void){itsProducers.clear();}
	std::vector<NFmiProducerInfo>& Producers(void) {return itsProducers;}
	void InitFromSettings(const std::string &theInitNameSpace);
	const std::string& GetInitializeLogStr(void) const {return itsInitializeLogStr;}
	bool ExistProducer(unsigned int index1Based) const;
	NFmiProducerInfo& Producer(unsigned int index1Based);
	unsigned int FindProducerInfo(const NFmiProducer &theProducer);
	NFmiString GetProducerAndLevelTypeString(const NFmiProducer &theProducer, const NFmiLevel &theLevel, const NFmiMetTime &theOriginTime, bool fEncloseInBracers);
private:
	NFmiProducerInfo GetProducerInfoFromSettings(const std::string &theUsedNameSpaceBase);

	std::vector<NFmiProducerInfo> itsProducers; // T‰h‰n voi siis laittaa tuottajia kuinka paljon tahansa,
												// mutta kannattaa sijoittaa alkuun t‰rkeimm‰t, koska
												// Editori p‰‰tt‰‰ eri ikkunoissa/tyˆkaluissa, kuinka monta
												// tuottajaa (alkup‰‰st‰) se ottaa k‰yttˆˆn. Esim. Luotaus
												// n‰ytˆn malli valinta radio-buttoneita on esim. vain kolme (t‰ll‰ hetkell‰)
	std::string itsInitializeLogStr; // jos luokka initialisoidaan settingeist‰, t‰h‰n talletetaan loki stringi
};

struct NFmiProducerHelperInfo
{
	NFmiProducerHelperInfo(void)
	:itsName()
	,itsProducerId(kFmiNoProducer)
	,itsDataType(NFmiInfoData::kNoDataType)
	,fGroundData(false)
	{}

	NFmiProducerHelperInfo(const std::string theName, int theProducerId, NFmiInfoData::Type theDataType, bool groundData = true)
	:itsName(theName)
	,itsProducerId(theProducerId)
	,itsDataType(theDataType)
	,fGroundData(groundData)
	{}

	static NFmiProducerHelperInfo MakeProducerHelperInfo(NFmiProducerInfo &theProducerInfo, NFmiInfoData::Type theType, bool groundData = true)
	{
		return NFmiProducerHelperInfo(theProducerInfo.Name(), theProducerInfo.ProducerId(), theType, groundData);
	}

	std::string itsName;
	unsigned long itsProducerId;
	NFmiInfoData::Type itsDataType;
	bool fGroundData;
};


#endif // __NFMIPRODUCERSYSTEM_H__
