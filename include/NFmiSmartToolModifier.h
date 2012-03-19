#pragma once
// Luokan on tarkoitus kovata nykyinen NFmiSmartToolModifier-luokka.
// Vanha luokka on rˆnsyillyt pahasti ja nyt on aika siivota j‰ljet
// kirjoittamaal toiminnallisuus uusiksi. T‰ss‰ tulee mukaan myˆs
// uuden NFmiInfoOrganizer-luokan k‰yttˆ.
// TODO: 
// 1. Keksi parempi nimi tai muuta lopuksi NFmiSmartToolModifier-nimiseksi ja tuhoa alkuper‰inen luokka.
// 2. Siivoa rajapintaa jos pystyt.
// 3. Muuta luokka k‰ytt‰‰‰n NFmiOwnerInfo-luokkaa, t‰m‰ tosin piilossa, koska rajapinnoissa k‰ytet‰‰n NFmiFastQueryInfo.
// 4. Lis‰‰ tuki edellisiin mallidatoihin eli T_HIR[-1] viittaa edelliseen hirlam-ajoon.
// 5. Lis‰‰ tuki havaintodatan k‰ytˆlle (muuta asema-data hilaksi ja k‰yt‰ laskuissa.)
// 6. Voisi tehd‰ parserin fiksummin (boost:in tms avulla)
//
// T‰m‰ luokka hoitaa koko smarttool-toiminnan. Sill‰ on tietokanta,
// tulkki, ja erilaisia maski/operaatio generaattoreita, joiden avulla
// laskut suoritetaan.
//**********************************************************

#include "NFmiParamBag.h"
#include "NFmiDataMatrix.h"
#include "NFmiInfoData.h"
#include "NFmiLevelType.h"
#include "NFmiAreaMask.h"

#include <string>
#include <boost/shared_ptr.hpp>

class NFmiInfoOrganizer;
class NFmiSmartToolIntepreter;
class NFmiTimeDescriptor;
class NFmiAreaMaskInfo;
class NFmiFastQueryInfo;
class NFmiAreaMaskSectionInfo;
class NFmiAreaMask;
class NFmiDataModifier;
class NFmiDataIterator;
class NFmiDataIdent;
class NFmiMetTime;
class NFmiPoint;
class NFmiSmartToolCalculationBlock;
class NFmiMacroParamValue;
class NFmiLevel;
class NFmiSmartToolCalculationSection;
class NFmiSmartToolCalculationSectionInfo;
class NFmiSmartToolCalculation;
class NFmiSmartToolCalculationInfo;
class NFmiSmartToolCalculationBlockInfo;
class NFmiSmartToolCalculationBlockInfoVector;
class MyGrid;
class NFmiThreadCallBacks;

class NFmiSmartToolCalculationBlockVector
{
public:
	typedef checkedVector<boost::shared_ptr<NFmiSmartToolCalculationBlock> >::iterator Iterator;

	NFmiSmartToolCalculationBlockVector(void);
	NFmiSmartToolCalculationBlockVector(const NFmiSmartToolCalculationBlockVector &theOther);
	~NFmiSmartToolCalculationBlockVector(void);
	boost::shared_ptr<NFmiFastQueryInfo> FirstVariableInfo(void);
	void SetTime(const NFmiMetTime &theTime);
	void Calculate(const NFmiCalculationParams &theCalculationParams, NFmiMacroParamValue &theMacroParamValue);
	void Calculate_ver2(const NFmiCalculationParams &theCalculationParams);
	void Add(const boost::shared_ptr<NFmiSmartToolCalculationBlock> &theBlock);
	Iterator Begin(void) {return itsCalculationBlocks.begin();}
	Iterator End(void) {return itsCalculationBlocks.end();}

private:
	// luokka ei omista vektorissa olevia otuksia, Clear pit‰‰ kutsua erikseen!!!
	checkedVector<boost::shared_ptr<NFmiSmartToolCalculationBlock> > itsCalculationBlocks;
};

