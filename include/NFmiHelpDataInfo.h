// NFmiHelpDataInfo.h
//
// NFmiHelpDataInfo-luokka hoitaa yhden Meteorologin editorin apu-datan
// tiedot. Se sis‰lt‰‰ tiedon poluista, tiedosto filttereist‰ ja mik‰
// on viimeksi ladatun datan tiedoston aikaleima jne.
//
// NFmiHelpDataInfoSystem-luokka taas pit‰‰ sis‰ll‰‰n kaikki Metkun editorin 
// k‰ytt‰m‰t apudatat.

#ifndef __NFMIHELPDATAINFO_H__
#define __NFMIHELPDATAINFO_H__

#include "NFmiInfoData.h"
#include "NFmiDataIdent.h"
#include "NFmiDataMatrix.h" // t‰‰lt‰ tulee checkedVector
#include "NFmiProducerName.h" // t‰‰lt‰ tulee checkedVector

#include <iosfwd>
#include <string>

class NFmiArea;

class NFmiHelpDataInfo
{
public:
	NFmiHelpDataInfo(void);
	NFmiHelpDataInfo(const NFmiHelpDataInfo &theOther);
	~NFmiHelpDataInfo(void)
	{Clear();}

	void InitFromSettings(const std::string &theInitNameSpace, const std::string &theRootDir);
	// void StoreToSettings(void);  // HUOM! ei toteuteta ainakaan viel‰ talletusta

	NFmiHelpDataInfo& operator=(const NFmiHelpDataInfo &theOther);
	void Clear(void);
	const std::string& FileNameFilter(void) const {return itsFileNameFilter;}
	void FileNameFilter(const std::string &newValue) {itsFileNameFilter = newValue;}
	const std::string& LatestFileName(void) const {return itsLatestFileName;}
	void LatestFileName(const std::string &newName) {itsLatestFileName = newName;}
	NFmiInfoData::Type DataType(void) const {return itsDataType;}
	void DataType(NFmiInfoData::Type newValue) {itsDataType = newValue;}
	time_t LatestFileTimeStamp(void) const {return itsLatestFileTimeStamp;}
	void LatestFileTimeStamp(time_t newValue) {itsLatestFileTimeStamp = newValue;}
	int FakeProducerId(void) const {return itsFakeProducerId;}
	void FakeProducerId(int newValue) { itsFakeProducerId = newValue; }
	const NFmiDataIdent& ImageDataIdent(void) const {return itsImageDataIdent;}
	void ImageDataIdent(const NFmiDataIdent& newValue) { itsImageDataIdent = newValue; }
	const NFmiArea* ImageArea(void) const {return itsImageArea;}
	void ImageArea(NFmiArea *newValue) { if (!itsImageArea) itsImageArea = newValue; } // Can only be called once
	bool NotifyOnLoad(void) const {return fNotifyOnLoad;}
	void NotifyOnLoad(bool newValue) {fNotifyOnLoad = newValue;}
	const std::string& NotificationLabel(void) const {return itsNotificationLabel;}
	void NotificationLabel(const std::string &newValue) {itsNotificationLabel = newValue;}
	const std::string& CustomMenuFolder(void) const {return itsCustomMenuFolder;}
	void CustomMenuFolder(const std::string &newValue) {itsCustomMenuFolder = newValue;}

	int ReportNewDataTimeStepInMinutes(void) const {return itsReportNewDataTimeStepInMinutes;}
	void ReportNewDataTimeStepInMinutes(int newValue) {itsReportNewDataTimeStepInMinutes = newValue;}
	const std::string& ReportNewDataLabel(void) const {return itsReportNewDataLabel;}
	void ReportNewDataLabel(const std::string &newValue) {itsReportNewDataLabel = newValue;}
	const std::string& CombineDataPathAndFileName(void) const {return itsCombineDataPathAndFileName;}
	void CombineDataPathAndFileName(const std::string &newValue) {itsCombineDataPathAndFileName = newValue;}
	int CombineDataMaxTimeSteps(void) const {return itsCombineDataMaxTimeSteps;}
	void CombineDataMaxTimeSteps(int newValue) {itsCombineDataMaxTimeSteps = newValue;}

private:

	std::string itsFileNameFilter; // tiedostonimi filtteri polun kera esim. d:\weto\wrk\data\in\data_1_*.sqd
	std::string itsLatestFileName; // kun filtterill‰ on haettu tiedostot ja uusin on lˆytynyt, talletetaan se t‰h‰n
	NFmiInfoData::Type itsDataType; // esim. analyysi, havainto jne.
	time_t itsLatestFileTimeStamp; // viimeksi luetus
	int itsFakeProducerId; // joskus pit‰‰ muuttaa tuottaja id ennenkuin data otetaan k‰yttˆˆn esim. kepa data, jos t‰m‰ on 0, ei tehd‰ mit‰‰n

