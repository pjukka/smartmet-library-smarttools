//**********************************************************
// C++ Class Name : NFmiInfoOrganizer
// ---------------------------------------------------------
// Filetype: (HEADER)
// Filepath: D:/projekti/GDPro/GDTemp/NFmiInfoOrganizer.h
//
//
// GDPro Properties
// ---------------------------------------------------
//  - GD Symbol Type    : CLD_Class
//  - GD Method         : UML ( 2.1.4 )
//  - GD System Name    : Met-editor Plan 2
//  - GD View Type      : Class Diagram
//  - GD View Name      : Markon ehdotus
// ---------------------------------------------------
//  Author         : pietarin
//  Creation Date  : Tues - Feb 9, 1999
//
//
//  Description:
//   Sis‰lt‰‰ listan johon voidaan laitta SmartInfoja. Metodi Info
//   palauttaa pointterin infoon, joka sis‰lt‰‰ tunnistimena annetun
//   parametrin. Listalla ei ole j‰rjestyst‰.
//
//  Change Log:
// 1999.08.26/Marko	Laitoin organizeriin eri tyyppisten datojen talletuksen
//					mahdollisuuden. Editoitavaa dataa voi olla vain yksi, mutta
//					katsottavaa/maskattavaa dataa voi olla useita erilaisia ja statio-
//					n‰‰risi‰ (esim. topografia) datoja voi olla myˆs useita erilaisia.
// 1999.08.26/Marko	Laitoin level tiedon DrawParam:in luonti-kutsuun varten.
// 1999.09.22/Marko	DrawParamin luonnissa otetaan huomioon datan tyyppi t‰st‰l‰htien.
// 1999.09.22/Marko	Lis‰sin EditedDatan kopion, jota k‰ytet‰‰n visualisoimaan dataan tehtyj‰ muutoksia.
//
//**********************************************************
#ifndef  NFMIINFOORGANIZER_H
#define  NFMIINFOORGANIZER_H

#include "NFmiSortedPtrList.h"
#include "NFmiParamBag.h"
#include "NFmiParameterName.h"
//#include "FmiNMeteditLibraryDefinitions.h"
#include "NFmiInfoData.h"
#include "NFmiDataMatrix.h"
#include "NFmiPoint.h"
#include "NFmiSmartInfo.h"
#include <vector>

class NFmiDrawParamFactory;
class NFmiDrawParamList;
class NFmiDrawParam;
class NFmiQueryData;
class NFmiLevel;
class NFmiQueryInfo;
class NFmiLevelBag;
class NFmiArea;

class NFmiInfoOrganizer
{
 public:

	NFmiInfoOrganizer (void);
	~NFmiInfoOrganizer (void);

	bool Init(const std::string &theDrawParamPath, bool createDrawParamFileIfNotExist, bool createEditedDataCopy, bool fUseOnePressureLevelDrawParam);

