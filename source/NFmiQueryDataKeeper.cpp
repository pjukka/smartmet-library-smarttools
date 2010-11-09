
#include "NFmiQueryDataKeeper.h"
#include "NFmiOwnerInfo.h"

// ************* NFmiQueryDataKeeper-class **********************

NFmiQueryDataKeeper::NFmiQueryDataKeeper(boost::shared_ptr<NFmiOwnerInfo> &theOriginalData)
:itsData(theOriginalData)
,itsLastTimeUsedtimer()
,itsKeepInMemoryTime(10)
,itsIndex(0)
,itsIteratorList()
{
}

NFmiQueryDataKeeper::~NFmiQueryDataKeeper(void)
{
}

boost::shared_ptr<NFmiOwnerInfo> NFmiQueryDataKeeper::OriginalData(void) 
{
	return itsData;
}

// Tämä palauttaa vapaana olevan Info-iteraattori kopion dataan.
boost::shared_ptr<NFmiFastQueryInfo> NFmiQueryDataKeeper::GetIter(void)
{
	// TODO: Tämä vaatii sitten säie turvallisen lukituksen

	// Katsotaan onko listassa yhtään Info-iteraattoria, joka ei ole käytössä (ref-count = 1)
	for(size_t i = 0; i < itsIteratorList.size(); i++)
	{
		if(itsIteratorList[i].use_count() <= 1)
			return itsIteratorList[i];
	}

	// Ei löytynyt vapaata (tai ollenkaan) Info-iteraattoria, pitää luoda sellainen ja lisätä listaan ja paluttaa se
	boost::shared_ptr<NFmiFastQueryInfo> infoIter(new NFmiOwnerInfo(*(OriginalData().get())));
	itsIteratorList.push_back(infoIter);
	return infoIter;
}

// ************* NFmiQueryDataSetKeeper-class **********************

NFmiQueryDataSetKeeper::NFmiQueryDataSetKeeper(void)
:itsQueryDatas()
,itsMaxLatestDataCount(0)
,itsModelRunTimeGap(6*60)
,itsFilePattern()
,itsLatestOriginTime()
{
}

NFmiQueryDataSetKeeper::NFmiQueryDataSetKeeper(boost::shared_ptr<NFmiOwnerInfo> &theData)
{
	AddData(theData, true); // true tarkoittaa että kyse on 1. lisättävästä datasta
}

NFmiQueryDataSetKeeper::~NFmiQueryDataSetKeeper(void)
{
}

// Lisätätään annettu data keeper-settiin.
// Jos	itsMaxLatestDataCount on 0, tyhjennnetään olemassa olevat listat ja datat ja laitetaan annettu data käyttöön.
// Jos	itsMaxLatestDataCount on > 0, katsotaan mihin kohtaan (mille indeksille) data sijoittuu, mahdollisesti vanhimman datan joutuu siivoamaan pois.
void NFmiQueryDataSetKeeper::AddData(boost::shared_ptr<NFmiOwnerInfo> &theData, bool fFirstData)
{
	if(theData)
	{
		if(fFirstData || itsMaxLatestDataCount == 0)
		{
			itsQueryDatas.clear(); 
			itsQueryDatas.push_back(boost::shared_ptr<NFmiQueryDataKeeper>(new NFmiQueryDataKeeper(theData)));
			itsFilePattern = theData->DataFilePattern();
			itsLatestOriginTime = theData->OriginTime();
		}
		else
			AddDataToSet(theData);
	}
}

