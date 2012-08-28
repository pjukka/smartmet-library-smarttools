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

	void InitFromSettings(const std::string &theBaseKey, const std::string &theName, const NFmiHelpDataInfoSystem &theHelpDataSystem);
	// void StoreToSettings(void);  // HUOM! ei toteuteta ainakaan viel‰ talletusta

	NFmiHelpDataInfo& operator=(const NFmiHelpDataInfo &theOther);
	void Clear(void);

	bool IsCombineData(void) const {return itsCombineDataPathAndFileName.empty() == false;}
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
	void ImageArea(NFmiArea *newValue);
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
	bool ForceFileFilterName(void) const {return fForceFileFilterName;}
	void ForceFileFilterName(bool newValue) {fForceFileFilterName = newValue;}
	int AdditionalArchiveFileCount(void) const {return itsAdditionalArchiveFileCount;}
	void AdditionalArchiveFileCount(int newValue) {itsAdditionalArchiveFileCount = newValue;}
	bool IsEnabled(void) const {return fEnable;}
	void Enable(bool newValue) {fEnable = newValue;}
	bool NonFixedTimeGab(void) const {return fNonFixedTimeGab;}
	void NonFixedTimeGab(bool newValue) {fNonFixedTimeGab = newValue;}

	const std::string& PartialDataCacheFileNameFilter(void) const {return itsPartialDataCacheFileNameFilter;}
	void PartialDataCacheFileNameFilter(const std::string &newValue) {itsPartialDataCacheFileNameFilter = newValue;}
private:

	std::string itsName; // t‰ll‰ nimell‰ erotetaan konffi-tiedostoissa eri datat
	std::string itsFileNameFilter; // tiedostonimi filtteri polun kera esim. d:\weto\wrk\data\in\data_1_*.sqd
	std::string itsPartialDataCacheFileNameFilter; // cachen k‰yttˆ ja partial-datat ovat oma lukunsa ja niit‰ varten pit‰‰ tehd‰ filefiltteri
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
	int itsAdditionalArchiveFileCount;	// defaultti on 0, joitakin datoja (esim. kepa-datoja, joita tuotetaan n. 15-20 per p‰iv‰) 
										// halutaan s‰ilytt‰‰ enemm‰n kuin muita NFmiInfoOrganizer:issa. T‰ll‰ s‰‰dˆll‰ saadaan pidetty‰ ylim‰‰r‰iset datat.
	bool fEnable; // T‰m‰ on pakollinen s‰‰tˆ optio siit‰, ett‰ onko data k‰ytˆss‰. T‰m‰ s‰‰t‰‰ ladataanko dataa cacheen, SmartMEtiin, tehd‰‰nkˆ soundinIndex-dataa, 
				  // yhdistelm‰ datoja. Lis‰ksi tuleeko data popup-valikoihin, tai mihink‰‰n muihin datavalikoihin.
	bool fNonFixedTimeGab;  // T‰m‰ on valinnainen asetus. Onko datan malliajo v‰li m‰‰rittelem‰tˆn. Esim. silam-datat ovat ep‰m‰‰r‰isesti ajettuja, 
							// myˆs virallinen editoitu data on m‰‰rittelem‰tˆn, mutta se hoidetaan erikois tapauksena.
};

class NFmiHelpDataInfoSystem
{
public:
	NFmiHelpDataInfoSystem(void)
	:itsDynamicHelpDataInfos()
	,itsStaticHelpDataInfos()
	,itsCacheDirectory()
	,itsCacheTmpDirectory()
	,itsCachePartialDataDirectory()
	,itsCacheTmpFileNameFix()
	,fUseQueryDataCache(false)
	,fDoCleanCache(false)
	,itsCacheFileKeepMaxDays(-1)
	,itsCacheMaxFilesPerPattern(-1)
	,itsCacheMediumFileSizeMB(40)
	,itsCacheLargeFileSizeMB(200)
	,itsCacheMaximumFileSizeMB(10000)
	,itsBaseNameSpace()
	{}

	void Clear(void)
	{}
	void InitFromSettings(const std::string &theInitNameSpace, std::string theHelpEditorFileNameFilter = "", std::string theHelpDataName = "");
	void StoreToSettings(void);
	void InitSettings(const NFmiHelpDataInfoSystem &theOther, bool fDoHelpDataInfo);

