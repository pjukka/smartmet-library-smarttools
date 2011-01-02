#pragma once


#include "NFmiMilliSecondTimer.h"
#include "NFmiDataMatrix.h"
#include "NFmiMetTime.h"
#include "NFmiInfoData.h"
#include "boost/shared_ptr.hpp"
#include <list>
#include <set>

class NFmiOwnerInfo;
class NFmiFastQueryInfo;

// NFmiQueryDataKeeper on luokka joka pit‰‰ kirjanpitoa NFmiInfoOrganizer-luokassa
// s‰ilytett‰vist‰ queryDatoista. 
// HUOM! T‰t‰ luokkaa ei ole tarkoitettu k‰ytett‰v‰ksi kuin NFmiInfoOrganizerin sis‰isesti!!!
class NFmiQueryDataKeeper
{
public:
	NFmiQueryDataKeeper(void);
	NFmiQueryDataKeeper(boost::shared_ptr<NFmiOwnerInfo> &theOriginalData);
	~NFmiQueryDataKeeper(void);

	boost::shared_ptr<NFmiOwnerInfo> OriginalData(void); // T‰t‰ saa k‰ytt‰‰ vain NFmiInfoOrganizer-luokka sis‰isesti, koska t‰t‰ ei ole tarkoitus palauttaa, 
														// kun tarvitaan moni-s‰ie turvallinen info-iteraattori kopio, k‰ytet‰‰n mieluummin GetIter-metodia.
	boost::shared_ptr<NFmiFastQueryInfo> GetIter(void); // T‰m‰ palauttaa vapaana olevan Info-iteraattori kopion dataan.
	int Index(void) const {return itsIndex;}
	void Index(int newValue) {itsIndex = newValue;}
	const NFmiMetTime& OriginTime(void) const {return itsOriginTime;}
	const std::string& DataFileName(void) {return itsDataFileName;}
	int LastUsedInMS(void) const;

private:
	boost::shared_ptr<NFmiOwnerInfo> itsData; // t‰m‰ on originaali data
	NFmiMilliSecondTimer itsLastTimeUsedTimer;  // aina kun kyseist‰ dataa k‰ytet‰‰n, k‰ytet‰‰n StartTimer-metodia, jotta myˆhemmin voidaan 
												// laskea, voidaanko kyseinen data siivota pois muistista (jos dataa ei ole k‰ytetty tarpeeksi pitk‰‰n aikaan)
	int itsIndex; // malliajo datoissa 0 arvo tarkoittaa viimeisint‰ ja -1 sit‰ edellist‰ jne.
	checkedVector<boost::shared_ptr<NFmiFastQueryInfo> > itsIteratorList; // originaali datasta tehn‰‰n tarvittaessa n kpl iteraattori kopioita, ulkopuoliset rutiinit/s‰ikeet
																		// k‰ytt‰v‰t aina vain iteraattori-kopioita alkuper‰isest‰, jolloin niit‰ voidaan k‰ytt‰‰ eri s‰ikeiss‰ yht'aikaa.
																		// n‰m‰ luodaan on demandina, eli jos InfoOrganizerilta pyydet‰‰n dataa, ja listassa ei ole vapaata iteraattoria
																		// luodaan t‰llˆin uusi kopio joka palautetaan.
																		// TODO: Miten tied‰n ett‰ joku rutiini/s‰ie on lopettanut iteraattorin k‰ytˆn? Ehk‰ shared_ptr:n use_count:in avulla?
	NFmiMetTime itsOriginTime; // t‰h‰n talletetaan datan origin-time vertailuja helpottamaan
	std::string itsDataFileName; // t‰h‰n talletetaan datan tiedosto nimi
};

// NFmiQueryDataSetKeeper-luokka pit‰‰ kirjaa n kpl viimeisit‰ malliajoista/datasta
class NFmiQueryDataSetKeeper
{
public:
	typedef std::list<boost::shared_ptr<NFmiQueryDataKeeper> > ListType;

	NFmiQueryDataSetKeeper(void);
	NFmiQueryDataSetKeeper(boost::shared_ptr<NFmiOwnerInfo> &theData, int theMaxLatestDataCount = 0, int theModelRunTimeGap = 0, int theKeepInMemoryTime = 5);
	~NFmiQueryDataSetKeeper(void);

	void AddData(boost::shared_ptr<NFmiOwnerInfo> &theData, bool fFirstData = false);
	boost::shared_ptr<NFmiQueryDataKeeper> GetDataKeeper(int theIndex = 0);
	const std::string& FilePattern(void) const {return itsFilePattern;}
	void FilePattern(const std::string &newValue) {itsFilePattern = newValue;}
	int MaxLatestDataCount(void) const {return itsMaxLatestDataCount;}
	void MaxLatestDataCount(int newValue) {itsMaxLatestDataCount = newValue;}
	int ModelRunTimeGap(void) const {return itsModelRunTimeGap;}
	void ModelRunTimeGap(int newValue) {itsModelRunTimeGap = newValue;}
	std::set<std::string> GetAllFileNames(void);
	int CleanUnusedDataFromMemory(void);
	int KeepInMemoryTime(void) const {return itsKeepInMemoryTime;}
	void KeepInMemoryTime(int newValue) {itsKeepInMemoryTime = newValue;}

	size_t DataCount(void);
	size_t DataByteCount(void);

private:
	void AddDataToSet(boost::shared_ptr<NFmiOwnerInfo> &theData);
	void RecalculateIndexies(const NFmiMetTime &theLatestOrigTime);
	void DeleteTooOldDatas(void);
	bool DoOnDemandOldDataLoad(int theIndex);
	void ReadAllOldDatasInMemory(void);
	bool ReadDataFileInUse(const std::string &theFileName);
	bool CheckKeepTime(ListType::iterator &it);

	ListType itsQueryDatas; // t‰ss‰ on n kpl viimeisint‰ malliajoa tallessa (tai esim. havaintojen tapauksessa vain viimeisin data)
	int itsMaxLatestDataCount; // kuinka monta viimeisint‰ malliajoa/dataa maksimiss‰‰n kullekin datalle on, 0 jos kyse esim. havainnoista, joille ei ole kuin viimeisin data.
	int itsModelRunTimeGap; // mill‰ ajov‰leill‰ kyseisen datan mallia ajetaan (yksikkˆ minuutteja), jos kyse havainnosta, eli ei ole kuin viimeinen data, arvo 0 ja jos kyse esim. editoidusta datasta (ep‰m‰‰r‰inen ilmestymisv‰li) on arvo -1.
	std::string itsFilePattern; // erilaiset datat erotellaan fileFilterin avulla (esim. "D:\smartmet\wrk\data\local\*_hirlam_skandinavia_mallipinta.sqd")
	NFmiMetTime itsLatestOriginTime; // t‰h‰n talletetaan aina viimeisimm‰n datan origin-time vertailuja helpottamaan
	NFmiInfoData::Type itsDataType; // t‰h‰n laitetaan 1. datan datattyyppi (pit‰isi olla yhten‰inen kaikille setiss‰ oleville datoille)
	int itsKeepInMemoryTime; // kuinka kauan pidet‰‰n data muistissa, jos sit‰ ei ole k‰ytetty. yksikkˆ on minuutteja
};