	// Kehit‰ t‰h‰n perhe infojen palautus funktioita, jotka korvaavat vanhat
  	NFmiSmartInfo* Info(NFmiDrawParam &theDrawParam, bool fCrossSectionInfoWanted = false);
  	NFmiSmartInfo* Info (const NFmiDataIdent& theIdent, const NFmiLevel* theLevel, NFmiInfoData::Type theType);

// n‰m‰ infon palauttavat metodit ovat toistaiseksi vain apu funktioita, jotka toimivat
// vajavaisesti. Niist‰ pit‰‰ tehd‰ fiksumpia, kun tulee enemm‰n dataa talteen!
	NFmiSmartInfo* EditedInfo(void){return itsEditedData;}; // t‰m‰ toimii jos rajoitetaan editoitavien infojen m‰‰r‰‰
	NFmiSmartInfo* EditedInfoCopy(void){return itsEditedDataCopy;};
	NFmiSmartInfo* ViewableInfo(void); // t‰m‰ toimii vajavaisesti, koska se palauttaa aina 1. kyseisen tyyppisen infon
	checkedVector<NFmiSmartInfo*> GetInfos(const std::string &theFileNameFilter); // palauttaa vectorin halutunlaisia infoja, vectori ei omista pointtereita, joten infoja ei saa tuhota
	checkedVector<NFmiSmartInfo*> GetInfos(NFmiInfoData::Type theDataType); // palauttaa vectorin halutunlaisia infoja, vectori ei omista pointtereita, joten infoja ei saa tuhota
	checkedVector<NFmiSmartInfo*> GetInfos(int theProducerId, int theProducerId2 = -1, int theProducerId3 = -1, int theProducerId4 = -1); // palauttaa vectorin halutun tuottajan infoja, vectori ei omista pointtereita, joten infoja ei saa tuhota
	checkedVector<NFmiSmartInfo*> GetInfos(NFmiInfoData::Type theType, bool fGroundData, int theProducerId, int theProducerId2 = -1); // palauttaa vectorin halutun tuottajan infoja, vectori ei omista pointtereita, joten infoja ei saa tuhota
	NFmiSmartInfo* ViewableInfo(int theIndex); // palauttaa halutun indeksin infon (huono viritys, KORJAA!!!!)
	NFmiSmartInfo* ObservationInfo(void); // t‰m‰ toimii vajavaisesti, koska se palauttaa aina 1. kyseisen tyyppisen infon
	NFmiSmartInfo* ObservationInfo(int theIndex); // palauttaa halutun indeksin infon (huono viritys, KORJAA!!!!)
//	NFmiSmartInfo* CompareModelsInfo(void); // t‰m‰ toimii vajavaisesti, koska se palauttaa aina 1. kyseisen tyyppisen infon
//	NFmiSmartInfo* CompareModelsInfo(const NFmiProducer& theProducer);
	NFmiSmartInfo* KepaDataInfo(void); // t‰m‰ toimii vajavaisesti, koska se palauttaa aina 1. kyseisen tyyppisen infon
	NFmiSmartInfo* ClimatologyInfo(void); // t‰m‰ toimii vajavaisesti, koska se palauttaa aina 1. kyseisen tyyppisen infon
	NFmiSmartInfo* AnalyzeDataInfo(const NFmiProducer& theProducer); // t‰m‰ toimii vajavaisesti, koska se palauttaa aina 1. kyseisen tyyppisen infon
	NFmiSmartInfo* AnalyzeDataInfo(void); // palauttaa tietyss‰ prioriteetti j‰rjestyksess‰ jonkun analyysi datan mit‰ systeemist‰ lˆytyy

	NFmiSmartInfo* FindInfo(NFmiInfoData::Type theDataType); // Hakee 1. tietyn datatyypin infon
	NFmiSmartInfo* FindInfo(NFmiInfoData::Type theDataType, int theIndex); // Hakee indeksin mukaisen tietyn datatyypin infon

	NFmiSmartInfo* FindInfo(NFmiInfoData::Type theDataType, const NFmiProducer &theProducer, bool fGroundData, int theIndex = 0); // Hakee indeksin mukaisen tietyn datatyypin infon
	NFmiSmartInfo* FindSoundingInfo(const NFmiProducer &theProducer); // Hakee parhaan luotaus infon tuottajalle

	NFmiParamBag GetParams(int theProducerId1, int theProducerId2, NFmiInfoData::Type theIgnoreDataType1, NFmiInfoData::Type theIgnoreDataType2, NFmiInfoData::Type theIgnoreDataType3);
	NFmiParamBag EditedParams(void); // itsEditedData infon parambagi
	NFmiParamBag ViewableParams(void); // kaikkien apudatojen parametrit yhdess‰ bagissa (joita voidaan katsoa/maskata)
	NFmiParamBag ViewableParams(int theIndex); // vain halutun indeksin parametrit (HUONO VIRITYS KORJAA!!!!)
	NFmiParamBag StaticParams(void); // kaikkien staattisten (ei muutu ajan mukana) datojen parambag (esim. topografia)
	NFmiParamBag ObservationParams(void); // kaikkien staattisten (ei muutu ajan mukana) datojen parambag (esim. topografia)
	NFmiParamBag ObservationParams(int theIndex); // vain halutun indeksin parametrit (HUONO VIRITYS KORJAA!!!!)
//	NFmiParamBag CompareModelsParams(void); // palauttaa vain 1. kyseisen datan parametrit!

//	NFmiSmartInfo* CreateInfo(FmiParameterName theParamName, const NFmiLevel* theLevel, NFmiInfoData::Type theType);
	NFmiSmartInfo* CreateInfo(const NFmiDataIdent& theDataIdent, const NFmiLevel* theLevel, NFmiInfoData::Type theType, bool fTryParIdAlso);
	NFmiSmartInfo* CreateInfo(NFmiSmartInfo* theUsedInfo, const NFmiDataIdent& theDataIdent, const NFmiLevel* theLevel, NFmiInfoData::Type theType);