	NFmiHelpDataInfo& DynamicHelpDataInfo(int theIndex);
	NFmiHelpDataInfo& StaticHelpDataInfo(int theIndex);
	int DynamicCount(void) const{return static_cast<int>(itsDynamicHelpDataInfos.size());}
	int StaticCount(void) const{return static_cast<int>(itsStaticHelpDataInfos.size());}
	const NFmiArea* GetDataFilePatternAndArea(NFmiInfoData::Type theDataType, FmiProducerName theProdId, FmiParameterName theParamId, const NFmiArea *theZoomedArea, std::string &theFilePattern, bool fDemandMatchingArea);
	NFmiDataIdent GetNextSatelChannel(const NFmiDataIdent &theDataIdent, FmiDirection theDir);
	void AddDynamic(const NFmiHelpDataInfo &theInfo);
	void AddStatic(const NFmiHelpDataInfo &theInfo);
	void ResetAllDynamicDataTimeStamps();
	NFmiHelpDataInfo* FindHelpDataInfo(const std::string &theFileNameFilter);
	std::vector<std::string> GetUniqueCustomMenuList(void);
	std::vector<NFmiHelpDataInfo> GetCustomMenuHelpDataList(const std::string &theCustomFolder);
	const checkedVector<NFmiHelpDataInfo>& DynamicHelpDataInfos(void) const {return itsDynamicHelpDataInfos;}
	const checkedVector<NFmiHelpDataInfo>& StaticHelpDataInfos(void) const {return itsStaticHelpDataInfos;}

	const std::string& CacheDirectory(void) const {return itsCacheDirectory;}
	void CacheDirectory(const std::string &newValue) {itsCacheDirectory = newValue;}
	const std::string& CacheTmpDirectory(void) const {return itsCacheTmpDirectory;}
	void CacheTmpDirectory(const std::string &newValue) {itsCacheTmpDirectory = newValue;}
	const std::string& CacheTmpFileNameFix(void) const {return itsCacheTmpFileNameFix;}
	void CacheTmpFileNameFix(const std::string &newValue) {itsCacheTmpFileNameFix = newValue;}
	bool UseQueryDataCache(void) const {return fUseQueryDataCache;}
	void UseQueryDataCache(bool newvalue) {fUseQueryDataCache = newvalue;}
	bool DoCleanCache(void) const {return fDoCleanCache;}
	void DoCleanCache(bool newValue) {fDoCleanCache = newValue;}
	float CacheFileKeepMaxDays(void) const {return itsCacheFileKeepMaxDays;}
	void CacheFileKeepMaxDays(float newValue) {itsCacheFileKeepMaxDays = newValue;}
	int CacheMaxFilesPerPattern(void) const {return itsCacheMaxFilesPerPattern;}
	void CacheMaxFilesPerPattern(int newValue) {itsCacheMaxFilesPerPattern = newValue;}
	double CacheMediumFileSizeMB(void) const {return itsCacheMediumFileSizeMB;}
	void CacheMediumFileSizeMB(double newValue) {itsCacheMediumFileSizeMB = newValue;}
	double CacheLargeFileSizeMB(void) const {return itsCacheLargeFileSizeMB;}
	void CacheLargeFileSizeMB(double newValue) {itsCacheLargeFileSizeMB = newValue;}
	double CacheMaximumFileSizeMB(void) const {return itsCacheMaximumFileSizeMB;}
	void CacheMaximumFileSizeMB(double newValue) {itsCacheMaximumFileSizeMB = newValue;}
	const std::string& CachePartialDataDirectory(void) const {return itsCachePartialDataDirectory;}
	void CachePartialDataDirectory(const std::string &newValue) {itsCachePartialDataDirectory = newValue;}
private:
	void InitDataType(const std::string &theBaseKey, checkedVector<NFmiHelpDataInfo> &theHelpDataInfos, bool fStaticData);
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
	std::string itsCachePartialDataDirectory; // combineData-threadin k‰ytt‰m‰t datat kopioidaan t‰nne cacheen
	std::string itsCacheTmpFileNameFix; // t‰m‰ name fix lis‰t‰‰n tmp tiedosto nimen alkuun ja loppuun
	bool fUseQueryDataCache; // onko cachetus systeemi p‰‰ll‰ vai ei?
	bool fDoCleanCache; // siivotaanko cachea vai ei
	float itsCacheFileKeepMaxDays; // kuinka vanhat tiedostot ainakin siivotaan pois (esim. 1.5 on 1.5 p‰iv‰‰ eli 36 tuntia) jos luku on <= 0 ei t‰t‰ k‰ytet‰
	int itsCacheMaxFilesPerPattern; // kuinka monta tiedostoa maksimissaan pidet‰‰n kutakin tiedosto patternia kohden, jos luku <= 0, ei t‰t‰ k‰ytet‰

	// medium ja large koot jakavat cachetettavat qdatat kolmeen osaan:
	// 1. pienet tiedostot 0 <= size < medium
	// 2. keskikokoiset tiedostot medium <= size < large
	// 3. isot tiedostot large <= size < ‰‰retˆn
	// Jokaiselle kokoluokalle tehd‰‰n SmartMetissa oma datan kopiointi threadi, n‰in isot tiedostot 
	// eiv‰t j‰‰ blokkaamaan pinempien tiedostojen kopiointia.
	double itsCacheMediumFileSizeMB; 
	double itsCacheLargeFileSizeMB;
	double itsCacheMaximumFileSizeMB; // t‰t‰ isompia tiedostoja ei cache suostu siirt‰m‰‰n

	std::string itsBaseNameSpace;
};

#endif // __NFMIHELPDATAINFO_H__
