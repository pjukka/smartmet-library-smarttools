//© Ilmatieteenlaitos/Marko.
//Original 5.10.2007
//
// namespacessa on muutamia helper funktioita ja 'ylim‰‰r‰isen' datan talletus luokka.
//---------------------------------------------------------- NFmiDataStoringHelpers.h

#ifndef __NFMIDATASTORINGHELPERS_H__
#define __NFMIDATASTORINGHELPERS_H__

#include <iterator>
#include <vector>
#include <string>
#include "NFmiString.h"
#include "NFmiStringTools.h"
#include "NFmiTimeBag.h"


namespace NFmiDataStoringHelpers
{
	// yleisi‰ container-kirjoitus ja luku funktioita
	// theReallyStoredSize on s‰‰stˆ‰ tarkoitettu parametri. Eli jos se on oletusarvossa -1, talletus tapahtuu normaalista
	// eli kaikki talletetaan mit‰ annetussa containerissa on. Jos parametrin arvo on jotain muuta,
	// talletetaan vain niin monta itemia, kuin parametrissa on annettu.
	template<typename T>
	inline void WriteContainer(const T & theContainer, std::ostream& os, const std::string &theSeparator, int theReallyStoredSize = -1)
	{

#if defined(_MSC_VER) && (_MSC_VER >= 1500)
// MSVC 2008 ei sitten osannut en‰‰ k‰‰nt‰‰ alempana olevaa koodia (KUN annetussa containerissa 
// oli luokan sis‰isi‰ luokkia), joten tein for loopin, joka meni l‰pi.
		size_t storedCount = theReallyStoredSize;
		if(storedCount > theContainer.size())
			storedCount = theContainer.size();

		os << storedCount << std::endl;
		for(size_t i = 0; i < storedCount; i++)
		{
			os << theContainer[i];
			if(i < storedCount-1) // ei laiteta separaattoria viimeisen j‰lkeen
				os << theSeparator;
		}
#else
		if(theReallyStoredSize < 0 || theReallyStoredSize >= static_cast<int>(theContainer.size()))
		{
			os << theContainer.size() << std::endl;
			copy(theContainer.begin(), theContainer.end(), std::ostream_iterator<typename T::value_type>(os, theSeparator.c_str()));
		}
		else
		{
			os << theReallyStoredSize << std::endl;
			if(theReallyStoredSize > 0)
			{
				copy(theContainer.begin(), theContainer.begin()+theReallyStoredSize, std::ostream_iterator<typename T::value_type>(os, theSeparator.c_str()));
			}
		}
#endif
	}

	template< typename Container>
	inline void ReadContainer(Container & theContainer, std::istream& is)
	{
		typename Container::size_type ssize = 0;
		is >> ssize;
		theContainer.resize(ssize);
		for(typename Container::size_type i=0; i< ssize; i++)
		{
			typename Container::value_type tmp;
			is >> tmp;
			theContainer[i] = tmp;
		}
	}

	// theUsedCurrentTime on ns. sein‰kello aika. Se pit‰‰ antaa kaikille n‰ille aikafunktioille. Siin‰ on normaalisti 
	// nykyhetken kellon aika minuutin tarkkuudella. Mutta jos ollaan SmartMetin CaseStudy-moodissa, laitetaankin siihen
	// kyseiseen CaseStudyyn talletettu sein‰kelloaika. T‰m‰n avulla voidaan CaseStudy-moodissa k‰ytt‰‰ normaaleja 
	// n‰yttˆmakroja niin ett‰ makron ajat kelautuvat tarvittaessa aina vuosien p‰‰h‰n.

	void WriteTimeWithOffsets(const NFmiMetTime &theUsedCurrentTime, const NFmiMetTime &theTime, std::ostream& os);
	void ReadTimeWithOffsets(const NFmiMetTime &theUsedCurrentTime, NFmiMetTime &theTime, std::istream& is);
	void WriteTimeBagWithOffSets(const NFmiMetTime &theUsedCurrentTime, const NFmiTimeBag &theTimeBag, std::ostream& os);
	void ReadTimeBagWithOffSets(const NFmiMetTime &theUsedCurrentTime, NFmiTimeBag &theTimeBag, std::istream& is);
	std::string GetTimeBagOffSetStr(const NFmiMetTime &theUsedCurrentTime, const NFmiTimeBag &theTimeBag);
	NFmiTimeBag GetTimeBagOffSetFromStr(const NFmiMetTime &theUsedCurrentTime, const std::string &theTimeBagStr);
	void SetUsedViewMacroTime(const NFmiMetTime &theTime);
	NFmiMetTime GetUsedViewMacroTime(void);

	// Luokka jota k‰yttet‰‰n itsens‰ kirjoittavien luokkien
	// tulevaisuudessa lis‰ttyjen datojen tallettamiseen.
	// Eli luokka joka sis‰lt‰‰ n klp float lukuja ja m kpl
	// string olioita. Vanha versio osaa lukea uudemman version sis‰‰ns‰
	// vaikka ei ymm‰rr‰ sis‰ltˆ‰. Ja uusi versio yritt‰‰ lukea niin paljon
	// kuin lˆyt‰‰ vanhasta versiosta.
	struct NFmiExtraDataStorage
	{
		void Clear(void);
		void Add(double theValue);
		void Add(const std::string &theValue);

		void Write(std::ostream& os) const;
		void Read(std::istream& is);

		std::vector<double> itsDoubleValues;
		std::vector<std::string> itsStringValues;
	};

} // end of namespace

inline std::ostream& operator<<(std::ostream& os, const NFmiDataStoringHelpers::NFmiExtraDataStorage& item){item.Write(os); return os;}
inline std::istream& operator>>(std::istream& is, NFmiDataStoringHelpers::NFmiExtraDataStorage& item){item.Read(is); return is;}

#endif//__NFMIDATASTORINGHELPERS_H__