class NFmiSmartToolCalculationBlock
{
public:
	NFmiSmartToolCalculationBlock(void);
	NFmiSmartToolCalculationBlock(const NFmiSmartToolCalculationBlock &theOther);
	~NFmiSmartToolCalculationBlock(void);
	boost::shared_ptr<NFmiFastQueryInfo> FirstVariableInfo(void);
	void Time(const NFmiMetTime &theTime);
	void Calculate(const NFmiCalculationParams &theCalculationParams, NFmiMacroParamValue &theMacroParamValue);
	void Calculate_ver2(const NFmiCalculationParams &theCalculationParams, bool fDoMiddlePartOnly = false);

	boost::shared_ptr<NFmiSmartToolCalculationSection> itsFirstCalculationSection;
	boost::shared_ptr<NFmiSmartToolCalculation> itsIfAreaMaskSection;
	boost::shared_ptr<NFmiSmartToolCalculationBlockVector> itsIfCalculationBlocks;
	boost::shared_ptr<NFmiSmartToolCalculation> itsElseIfAreaMaskSection;
	boost::shared_ptr<NFmiSmartToolCalculationBlockVector> itsElseIfCalculationBlocks;
	boost::shared_ptr<NFmiSmartToolCalculationBlockVector> itsElseCalculationBlocks;
	boost::shared_ptr<NFmiSmartToolCalculationSection> itsLastCalculationSection;
};

#ifdef FMI_SUPPORT_STATION_DATA_SMARTTOOL
class NFmiEditMapGeneralDataDoc;
#endif // FMI_SUPPORT_STATION_DATA_SMARTTOOL

class NFmiSmartToolModifier
{
public:
	void InitSmartTool(const std::string &theSmartToolText, bool fThisIsMacroParamSkript = false);
	void ModifyData(NFmiTimeDescriptor* theModifiedTimes, bool fSelectedLocationsOnly, bool isMacroParamCalculation, NFmiThreadCallBacks *theThreadCallBacks);
	void ModifyData_ver2(NFmiTimeDescriptor* theModifiedTimes, bool fSelectedLocationsOnly, bool isMacroParamCalculation, NFmiThreadCallBacks *theThreadCallBacks);
	float CalcSmartToolValue(const NFmiMetTime &theTime, const NFmiPoint &theLatlon);
	void CalcCrossSectionSmartToolValues(NFmiDataMatrix<float> &theValues, checkedVector<float> &thePressures, checkedVector<NFmiPoint> &theLatlonPoints, const checkedVector<NFmiMetTime> &thePointTimes);

	NFmiSmartToolModifier(NFmiInfoOrganizer* theInfoOrganizer);
	~NFmiSmartToolModifier(void);

