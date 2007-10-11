//© Ilmatieteenlaitos/Marko.
//Original 5.10.2007
//
// namespacessa on muutamia helper funktioita ja 'ylim‰‰r‰isen' datan talletus luokka.
//---------------------------------------------------------- NFmiDataStoringHelpers.h

#ifndef __NFMIDATASTORINGHELPERS_H__
#define __NFMIDATASTORINGHELPERS_H__

#include <vector>
#include <string>
#include "NFmiString.h"
#include "NFmiStringTools.h"
#include "NFmiMetTime.h"


namespace NFmiDataStoringHelpers
{
	// yleisi‰ container-kirjoitus ja luku funktioita
	// theReallyStoredSize on s‰‰stˆ‰ tarkoitettu parametri. Eli jos se on oletusarvossa -1, talletus tapahtuu normaalista
	// eli kaikki talletetaan mit‰ annetussa containerissa on. Jos parametrin arvo on jotain muuta,
	// talletetaan vain niin monta itemia, kuin parametrissa on annettu.
	template<typename T>
	inline void WriteContainer(const T & theContainer, std::ostream& os, const std::string &theSeparator, int theReallyStoredSize = -1)
	{
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

	inline void WriteTimeWithOffsets(const NFmiMetTime &theTime, std::ostream& os)
	{
		short utcHour = theTime.GetHour();
		short utcMinute = theTime.GetMin();
		NFmiMetTime aTime(60);
		aTime.SetHour(0);
		long hourShift = aTime.DifferenceInHours(theTime);
		long usedDayShift = hourShift/24;
		if(hourShift > 0)
			usedDayShift++;
		os << utcHour << " " << utcMinute << " " << usedDayShift << std::endl;
	}

	inline void ReadTimeWithOffsets(NFmiMetTime &theTime, std::istream& is)
	{
		short utcHour = 0;
		short utcMinute = 0;
		long dayShift = 0;
		is >> utcHour >> utcMinute >> dayShift;

		NFmiMetTime aTime(60);
		aTime.SetHour(0);
		aTime.ChangeByDays(-dayShift);
		aTime.SetHour(utcHour);
		aTime.SetMin(utcMinute);
		theTime = aTime;
	}

	// Luokka jota k‰yttet‰‰n itsens‰ kirjoittavien luokkien
	// tulevaisuudessa lis‰ttyjen datojen tallettamiseen.
	// Eli luokka joka sis‰lt‰‰ n klp float lukuja ja m kpl
	// string olioita. Vanha versio osaa lukea uudemman version sis‰‰ns‰
	// vaikka ei ymm‰rr‰ sis‰ltˆ‰. Ja uusi versio yritt‰‰ lukea niin paljon
	// kuin lˆyt‰‰ vanhasta versiosta.
	struct NFmiExtraDataStorage
	{
		void Clear(void)
		{
			itsDoubleValues.clear();
			itsStringValues.clear();
		}
		void Add(double theValue){itsDoubleValues.push_back(theValue);}
		void Add(const std::string &theValue){itsStringValues.push_back(theValue);}

		void Write(std::ostream& os) const
		{
			size_t ssize = itsDoubleValues.size();
			os << ssize << std::endl;
			size_t i = 0;
			for(i=0; i<ssize; i++)
			{
				os << itsDoubleValues[i] << " ";
			}
			if(ssize > 0)
				os << std::endl;

			ssize = itsStringValues.size();
			os << ssize << std::endl;
			for(i=0; i<ssize; i++)
			{
				// muutetaan std::string NFmiString:iksi ett‰ saadaan stringin pituus mukaan kirjoitukseen
				// luku vaiheessa muuten hˆmma tˆkk‰‰ ensimm‰iseen white spaceen
				NFmiString tmpStr(itsStringValues[i]);
				os << tmpStr; // NFmiString heitt‰‰ itse endl:in per‰‰n.
			}
			if(ssize > 0)
				os << std::endl;
		}

		void Read(std::istream& is)
		{
			static size_t maxAllowedVectorSize = 200;
			Clear();

			size_t ssize = 0;
			is >> ssize;
			if(ssize > maxAllowedVectorSize)
				throw std::runtime_error(std::string("NFmiExtraDataStorage::Read double vector size is over ") + NFmiStringTools::Convert<int>(200) + ", propably error failing read...");
			itsDoubleValues.resize(ssize);
			size_t i = 0;
			for(i=0; i<ssize; i++)
			{
				is >> itsDoubleValues[i];
			}

			is >> ssize;
			if(ssize > maxAllowedVectorSize)
				throw std::runtime_error(std::string("NFmiExtraDataStorage::Read string vector size is over ") + NFmiStringTools::Convert<int>(200) + ", propably error failing read...");
			itsStringValues.resize(ssize);
			for(i=0; i<ssize; i++)
			{
				// Luetaan NFmiString otus sis‰‰n ja muutetaan se std::string:iksi
				// ett‰ saadaan stringin pituus mukaan kirjoitukseen
				// luku vaiheessa muuten hˆmma tˆkk‰‰ ensimm‰iseen white spaceen
				NFmiString tmpStr;
				is >> tmpStr;
				itsStringValues[i] = tmpStr;
			}
		}

		std::vector<double> itsDoubleValues;
		std::vector<std::string> itsStringValues;
	};

} // end of namespace

inline std::ostream& operator<<(std::ostream& os, const NFmiDataStoringHelpers::NFmiExtraDataStorage& item){item.Write(os); return os;}
inline std::istream& operator>>(std::istream& is, NFmiDataStoringHelpers::NFmiExtraDataStorage& item){item.Read(is); return is;}

#endif//__NFMIDATASTORINGHELPERS_H__
