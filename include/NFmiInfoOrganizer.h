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
#include "FmiNMeteditLibraryDefinitions.h"
#include "NFmiInfoData.h"
#include <vector>

class NFmiDrawParamFactory;
class NFmiDrawParamList;
class NFmiDrawParam;
class NFmiQueryData;
class NFmiProducerList;
class NFmiSmartInfo;
class NFmiLevel;
class NFmiQueryInfo;
class NFmiLevelBag;

class NFmiInfoOrganizer 
{
 public:
	 	
	NFmiInfoOrganizer (bool theIsToolMasterAvailable);
	~NFmiInfoOrganizer (void); 

 	bool Init (void);

// n‰m‰ infon palauttavat metodit ovat toistaiseksi vain apu funktioita, jotka toimivat
// vajavaisesti. Niist‰ pit‰‰ tehd‰ fiksumpia, kun tulee enemm‰n dataa talteen!
	NFmiSmartInfo* EditedInfo(void){return itsEditedData;}; // t‰m‰ toimii jos rajoitetaan editoitavien infojen m‰‰r‰‰
	NFmiSmartInfo* EditedInfoCopy(void){return itsEditedDataCopy;}; 
	NFmiSmartInfo* ViewableInfo(void); // t‰m‰ toimii vajavaisesti, koska se palauttaa aina 1. kyseisen tyyppisen infon
	std::vector<NFmiSmartInfo*> GetInfos(NFmiInfoData::Type theDataType); // palauttaa vectorin halutunlaisia infoja, vectori ei omista pointtereita, joten infoja ei saa tuhota
	NFmiSmartInfo* ViewableInfo(int theIndex); // palauttaa halutun indeksin infon (huono viritys, KORJAA!!!!)
	NFmiSmartInfo* ObservationInfo(void); // t‰m‰ toimii vajavaisesti, koska se palauttaa aina 1. kyseisen tyyppisen infon
	NFmiSmartInfo* ObservationInfo(int theIndex); // palauttaa halutun indeksin infon (huono viritys, KORJAA!!!!)
	NFmiSmartInfo* CompareModelsInfo(void); // t‰m‰ toimii vajavaisesti, koska se palauttaa aina 1. kyseisen tyyppisen infon
	NFmiSmartInfo* CompareModelsInfo(const NFmiProducer& theProducer);
	NFmiSmartInfo* KepaDataInfo(void); // t‰m‰ toimii vajavaisesti, koska se palauttaa aina 1. kyseisen tyyppisen infon
	NFmiSmartInfo* ClimatologyInfo(void); // t‰m‰ toimii vajavaisesti, koska se palauttaa aina 1. kyseisen tyyppisen infon
	NFmiSmartInfo* AnalyzeDataInfo(void); // t‰m‰ toimii vajavaisesti, koska se palauttaa aina 1. kyseisen tyyppisen infon

	NFmiSmartInfo* FindInfo(NFmiInfoData::Type theDataType); // Hakee 1. tietyn datatyypin infon
	NFmiSmartInfo* FindInfo(NFmiInfoData::Type theDataType, int theIndex); // Hakee indeksin mukaisen tietyn datatyypin infon

	NFmiSmartInfo* FindInfo(NFmiInfoData::Type theDataType, const NFmiProducer &theProducer, bool fGroundData, int theIndex = 0); // Hakee indeksin mukaisen tietyn datatyypin infon

	NFmiParamBag EditedParams(void); // itsEditedData infon parambagi
	NFmiParamBag ViewableParams(void); // kaikkien apudatojen parametrit yhdess‰ bagissa (joita voidaan katsoa/maskata)
	NFmiParamBag ViewableParams(int theIndex); // vain halutun indeksin parametrit (HUONO VIRITYS KORJAA!!!!)
	NFmiParamBag StaticParams(void); // kaikkien staattisten (ei muutu ajan mukana) datojen parambag (esim. topografia)
	NFmiParamBag ObservationParams(void); // kaikkien staattisten (ei muutu ajan mukana) datojen parambag (esim. topografia)
	NFmiParamBag ObservationParams(int theIndex); // vain halutun indeksin parametrit (HUONO VIRITYS KORJAA!!!!)
	NFmiParamBag CompareModelsParams(void); // palauttaa vain 1. kyseisen datan parametrit!

