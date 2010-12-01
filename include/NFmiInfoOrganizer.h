#pragma once

// Luokan teht‰v‰ on toimia eri queryDatojen ja niiden iteraattoreiden
// s‰ilytyspaikkana SmartMet ja SmartToolFilter ohjelmille.
// Alkuper‰inen NFmiInfoOrganizer-luokka on kauheaa syltty‰ ja se pit‰‰ 
// tehd‰ t‰ss‰ uusiksi:
// --------------------
// 1. Rajapinta on siivottava. 
// 2. Teht‰v‰ selke‰mm‰t data/info -pyynti funktiot.
//
// Lis‰ksi tulee uusia ominaisuuksia:
// ----------------------------------
// 1. Tuettava multi-threddausta
// 2. Osattava hanskata vanhoja malliajo datoja (esim. hirlam[-1])
// 3. Muistin siivous ja datojen tuhoamisen automatisointi.
// 4. Data voidaan tuhota infoOrganizerista vaikka sit‰ viel‰ joku k‰ytt‰‰
// omassa threadissaa ja threadi sitten lopuksi itse tuhoaa automaattisesti datan.
//
// Toteutus: NFmiOwnerInfo on tavallisten queryDatojen s‰ilytys luokka. 
// NFmiSmartInfo2 on k‰ytˆss‰ vain editoitavalle datalle.
// Huom! Myˆs SmartMetiin tiputetut datat pit‰‰ lis‰t‰ t‰nne fileFiltterin kanssa.
// TODO: keksi parempi nimi tai muuta lopuksi NFmiInfoOrganizer-nimiseksi ja 
// tuhoa alkuper‰inen luokka.

#include "NFmiPoint.h"
#include "NFmiDataMatrix.h"
#include "NFmiInfoData.h"
#include "NFmiParamBag.h"
#include "NFmiProducerName.h"
#include "boost/shared_ptr.hpp"
#include <map>

class NFmiSmartInfo2;
class NFmiOwnerInfo;
class NFmiFastQueryInfo;
class NFmiDrawParamFactory;
class NFmiDrawParam;
class NFmiQueryData;
class NFmiQueryDataSetKeeper;
class NFmiQueryDataKeeper;
class NFmiDataIdent;
class NFmiLevel;
class NFmiProducer;
class NFmiQueryInfo;
class NFmiTimeDescriptor;

class NFmiInfoOrganizer
{
public:
	typedef std::map<std::string, boost::shared_ptr<NFmiQueryDataSetKeeper> > MapType;

	NFmiInfoOrganizer(void);
	~NFmiInfoOrganizer(void);

	bool Init(const std::string &theDrawParamPath, bool createDrawParamFileIfNotExist, bool createEditedDataCopy, bool fUseOnePressureLevelDrawParam);
	bool AddData(NFmiQueryData* theData
					  ,const std::string& theDataFileName
					  ,const std::string& theDataFilePattern
					  ,NFmiInfoData::Type theDataType
					  ,int theUndoLevel = 0);