	// Seuraavat koskevat vain image-tyyppisi‰ juttuja (muissa n‰it‰ ei lueta/talleteta tiedostoon)
	std::string itsImageProjectionString; // jos data on image-tyyppist‰ (satel,radar, jne.), on tiedostossa
									// stringi, joka sis‰lt‰‰ projektio tiedon jonka NFmiAreaFactory ymm‰rt‰‰
	NFmiDataIdent itsImageDataIdent; // tieto image paramin id:st‰, nimest‰ (vain id ja nimi talletetaan tiedostoon)
	NFmiArea *itsImageArea; // t‰h‰n luodaan ed. stringin avulla projektio, t‰m‰ ei ole tallessa tiedostossa
	// Edelliset koskevat vain image-tyyppisi‰ juttuja

	bool fNotifyOnLoad; // Jos datan latauksen yhteydess‰ halutaan tehd‰ ilmoitus, t‰m‰ on true. Oletus arvo on false
	std::string itsNotificationLabel; // Jos notifikaatioon halutaan tietty sanoma, se voidaan antaa t‰h‰n. Defaulttina annetaan tiedoston nimi
	std::string itsCustomMenuFolder; // Jos data halutaan laittaa haluttuun hakemistoon param-popupeissa, tehd‰‰ sellainen asetus helpdata 
								// konffeihin. Defaulttina tyhj‰, jolloin data menee 'normaaliin' paikkaansa valikoissa.
	int itsReportNewDataTimeStepInMinutes; // Default arvo on 0, jolloin t‰ll‰ ei ole vaikutusta. T‰m‰n avulla voidaan sanoa
											// ett‰ SmartMetin pit‰‰ tehd‰ raportointia 'puhekuplilla', kun tulee uutta dataa esim. uudelle tunnille.
	std::string itsReportNewDataLabel; // Jos halutaan tietty teksti viestiin, se lis‰t‰‰n t‰h‰n.
	std::string itsCombineDataPathAndFileName; // jos t‰m‰ on m‰‰ritelty, tehd‰‰n fileNameFilterist‰ (hakemistosta) lˆytyvist‰ datoista yhdistelm‰ 
											   // data ja se talletetaan t‰h‰n hakemistoon annetulla nimell‰ ja aikaleimalla (nimess‰ t‰hden tilalle laitetaan aikaleima)
	int itsCombineDataMaxTimeSteps; // jos ei haluaa rajoittaa kuinka iso yhdistelm‰ datasta tehd‰‰n, t‰m‰n voi m‰‰ritell‰
	std::string itsBaseNameSpace;
};

class NFmiHelpDataInfoSystem
{
public:
	NFmiHelpDataInfoSystem(void)
	:itsDynamicHelpDataInfos()
	,itsStaticHelpDataInfos()
	,itsRootDirectory()
	,itsBaseNameSpace()
	{}

	void Clear(void)
	{}
	void InitFromSettings(const std::string &theInitNameSpace, std::string theHelpEditorFileNameFilter = "");
	// void StoreToSettings(void);  // HUOM! ei toteuteta ainakaan viel‰ talletusta

	NFmiHelpDataInfo& DynamicHelpDataInfo(int theIndex);
	NFmiHelpDataInfo& StaticHelpDataInfo(int theIndex);
	int DynamicCount(void) const{return static_cast<int>(itsDynamicHelpDataInfos.size());}
	int StaticCount(void) const{return static_cast<int>(itsStaticHelpDataInfos.size());}
	const NFmiArea* GetDataFilePatternAndArea(NFmiInfoData::Type theDataType, FmiProducerName theProdId, FmiParameterName theParamId, const NFmiArea *theZoomedArea, std::string &theFilePattern, bool fDemandMatchingArea);
	NFmiDataIdent GetNextSatelChannel(const NFmiDataIdent &theDataIdent, FmiDirection theDir);
	void AddDynamic(const NFmiHelpDataInfo &theInfo);
	void AddStatic(const NFmiHelpDataInfo &theInfo);
	const std::string& RootDirectory(void) const {return itsRootDirectory;}
	void RootDirectory(const std::string &theRoot) {itsRootDirectory = theRoot;}
	void MarkAllDynamicDatasAsNotReaded();
	NFmiHelpDataInfo* FindHelpDataInfo(const std::string &theFileNameFilter);
	std::vector<std::string> GetUniqueCustomMenuList(void);
	std::vector<NFmiHelpDataInfo> GetCustomMenuHelpDataList(const std::string &theCustomFolder);
	const checkedVector<NFmiHelpDataInfo>& DynamicHelpDataInfos(void) const {return itsDynamicHelpDataInfos;}
private:
	void InitDataType(const std::string &theBaseKey, const std::string &theRootDir, checkedVector<NFmiHelpDataInfo> &theHelpDataInfos);
	bool IsSameTypeProjections(const NFmiArea *theFirst, const NFmiArea *theSecond);

	checkedVector<NFmiHelpDataInfo> itsDynamicHelpDataInfos; // t‰h‰n tulee jatkuvasti p‰ivitett‰v‰t datat kuten havainnot, tutka ja analyysi datat
	checkedVector<NFmiHelpDataInfo> itsStaticHelpDataInfos; // t‰h‰n tulee kerran ladattavat jutut kuten maa/meri maskit ja klimatologiset jutut
	std::string itsRootDirectory; // jos halutaan, k‰yttet‰‰n root directoria helpottamaan datan latauksia

	std::string itsBaseNameSpace;
};

#endif // __NFMIHELPDATAINFO_H__