// Oletus: annettu data lisätään tähän data settiin niin että etsitään sen paikka (indeksi).
// Siirretään kaikkia datoja tarpeen mukaan indekseissä.
// Jos datoja on liikaa setissä, poistetaan ylimääräiset (yli max indeksiset).
// Jos sama data löytyy jo setistä (= sama origin-aika), korvaa listassa oleva tällä (esim. on tehty uusi korjattu malliajo datahaku).
void NFmiQueryDataSetKeeper::AddDataToSet(boost::shared_ptr<NFmiOwnerInfo> &theData)
{
	// etsi ja korvaa, jos setistä löytyy jo saman origin-timen data
	NFmiMetTime origTime = theData->OriginTime();
	NFmiMetTime latestOrigTime = origTime;
	for(ListType::iterator it = itsQueryDatas.begin(); it != itsQueryDatas.end(); ++it)
	{
		const NFmiMetTime &currentOrigTime = (*it)->OriginalData()->OriginTime();
		if(latestOrigTime < currentOrigTime)
			latestOrigTime = currentOrigTime; // etsitään samalla viimeisintä origin aikaa listasta

		if(currentOrigTime == origTime)
		{
			*it = boost::shared_ptr<NFmiQueryDataKeeper>(new NFmiQueryDataKeeper(theData));
			// koska kyseessä oli vain toisen datan korvaava toimenpide, voidaan lisäys-operaatio lopettaa tähän....
			return ;
		}
	}

	// Tämä on data uudella origin ajalla. 
	// 1. Lisää se listaan.
	itsQueryDatas.push_back(boost::shared_ptr<NFmiQueryDataKeeper>(new NFmiQueryDataKeeper(theData)));
	// 2. Laske kaikille setin datoille indeksit uudestaan.
	RecalculateIndexies(latestOrigTime);
	// 3. Poista listasta kaikki datat joiden indeksi on suurempi kuin itsMaxLatestDataCount:in arvo sallii.
	DeleteTooOldDatas();
}

static int CalcIndex(const NFmiMetTime &theLatestOrigTime, const NFmiMetTime &theOrigCurrentTime, int theModelRunTimeGap)
{
	int diffInMinutes = theLatestOrigTime.DifferenceInMinutes(theOrigCurrentTime);
	return round(diffInMinutes/theModelRunTimeGap);
}

void NFmiQueryDataSetKeeper::RecalculateIndexies(const NFmiMetTime &theLatestOrigTime)
{
	for(ListType::iterator it = itsQueryDatas.begin(); it != itsQueryDatas.end(); ++it)
		(*it)->Index(::CalcIndex(theLatestOrigTime, (*it)->OriginalData()->OriginTime(), itsModelRunTimeGap));
}

struct OldDataRemover
{
	OldDataRemover(int theMaxLatestDataCount)
	:itsMaxLatestDataCount(theMaxLatestDataCount)
	{
	}

	bool operator()(boost::shared_ptr<NFmiQueryDataKeeper> &theDataKeeper)
	{
		if(itsMaxLatestDataCount < theDataKeeper->Index())
			return true;
		return false;
	}

	int itsMaxLatestDataCount;
};


void NFmiQueryDataSetKeeper::DeleteTooOldDatas(void)
{
	itsQueryDatas.remove_if(OldDataRemover(itsMaxLatestDataCount));
}

boost::shared_ptr<NFmiQueryDataKeeper> NFmiQueryDataSetKeeper::GetDataKeeper(int theIndex)
{
	for(ListType::iterator it = itsQueryDatas.begin(); it != itsQueryDatas.end(); ++it)
	{
		if((*it)->Index() == theIndex)
			return (*it);
	}
	// jos ei löytynyt, palautetaan tyhjä
	return boost::shared_ptr<NFmiQueryDataKeeper>();
}

size_t NFmiQueryDataSetKeeper::DataCount(void)
{
	return itsQueryDatas.size(); 
}

size_t NFmiQueryDataSetKeeper::DataByteCount(void)
{
	size_t sizeInBytes = 0;
	for(ListType::iterator it = itsQueryDatas.begin(); it != itsQueryDatas.end(); ++it)
		sizeInBytes += (*it)->OriginalData()->Size() * sizeof(float);

	return sizeInBytes;
}