	NFmiSmartInfo* CreateInfo(FmiParameterName theParamName, const NFmiLevel* theLevel, NFmiInfoData::Type theType);
	NFmiSmartInfo* CreateInfo(const NFmiDataIdent& theDataIdent, const NFmiLevel* theLevel, NFmiInfoData::Type theType);
	NFmiSmartInfo* CreateInfo(NFmiSmartInfo* theUsedInfo, const NFmiDataIdent& theDataIdent, const NFmiLevel* theLevel, NFmiInfoData::Type theType);

	// SmartToolModifier tarvitsee ohuen kopion (eli NFmiQueryData ei kopioidu)
	NFmiSmartInfo* CreateShallowCopyInfo(FmiParameterName theParamName, const NFmiLevel* theLevel, NFmiInfoData::Type theType);
	NFmiSmartInfo* CreateShallowCopyInfo(const NFmiDataIdent& theDataIdent, const NFmiLevel* theLevel, NFmiInfoData::Type theType);

	NFmiDrawParam* CreateDrawParam(FmiParameterName theParamName, const NFmiLevel* theLevel, NFmiInfoData::Type theType);
	NFmiDrawParam* CreateEmptyInfoDrawParam(FmiParameterName theParamName); // luo drawparam ilman infoa
	NFmiDrawParam* CreateDrawParam(const NFmiDataIdent& theDataIdent, const NFmiLevel* theLevel, NFmiInfoData::Type theType);
	NFmiDrawParam* CreateDrawParam(NFmiSmartInfo* theUsedInfo, const NFmiDataIdent& theDataIdent, const NFmiLevel* theLevel, NFmiInfoData::Type theType);
	bool AddData(NFmiQueryData* theData
					  ,const NFmiString& theDataFileName
					  ,NFmiInfoData::Type theDataType
					  ,int theUndoLevel = 0);

	bool Clear (void);
	void ClearData(NFmiInfoData::Type theDataType);
	void ClearThisKindOfData(NFmiQueryInfo* theInfo);
 	NFmiProducerList* ProducerList(void); // k‰y l‰pi kaikki SmartInfot ja pyyt‰‰ ensimm‰iselt‰ parametrilta tuottajan (viel‰ ainakaan SmartInfolla ei ole montaa tuottajaa)
	NFmiLevelBag* GetAndCreateViewableInfoWithManyLevelsOrZeroPointer(void);

	const NFmiString& WorkingDirectory(void) const {return itsWorkingDirectory;};
	void WorkingDirectory(const NFmiString& newValue){itsWorkingDirectory = newValue;};
	void UpdateEditedDataCopy(void); // 28.09.1999/Marko
 private:
	int CountData(void);
 	bool Remove(void);	
	bool IsInfosTwoOfTheKind(NFmiQueryInfo* theInfo1, NFmiQueryInfo* theInfo2);

 public: // n‰m‰ ovat pikaviritys k‰ytˆss‰ n‰m‰ metodit pit‰isi laittaa takaisin privateiksi
	bool Reset (void);
	bool Next (void);
	NFmiSmartInfo* Current (void);

 private:
	NFmiSmartInfo* Info (const FmiParameterName& theParam, bool& fSubParameter, const NFmiLevel* theLevel, NFmiInfoData::Type theType);
  	NFmiSmartInfo* Info (const NFmiDataIdent& theIdent, bool& fSubParameter, const NFmiLevel* theLevel, NFmiInfoData::Type theType); 
    bool Add (NFmiSmartInfo* theInfo);
	NFmiParamBag GetParams(NFmiInfoData::Type theDataType);

// Attributes
  	NFmiSortedPtrList<NFmiSmartInfo> itsList; // error when compiling NFmiInfoOrganizer.cpp//binary '<' : 'class NFmiSmartInfo' does not define this operator or a conversion to a type acceptable to the predefined operator
	NFmiPtrList<NFmiSmartInfo>::Iterator itsIter;
 	NFmiDrawParamFactory* itsDrawParamFactory;
	NFmiString itsWorkingDirectory;
	NFmiSmartInfo* itsEditedData; // editoitavaa dataa voi olla vain yksi kerrallaan, joten laitoin sen erilleen tehokkuuden takia.
	NFmiSmartInfo* itsEditedDataCopy; // t‰m‰ on editoitavan datan kopio, mit‰ k‰ytt‰j‰ voi halutessaan p‰ivitt‰‰, k‰ytet‰‰n visualisoimaan tehtyj‰ muutoksia datassa
	bool fToolMasterAvailable;
};

#endif
