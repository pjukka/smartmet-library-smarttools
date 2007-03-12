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

	NFmiHelpDataInfo& operator=(const NFmiHelpDataInfo &theOther);
	void Clear(void);
	//@{ \name Kirjoitus- ja luku-operaatiot
//	std::ostream & Write(std::ostream & file) const; // ei ole ollut tarveta toteuttaa viel‰!
	std::istream & Read(std::istream & file);
	//@}
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

private:
	bool ReadNextLine(std::istream & file, std::string &theLine);

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
};

//inline std::ostream & operator<<(std::ostream & os, const NFmiHelpDataInfo & item)
//{  return item.Write(os);}
inline std::istream & operator>>(std::istream & is, NFmiHelpDataInfo & item)
{  return item.Read(is);}


class NFmiHelpDataInfoSystem
{
public:
	NFmiHelpDataInfoSystem(void)
	:itsDynamicHelpDataInfos()
	,itsStaticHelpDataInfos()
	,itsRootDirectory()
	{}

	void Clear(void)
	{}
	//@{ \name Kirjoitus- ja luku-operaatiot
//	std::ostream & Write(std::ostream & file) const;
	std::istream & Read(std::istream & file);
	//@}
	NFmiHelpDataInfo& DynamicHelpDataInfo(int theIndex);
	NFmiHelpDataInfo& StaticHelpDataInfo(int theIndex);
	int DynamicCount(void) const{return itsDynamicHelpDataInfos.size();}
	int StaticCount(void) const{return itsStaticHelpDataInfos.size();}
	const NFmiArea* GetDataFilePatternAndArea(NFmiInfoData::Type theDataType, FmiProducerName theProdId, FmiParameterName theParamId, const NFmiArea *theZoomedArea, std::string &theFilePattern, bool fDemandMatchingArea);
	NFmiDataIdent GetNextSatelChannel(const NFmiDataIdent &theDataIdent, FmiDirection theDir);
	void AddDynamic(const NFmiHelpDataInfo &theInfo);
	void AddStatic(const NFmiHelpDataInfo &theInfo);
	const std::string& RootDirectory(void) const {return itsRootDirectory;}
	void RootDirectory(const std::string &theRoot) {itsRootDirectory = theRoot;}
	bool Init(const std::string &theBaseNameSpaceStr, std::string theHelpEditorFileNameFilter = "");

private:
	bool IsSameTypeProjections(const NFmiArea *theFirst, const NFmiArea *theSecond);

	checkedVector<NFmiHelpDataInfo> itsDynamicHelpDataInfos; // t‰h‰n tulee jatkuvasti p‰ivitett‰v‰t datat kuten havainnot, tutka ja analyysi datat
	checkedVector<NFmiHelpDataInfo> itsStaticHelpDataInfos; // t‰h‰n tulee kerran ladattavat jutut kuten maa/meri maskit ja klimatologiset jutut
	std::string itsRootDirectory; // jos halutaan, k‰yttet‰‰n root directoria helpottamaan datan latauksia
};

//inline std::ostream & operator<<(std::ostream & os, const NFmiHelpDataInfoSystem & item)
//{  return item.Write(os);}
inline std::istream & operator>>(std::istream & is, NFmiHelpDataInfoSystem & item)
{  return item.Read(is);}

#endif // __NFMIHELPDATAINFO_H__
