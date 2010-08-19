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
class NFmiHelpDataInfoSystem;

class NFmiHelpDataInfo
{
public:
	NFmiHelpDataInfo(void);
	NFmiHelpDataInfo(const NFmiHelpDataInfo &theOther);
	~NFmiHelpDataInfo(void)
	{Clear();}

	void InitFromSettings(const std::string &theBaseKey, const std::string &theName);
	// void StoreToSettings(void);  // HUOM! ei toteuteta ainakaan viel‰ talletusta

	NFmiHelpDataInfo& operator=(const NFmiHelpDataInfo &theOther);
	void Clear(void);

	const std::string& Name(void) const {return itsName;}
	void Name(const std::string &newValue) {itsName = newValue;}
	const std::string& FileNameFilter(void) const {return itsFileNameFilter;}
	void FileNameFilter(const std::string &newValue, bool forceFileNameFilter = false);
	// t‰m‰ on viritys, ett‰ olisi funktio, jolla voidaan pyyt‰‰ k‰ytetty fileFilter, riippuen siit‰ onko cache k‰ytˆss‰ vai ei
	const std::string UsedFileNameFilter(const NFmiHelpDataInfoSystem &theHelpDataInfoSystem) const;
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
	bool MakeSoundingIndexData(void) const {return fMakeSoundingIndexData;}
	void MakeSoundingIndexData(bool newValue) {fMakeSoundingIndexData = newValue;}

private:

	std::string itsName; // t‰ll‰ nimell‰ erotetaan konffi-tiedostoissa eri datat
	std::string itsFileNameFilter; // tiedostonimi filtteri polun kera esim. d:\weto\wrk\data\in\data_1_*.sqd
	bool fForceFileFilterName; // jos t‰m‰ on erikoistilanteissa asetettu true:ksi, ei v‰litet‰ mahdillisista cachen k‰ytˆist‰, vaan UsedFilefilter-metodi palauttaa itsFileNameFilter:in arvon
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
	bool fMakeSoundingIndexData; // jos t‰m‰ on true, SmartMet tekee datasta oman sounding-index datan
	std::string itsBaseNameSpace;
};

class NFmiHelpDataInfoSystem
{
public:
	NFmiHelpDataInfoSystem(void)
	:itsDynamicHelpDataInfos()
	,itsStaticHelpDataInfos()
	,itsCacheDirectory()
	,itsCacheTmpDirectory()
	,itsCacheTmpFileNameFix()
	,fUseQueryDataCache(false)
	,fDoCeanCache(false)
	,itsCacheFileKeepMaxDays(-1)
	,itsCacheMaxFilesPerPattern(-1)
	,itsBaseNameSpace()
	{}

	void Clear(void)
	{}
	void InitFromSettings(const std::string &theInitNameSpace, std::string theHelpEditorFileNameFilter = "");
	// void StoreToSettings(void);  // HUOM! ei toteuteta ainakaan viel‰ talletusta
	void InitSettings(const NFmiHelpDataInfoSystem &theOther, bool fDoHelpDataInfo);

	NFmiHelpDataInfo& DynamicHelpDataInfo(int theIndex);
	NFmiHelpDataInfo& StaticHelpDataInfo(int theIndex);
	int DynamicCount(void) const{return static_cast<int>(itsDynamicHelpDataInfos.size());}
	int StaticCount(void) const{return static_cast<int>(itsStaticHelpDataInfos.size());}
	const NFmiArea* GetDataFilePatternAndArea(NFmiInfoData::Type theDataType, FmiProducerName theProdId, FmiParameterName theParamId, const NFmiArea *theZoomedArea, std::string &theFilePattern, bool fDemandMatchingArea);
	NFmiDataIdent GetNextSatelChannel(const NFmiDataIdent &theDataIdent, FmiDirection theDir);
	void AddDynamic(const NFmiHelpDataInfo &theInfo);
	void AddStatic(const NFmiHelpDataInfo &theInfo);
	void MarkAllDynamicDatasAsNotReaded();
	NFmiHelpDataInfo* FindHelpDataInfo(const std::string &theFileNameFilter);
	std::vector<std::string> GetUniqueCustomMenuList(void);
	std::vector<NFmiHelpDataInfo> GetCustomMenuHelpDataList(const std::string &theCustomFolder);
	const checkedVector<NFmiHelpDataInfo>& DynamicHelpDataInfos(void) const {return itsDynamicHelpDataInfos;}