	// ***************************************************************************************************************
	// T‰ss‰ per‰ss‰ on pienin mahdollinen julkinen rajapinta, jonka sain siivottua originaali NFmiInfoOrganizr:ista. 
	// Ne pit‰‰ muuttaa viel‰ share_ptr ja uusien Info-luokkien mukaisiksi.
	// HUOM! Ett‰ multi-threaddaus onnistuisi, pit‰‰ kaikki info-olioiden palautus funktioiden palauttaa kopio
	// iteraattori-infosta. Pit‰isikˆ jokaiselle datalle luoda on-demandina kopio-info-olio lista, josta palautus olisi nopea?
	// Pit‰‰kˆ lis‰ksi laittaa extra-infoa eri datoista, ett‰ siivous rutiini tiet‰‰ milloin niit‰ on viimeksi k‰ytetty,
	// mit‰ muuta infoa tarvitaan kirjanpitoon ja muuhin?
	// ***************************************************************************************************************
	boost::shared_ptr<NFmiFastQueryInfo> Info(NFmiDrawParam &theDrawParam, bool fCrossSectionInfoWanted = false);
  	boost::shared_ptr<NFmiFastQueryInfo> Info(const NFmiDataIdent& theIdent, const NFmiLevel* theLevel, NFmiInfoData::Type theType, bool fUseParIdOnly = false, bool fLevelData = false);
	checkedVector<boost::shared_ptr<NFmiFastQueryInfo> > GetInfos(const std::string &theFileNameFilter); // palauttaa vectorin halutunlaisia infoja, vectori ei omista pointtereita, joten infoja ei saa tuhota
	checkedVector<boost::shared_ptr<NFmiFastQueryInfo> > GetInfos(int theProducerId, int theProducerId2 = -1, int theProducerId3 = -1, int theProducerId4 = -1); // palauttaa vectorin halutun tuottajan infoja, vectori ei omista pointtereita, joten infoja ei saa tuhota
	checkedVector<boost::shared_ptr<NFmiFastQueryInfo> > GetInfos(NFmiInfoData::Type theDataType);
	checkedVector<boost::shared_ptr<NFmiFastQueryInfo> > GetInfos(NFmiInfoData::Type theType, bool fGroundData, int theProducerId, int theProducerId2 = -1); // palauttaa vectorin halutun tuottajan infoja, vectori ei omista pointtereita, joten infoja ei saa tuhota
	boost::shared_ptr<NFmiFastQueryInfo> FindInfo(NFmiInfoData::Type theDataType, int theIndex = 0); // Hakee indeksin mukaisen tietyn datatyypin infon
	boost::shared_ptr<NFmiFastQueryInfo> FindInfo(NFmiInfoData::Type theDataType, const NFmiProducer &theProducer, bool fGroundData, int theIndex = 0); // Hakee indeksin mukaisen tietyn datatyypin infon
	boost::shared_ptr<NFmiFastQueryInfo> FindSoundingInfo(const NFmiProducer &theProducer); // Hakee parhaan luotaus infon tuottajalle

	// HUOM! N‰m‰ makroParamData jutut pit‰‰ mietti‰ uusiksi, jos niit‰ aletaan k‰sittelem‰‰n eri s‰ikeiss‰. T‰llˆin
	// Niist‰ pit‰‰ luoda aina ilmeisesti paikalliset kopiot?!?!
	boost::shared_ptr<NFmiFastQueryInfo> MacroParamData(void);
	boost::shared_ptr<NFmiFastQueryInfo> CrossSectionMacroParamData(void);

	NFmiParamBag GetParams(int theProducerId1);

	NFmiDrawParam* CreateDrawParam(const NFmiDataIdent& theDataIdent, const NFmiLevel* theLevel, NFmiInfoData::Type theType);
	NFmiDrawParam* CreateCrossSectionDrawParam(const NFmiDataIdent& theDataIdent, NFmiInfoData::Type theType);

	bool Clear(void);
	void ClearData(NFmiInfoData::Type theDataType);
	void ClearThisKindOfData(NFmiQueryInfo* theInfo, NFmiInfoData::Type theDataType, const std::string &theFileNamePattern, NFmiTimeDescriptor &theRemovedDatasTimesOut);
	void ClearDynamicHelpData();

	const std::string& WorkingDirectory(void) const {return itsWorkingDirectory;};
	void WorkingDirectory(const std::string& newValue){itsWorkingDirectory = newValue;};
	void UpdateEditedDataCopy(void); // 28.09.1999/Marko
	NFmiDataMatrix<float>& MacroParamMissingValueMatrix(void){return itsMacroParamMissingValueMatrix;}
	NFmiDataMatrix<float>& CrossSectionMacroParamMissingValueMatrix(void){return itsCrossSectionMacroParamMissingValueMatrix;}

	void SetDrawParamPath(const std::string &theDrawParamPath);
	const std::string GetDrawParamPath(void);
	void SetMacroParamDataGridSize(int x, int y);
	void SetMacroParamDataMinGridSize(int x, int y);
	void SetMacroParamDataMaxGridSize(int x, int y);