	bool IsMacroRunnable(void) const {return fMacroRunnable;}
	const std::string& GetErrorText(void) const {return itsErrorText;}
	const std::string& IncludeDirectory(void) const {return itsIncludeDirectory;}
	void IncludeDirectory(const std::string& newValue) {itsIncludeDirectory = newValue;}
	NFmiParamBag ModifiedParams(void);
	const std::string& GetStrippedMacroText(void) const;
	bool IsInterpretedSkriptMacroParam(void); // kun intepreter on tulkinnut smarttool-tekstin, voidaan kysy‰, onko kyseinen makro ns. macroParam-skripti eli sis‰lt‰‰kˆ se RESULT = ??? tapaista teksti‰
private:
	void ModifyData(NFmiTimeDescriptor* theModifiedTimes, bool fSelectedLocationsOnly, bool isMacroParamCalculation, NFmiMacroParamValue &theMacroParamValue, NFmiThreadCallBacks *theThreadCallBacks);
	float CalcSmartToolValue(NFmiMacroParamValue &theMacroParamValue);
	boost::shared_ptr<NFmiAreaMask> CreatePeekFunctionAreaMask(const NFmiAreaMaskInfo &theAreaMaskInfo, bool &fMustUsePressureInterpolation);
	boost::shared_ptr<NFmiAreaMask> CreateMetFunctionAreaMask(const NFmiAreaMaskInfo &theAreaMaskInfo, bool &fMustUsePressureInterpolation);
	void SetInfosMaskType(const boost::shared_ptr<NFmiFastQueryInfo> &theInfo);
	boost::shared_ptr<NFmiFastQueryInfo> UsedMacroParamData(void);
	void ModifyConditionalData(const boost::shared_ptr<NFmiSmartToolCalculationBlock> &theCalculationBlock, NFmiMacroParamValue &theMacroParamValue, NFmiThreadCallBacks *theThreadCallBacks);
	void ModifyConditionalData_ver2(const boost::shared_ptr<NFmiSmartToolCalculationBlock> &theCalculationBlock, NFmiThreadCallBacks *theThreadCallBacks);
	void ModifyBlockData(const boost::shared_ptr<NFmiSmartToolCalculationBlock> &theCalculationBlock, NFmiMacroParamValue &theMacroParamValue, NFmiThreadCallBacks *theThreadCallBacks);
	void ModifyBlockData_ver2(const boost::shared_ptr<NFmiSmartToolCalculationBlock> &theCalculationBlock, NFmiThreadCallBacks *theThreadCallBacks);
	boost::shared_ptr<NFmiSmartToolCalculationBlockVector> CreateCalculationBlockVector(const boost::shared_ptr<NFmiSmartToolCalculationBlockInfoVector> &theBlockInfoVector);
	boost::shared_ptr<NFmiSmartToolCalculationBlock> CreateCalculationBlock(NFmiSmartToolCalculationBlockInfo &theBlockInfo);
	boost::shared_ptr<NFmiFastQueryInfo> CreateRealScriptVariableInfo(const NFmiDataIdent &theDataIdent);
	boost::shared_ptr<NFmiFastQueryInfo> GetScriptVariableInfo(const NFmiDataIdent &theDataIdent);
	void ClearScriptVariableInfos(void);
	boost::shared_ptr<NFmiFastQueryInfo> CreateScriptVariableInfo(const NFmiDataIdent &theDataIdent);
	boost::shared_ptr<NFmiAreaMask> CreateCalculatedAreaMask(const NFmiAreaMaskInfo &theAreaMaskInfo);
	void GetParamValueLimits(const NFmiAreaMaskInfo &theAreaMaskInfo, float *theLowerLimit, float *theUpperLimit, bool *fCheckLimits);
	boost::shared_ptr<NFmiDataModifier> CreateIntegrationFuction(const NFmiAreaMaskInfo &theAreaMaskInfo);
	boost::shared_ptr<NFmiDataIterator> CreateIterator(const NFmiAreaMaskInfo &theAreaMaskInfo, const boost::shared_ptr<NFmiFastQueryInfo> &theInfo);
	void ModifyData2(boost::shared_ptr<NFmiSmartToolCalculationSection> &theCalculationSection, NFmiMacroParamValue &theMacroParamValue, NFmiThreadCallBacks *theThreadCallBacks);
	void ModifyData2_ver2(boost::shared_ptr<NFmiSmartToolCalculationSection> &theCalculationSection, NFmiThreadCallBacks *theThreadCallBacks);
	boost::shared_ptr<NFmiAreaMask> CreateAreaMask(const NFmiAreaMaskInfo &theInfo);
	boost::shared_ptr<NFmiAreaMask> CreateEndingAreaMask(void);
	boost::shared_ptr<NFmiFastQueryInfo> CreateInfo(const NFmiAreaMaskInfo &theAreaMaskInfo, bool &mustUsePressureInterpolation);
	boost::shared_ptr<NFmiFastQueryInfo> CreateInfo(const NFmiAreaMaskInfo &theAreaMaskInfo, bool &mustUsePressureInterpolation, unsigned long theWantedParamId);
	boost::shared_ptr<NFmiFastQueryInfo> GetPossibleLevelInterpolatedInfo(const NFmiAreaMaskInfo &theAreaMaskInfo, bool &mustUsePressureInterpolation);
	void CreateCalculationModifiers(void);
	void CreateFirstCalculationSection(void);
	boost::shared_ptr<NFmiSmartToolCalculationSection> CreateCalculationSection(const boost::shared_ptr<NFmiSmartToolCalculationSectionInfo> &theCalcSectionInfo);
	boost::shared_ptr<NFmiSmartToolCalculation> CreateCalculation(const boost::shared_ptr<NFmiSmartToolCalculationInfo> &theCalcInfo);
	boost::shared_ptr<NFmiSmartToolCalculation> CreateConditionalSection(const boost::shared_ptr<NFmiAreaMaskSectionInfo> &theAreaMaskSectionInfo);
	boost::shared_ptr<NFmiAreaMask> CreateSoundingIndexFunctionAreaMask(const NFmiAreaMaskInfo &theAreaMaskInfo);
	boost::shared_ptr<NFmiFastQueryInfo> CreateCopyOfAnalyzeInfo(const NFmiDataIdent& theDataIdent, const NFmiLevel* theLevel);
	boost::shared_ptr<NFmiFastQueryInfo> GetWantedAreaMaskData(const NFmiAreaMaskInfo &theAreaMaskInfo, bool fUseParIdOnly, NFmiInfoData::Type theOverRideDataType = NFmiInfoData::kNoDataType, FmiLevelType theOverRideLevelType = kFmiNoLevelType);
	boost::shared_ptr<NFmiFastQueryInfo> GetInfoFromOrganizer(const NFmiDataIdent& theIdent, const NFmiLevel* theLevel, NFmiInfoData::Type theType, bool fUseParIdOnly = false, bool fLevelData = false, int theModelRunIndex = 0);