	// SmartToolModifier tarvitsee ohuen kopion (eli NFmiQueryData ei kopioidu)
//	NFmiSmartInfo* CreateShallowCopyInfo(FmiParameterName theParamName, const NFmiLevel* theLevel, NFmiInfoData::Type theType);
	NFmiSmartInfo* CreateShallowCopyInfo(const NFmiDataIdent& theDataIdent, const NFmiLevel* theLevel, NFmiInfoData::Type theType, bool fUseParIdOnly, bool fLevelData);

//	NFmiDrawParam* CreateDrawParam(FmiParameterName theParamName, const NFmiLevel* theLevel, NFmiInfoData::Type theType);
//	NFmiDrawParam* CreateEmptyInfoDrawParam(const NFmiDataIdent& theDataIdent); // luo drawparam ilman infoa
	NFmiDrawParam* CreateDrawParam(const NFmiDataIdent& theDataIdent, const NFmiLevel* theLevel, NFmiInfoData::Type theType);
	NFmiDrawParam* CreateCrossSectionDrawParam(const NFmiDataIdent& theDataIdent, NFmiInfoData::Type theType);
	NFmiDrawParam* CreateDrawParam(NFmiSmartInfo* theUsedInfo, const NFmiDataIdent& theDataIdent, const NFmiLevel* theLevel, NFmiInfoData::Type theType);
	bool AddData(NFmiQueryData* theData
					  ,const std::string& theDataFileName
					  ,const std::string& theDataFilePattern
					  ,NFmiInfoData::Type theDataType
					  ,int theUndoLevel = 0);

	bool Clear (void);
	void ClearData(NFmiInfoData::Type theDataType);
	void ClearThisKindOfData(NFmiQueryInfo* theInfo, NFmiInfoData::Type theDataType, const std::string &theFileNamePattern, NFmiTimeDescriptor &theRemovedDatasTimesOut);
	NFmiLevelBag* GetAndCreateViewableInfoWithManyLevelsOrZeroPointer(void);
	void ClearDynamicHelpData();

	const std::string& WorkingDirectory(void) const {return itsWorkingDirectory;};
	void WorkingDirectory(const std::string& newValue){itsWorkingDirectory = newValue;};
	void UpdateEditedDataCopy(void); // 28.09.1999/Marko
	NFmiSmartInfo* MacroParamData(void) {return itsMacroParamData;} // t‰t‰ tarvitaan asettamaan mm. aikadescriptoria ja ehk‰ muita descriptoreita
	NFmiDataMatrix<float>& MacroParamMissingValueMatrix(void){return itsMacroParamMissingValueMatrix;}
	NFmiSmartInfo* CrossSectionMacroParamData(void) {return itsCrossSectionMacroParamData;}
	NFmiDataMatrix<float>& CrossSectionMacroParamMissingValueMatrix(void){return itsCrossSectionMacroParamMissingValueMatrix;}
	NFmiSmartInfo* SoundingParamData(void) {return itsSoundingParamData;}
	NFmiDataMatrix<float>& SoundingParamMissingValueMatrix(void) {return itsSoundingParamMissingValueMatrix;}

	void SetDrawParamPath(const std::string &theDrawParamPath);
	const std::string GetDrawParamPath(void);
	void SetMacroParamDataGridSize(int x, int y);
	void SetMacroParamDataMinGridSize(int x, int y);
	void SetMacroParamDataMaxGridSize(int x, int y);
	void SetSoundingParamDataGridSize(int x, int y);
	void UpdateMapArea(const NFmiArea *theArea);

	const NFmiPoint& SoundingParamGridSize(void) const {return itsSoundingParamGridSize;}
	const NFmiPoint& GetMacroParamDataGridSize(void) const {return itsMacroParamGridSize;}
	const NFmiPoint& GetMacroParamDataMaxGridSize(void) const {return itsMacroParamMaxGridSize;}
	const NFmiPoint& GetMacroParamDataMinGridSize(void) const {return itsMacroParamMinGridSize;}
	int CountData(void);
	int CountDataSize(void);
	void UpdateCrossSectionMacroParamDataSize(int x, int y);
 private:
 	bool Remove(void);
	bool IsInfosTwoOfTheKind(NFmiQueryInfo* theInfo1, NFmiInfoData::Type theType1, const std::string &theFileNamePattern, NFmiSmartInfo* theSmartInfo2);
	void UpdateSpecialDataArea(const NFmiArea *theArea, const NFmiPoint &theGridSize, NFmiInfoData::Type theType, NFmiSmartInfo ** theData, NFmiDataMatrix<float> &theMissingValueMatrix);
	void UpdateMacroParamData(void);
	void UpdateSoundingParamData(void);