	const NFmiPoint& GetMacroParamDataGridSize(void) const {return itsMacroParamGridSize;}
	const NFmiPoint& GetMacroParamDataMaxGridSize(void) const {return itsMacroParamMaxGridSize;}
	const NFmiPoint& GetMacroParamDataMinGridSize(void) const {return itsMacroParamMinGridSize;}
	int CountData(void);
	double CountDataSize(void);
	void UpdateCrossSectionMacroParamDataSize(int x, int y);
	void UpdateMacroParamDataSize(int x, int y);
	// ***************************************************************************************************************

private:
	// estet‰‰n kopi konstruktorin ja sijoitus operaattoreiden luonti
	NFmiInfoOrganizer& operator=(const NFmiInfoOrganizer&);
	NFmiInfoOrganizer(const NFmiInfoOrganizer&);

	bool AddEditedData(NFmiSmartInfo2 *theEditedData, int theUndoLevel);
	bool Add(NFmiOwnerInfo* theInfo);

	boost::shared_ptr<NFmiFastQueryInfo> GetInfo(const NFmiDataIdent& theIdent, const NFmiLevel* theLevel, NFmiInfoData::Type theType, bool fUseParIdOnly);
	boost::shared_ptr<NFmiFastQueryInfo> CrossSectionInfo(const NFmiDataIdent& theDataIdent, NFmiInfoData::Type theType);
	boost::shared_ptr<NFmiFastQueryInfo> GetSynopPlotParamInfo(NFmiInfoData::Type theType);
	boost::shared_ptr<NFmiFastQueryInfo> GetSoundingPlotParamInfo(NFmiInfoData::Type theType);
	boost::shared_ptr<NFmiFastQueryInfo> GetWantedProducerInfo(NFmiInfoData::Type theType, FmiProducerName theProducerName);
	NFmiDrawParam* CreateSynopPlotDrawParam(const NFmiDataIdent& theDataIdent, const NFmiLevel* theLevel, NFmiInfoData::Type theType);
	bool IsInfosTwoOfTheKind(NFmiQueryInfo* theInfo1, NFmiInfoData::Type theType1, const std::string &theFileNamePattern, boost::shared_ptr<NFmiFastQueryInfo> &theInfo2);

	boost::shared_ptr<NFmiQueryDataKeeper> itsEditedDataKeeper; // pit‰‰ sis‰ll‰‰n oikeasti NFmiSmartInfo2-olion
	boost::shared_ptr<NFmiQueryDataKeeper> itsCopyOfEditedDataKeeper;
	MapType itsDataMap; // sijoitus mappiin tapahtuu filepatternin avulla

 	boost::shared_ptr<NFmiDrawParamFactory> itsDrawParamFactory;
	std::string itsWorkingDirectory;
	NFmiPoint itsMacroParamGridSize;
	NFmiPoint itsMacroParamMinGridSize;
	NFmiPoint itsMacroParamMaxGridSize;
	boost::shared_ptr<NFmiFastQueryInfo> itsMacroParamData; // makro-parametrien laskuja varten pit‰‰ pit‰‰ yll‰ yhden hilan kokoista dataa (yksi aika,param ja level, editoitavan datan hplaceDesc)
	NFmiDataMatrix<float> itsMacroParamMissingValueMatrix; // t‰h‰n talletetaan editoitavan datan hilan suuruinen kFloatMissing:eilla alustettu matriisi ett‰ sill‰ voi alustaa makroParam dataa ennen laskuja
	boost::shared_ptr<NFmiFastQueryInfo> itsCrossSectionMacroParamData; // poikkileikkaus makro-parametrien laskuja varten pit‰‰ pit‰‰ yll‰ yhden hilan kokoista dataa (yksi aika,param ja level, editoitavan datan hplaceDesc)
	NFmiDataMatrix<float> itsCrossSectionMacroParamMissingValueMatrix; // t‰h‰n talletetaan editoitavan datan hilan suuruinen kFloatMissing:eilla alustettu matriisi ett‰ sill‰ voi alustaa makroParam dataa ennen laskuja
	bool fCreateEditedDataCopy; // luodaanko vai eikˆ luoda kopiota editoidusta datasta
};
