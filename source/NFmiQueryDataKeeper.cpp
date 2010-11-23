
#include "NFmiQueryDataKeeper.h"
#include "NFmiSmartInfo2.h"

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

// T‰m‰ palauttaa vapaana olevan Info-iteraattori kopion dataan.
boost::shared_ptr<NFmiFastQueryInfo> NFmiQueryDataKeeper::GetIter(void)
{
	// TODO: T‰m‰ vaatii sitten s‰ie turvallisen lukituksen

	// Katsotaan onko listassa yht‰‰n Info-iteraattoria, joka ei ole k‰ytˆss‰ (ref-count = 1)
	for(size_t i = 0; i < itsIteratorList.size(); i++)
	{
		if(itsIteratorList[i].use_count() <= 1)
			return itsIteratorList[i];
	}

	// Ei lˆytynyt vapaata (tai ollenkaan) Info-iteraattoria, pit‰‰ luoda sellainen ja lis‰t‰ listaan ja paluttaa se
	boost::shared_ptr<NFmiFastQueryInfo> infoIter;
	if(OriginalData()->DataType() == NFmiInfoData::kEditable)
		infoIter = boost::shared_ptr<NFmiFastQueryInfo>(new NFmiSmartInfo2(*(dynamic_cast<NFmiSmartInfo2*>(OriginalData().get())))); // HUOM! Vain editoitu data on smartInfo2 -tyyppi‰, ja clone ei sovi t‰ss‰ koska nyt tehd‰‰n 'matala' kopio. Ett‰ saataisiin kaunis ratkaisu, pit‰isi tehd‰ joku shallowClone virtuaali metodi
	else
		infoIter = boost::shared_ptr<NFmiFastQueryInfo>(new NFmiOwnerInfo(*(OriginalData().get())));

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
	AddData(theData, true); // true tarkoittaa ett‰ kyse on 1. lis‰tt‰v‰st‰ datasta
}

NFmiQueryDataSetKeeper::~NFmiQueryDataSetKeeper(void)
{
}

// Lis‰t‰t‰‰n annettu data keeper-settiin.
// Jos	itsMaxLatestDataCount on 0, tyhjennnet‰‰n olemassa olevat listat ja datat ja laitetaan annettu data k‰yttˆˆn.
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

// Oletus: annettu data lis‰t‰‰n t‰h‰n data settiin niin ett‰ etsit‰‰n sen paikka (indeksi).
// Siirret‰‰n kaikkia datoja tarpeen mukaan indekseiss‰.
// Jos datoja on liikaa setiss‰, poistetaan ylim‰‰r‰iset (yli max indeksiset).
// Jos sama data lˆytyy jo setist‰ (= sama origin-aika), korvaa listassa oleva t‰ll‰ (esim. on tehty uusi korjattu malliajo datahaku).
void NFmiQueryDataSetKeeper::AddDataToSet(boost::shared_ptr<NFmiOwnerInfo> &theData)
{
	// etsi ja korvaa, jos setist‰ lˆytyy jo saman origin-timen data
	NFmiMetTime origTime = theData->OriginTime();
	NFmiMetTime latestOrigTime = origTime;
	for(ListType::iterator it = itsQueryDatas.begin(); it != itsQueryDatas.end(); ++it)
	{
		const NFmiMetTime &currentOrigTime = (*it)->OriginalData()->OriginTime();
		if(latestOrigTime < currentOrigTime)
			latestOrigTime = currentOrigTime; // etsit‰‰n samalla viimeisint‰ origin aikaa listasta

		if(currentOrigTime == origTime)
		{
			*it = boost::shared_ptr<NFmiQueryDataKeeper>(new NFmiQueryDataKeeper(theData));
			// koska kyseess‰ oli vain toisen datan korvaava toimenpide, voidaan lis‰ys-operaatio lopettaa t‰h‰n....
			return ;
		}
	}

	// T‰m‰ on data uudella origin ajalla. 
	// 1. Lis‰‰ se listaan.
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
	// jos ei lˆytynyt, palautetaan tyhj‰
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