 public: // n‰m‰ ovat pikaviritys k‰ytˆss‰ n‰m‰ metodit pit‰isi laittaa takaisin privateiksi
	bool Reset (void);
	bool Next (void);
	NFmiSmartInfo* Current (void);

 private:
  	NFmiSmartInfo* Info (const NFmiDataIdent& theIdent, bool& fSubParameter, const NFmiLevel* theLevel, NFmiInfoData::Type theType, bool fIgnoreProducerName = false);
	NFmiSmartInfo* Info (const FmiParameterName& theParam, bool& fSubParameter, const NFmiLevel* theLevel, NFmiInfoData::Type theType);
	NFmiSmartInfo* CrossSectionInfo(const NFmiDataIdent& theDataIdent, bool& fSubParameter, NFmiInfoData::Type theType, bool fIgnoreProducerName = false);
    bool Add (NFmiSmartInfo* theInfo);
	NFmiParamBag GetParams(NFmiInfoData::Type theDataType);
	NFmiSmartInfo* GetSynopPlotParamInfo(bool& fSubParameter, NFmiInfoData::Type theType);
	NFmiSmartInfo* GetSoundingPlotParamInfo(bool& fSubParameter, NFmiInfoData::Type theType);
	NFmiDrawParam* CreateSynopPlotDrawParam(const NFmiDataIdent& theDataIdent
											,const NFmiLevel* theLevel
											,NFmiInfoData::Type theType);

// Attributes
  	NFmiSortedPtrList<NFmiSmartInfo> itsList; // error when compiling NFmiInfoOrganizer.cpp//binary '<' : 'class NFmiSmartInfo' does not define this operator or a conversion to a type acceptable to the predefined operator
	NFmiPtrList<NFmiSmartInfo>::Iterator itsIter;
 	NFmiDrawParamFactory* itsDrawParamFactory;
	std::string itsWorkingDirectory;
	NFmiSmartInfo* itsEditedData; // editoitavaa dataa voi olla vain yksi kerrallaan, joten laitoin sen erilleen tehokkuuden takia.
	NFmiSmartInfo* itsEditedDataCopy; // t‰m‰ on editoitavan datan kopio, mit‰ k‰ytt‰j‰ voi halutessaan p‰ivitt‰‰, k‰ytet‰‰n visualisoimaan tehtyj‰ muutoksia datassa

	NFmiPoint itsSoundingParamGridSize;
	NFmiPoint itsMacroParamGridSize;
	NFmiPoint itsMacroParamMinGridSize;
	NFmiPoint itsMacroParamMaxGridSize;
	NFmiSmartInfo* itsMacroParamData; // makro-parametrien laskuja varten pit‰‰ pit‰‰ yll‰ yhden hilan kokoista dataa (yksi aika,param ja level, editoitavan datan hplaceDesc)
	NFmiDataMatrix<float> itsMacroParamMissingValueMatrix; // t‰h‰n talletetaan editoitavan datan hilan suuruinen kFloatMissing:eilla alustettu matriisi ett‰ sill‰ voi alustaa makroParam dataa ennen laskuja
	NFmiSmartInfo* itsSoundingParamData; // sounding-parametrien laskuja varten pit‰‰ pit‰‰ yll‰ yhden hilan kokoista dataa (yksi aika,param ja level, editoitavan datan hplaceDesc)
	NFmiDataMatrix<float> itsSoundingParamMissingValueMatrix; // t‰h‰n talletetaan editoitavan datan hilan suuruinen kFloatMissing:eilla alustettu matriisi ett‰ sill‰ voi alustaa makroParam dataa ennen laskuja
	NFmiSmartInfo* itsCrossSectionMacroParamData; // poikkileikkaus makro-parametrien laskuja varten pit‰‰ pit‰‰ yll‰ yhden hilan kokoista dataa (yksi aika,param ja level, editoitavan datan hplaceDesc)
	NFmiDataMatrix<float> itsCrossSectionMacroParamMissingValueMatrix; // t‰h‰n talletetaan editoitavan datan hilan suuruinen kFloatMissing:eilla alustettu matriisi ett‰ sill‰ voi alustaa makroParam dataa ennen laskuja

	bool fCreateEditedDataCopy; // luodaanko vai eikˆ luoda kopiota editoidusta datasta

	// estet‰‰n kopi konstruktorin ja sijoitus operaattoreiden luonti
	NFmiInfoOrganizer& operator=(const NFmiInfoOrganizer&);
	NFmiInfoOrganizer(const NFmiInfoOrganizer&);
};

#endif