	NFmiInfoOrganizer *itsInfoOrganizer; // eli database, ei omista ei tuhoa
	NFmiSmartToolIntepreter *itsSmartToolIntepreter; // omistaa, tuhoaa
	bool fMacroRunnable;
	std::string itsErrorText;

	bool fModifySelectedLocationsOnly;
	checkedVector<boost::shared_ptr<NFmiFastQueryInfo> > itsScriptVariableInfos; // mahdolliset skripti-muuttujat talletetaan t‰nne
	std::string itsIncludeDirectory; // mist‰ ladataan mahd. include filet

	NFmiTimeDescriptor *itsModifiedTimes; // ei omista/tuhoa
	bool fMacroParamCalculation; // t‰m‰ tieto tarvitaan scriptVariablejen kanssa, jos true, k‰ytet‰‰n pohjana macroParam-infoa, muuten editoitua dataa

	// N‰m‰ muuttujat ovat sit‰ varten ett‰ SumZ ja MinH tyyppisiss‰ funktoissa
	// k‰ytet‰‰n parasta mahdollista level-dataa. Eli ensin hybridi ja sitten painepinta dataa.
	bool fHeightFunctionFlag; // ollaanko tekem‰ss‰ SumZ tai MinH tyyppisen funktion calculaatio-otusta
	bool fUseLevelData; // kun t‰m‰ flagi on p‰‰ll‰, k‰ytet‰‰n CreateInfo-metodissa hybridi-datoja jos mahd. ja sitten painepinta datoja.
	bool fDoCrossSectionCalculation; // kun t‰m‰ flagi on p‰‰ll‰, ollaan laskemassa poikkileikkauksia ja silloin level-infot yritet‰‰n tehd‰ ensin hybrididatasta ja sitten painepintadatasta
	int itsCommaCounter; // tarvitaan laskemaan pilkkuja, kun lasketaan milloin level-dataa pit‰‰ k‰ytt‰‰.
	int itsParethesisCounter; // kun k‰ytet‰‰n esim. Sumz-funktion 2. pilkun j‰lkeen level-dataa,
							  // pit‰‰ laskea sulkujen avulla, milloin funktio loppuu.
							  // HUOM! sulkujen lis‰ksi pit‰‰ laskea myˆs erilaisten funktioiden alut.

	boost::shared_ptr<MyGrid> itsWorkingGrid; // T‰h‰n talletetaan ns. tyˆskentely gidi, eli miss‰ on tyˆskentely alueen area-m‰‰ritys ja laskennallinen hila koko.

#ifdef FMI_SUPPORT_STATION_DATA_SMARTTOOL
public:
	void SetGeneralDoc(NFmiEditMapGeneralDataDoc *theDoc);
private:
	NFmiEditMapGeneralDataDoc *itsDoc;
#endif // FMI_SUPPORT_STATION_DATA_SMARTTOOL

};