	const std::string& CacheDirectory(void) const {return itsCacheDirectory;}
	void CacheDirectory(const std::string &newValue) {itsCacheDirectory = newValue;}
	const std::string& CacheTmpDirectory(void) const {return itsCacheTmpDirectory;}
	void CacheTmpDirectory(const std::string &newValue) {itsCacheTmpDirectory = newValue;}
	const std::string& CacheTmpFileNameFix(void) const {return itsCacheTmpFileNameFix;}
	void CacheTmpFileNameFix(const std::string &newValue) {itsCacheTmpFileNameFix = newValue;}
	bool UseQueryDataCache(void) const {return fUseQueryDataCache;}
	void UseQueryDataCache(bool newvalue) {fUseQueryDataCache = newvalue;}
	bool DoCeanCache(void) const {return fDoCeanCache;}
	void DoCeanCache(bool newValue) {fDoCeanCache = newValue;}
	float CacheFileKeepMaxDays(void) const {return itsCacheFileKeepMaxDays;}
	void CacheFileKeepMaxDays(float newValue) {itsCacheFileKeepMaxDays = newValue;}
	int CacheMaxFilesPerPattern(void) const {return itsCacheMaxFilesPerPattern;}
	void CacheMaxFilesPerPattern(int newValue) {itsCacheMaxFilesPerPattern = newValue;}

private:
	void InitDataType(const std::string &theBaseKey, checkedVector<NFmiHelpDataInfo> &theHelpDataInfos);
	bool IsSameTypeProjections(const NFmiArea *theFirst, const NFmiArea *theSecond);

	checkedVector<NFmiHelpDataInfo> itsDynamicHelpDataInfos; // t‰h‰n tulee jatkuvasti p‰ivitett‰v‰t datat kuten havainnot, tutka ja analyysi datat
	checkedVector<NFmiHelpDataInfo> itsStaticHelpDataInfos; // t‰h‰n tulee kerran ladattavat jutut kuten maa/meri maskit ja klimatologiset jutut

	// SmartMet voidaan laittaa k‰ytt‰m‰‰n queryData cachetusta, jolloin verkkopalvelimelta
	// luetaan data m‰‰r‰ttyyn cache-hakemistoon omalle kovalevylle. N‰in voidaan
	// v‰ltt‰‰ mahdolliset verkko-ongelmat memory-mapattujen dataojen kanssa.
	std::string itsCacheDirectory; // qdatat kopioidaan haluttaessa t‰h‰n hakemistoon
	std::string itsCacheTmpDirectory; // qdatat kopioidaan ensin t‰h‰n hakemistoon tmp-nimell‰ ja lopuksi renametaan 
									  // oiekaan hakemistoon oikealla nimell‰
									  // HUOM!! ett‰ tmp-file rename toimisi ns 'atomisesti', on hakemistojen oltava samalla levypartitiolla!!!
	std::string itsCacheTmpFileNameFix; // t‰m‰ name fix lis‰t‰‰n tmp tiedosto nimen alkuun ja loppuun
	bool fUseQueryDataCache; // onko cachetus systeemi p‰‰ll‰ vai ei?
	bool fDoCeanCache; // siivotaanko cachea vai ei
	float itsCacheFileKeepMaxDays; // kuinka vanhat tiedostot ainakin siivotaan pois (esim. 1.5 on 1.5 p‰iv‰‰ eli 36 tuntia) jos luku on <= 0 ei t‰t‰ k‰ytet‰
	int itsCacheMaxFilesPerPattern; // kuinka monta tiedostoa maksimissaan pidet‰‰n kutakin tiedosto patternia kohden, jos luku <= 0, ei t‰t‰ k‰ytet‰

	std::string itsBaseNameSpace;
};

#endif // __NFMIHELPDATAINFO_H__
