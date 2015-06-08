//**********************************************************
// C++ Class Name : NFmiSmartToolModifier
// ---------------------------------------------------------
//  Author         : pietarin
//  Creation Date  : 8.11. 2010
//**********************************************************
#ifdef _MSC_VER
#pragma warning(disable : 4786) // poistaa n kpl VC++ kääntäjän varoitusta
#endif

#include "NFmiAreaMaskInfo.h"
#include "NFmiAreaMaskSectionInfo.h"
#include "NFmiCalculationConstantValue.h"
#include "NFmiDictionaryFunction.h"
#include "NFmiDrawParam.h"
#include "NFmiInfoAreaMaskSoundingIndex.h"
#include "NFmiInfoOrganizer.h"
#include "NFmiMetEditorTypes.h"
#include "NFmiSmartInfo.h"
#include "NFmiSmartToolCalculation.h"
#include "NFmiSmartToolCalculationInfo.h"
#include "NFmiSmartToolCalculationSection.h"
#include "NFmiSmartToolCalculationSectionInfo.h"
#include "NFmiSmartToolIntepreter.h"
#include "NFmiSmartToolModifier.h"

#include <NFmiBitMask.h>
#include <NFmiCalculatedAreaMask.h>
#include <NFmiDataModifierClasses.h>
#include <NFmiFastQueryInfo.h>
#include <NFmiInfoAreaMask.h>
#include <NFmiQueryData.h>
#include <NFmiQueryDataUtil.h>
#include <NFmiRelativeDataIterator.h>
#include <NFmiRelativeTimeIntegrationIterator.h>

#include <stdexcept>

#ifdef _MSC_VER
#pragma warning (disable : 4244 4267 4512) // boost:in thread kirjastosta tulee ikävästi 4244 varoituksia
#endif
#include <boost/thread.hpp>

#ifdef _MSC_VER
#pragma warning (default : 4244 4267 4512) // laitetaan 4244 takaisin päälle, koska se on tärkeä (esim. double -> int auto castaus varoitus)
#endif

using namespace std;

static boost::shared_ptr<NFmiFastQueryInfo> CreateShallowCopyOfHighestInfo(const boost::shared_ptr<NFmiFastQueryInfo> &theInfo)
{
	if(theInfo)
	{
		NFmiSmartInfo *smartInfo = dynamic_cast<NFmiSmartInfo*>(theInfo.get());
		if(smartInfo)
			return boost::shared_ptr<NFmiFastQueryInfo>(new NFmiSmartInfo(*smartInfo));

		NFmiOwnerInfo *ownerInfo = dynamic_cast<NFmiOwnerInfo*>(theInfo.get());
		if(ownerInfo)
			return boost::shared_ptr<NFmiFastQueryInfo>(new NFmiOwnerInfo(*ownerInfo));

		NFmiFastQueryInfo *fastInfo = dynamic_cast<NFmiFastQueryInfo*>(theInfo.get());
		if(fastInfo)
			return boost::shared_ptr<NFmiFastQueryInfo>(new NFmiFastQueryInfo(*fastInfo));
	}

	return boost::shared_ptr<NFmiFastQueryInfo>();
}

static checkedVector<boost::shared_ptr<NFmiSmartToolCalculationBlock> > DoShallowCopy(const checkedVector<boost::shared_ptr<NFmiSmartToolCalculationBlock> > &theCalculationBlockVector)
{
	checkedVector<boost::shared_ptr<NFmiSmartToolCalculationBlock> > returnVector(theCalculationBlockVector.size());
	for(size_t i = 0; i < theCalculationBlockVector.size(); i++)
		returnVector[i] = boost::shared_ptr<NFmiSmartToolCalculationBlock>(new NFmiSmartToolCalculationBlock(*theCalculationBlockVector[i]));
	return returnVector;
}

NFmiSmartToolCalculationBlockVector::NFmiSmartToolCalculationBlockVector(void)
:itsCalculationBlocks()
{
}

NFmiSmartToolCalculationBlockVector::NFmiSmartToolCalculationBlockVector(const NFmiSmartToolCalculationBlockVector &theOther)
:itsCalculationBlocks(::DoShallowCopy(theOther.itsCalculationBlocks))
{
}

NFmiSmartToolCalculationBlockVector::~NFmiSmartToolCalculationBlockVector(void)
{
}

boost::shared_ptr<NFmiFastQueryInfo> NFmiSmartToolCalculationBlockVector::FirstVariableInfo(void)
{
	Iterator it = Begin();
	Iterator endIt = End();
	for( ; it != endIt; ++it)
		if((*it)->FirstVariableInfo()) // pitäisi löytyä aina jotain!!!
			return (*it)->FirstVariableInfo();
	return boost::shared_ptr<NFmiFastQueryInfo>();
}

void NFmiSmartToolCalculationBlockVector::SetTime(const NFmiMetTime &theTime)
{
	Iterator it = Begin();
	Iterator endIt = End();
	for( ; it != endIt; ++it)
		(*it)->Time(theTime);
}

void NFmiSmartToolCalculationBlockVector::Calculate(const NFmiCalculationParams &theCalculationParams, NFmiMacroParamValue &theMacroParamValue)
{
	Iterator it = Begin();
	Iterator endIt = End();
	for( ; it != endIt; ++it)
		(*it)->Calculate(theCalculationParams, theMacroParamValue);
}

void NFmiSmartToolCalculationBlockVector::Calculate_ver2(const NFmiCalculationParams &theCalculationParams)
{
	Iterator it = Begin();
	Iterator endIt = End();
	for( ; it != endIt; ++it)
		(*it)->Calculate_ver2(theCalculationParams);
}

void NFmiSmartToolCalculationBlockVector::Add(const boost::shared_ptr<NFmiSmartToolCalculationBlock> &theBlock)
{ // ottaa omistukseen theBlock:in!!
	itsCalculationBlocks.push_back(theBlock);
}

NFmiSmartToolCalculationBlock::NFmiSmartToolCalculationBlock(void)
:itsFirstCalculationSection()
,itsIfAreaMaskSection()
,itsIfCalculationBlocks()
,itsElseIfAreaMaskSection()
,itsElseIfCalculationBlocks()
,itsElseCalculationBlocks()
,itsLastCalculationSection()
{
}

NFmiSmartToolCalculationBlock::NFmiSmartToolCalculationBlock(const NFmiSmartToolCalculationBlock &theOther)
:itsFirstCalculationSection(theOther.itsFirstCalculationSection ? new NFmiSmartToolCalculationSection(*theOther.itsFirstCalculationSection) : 0)
,itsIfAreaMaskSection(theOther.itsIfAreaMaskSection ? new NFmiSmartToolCalculation(*theOther.itsIfAreaMaskSection) : 0)
,itsIfCalculationBlocks(theOther.itsIfCalculationBlocks ? new NFmiSmartToolCalculationBlockVector(*theOther.itsIfCalculationBlocks) : 0)
,itsElseIfAreaMaskSection(theOther.itsElseIfAreaMaskSection ? new NFmiSmartToolCalculation(*theOther.itsElseIfAreaMaskSection) : 0)
,itsElseIfCalculationBlocks(theOther.itsElseIfCalculationBlocks ? new NFmiSmartToolCalculationBlockVector(*theOther.itsElseIfCalculationBlocks) : 0)
,itsElseCalculationBlocks(theOther.itsElseCalculationBlocks ? new NFmiSmartToolCalculationBlockVector(*theOther.itsElseCalculationBlocks) : 0)
,itsLastCalculationSection(theOther.itsLastCalculationSection ? new NFmiSmartToolCalculationSection(*theOther.itsLastCalculationSection) : 0)
{
}

NFmiSmartToolCalculationBlock::~NFmiSmartToolCalculationBlock(void)
{
}

boost::shared_ptr<NFmiFastQueryInfo> NFmiSmartToolCalculationBlock::FirstVariableInfo(void)
{
	boost::shared_ptr<NFmiFastQueryInfo> info;
	if(itsFirstCalculationSection)
		info = itsFirstCalculationSection->FirstVariableInfo();
	if(info == 0 && itsIfCalculationBlocks)
		info = itsIfCalculationBlocks->FirstVariableInfo();
	if(info == 0 && itsElseIfCalculationBlocks)
		info = itsElseIfCalculationBlocks->FirstVariableInfo();
	if(info == 0 && itsElseCalculationBlocks)
		info = itsElseCalculationBlocks->FirstVariableInfo();
	if(info == 0 && itsLastCalculationSection)
		info = itsLastCalculationSection->FirstVariableInfo();
	return info;
}

void NFmiSmartToolCalculationBlock::Time(const NFmiMetTime &theTime)
{
	if(itsFirstCalculationSection)
		itsFirstCalculationSection->SetTime(theTime);

	if(itsIfAreaMaskSection)
		itsIfAreaMaskSection->Time(theTime);
	if(itsIfCalculationBlocks)
		itsIfCalculationBlocks->SetTime(theTime);
	if(itsElseIfAreaMaskSection)
		itsElseIfAreaMaskSection->Time(theTime);
	if(itsElseIfCalculationBlocks)
		itsElseIfCalculationBlocks->SetTime(theTime);
	if(itsElseCalculationBlocks)
		itsElseCalculationBlocks->SetTime(theTime);

	if(itsLastCalculationSection)
		itsLastCalculationSection->SetTime(theTime);
}

void NFmiSmartToolCalculationBlock::Calculate(const NFmiCalculationParams &theCalculationParams, NFmiMacroParamValue &theMacroParamValue)
{
	if(itsFirstCalculationSection)
		itsFirstCalculationSection->Calculate(theCalculationParams, theMacroParamValue);

	if(itsIfAreaMaskSection && itsIfAreaMaskSection->IsMasked(theCalculationParams))
		itsIfCalculationBlocks->Calculate(theCalculationParams, theMacroParamValue);
	else if(itsElseIfAreaMaskSection && itsElseIfAreaMaskSection->IsMasked(theCalculationParams))
		itsElseIfCalculationBlocks->Calculate(theCalculationParams, theMacroParamValue);
	else if(itsElseCalculationBlocks)
		itsElseCalculationBlocks->Calculate(theCalculationParams, theMacroParamValue);

	if(itsLastCalculationSection)
		itsLastCalculationSection->Calculate(theCalculationParams, theMacroParamValue);
}

void NFmiSmartToolCalculationBlock::Calculate_ver2(const NFmiCalculationParams &theCalculationParams, bool fDoMiddlePartOnly)
{
	if(fDoMiddlePartOnly == false)
	{
		if(itsFirstCalculationSection)
			itsFirstCalculationSection->Calculate_ver2(theCalculationParams);
	}

	if(itsIfAreaMaskSection && itsIfAreaMaskSection->IsMasked(theCalculationParams))
		itsIfCalculationBlocks->Calculate_ver2(theCalculationParams);
	else if(itsElseIfAreaMaskSection && itsElseIfAreaMaskSection->IsMasked(theCalculationParams))
		itsElseIfCalculationBlocks->Calculate_ver2(theCalculationParams);
	else if(itsElseCalculationBlocks)
		itsElseCalculationBlocks->Calculate_ver2(theCalculationParams);

	if(fDoMiddlePartOnly == false)
	{
		if(itsLastCalculationSection)
			itsLastCalculationSection->Calculate_ver2(theCalculationParams);
	}
}

//--------------------------------------------------------
// Constructor/Destructor
//--------------------------------------------------------
NFmiSmartToolModifier::NFmiSmartToolModifier(NFmiInfoOrganizer* theInfoOrganizer)
:itsInfoOrganizer(theInfoOrganizer)
,itsSmartToolIntepreter(new NFmiSmartToolIntepreter(0))
,fModifySelectedLocationsOnly(false)
,itsIncludeDirectory()
,itsModifiedTimes(0)
,fMacroParamCalculation(false)
,fHeightFunctionFlag(false)
,fUseLevelData(false)
,fDoCrossSectionCalculation(false)
,itsCommaCounter(0)
,itsParethesisCounter(0)
,itsWorkingGrid(new MyGrid())
,itsModifiedLevel()
#ifdef FMI_SUPPORT_STATION_DATA_SMARTTOOL
,itsDoc(0)
#endif // FMI_SUPPORT_STATION_DATA_SMARTTOOL
{
}
NFmiSmartToolModifier::~NFmiSmartToolModifier(void)
{
}
//--------------------------------------------------------
// InitSmartTool
//--------------------------------------------------------
// Tulkitsee macron, luo tavittavat systeemit, että makro voidaan suorittaa.
void NFmiSmartToolModifier::InitSmartTool(const std::string &theSmartToolText, bool fThisIsMacroParamSkript)
{
	fMacroRunnable = true;
	itsErrorText = "";
	try
	{
		itsSmartToolIntepreter->IncludeDirectory(itsIncludeDirectory);
		itsSmartToolIntepreter->Interpret(theSmartToolText, fThisIsMacroParamSkript);
	}
	catch(...)
	{
		fMacroRunnable = false;
		throw  ;
	}
}

boost::shared_ptr<NFmiSmartToolCalculationBlockVector> NFmiSmartToolModifier::CreateCalculationBlockVector(const boost::shared_ptr<NFmiSmartToolCalculationBlockInfoVector> &theBlockInfoVector)
{
	if(theBlockInfoVector && (!theBlockInfoVector->Empty()))
	{
		boost::shared_ptr<NFmiSmartToolCalculationBlockVector> vec(new NFmiSmartToolCalculationBlockVector());
		NFmiSmartToolCalculationBlockInfoVector::Iterator it = theBlockInfoVector->Begin();
		NFmiSmartToolCalculationBlockInfoVector::Iterator endIt = theBlockInfoVector->End();
		for( ; it != endIt; ++it)
			vec->Add(CreateCalculationBlock(*((*it).get())));
		return vec;
	}
	return boost::shared_ptr<NFmiSmartToolCalculationBlockVector>();
}

boost::shared_ptr<NFmiSmartToolCalculationBlock> NFmiSmartToolModifier::CreateCalculationBlock(NFmiSmartToolCalculationBlockInfo &theBlockInfo)
{
	boost::shared_ptr<NFmiSmartToolCalculationBlock> block(new NFmiSmartToolCalculationBlock());

	block->itsFirstCalculationSection = CreateCalculationSection(theBlockInfo.itsFirstCalculationSectionInfo);
	block->itsIfAreaMaskSection = CreateConditionalSection(theBlockInfo.itsIfAreaMaskSectionInfo);
	if(block->itsIfAreaMaskSection)
	{
		block->itsIfCalculationBlocks = CreateCalculationBlockVector(theBlockInfo.itsIfCalculationBlockInfos);
		if(!block->itsIfCalculationBlocks)
		{
			string errorText(::GetDictionaryString("SmartToolModifierErrorIfClause"));
			throw runtime_error(errorText);
		}
		block->itsElseIfAreaMaskSection = CreateConditionalSection(theBlockInfo.itsElseIfAreaMaskSectionInfo);
		if(block->itsElseIfAreaMaskSection)
		{
			block->itsElseIfCalculationBlocks = CreateCalculationBlockVector(theBlockInfo.itsElseIfCalculationBlockInfos);
			if(!block->itsElseIfCalculationBlocks)
			{
				string errorText(::GetDictionaryString("SmartToolModifierErrorElseIfClause"));
				throw runtime_error(errorText);
			}
		}
		if(theBlockInfo.fElseSectionExist)
		{
			block->itsElseCalculationBlocks = CreateCalculationBlockVector(theBlockInfo.itsElseCalculationBlockInfos);
			if(!block->itsElseCalculationBlocks)
			{
				string errorText(::GetDictionaryString("SmartToolModifierErrorElseClause"));
				throw runtime_error(errorText);
			}
		}
	}
	block->itsLastCalculationSection = CreateCalculationSection(theBlockInfo.itsLastCalculationSectionInfo);
	return block;
}

boost::shared_ptr<NFmiSmartToolCalculation> NFmiSmartToolModifier::CreateConditionalSection(const boost::shared_ptr<NFmiAreaMaskSectionInfo> &theAreaMaskSectionInfo)
{
	boost::shared_ptr<NFmiSmartToolCalculation> areaMaskHandler;
	if(theAreaMaskSectionInfo)
	{
		checkedVector<boost::shared_ptr<NFmiAreaMaskInfo> > &maskInfos = theAreaMaskSectionInfo->GetAreaMaskInfoVector();
		size_t size = maskInfos.size();
		if(size)
		{
			areaMaskHandler = boost::shared_ptr<NFmiSmartToolCalculation>(new NFmiSmartToolCalculation());
			areaMaskHandler->SetCalculationText(theAreaMaskSectionInfo->GetCalculationText());
			for(size_t i=0; i<size; i++)
// HUOM!!!! editoitavaN DATAN QDatasta pitää tehdä kopiot, muuten maskit eivät toimi
// kaikissa tilanteissa oikein!! KORJAA TÄMÄ!!!!!
				areaMaskHandler->AddCalculation(CreateAreaMask(*maskInfos[i]));
			// loppuun lisätään vielä lopetus 'merkki'
			areaMaskHandler->AddCalculation(CreateEndingAreaMask());

			return areaMaskHandler;
		}
	}
	return areaMaskHandler;
}

boost::shared_ptr<NFmiSmartToolCalculationSection> NFmiSmartToolModifier::CreateCalculationSection(const boost::shared_ptr<NFmiSmartToolCalculationSectionInfo> &theCalcSectionInfo)
{
	boost::shared_ptr<NFmiSmartToolCalculationSection> section;
	if(theCalcSectionInfo)
	{
		checkedVector<boost::shared_ptr<NFmiSmartToolCalculationInfo> > &calcInfos = theCalcSectionInfo->GetCalculationInfos();
		size_t size = calcInfos.size();
		if(size)
		{
			section = boost::shared_ptr<NFmiSmartToolCalculationSection>(new NFmiSmartToolCalculationSection());
			for(size_t i=0; i<size; i++)
			{
				section->AddCalculations(CreateCalculation(calcInfos[i]));
			}
			return section;
		}
	}
	return section;
}

boost::shared_ptr<NFmiSmartToolCalculation> NFmiSmartToolModifier::CreateCalculation(const boost::shared_ptr<NFmiSmartToolCalculationInfo> &theCalcInfo)
{
	boost::shared_ptr<NFmiSmartToolCalculation> calculation;
	size_t size = theCalcInfo->GetCalculationOperandInfoVector().size();
	if(size)
	{
		checkedVector<boost::shared_ptr<NFmiAreaMaskInfo> > &areaMaskInfos = theCalcInfo->GetCalculationOperandInfoVector();
		calculation = boost::shared_ptr<NFmiSmartToolCalculation>(new NFmiSmartToolCalculation());
		calculation->SetCalculationText(theCalcInfo->GetCalculationText());
		calculation->SetCalculationText(theCalcInfo->GetCalculationText());
		bool mustUsePressureInterpolation = false; // tätäei käytetä tässä, mutta pakko laittaa metodin interfacen takia
		calculation->SetResultInfo(CreateInfo(*theCalcInfo->GetResultDataInfo(), mustUsePressureInterpolation));
		if(calculation->GetResultInfo() && calculation->GetResultInfo()->Grid())
			itsWorkingGrid = boost::shared_ptr<MyGrid>(new MyGrid(*calculation->GetResultInfo()->Grid())); // tämä työskentely alueen hila ja area otettava talteen

		float lowerLimit = kFloatMissing;
		float upperLimit = kFloatMissing;
		bool checkLimits = true; // yleensä parametreille käytetdään min/max rajoja, mutta ei esim TotalWind tai W&C:lle
		GetParamValueLimits(*theCalcInfo->GetResultDataInfo(), &lowerLimit, &upperLimit, &checkLimits);
		calculation->SetLimits(lowerLimit, upperLimit, checkLimits);
		calculation->AllowMissingValueAssignment(theCalcInfo->AllowMissingValueAssignment());
		for(size_t i=0; i<size; i++)
		{
			if(areaMaskInfos[i] != 0)
				calculation->AddCalculation(CreateAreaMask(*areaMaskInfos[i])); // HUOM! TÄHÄN KAATUU JOSKUS, TUTKI ASIAA!!!!!
			else
			{
				string errStr("Error in application: NFmiSmartToolModifier::CreateCalculation - zero pointer error with calculation\n");
				errStr += theCalcInfo->GetCalculationText();
				errStr += "\nNotify application developer with this.";
				throw runtime_error(errStr);
			}
		}
		// loppuun lisätään vielä loputus 'merkki'
		calculation->AddCalculation(CreateEndingAreaMask());
		return calculation;
	}
	return calculation;
}

// Laskee smarttool-systeemin avulla halutun poikkileikkauksen arvo-matriisin.
// Tämä on universaali metodi, joka hoitaa kaikki kolme tapausta: poikkileikkaus, aika-poikkileikkaus ja reitti-poikkileikkaus.
// Parametreina annetaa aina sopiva pisteet ja ajat, että saadaan eri efektit aikaan. Eli aika-poikkileikkauksessa
// kaikki pisteet ovat samoja, normaali poikkileikkauksessa kaikki ajat ovat samoja ja reitti versiossa ajat ja pisteet muuttuvat.
void NFmiSmartToolModifier::CalcCrossSectionSmartToolValues(NFmiDataMatrix<float> &theValues, checkedVector<float> &thePressures, checkedVector<NFmiPoint> &theLatlonPoints, const checkedVector<NFmiMetTime> &thePointTimes)
{
	fDoCrossSectionCalculation = true;
	size_t sizeX = theLatlonPoints.size();
	if(sizeX != thePointTimes.size())
		throw runtime_error("NFmiSmartToolModifier::CalcCrossSectionSmartToolValues - latlon point count and time count must be the same, Error in program.");
	size_t sizeY = thePressures.size();
	if(sizeX < 1 || sizeY < 1)
		throw runtime_error("NFmiSmartToolModifier::CalcCrossSectionSmartToolValues - invalid data-matrix size, Error in program.");
	theValues.Resize(sizeX, sizeY, kFloatMissing);

	NFmiMacroParamValue macroParamValue;
	macroParamValue.fSetValue = true;
	macroParamValue.fDoCrossSectionCalculations = true;

	// lasketaan läpi yksittäisiä arvoja kaikille halutuille pisteille
	for(size_t i = 0; i<sizeX; i++)
	{
		macroParamValue.itsLatlon = theLatlonPoints[i];
		macroParamValue.itsTime = thePointTimes[i];
		NFmiTimeBag timeBag(macroParamValue.itsTime, macroParamValue.itsTime, 60);
		NFmiTimeDescriptor times(macroParamValue.itsTime, timeBag);
		itsInfoOrganizer->CrossSectionMacroParamData()->SetTimeDescriptor(times); // asetetaan makroData-infon aikasysteemi currentin kartan kohtaan (feikki datassa vain yksi aika ja se pitää säätää kohdalleen, että laskut onnistuvat)
		for(size_t j = 0; j<sizeY; j++)
		{
			macroParamValue.itsPressureHeight = thePressures[j];
			float value = CalcSmartToolValue(macroParamValue);
			theValues[i][j] = value;
		}
	}
}

float NFmiSmartToolModifier::CalcSmartToolValue(NFmiMacroParamValue &theMacroParamValue)
{
	NFmiTimeBag validTimes(theMacroParamValue.itsTime, theMacroParamValue.itsTime, 60);
	NFmiTimeDescriptor times(validTimes, theMacroParamValue.itsTime);
	// oikeasti tässä ei modifioida mitään, vaan palautetaan vain yksi arvo
	ModifyData(&times, false, true, theMacroParamValue, 0);
	return theMacroParamValue.itsValue;
}

float NFmiSmartToolModifier::CalcSmartToolValue(const NFmiMetTime &theTime, const NFmiPoint &theLatlon)
{
	NFmiMacroParamValue macroParamValue;
	macroParamValue.fSetValue = true;
	macroParamValue.itsLatlon = theLatlon;
	macroParamValue.itsTime = theTime;
	return CalcSmartToolValue(macroParamValue);
}


//--------------------------------------------------------
// ModifyData
//--------------------------------------------------------
// Suorittaa varsinaiset modifikaatiot. Käyttäjä voi antaa parametrina rajoitetun ajan
// modifioinneille, jos theModifiedTimes on 0-pointteri, tehdään operaatiot kaikille
// datan ajoille.
void NFmiSmartToolModifier::ModifyData(NFmiTimeDescriptor* theModifiedTimes, bool fSelectedLocationsOnly, bool isMacroParamCalculation, NFmiThreadCallBacks *theThreadCallBacks)
{
	NFmiMacroParamValue macroParamValue;
	ModifyData(theModifiedTimes, fSelectedLocationsOnly, isMacroParamCalculation, macroParamValue, theThreadCallBacks);
}

// tässä lasketaan jos käytössä on progress-dialogi, smarttool:in kokonais steppi määrä ja asetetaan se dialogille
static void CalcTotalProgressStepCount(checkedVector<NFmiSmartToolCalculationBlockInfo> &theCalcInfoVector, NFmiTimeDescriptor *theModifiedTimes, NFmiThreadCallBacks *theThreadCallBacks)
{
	if(theThreadCallBacks && theModifiedTimes)
	{
		int sizeTimes = static_cast<int>(theModifiedTimes->Size());
		int totalStepCount = 0;
		for(size_t i=0; i < theCalcInfoVector.size(); i++)
		{
			NFmiSmartToolCalculationBlockInfo &blockInfo = theCalcInfoVector[i];
			if(blockInfo.itsFirstCalculationSectionInfo)
				totalStepCount += static_cast<int>(blockInfo.itsFirstCalculationSectionInfo->GetCalculationInfos().size() * sizeTimes);
            if(blockInfo.itsIfAreaMaskSectionInfo && blockInfo.itsIfAreaMaskSectionInfo->GetAreaMaskInfoVector().size())
				totalStepCount += sizeTimes;
			if(blockInfo.itsLastCalculationSectionInfo)
				totalStepCount += static_cast<int>(blockInfo.itsLastCalculationSectionInfo->GetCalculationInfos().size() * sizeTimes);
		}
		NFmiQueryDataUtil::SetRange(theThreadCallBacks, 0, totalStepCount, 1);
	}
}

void NFmiSmartToolModifier::ModifyData(NFmiTimeDescriptor* theModifiedTimes, bool fSelectedLocationsOnly, bool isMacroParamCalculation, NFmiMacroParamValue &theMacroParamValue, NFmiThreadCallBacks *theThreadCallBacks)
{
	itsModifiedTimes = theModifiedTimes;
	fMacroParamCalculation = isMacroParamCalculation;
	fModifySelectedLocationsOnly = fSelectedLocationsOnly;
	try
	{
		checkedVector<NFmiSmartToolCalculationBlockInfo>& smartToolCalculationBlockInfos = itsSmartToolIntepreter->SmartToolCalculationBlocks();
		::CalcTotalProgressStepCount(smartToolCalculationBlockInfos, theModifiedTimes, theThreadCallBacks);
		size_t size = smartToolCalculationBlockInfos.size();
		for(size_t i=0; i<size; i++)
		{
			NFmiSmartToolCalculationBlockInfo blockInfo = smartToolCalculationBlockInfos[i];
			boost::shared_ptr<NFmiSmartToolCalculationBlock> block = CreateCalculationBlock(blockInfo);
			if(block)
			{
				ModifyBlockData(block, theMacroParamValue, theThreadCallBacks);
			}
		}
		ClearScriptVariableInfos(); // lopuksi nämä skripti-muuttujat tyhjennetään
	}
	catch(...)
	{
		ClearScriptVariableInfos(); // lopuksi nämä skripti-muuttujat tyhjennetään
		fMacroRunnable = false;
		throw ;
	}
}

void NFmiSmartToolModifier::ModifyData_ver2(NFmiTimeDescriptor* theModifiedTimes, bool fSelectedLocationsOnly, bool isMacroParamCalculation, NFmiThreadCallBacks *theThreadCallBacks)
{
	itsModifiedTimes = theModifiedTimes;
	fMacroParamCalculation = isMacroParamCalculation;
	fModifySelectedLocationsOnly = fSelectedLocationsOnly;
	try
	{
		checkedVector<NFmiSmartToolCalculationBlockInfo>& smartToolCalculationBlockInfos = itsSmartToolIntepreter->SmartToolCalculationBlocks();
		::CalcTotalProgressStepCount(smartToolCalculationBlockInfos, theModifiedTimes, theThreadCallBacks);
		size_t size = smartToolCalculationBlockInfos.size();
		for(size_t i=0; i<size; i++)
		{
			NFmiSmartToolCalculationBlockInfo blockInfo = smartToolCalculationBlockInfos[i];
			boost::shared_ptr<NFmiSmartToolCalculationBlock> block = CreateCalculationBlock(blockInfo);
			if(block)
			{
				ModifyBlockData_ver2(block, theThreadCallBacks);
			}
		}
		ClearScriptVariableInfos(); // lopuksi nämä skripti-muuttujat tyhjennetään
	}
	catch(...)
	{
		ClearScriptVariableInfos(); // lopuksi nämä skripti-muuttujat tyhjennetään
		fMacroRunnable = false;
		throw ;
	}
}

// Kun intepreter on tulkinnut smarttool-tekstin, voidaan kysyä, onko kyseinen makro ns. macroParam-skripti eli sisältääkö se RESULT = ??? tapaista tekstiä
bool NFmiSmartToolModifier::IsInterpretedSkriptMacroParam(void)
{
	return itsSmartToolIntepreter ? itsSmartToolIntepreter->IsInterpretedSkriptMacroParam() : false;
}

void NFmiSmartToolModifier::ModifyBlockData(const boost::shared_ptr<NFmiSmartToolCalculationBlock> &theCalculationBlock, NFmiMacroParamValue &theMacroParamValue, NFmiThreadCallBacks *theThreadCallBacks)
{
	// HUOM!! Koska jostain syystä alku ja loppu CalculationSection:it lasketaan erikseen, pitää muistaa 
	// että ModifyConditionalData-osiossa ei saa käsitellä näitä sectioneita!!!!
	// Ok, nyt tiedän, että tämä johtuu siitä että ModifyData2(_ver2) -funktioissa laskut suoritetaan aina rivi kerrallaa (kaikki ajat ja paikat lääpi),
	// kun taas if-elseif-else -rakenteissa lasketaan koko hökötys kerrallaan läpi.
	ModifyData2(theCalculationBlock->itsFirstCalculationSection, theMacroParamValue, theThreadCallBacks);
	ModifyConditionalData(theCalculationBlock, theMacroParamValue, theThreadCallBacks);
	ModifyData2(theCalculationBlock->itsLastCalculationSection, theMacroParamValue, theThreadCallBacks);
}

void NFmiSmartToolModifier::ModifyBlockData_ver2(const boost::shared_ptr<NFmiSmartToolCalculationBlock> &theCalculationBlock, NFmiThreadCallBacks *theThreadCallBacks)
{
	// HUOM!! Koska jostain syystä alku ja loppu CalculationSection:it lasketaan erikseen, pitää muistaa 
	// että ModifyConditionalData-osiossa ei saa käsitellä näitä sectioneita!!!!
	// Ok, nyt tiedän, että tämä johtuu siitä että ModifyData2(_ver2) -funktioissa laskut suoritetaan aina rivi kerrallaa (kaikki ajat ja paikat lääpi),
	// kun taas if-elseif-else -rakenteissa lasketaan koko hökötys kerrallaan läpi.
	ModifyData2_ver2(theCalculationBlock->itsFirstCalculationSection, theThreadCallBacks);
	ModifyConditionalData_ver2(theCalculationBlock, theThreadCallBacks);
	ModifyData2_ver2(theCalculationBlock->itsLastCalculationSection, theThreadCallBacks);
}

void NFmiSmartToolModifier::ModifyConditionalData(const boost::shared_ptr<NFmiSmartToolCalculationBlock> &theCalculationBlock, NFmiMacroParamValue &theMacroParamValue, NFmiThreadCallBacks *theThreadCallBacks)
{
	if(theCalculationBlock->itsIfAreaMaskSection && theCalculationBlock->itsIfCalculationBlocks)
	{
		if(theCalculationBlock->FirstVariableInfo() == 0)
			throw runtime_error(::GetDictionaryString("SmartToolModifierErrorUnknownProblem"));
		boost::shared_ptr<NFmiFastQueryInfo> info(dynamic_cast<NFmiFastQueryInfo*>(theCalculationBlock->FirstVariableInfo()->Clone()));

		try
		{
			NFmiCalculationParams calculationParams;
			SetInfosMaskType(info);
			NFmiTimeDescriptor modifiedTimes(itsModifiedTimes ? *itsModifiedTimes : info->TimeDescriptor());
			for(modifiedTimes.Reset(); modifiedTimes.Next(); )
			{
				if(info->Time(modifiedTimes.Time()))
				{
					NFmiQueryDataUtil::CheckIfStopped(theThreadCallBacks);
					NFmiQueryDataUtil::DoStepIt(theThreadCallBacks); // stepataan vasta 0-tarkastuksen jälkeen!
					calculationParams.itsTime = modifiedTimes.Time();
					if(theMacroParamValue.fSetValue)
						calculationParams.itsTime = theMacroParamValue.itsTime;
					calculationParams.itsTimeIndex = info->TimeIndex();
					theCalculationBlock->itsIfAreaMaskSection->Time(calculationParams.itsTime); // yritetään optimoida laskuja hieman kun mahdollista
					theCalculationBlock->itsIfCalculationBlocks->SetTime(calculationParams.itsTime); // yritetään optimoida laskuja hieman kun mahdollista
					if(theCalculationBlock->itsElseIfAreaMaskSection && theCalculationBlock->itsElseIfCalculationBlocks)
					{
						theCalculationBlock->itsElseIfAreaMaskSection->Time(calculationParams.itsTime);
						theCalculationBlock->itsElseIfCalculationBlocks->SetTime(calculationParams.itsTime);
					}
					if(theCalculationBlock->itsElseCalculationBlocks)
						theCalculationBlock->itsElseCalculationBlocks->SetTime(calculationParams.itsTime);

					for(info->ResetLocation(); info->NextLocation(); )
					{
						calculationParams.itsLatlon = info->LatLon();
						if(theMacroParamValue.fSetValue)
						{
							calculationParams.itsLatlon = theMacroParamValue.itsLatlon;
							info->Location(calculationParams.itsLatlon); // pitää laittaa nearestlocation päälle, että tuloksia voidaan myöhemmin hakea interpolaation avulla
						}
						calculationParams.itsLocationIndex = info->LocationIndex(); // tämä locationindex juttu liittyy kai optimointiin, jota ei tehdä enää, pitäisikö poistaa
						if(theCalculationBlock->itsIfAreaMaskSection->IsMasked(calculationParams))
							theCalculationBlock->itsIfCalculationBlocks->Calculate(calculationParams, theMacroParamValue);
						else if(theCalculationBlock->itsElseIfAreaMaskSection && theCalculationBlock->itsElseIfCalculationBlocks && theCalculationBlock->itsElseIfAreaMaskSection->IsMasked(calculationParams))
						{
							theCalculationBlock->itsElseIfCalculationBlocks->Calculate(calculationParams, theMacroParamValue);
						}
						else if(theCalculationBlock->itsElseCalculationBlocks)
							theCalculationBlock->itsElseCalculationBlocks->Calculate(calculationParams, theMacroParamValue);
						if(theMacroParamValue.fSetValue)
						{
							return ; // eli jos oli yhden pisteen laskusta kyse, lopetetaan loppi heti
						}
					}
				}
			}
		}
		catch(...)
		{
			throw ;
		}
	}
}

// Kun dataa käydään läpi NextLocation-menetelmällä, ja kyseessä on NFmiSmartInfo-olio, on niillä tiedossa sisäinen bitmaski, jonka
// avulla osataan tarvittaessa hyppiä ei kiinnostavien paikkojen yli. Nyt en halua tehdä joka threadille aina Clone:a näistä infoista.
// Mutta otan talteen tarvittavan bitmaksin, jos sellainen oli käytössä ja hypin sen avulla ohi ei-toivottujen pisteiden.
static const NFmiBitMask* GetUsedBitmask(boost::shared_ptr<NFmiFastQueryInfo> &theInfo, bool modifySelectedLocationsOnly)
{
	if(modifySelectedLocationsOnly)
	{
		NFmiSmartInfo *smartInfo = dynamic_cast<NFmiSmartInfo *>(theInfo.get());
		if(smartInfo)
		{
			return &(smartInfo->Mask(NFmiMetEditorTypes::kFmiSelectionMask));
		}
	}
	return 0; // ei ole maskia käytössä
}

// globaali asetus luokka for_each-funktioon
template<typename T>
class TimeSetter
{
public:
	TimeSetter(const NFmiMetTime &theTime):itsTime(theTime){}
	void operator()(boost::shared_ptr<T> &theMask){theMask->Time(itsTime);}

	NFmiMetTime itsTime;
};

static void DoPartialGridCalculationBlockInThread(NFmiLocationIndexRangeCalculator &theLocationIndexRangeCalculator, boost::shared_ptr<NFmiFastQueryInfo> &theInfo, boost::shared_ptr<NFmiSmartToolCalculationBlock> &theCalculationBlock, NFmiCalculationParams &theCalculationParams, const NFmiBitMask *theUsedBitmask)
{
	try
	{
		unsigned long startIndex = 0;
		unsigned long endIndex = 0;
		for( ; theLocationIndexRangeCalculator.GetCurrentLocationRange(startIndex, endIndex); )
		{
			for(unsigned long i = startIndex; i <= endIndex; i++)
			{
				if(theUsedBitmask == 0 || theUsedBitmask->IsMasked(i))
				{
					if(theInfo->LocationIndex(i))
					{
						theCalculationParams.itsLatlon = theInfo->LatLon();
						theCalculationParams.itsLocationIndex = theInfo->LocationIndex();
						// TUON LOCATIONINDEX jutun voisi kai poistaa, kun kyseistä optimointi juttua ei kai enää käytetä
						theCalculationBlock->Calculate_ver2(theCalculationParams, true);
					}
				}
			}
		}
	}
	catch(std::exception & /* e */ )
	{
//		int x = 0;
//		std::cerr << "Error in DoPartialGridCalculationBlockInThread: " << e.what() << std::endl;
	}
	catch(...)
	{
//		int x = 0;
//		std::cerr << "Unknown Error in DoPartialGridCalculationBlockInThread." << std::endl;
	}
}

static void DoPartialGridCalculationInThread(NFmiLocationIndexRangeCalculator &theLocationIndexRangeCalculator, boost::shared_ptr<NFmiFastQueryInfo> &theInfo, boost::shared_ptr<NFmiSmartToolCalculation> &theCalculation, NFmiCalculationParams &theCalculationParams, const NFmiBitMask *theUsedBitmask)
{
	try
	{
		unsigned long startIndex = 0;
		unsigned long endIndex = 0;
		for( ; theLocationIndexRangeCalculator.GetCurrentLocationRange(startIndex, endIndex); )
		{
			for(unsigned long i = startIndex; i <= endIndex; i++)
			{
				if(theUsedBitmask == 0 || theUsedBitmask->IsMasked(i))
				{
					if(theInfo->LocationIndex(i))
					{
						theCalculationParams.itsLatlon = theInfo->LatLon();
						theCalculationParams.itsLocationIndex = theInfo->LocationIndex();
						// TUON LOCATIONINDEX jutun voisi kai poistaa, kun kyseistä optimointi juttua ei kai enää käytetä
						theCalculation->Calculate_ver2(theCalculationParams);
					}
				}
			}
		}
	}
	catch(...)
	{
		// pakko ottaa vain vastaan poikkeukset, ei tehdä mitään
	}
}

void NFmiSmartToolModifier::ModifyConditionalData_ver2(const boost::shared_ptr<NFmiSmartToolCalculationBlock> &theCalculationBlock, NFmiThreadCallBacks *theThreadCallBacks)
{
	if(theCalculationBlock->itsIfAreaMaskSection && theCalculationBlock->itsIfCalculationBlocks)
	{
		if(theCalculationBlock->FirstVariableInfo() == 0)
			throw runtime_error(::GetDictionaryString("SmartToolModifierErrorUnknownProblem"));
		boost::shared_ptr<NFmiFastQueryInfo> info(dynamic_cast<NFmiFastQueryInfo*>(theCalculationBlock->FirstVariableInfo()->Clone()));
		if(info == 0)
			return ;

		try
		{
			info->LatLon(); // tämä on pyydettävä kerran multi-thread jutuissa, koska tämä rakentaa kaikille info-kopioille yhteisen latlon-cache:n
			NFmiCalculationParams calculationParams;
			SetInfosMaskType(info);
			NFmiTimeDescriptor modifiedTimes(itsModifiedTimes ? *itsModifiedTimes : info->TimeDescriptor());
			const NFmiBitMask *usedBitmask = ::GetUsedBitmask(info, fModifySelectedLocationsOnly);

			unsigned int usedThreadCount = boost::thread::hardware_concurrency();
			std::vector<boost::shared_ptr<NFmiFastQueryInfo> > infoVector;
			for(size_t i=0; i < usedThreadCount; i++)
				infoVector.push_back(NFmiAreaMask::DoShallowCopy(info));
			std::vector<boost::shared_ptr<NFmiSmartToolCalculationBlock> > calculationBlockVector; // tehdään joka coren säikeelle oma calculaatioBlokki kopio
			for(size_t i=0; i < usedThreadCount; i++)
				calculationBlockVector.push_back(boost::shared_ptr<NFmiSmartToolCalculationBlock>(new NFmiSmartToolCalculationBlock(*theCalculationBlock)));

			for(modifiedTimes.Reset(); modifiedTimes.Next(); )
			{
				if(info->Time(modifiedTimes.Time()))
				{
					NFmiQueryDataUtil::CheckIfStopped(theThreadCallBacks);
					NFmiQueryDataUtil::DoStepIt(theThreadCallBacks); // stepataan vasta 0-tarkastuksen jälkeen!
					calculationParams.itsTime = modifiedTimes.Time();
					calculationParams.itsTimeIndex = info->TimeIndex();
					theCalculationBlock->Time(calculationParams.itsTime);
					std::for_each(calculationBlockVector.begin(), calculationBlockVector.end(), TimeSetter<NFmiSmartToolCalculationBlock>(calculationParams.itsTime)); // calculaatioiden kopioiden ajat pitää myös asettaa
					std::for_each(infoVector.begin(), infoVector.end(), TimeSetter<NFmiFastQueryInfo>(calculationParams.itsTime)); // info kopioiden ajat pitää myös asettaa
					std::vector<NFmiCalculationParams> calculationParamsVector;
					for(size_t i=0; i < usedThreadCount; i++)
						calculationParamsVector.push_back(calculationParams); // tallentaa kopiot, missä on jo aika oikein
					NFmiLocationIndexRangeCalculator locationIndexRangeCalculator(info->SizeLocations(), 100);

					boost::thread_group calcParts;
					for(unsigned int threadIndex = 0; threadIndex < usedThreadCount; threadIndex++)
						calcParts.add_thread(new boost::thread(::DoPartialGridCalculationBlockInThread, boost::ref(locationIndexRangeCalculator), infoVector[threadIndex], calculationBlockVector[threadIndex], calculationParamsVector[threadIndex], usedBitmask));
					calcParts.join_all(); // odotetaan että threadit lopettavat
				}
			}
		}
		catch(...)
		{
			throw ;
		}
	}
}

static void DoSafeMaskOperation(const boost::shared_ptr<NFmiFastQueryInfo> &theInfo1, boost::shared_ptr<NFmiFastQueryInfo> &theInfo2)
{
	NFmiSmartInfo *info1 = dynamic_cast<NFmiSmartInfo*>(theInfo1.get());
	NFmiSmartInfo *info2 = dynamic_cast<NFmiSmartInfo*>(theInfo2.get());
	if(info1 && info2)
	{
		info1->Mask(info2->Mask(NFmiMetEditorTypes::kFmiSelectionMask), NFmiMetEditorTypes::kFmiSelectionMask);
	}
}

void NFmiSmartToolModifier::SetInfosMaskType(const boost::shared_ptr<NFmiFastQueryInfo> &theInfo)
{
	if(fModifySelectedLocationsOnly)
	{
		boost::shared_ptr<NFmiFastQueryInfo> editedInfo = itsInfoOrganizer->FindInfo(NFmiInfoData::kEditable);
		if(theInfo->DataType() == NFmiInfoData::kScriptVariableData && editedInfo)
		{ // skripti muuttujalle pitää asettaa sama valittujen pisteiden maski kuin on editoidulla datalla
			::DoSafeMaskOperation(theInfo, editedInfo);
		}
		theInfo->MaskType(NFmiMetEditorTypes::kFmiSelectionMask);
	}
	else
		theInfo->MaskType(NFmiMetEditorTypes::kFmiNoMask);
}

void NFmiSmartToolModifier::ModifyData2(boost::shared_ptr<NFmiSmartToolCalculationSection> &theCalculationSection, NFmiMacroParamValue &theMacroParamValue, NFmiThreadCallBacks *theThreadCallBacks)
{
	if(theCalculationSection && theCalculationSection->FirstVariableInfo())
	{
		boost::shared_ptr<NFmiFastQueryInfo> info(dynamic_cast<NFmiFastQueryInfo*>(theCalculationSection->FirstVariableInfo()->Clone()));
		if(info == 0)
			return ;
		try
		{
			NFmiCalculationParams calculationParams;
			SetInfosMaskType(info);
			NFmiTimeDescriptor modifiedTimes(itsModifiedTimes ? *itsModifiedTimes : info->TimeDescriptor());

			// Muutin lasku systeemin suoritusta, koska tuli ongelmia mm. muuttujien kanssa, kun niitä käytettiin samassa calculationSectionissa
			// CalculationSection = lasku rivejä peräkkäin esim.
			// T = T + 1
			// P = P + 1
			// jne. ilman IF-lauseita
			// ENNEN laskettiin tälläinen sectio siten että käytiin läpi koko sectio samalla paikalla ja ajalla ja sitten siirryttiin eteenpäin.
			// NYT lasketaan aina yksi laskurivi läpi kaikkien aikojen ja paikkojen, ja sitten siirrytään seuraavalle lasku riville.
			size_t size = theCalculationSection->GetCalculations().size();
			for(size_t i=0; i<size; i++)
			{
				for(modifiedTimes.Reset(); modifiedTimes.Next(); )
				{
					calculationParams.itsTime = modifiedTimes.Time();
					if(theMacroParamValue.fSetValue)
						calculationParams.itsTime = theMacroParamValue.itsTime;
					if(info->Time(calculationParams.itsTime)) // asetetaan myös tämä, että saadaan oikea timeindex
					{
						NFmiQueryDataUtil::CheckIfStopped(theThreadCallBacks);
						NFmiQueryDataUtil::DoStepIt(theThreadCallBacks); // stepataan vasta 0-tarkastuksen jälkeen!
						theCalculationSection->SetTime(calculationParams.itsTime); // yritetään optimoida laskuja hieman kun mahdollista
						for(info->ResetLocation(); info->NextLocation(); )
						{
							calculationParams.itsLatlon = info->LatLon();
							if(theMacroParamValue.fSetValue)
							{
								calculationParams.itsLatlon = theMacroParamValue.itsLatlon;
								info->Location(calculationParams.itsLatlon); // pitää laittaa nearestlocation päälle, että tuloksia voidaan myöhemmin hakea interpolaation avulla
							}
							calculationParams.itsLocationIndex = info->LocationIndex();
							// TUON LOCATIONINDEX jutun voisi kai poistaa, kun kyseistä optimointi juttua ei kai enää käytetä
							theCalculationSection->GetCalculations()[i]->Calculate(calculationParams, theMacroParamValue);

							if(theMacroParamValue.fSetValue)
								break;
						}
					}
					if(theMacroParamValue.fSetValue)
						break;
				}

			}

		}
		catch(...)
		{
			throw ;
		}
	}
}

void NFmiSmartToolModifier::ModifyData2_ver2(boost::shared_ptr<NFmiSmartToolCalculationSection> &theCalculationSection, NFmiThreadCallBacks *theThreadCallBacks)
{
	if(theCalculationSection && theCalculationSection->FirstVariableInfo())
	{
		boost::shared_ptr<NFmiFastQueryInfo> info(dynamic_cast<NFmiFastQueryInfo*>(theCalculationSection->FirstVariableInfo()->Clone()));
		if(info == 0)
			return ;
		try
		{
			info->LatLon(); // tämä on pyydettävä kerran multi-thread jutuissa, koska tämä rakentaa kaikille info-kopioille yhteisen latlon-cache:n
			NFmiCalculationParams calculationParams;
			SetInfosMaskType(info);
			NFmiTimeDescriptor modifiedTimes(itsModifiedTimes ? *itsModifiedTimes : info->TimeDescriptor());
			const NFmiBitMask *usedBitmask = ::GetUsedBitmask(info, fModifySelectedLocationsOnly);

            unsigned int usedThreadCount = boost::thread::hardware_concurrency();
			std::vector<boost::shared_ptr<NFmiFastQueryInfo> > infoVector;
			for(size_t i=0; i < usedThreadCount; i++)
				infoVector.push_back(NFmiAreaMask::DoShallowCopy(info));

			// Muutin lasku systeemin suoritusta, koska tuli ongelmia mm. muuttujien kanssa, kun niitä käytettiin samassa calculationSectionissa
			// CalculationSection = lasku rivejä peräkkäin esim.
			// T = T + 1
			// P = P + 1
			// jne. ilman IF-lauseita
			// ENNEN laskettiin tälläinen sectio siten että käytiin läpi koko sectio samalla paikalla ja ajalla ja sitten siirryttiin eteenpäin.
			// NYT lasketaan aina yksi laskurivi läpi kaikkien aikojen ja paikkojen, ja sitten siirrytään seuraavalle lasku riville.
			checkedVector<boost::shared_ptr<NFmiSmartToolCalculation> > &calculationVector = theCalculationSection->GetCalculations();
			for(size_t i=0; i < calculationVector.size(); i++)
			{
				boost::shared_ptr<NFmiSmartToolCalculation> smartToolCalculation = calculationVector[i];
				std::vector<boost::shared_ptr<NFmiSmartToolCalculation> > calculationVector; // tehdään joka coren säikeelle oma calculaatio kopio
				for(size_t i=0; i < usedThreadCount; i++)
					calculationVector.push_back(boost::shared_ptr<NFmiSmartToolCalculation>(new NFmiSmartToolCalculation(*smartToolCalculation)));

				for(modifiedTimes.Reset(); modifiedTimes.Next(); )
				{
					calculationParams.itsTime = modifiedTimes.Time();
					if(info->Time(calculationParams.itsTime)) // asetetaan myös tämä, että saadaan oikea timeindex
					{
						NFmiQueryDataUtil::CheckIfStopped(theThreadCallBacks);
						NFmiQueryDataUtil::DoStepIt(theThreadCallBacks); // stepataan vasta 0-tarkastuksen jälkeen!
						smartToolCalculation->Time(calculationParams.itsTime); // yritetään optimoida laskuja hieman kun mahdollista
						std::for_each(calculationVector.begin(), calculationVector.end(), TimeSetter<NFmiSmartToolCalculation>(calculationParams.itsTime)); // calculaatioiden kopioiden ajat pitää myös asettaa
						std::for_each(infoVector.begin(), infoVector.end(), TimeSetter<NFmiFastQueryInfo>(calculationParams.itsTime)); // info kopioiden ajat pitää myös asettaa
						std::vector<NFmiCalculationParams> calculationParamsVector;
						for(size_t i=0; i < usedThreadCount; i++)
							calculationParamsVector.push_back(calculationParams); // tallentaa kopiot, missä on jo aika oikein
						NFmiLocationIndexRangeCalculator locationIndexRangeCalculator(info->SizeLocations(), 100);

						boost::thread_group calcParts;
						for(unsigned int threadIndex = 0; threadIndex < usedThreadCount; threadIndex++)
							calcParts.add_thread(new boost::thread(::DoPartialGridCalculationInThread, boost::ref(locationIndexRangeCalculator), infoVector[threadIndex], calculationVector[threadIndex], calculationParamsVector[threadIndex], usedBitmask));
						calcParts.join_all(); // odotetaan että threadit lopettavat
					}
				}

			}

		}
		catch(...)
		{
			throw ;
		}
	}
}

boost::shared_ptr<NFmiAreaMask> NFmiSmartToolModifier::CreatePeekFunctionAreaMask(const NFmiAreaMaskInfo &theAreaMaskInfo, bool &fMustUsePressureInterpolation)
{
	boost::shared_ptr<NFmiAreaMask> areaMask;
	// HUOM!! Tähän vaaditaan syvä data kopio!!!
	// JOS kyseessä on ehtolauseen muuttujasta, joka on editoitavaa dataa.
	boost::shared_ptr<NFmiFastQueryInfo> info = CreateInfo(theAreaMaskInfo, fMustUsePressureInterpolation);
	if(theAreaMaskInfo.GetUseDefaultProducer())
	{ // Pitää tehdä syvä kopio datasta, että datan muuttuminen ei vaikuta laskuihin.
		boost::shared_ptr<NFmiFastQueryInfo> tmp(dynamic_cast<NFmiFastQueryInfo*>(info->Clone()));
		info = tmp;
	}
	boost::shared_ptr<NFmiFastQueryInfo> editedInfo = itsInfoOrganizer->FindInfo(NFmiInfoData::kEditable);
	NFmiAreaMask::CalculationOperationType maskType = theAreaMaskInfo.GetOperationType();
	if(maskType == NFmiAreaMask::FunctionPeekXY)
		areaMask = boost::shared_ptr<NFmiAreaMask>(new NFmiInfoAreaMaskPeekXY(theAreaMaskInfo.GetMaskCondition(), NFmiAreaMask::kInfo, theAreaMaskInfo.GetDataType(), info, static_cast<int>(theAreaMaskInfo.GetOffsetPoint1().X()), static_cast<int>(theAreaMaskInfo.GetOffsetPoint1().Y()), NFmiAreaMask::kNoValue));
	else if(maskType == NFmiAreaMask::FunctionPeekXY2)
		areaMask = boost::shared_ptr<NFmiAreaMask>(new NFmiInfoAreaMaskPeekXY2(theAreaMaskInfo.GetMaskCondition(), NFmiAreaMask::kInfo, theAreaMaskInfo.GetDataType(), info, this->fMacroParamCalculation ? UsedMacroParamData() : editedInfo, static_cast<int>(theAreaMaskInfo.GetOffsetPoint1().X()), static_cast<int>(theAreaMaskInfo.GetOffsetPoint1().Y()), NFmiAreaMask::kNoValue));
	else if(maskType == NFmiAreaMask::FunctionPeekXY3)
		areaMask = boost::shared_ptr<NFmiAreaMask>(new NFmiInfoAreaMaskPeekXY3(theAreaMaskInfo.GetMaskCondition(), NFmiAreaMask::kInfo, theAreaMaskInfo.GetDataType(), info, this->fMacroParamCalculation ? UsedMacroParamData() : editedInfo, theAreaMaskInfo.GetOffsetPoint1().X(), theAreaMaskInfo.GetOffsetPoint1().Y(), NFmiAreaMask::kNoValue));

	if(fUseLevelData)
		itsParethesisCounter++;

	return areaMask;
}

boost::shared_ptr<NFmiFastQueryInfo> NFmiSmartToolModifier::CreateInfo(const NFmiAreaMaskInfo &theAreaMaskInfo, bool &mustUsePressureInterpolation, unsigned long theWantedParamId)
{
	NFmiAreaMaskInfo wantedAreaMaskInfo(theAreaMaskInfo);
	NFmiDataIdent dataIdent = wantedAreaMaskInfo.GetDataIdent();
	dataIdent.GetParam()->SetIdent(theWantedParamId);
	wantedAreaMaskInfo.SetDataIdent(dataIdent);
	return CreateInfo(wantedAreaMaskInfo, mustUsePressureInterpolation);
}

void DoErrorExceptionForMetFunction(const NFmiAreaMaskInfo &theAreaMaskInfo, const std::string &theStartStr, const std::string &theMiddleStr)
{
	std::string errorStr(theStartStr);
	errorStr += " '";
	errorStr += theAreaMaskInfo.GetMaskText();
	errorStr += "' ";
	errorStr += theMiddleStr;
	errorStr += ":\n";
	errorStr += theAreaMaskInfo.GetOrigLineText();
	throw runtime_error(errorStr);
}

boost::shared_ptr<NFmiAreaMask> NFmiSmartToolModifier::CreateMetFunctionAreaMask(const NFmiAreaMaskInfo &theAreaMaskInfo, bool &fMustUsePressureInterpolation)
{
	boost::shared_ptr<NFmiAreaMask> areaMask;
	// HUOM!! Tähän vaaditaan syvä data kopio!!!
	// JOS kyseessä on ehtolauseen muuttujasta, joka on editoitavaa dataa.
	boost::shared_ptr<NFmiFastQueryInfo> info = CreateInfo(theAreaMaskInfo, fMustUsePressureInterpolation);
	if(info == 0)
		DoErrorExceptionForMetFunction(theAreaMaskInfo, ::GetDictionaryString("Can't find wanted parameter for given function"), ::GetDictionaryString("with line"));

	if(theAreaMaskInfo.GetUseDefaultProducer())
	{ // Pitää tehdä syvä kopio datasta, että datan muuttuminen ei vaikuta laskuihin.
		boost::shared_ptr<NFmiFastQueryInfo> tmp(dynamic_cast<NFmiFastQueryInfo*>(info->Clone()));
		info = tmp;
	}
	NFmiAreaMask::FunctionType funcType = theAreaMaskInfo.GetFunctionType();
	bool peekAlongTudes = false;
	if(funcType == NFmiAreaMask::Grad2 || funcType == NFmiAreaMask::Divergence2 || funcType == NFmiAreaMask::Adv2 || funcType == NFmiAreaMask::Lap2 || funcType == NFmiAreaMask::Rot2)
		peekAlongTudes = true;
	if(funcType == NFmiAreaMask::Grad || funcType == NFmiAreaMask::Grad2 || funcType == NFmiAreaMask::Divergence || funcType == NFmiAreaMask::Divergence2)
	{
		areaMask = boost::shared_ptr<NFmiAreaMask>(new NFmiInfoAreaMaskGrad(theAreaMaskInfo.GetMaskCondition(), NFmiAreaMask::kInfo, theAreaMaskInfo.GetDataType(), info, peekAlongTudes, theAreaMaskInfo.MetFunctionDirection()));
		if(funcType == NFmiAreaMask::Divergence)
			dynamic_cast<NFmiInfoAreaMaskGrad*>(areaMask.get())->CalculateDivergence(true);
	}
	else if(funcType == NFmiAreaMask::Adv || funcType == NFmiAreaMask::Adv2)
	{
		boost::shared_ptr<NFmiFastQueryInfo> infoUwind = CreateInfo(theAreaMaskInfo, fMustUsePressureInterpolation, kFmiWindUMS);
		boost::shared_ptr<NFmiFastQueryInfo> infoVwind = CreateInfo(theAreaMaskInfo, fMustUsePressureInterpolation, kFmiWindVMS);
		if(infoUwind && infoVwind)
			areaMask = boost::shared_ptr<NFmiAreaMask>(new NFmiInfoAreaMaskAdvection(theAreaMaskInfo.GetMaskCondition(), NFmiAreaMask::kInfo, theAreaMaskInfo.GetDataType(), info, infoUwind, infoVwind, peekAlongTudes, theAreaMaskInfo.MetFunctionDirection()));
		else
			DoErrorExceptionForMetFunction(theAreaMaskInfo, ::GetDictionaryString("Can't find u- or -v wind components for wanted parameter in given function"), ::GetDictionaryString("with line"));
	}
	else if(funcType == NFmiAreaMask::Lap || funcType == NFmiAreaMask::Lap2)
		areaMask = boost::shared_ptr<NFmiAreaMask>(new NFmiInfoAreaMaskLaplace(theAreaMaskInfo.GetMaskCondition(), NFmiAreaMask::kInfo, theAreaMaskInfo.GetDataType(), info, peekAlongTudes, theAreaMaskInfo.MetFunctionDirection()));
	else if(funcType == NFmiAreaMask::Rot || funcType == NFmiAreaMask::Rot2)
	{
		if(theAreaMaskInfo.GetDataIdent().GetParamIdent() == kFmiTotalWindMS)
			areaMask = boost::shared_ptr<NFmiAreaMask>(new NFmiInfoAreaMaskRotor(theAreaMaskInfo.GetMaskCondition(), NFmiAreaMask::kInfo, theAreaMaskInfo.GetDataType(), info, peekAlongTudes, theAreaMaskInfo.MetFunctionDirection()));
		else
			DoErrorExceptionForMetFunction(theAreaMaskInfo, ::GetDictionaryString("Only usable param with rot-function in wind (=par19)"), ::GetDictionaryString("in the line"));
	}
	else
		DoErrorExceptionForMetFunction(theAreaMaskInfo, ::GetDictionaryString("SmartMet program error with Met-function"), ::GetDictionaryString("error with line"));

	return areaMask;
}

// Jos areaMaskin info on havaittua luotausdataa, pitää tehdä leveliin liittyvä fiksaus.
void NFmiSmartToolModifier::MakeSoundingLevelFix(boost::shared_ptr<NFmiAreaMask> &theAreaMask, const NFmiAreaMaskInfo &theAreaMaskInfo)
{
    if(theAreaMask)
    {
	    if(theAreaMask->Info()->LevelType() == kFmiSoundingLevel) 
	    { // Luotaus data on poikkeus, jonka haluttu painepinta level pitää asettaa tässä erikseen.
		    // Lisäksi levelType pitää vaihtaa pressuresta kFmiSoundingLevel!
		    NFmiLevel soundingLevel(kFmiSoundingLevel, theAreaMaskInfo.GetLevel()->GetName(), theAreaMaskInfo.GetLevel()->LevelValue());
		    theAreaMask->Level(soundingLevel);
	    }
    }
}

//--------------------------------------------------------
// CreateAreaMask
//--------------------------------------------------------
// HUOM!! Ei osaa hoitaa kuin editoitavia datoja!!!!
boost::shared_ptr<NFmiAreaMask> NFmiSmartToolModifier::CreateAreaMask(const NFmiAreaMaskInfo &theAreaMaskInfo)
{
	NFmiAreaMask::CalculationOperationType maskType = theAreaMaskInfo.GetOperationType();
	boost::shared_ptr<NFmiAreaMask> areaMask;
	bool mustUsePressureInterpolation = false;

	switch(maskType)
	{
		case NFmiAreaMask::InfoVariable:
			{
			// HUOM!! Tähän vaaditaan syvä data kopio!!!
			// JOS kyseessä on ehtolauseen muuttujasta, joka on editoitavaa dataa.
			boost::shared_ptr<NFmiFastQueryInfo> info = CreateInfo(theAreaMaskInfo, mustUsePressureInterpolation);
			// HUOM!!! pitäisikö tähän laittaa joku debug-info raportti, jos level on reset-tilassa

			areaMask = boost::shared_ptr<NFmiAreaMask>(new NFmiInfoAreaMask(theAreaMaskInfo.GetMaskCondition(), NFmiAreaMask::kInfo, info->DataType(), info));
			break;
			}
		case NFmiAreaMask::RampFunction:
			{
			NFmiInfoData::Type type = theAreaMaskInfo.GetDataType();
			if(type != NFmiInfoData::kCalculatedValue)
			{
				boost::shared_ptr<NFmiFastQueryInfo> info = CreateInfo(theAreaMaskInfo, mustUsePressureInterpolation);
				areaMask = boost::shared_ptr<NFmiAreaMask>(new NFmiCalculationRampFuction(theAreaMaskInfo.GetMaskCondition(), NFmiAreaMask::kInfo, theAreaMaskInfo.GetDataType(), info));
			}
			else
			{
				boost::shared_ptr<NFmiAreaMask> areaMask2 = CreateCalculatedAreaMask(theAreaMaskInfo);
				areaMask = boost::shared_ptr<NFmiAreaMask>(new NFmiCalculationRampFuctionWithAreaMask(theAreaMaskInfo.GetMaskCondition(), NFmiAreaMask::kInfo, theAreaMaskInfo.GetDataType(), areaMask2));
			}
			if(fUseLevelData)
				itsParethesisCounter++;
			break;
			}
		case NFmiAreaMask::FunctionAreaIntergration:
		case NFmiAreaMask::FunctionTimeIntergration:
			{
			// HUOM!! Tähän vaaditaan syvä data kopio!!!
			// JOS kyseessä on ehtolauseen muuttujasta, joka on editoitavaa dataa.
			boost::shared_ptr<NFmiFastQueryInfo> info = CreateInfo(theAreaMaskInfo, mustUsePressureInterpolation);
			if(theAreaMaskInfo.GetUseDefaultProducer())
			{ // Pitää tehdä syvä kopio datasta, että datan muuttuminen ei vaikuta laskuihin.
				boost::shared_ptr<NFmiFastQueryInfo> tmp(dynamic_cast<NFmiFastQueryInfo*>(info->Clone()));
				info = tmp;
			}
/*
			boost::shared_ptr<NFmiDataModifier> modifier = CreateIntegrationFuction(theAreaMaskInfo);
			boost::shared_ptr<NFmiDataIterator> iterator = CreateIterator(theAreaMaskInfo, info);
			areaMask = boost::shared_ptr<NFmiAreaMask>(new NFmiCalculationIntegrationFuction(iterator, modifier, NFmiAreaMask::kInfo, theAreaMaskInfo.GetDataType(), info));
			areaMask->SetFunctionType(theAreaMaskInfo.GetFunctionType());
*/
			int startX = static_cast<int>(theAreaMaskInfo.GetOffsetPoint1().X());
			int startY = static_cast<int>(theAreaMaskInfo.GetOffsetPoint1().Y());
			int endX = static_cast<int>(theAreaMaskInfo.GetOffsetPoint2().X());
			int endY = static_cast<int>(theAreaMaskInfo.GetOffsetPoint2().Y());
			if(maskType == NFmiAreaMask::FunctionAreaIntergration)
				areaMask = boost::shared_ptr<NFmiAreaMask>(new NFmiInfoRectAreaIntegrator(theAreaMaskInfo.GetMaskCondition(), NFmiAreaMask::kInfo, theAreaMaskInfo.GetDataType(), info, theAreaMaskInfo.GetFunctionType(), startX, endX, startY, endY));
			else
				areaMask = boost::shared_ptr<NFmiAreaMask>(new NFmiInfoTimeIntegrator(theAreaMaskInfo.GetMaskCondition(), NFmiAreaMask::kInfo, theAreaMaskInfo.GetDataType(), info, theAreaMaskInfo.GetFunctionType(), startX, startY));
			if(fUseLevelData)
				itsParethesisCounter++;
			break;
			}
		case NFmiAreaMask::FunctionPeekXY:
		case NFmiAreaMask::FunctionPeekXY2:
		case NFmiAreaMask::FunctionPeekXY3:
			{
			areaMask = CreatePeekFunctionAreaMask(theAreaMaskInfo, mustUsePressureInterpolation);
			break;
			}
		case NFmiAreaMask::MetFunction:
			{
			areaMask = CreateMetFunctionAreaMask(theAreaMaskInfo, mustUsePressureInterpolation);
			break;
			}
		case NFmiAreaMask::CalculatedVariable:
			{
			areaMask = CreateCalculatedAreaMask(theAreaMaskInfo);
			break;
			}
		case NFmiAreaMask::Constant:
			{
			areaMask = boost::shared_ptr<NFmiAreaMask>(new NFmiCalculationConstantValue(theAreaMaskInfo.GetMaskCondition().LowerLimit()));
			break;
			}
		case NFmiAreaMask::Operator:
			{
			areaMask = boost::shared_ptr<NFmiAreaMask>(new NFmiCalculationSpecialCase(theAreaMaskInfo.GetCalculationOperator()));
			break;
			}
		case NFmiAreaMask::StartParenthesis:
			{
			areaMask = boost::shared_ptr<NFmiAreaMask>(new NFmiCalculationSpecialCase(theAreaMaskInfo.GetCalculationOperator()));
			if(fUseLevelData)
				itsParethesisCounter++;
			break;
			}
		case NFmiAreaMask::EndParenthesis:
			{
			areaMask = boost::shared_ptr<NFmiAreaMask>(new NFmiCalculationSpecialCase(theAreaMaskInfo.GetCalculationOperator()));
			if(fUseLevelData)
			{
				itsParethesisCounter--;
				if(itsParethesisCounter <= 0)
				{
					fHeightFunctionFlag = false;
					fUseLevelData = false;
				}
			}
			break;
			}
		case NFmiAreaMask::CommaOperator:
			{
			areaMask = boost::shared_ptr<NFmiAreaMask>(new NFmiCalculationSpecialCase(theAreaMaskInfo.GetCalculationOperator()));
			if(fHeightFunctionFlag)
			{
				itsCommaCounter++;
				if(itsCommaCounter >= 2)
				{  // kun pilkku-laskuri tuli täyteen
					fUseLevelData = true; // on aika ruveta käyttämään level-dataa infoissa
					itsParethesisCounter = 1; // lisäksi ruvetaan metsästämään sulkuja,
											  // että tiedetään milloin funktio ja level datan käyttö loppuu
				}
			}
			break;
			}
		case NFmiAreaMask::Comparison:
			{
			areaMask = boost::shared_ptr<NFmiAreaMask>(new NFmiCalculationSpecialCase());
			areaMask->Condition(theAreaMaskInfo.GetMaskCondition());
			break;
			}
		case NFmiAreaMask::BinaryOperatorType:
			{
			areaMask = boost::shared_ptr<NFmiAreaMask>(new NFmiCalculationSpecialCase());
			areaMask->PostBinaryOperator(theAreaMaskInfo.GetBinaryOperator());
			break;
			}
		case NFmiAreaMask::MathFunctionStart:
			{
			areaMask = boost::shared_ptr<NFmiAreaMask>(new NFmiCalculationSpecialCase());
			areaMask->SetMathFunctionType(theAreaMaskInfo.GetMathFunctionType());
			if(fUseLevelData)
				itsParethesisCounter++;
			break;
			}
		case NFmiAreaMask::ThreeArgumentFunctionStart:
			{
			areaMask = boost::shared_ptr<NFmiAreaMask>(new NFmiCalculationSpecialCase());
			areaMask->SetFunctionType(theAreaMaskInfo.GetFunctionType());
			areaMask->IntegrationFunctionType(theAreaMaskInfo.IntegrationFunctionType());
			if(theAreaMaskInfo.IntegrationFunctionType() == 2 || theAreaMaskInfo.IntegrationFunctionType() == 3)
			{ // jos funktio oli SumZ tai MinH tyyppinen, laitetaan seuraavat jutut 'päälle'
				fHeightFunctionFlag = true;
				fUseLevelData = false;
				itsCommaCounter = 0;
			}
			break;
			}
        case NFmiAreaMask::VertFunctionStart:
            {
                if(theAreaMaskInfo.GetSecondaryFunctionType() == NFmiAreaMask::TimeRange)
                {
                    boost::shared_ptr<NFmiFastQueryInfo> info = CreateInfo(theAreaMaskInfo, mustUsePressureInterpolation);
                    areaMask = boost::shared_ptr<NFmiAreaMask>(new NFmiInfoAreaMaskTimeRange(theAreaMaskInfo.GetMaskCondition(), NFmiAreaMask::kInfo, info->DataType(), info, theAreaMaskInfo.GetFunctionType(), theAreaMaskInfo.FunctionArgumentCount()));
                }
                else if(theAreaMaskInfo.GetSecondaryFunctionType() == NFmiAreaMask::ProbRect || theAreaMaskInfo.GetSecondaryFunctionType() == NFmiAreaMask::ProbCircle)
                {
                    boost::shared_ptr<NFmiFastQueryInfo> info = CreateInfo(theAreaMaskInfo, mustUsePressureInterpolation);
                    areaMask = boost::shared_ptr<NFmiAreaMask>(new NFmiInfoAreaMaskProbFunc(theAreaMaskInfo.GetMaskCondition(), NFmiAreaMask::kInfo, info->DataType(), info, theAreaMaskInfo.GetFunctionType(), theAreaMaskInfo.GetSecondaryFunctionType(), theAreaMaskInfo.FunctionArgumentCount()));
                }
#ifdef FMI_SUPPORT_STATION_DATA_SMARTTOOL
                else if(theAreaMaskInfo.GetSecondaryFunctionType() == NFmiAreaMask::ClosestObsValue)
			    {
                    boost::shared_ptr<NFmiFastQueryInfo> info = CreateInfo(theAreaMaskInfo, mustUsePressureInterpolation);
            		if(info->IsGrid())
                        throw std::runtime_error("With closestvalue -function you must choose observation (station) data producer, not data with grid.");
                    NFmiNearestObsValue2GridMask *nearestObsValue2GridMask = new NFmiNearestObsValue2GridMask(NFmiAreaMask::kInfo, info->DataType(), info, theAreaMaskInfo.GetFunctionType(), theAreaMaskInfo.GetSecondaryFunctionType(), theAreaMaskInfo.FunctionArgumentCount());
				    nearestObsValue2GridMask->SetGriddingHelpers(itsWorkingGrid->itsArea, itsDoc, NFmiPoint(itsWorkingGrid->itsNX, itsWorkingGrid->itsNY));
				    areaMask = boost::shared_ptr<NFmiAreaMask>(nearestObsValue2GridMask);
                    MakeSoundingLevelFix(areaMask, theAreaMaskInfo);
			    }
#endif // FMI_SUPPORT_STATION_DATA_SMARTTOOL

                else
                {
                    NFmiAreaMask::FunctionType secondaryFunc = theAreaMaskInfo.GetSecondaryFunctionType();
                    fUseLevelData = true;
                    boost::shared_ptr<NFmiFastQueryInfo> info = CreateInfo(theAreaMaskInfo, mustUsePressureInterpolation);
                    if(secondaryFunc == NFmiAreaMask::TimeVertP || secondaryFunc == NFmiAreaMask::TimeVertFL || secondaryFunc == NFmiAreaMask::TimeVertZ || secondaryFunc == NFmiAreaMask::TimeVertHyb)
                        areaMask = boost::shared_ptr<NFmiAreaMask>(new NFmiInfoAreaMaskTimeVertFunc(theAreaMaskInfo.GetMaskCondition(), NFmiAreaMask::kInfo, info->DataType(), info, theAreaMaskInfo.GetFunctionType(), theAreaMaskInfo.GetSecondaryFunctionType(), theAreaMaskInfo.FunctionArgumentCount()));
                    else
                        areaMask = boost::shared_ptr<NFmiAreaMask>(new NFmiInfoAreaMaskVertFunc(theAreaMaskInfo.GetMaskCondition(), NFmiAreaMask::kInfo, info->DataType(), info, theAreaMaskInfo.GetFunctionType(), theAreaMaskInfo.GetSecondaryFunctionType(), theAreaMaskInfo.FunctionArgumentCount()));
                    fUseLevelData = false; // en tiedä pitääkö tämä laittaa takaisin falseksi, mutta laitan varmuuden vuoksi
                }
                break;
            }
		case NFmiAreaMask::DeltaZFunction:
			{
			areaMask = boost::shared_ptr<NFmiAreaMask>(new NFmiCalculationDeltaZValue());
			break;
			}
		case NFmiAreaMask::SoundingIndexFunction:
			{
			areaMask = CreateSoundingIndexFunctionAreaMask(theAreaMaskInfo);
			break;
			}
		default:
			throw runtime_error(::GetDictionaryString("SmartToolModifierErrorStrangeDataType"));
	}

	if(areaMask)
	{
		if(areaMask->Info() && areaMask->Info()->Grid() == 0)
		{ // jos oli info dataa ja vielä asemadatasta, tarkistetaan että kyse oli vielä infoData-tyypistä, muuten oli virheellinen lauseke
#ifdef FMI_SUPPORT_STATION_DATA_SMARTTOOL
            if(theAreaMaskInfo.GetSecondaryFunctionType() == NFmiAreaMask::ClosestObsValue)
            { // tämä on ok, ei tarvitse tehdä mitään
            }
			else if(maskType == NFmiAreaMask::InfoVariable)
			{
				boost::shared_ptr<NFmiFastQueryInfo> info = areaMask->Info();
				NFmiStation2GridMask *station2GridMask = new NFmiStation2GridMask(areaMask->MaskType(), areaMask->GetDataType(), info);
				station2GridMask->SetGriddingHelpers(itsWorkingGrid->itsArea, itsDoc, NFmiPoint(itsWorkingGrid->itsNX, itsWorkingGrid->itsNY));
				areaMask = boost::shared_ptr<NFmiAreaMask>(station2GridMask);
                MakeSoundingLevelFix(areaMask, theAreaMaskInfo);
			}
			else
#endif // FMI_SUPPORT_STATION_DATA_SMARTTOOL
			{
				std::string errStr;
				errStr += ::GetDictionaryString("Trying to use unsupported smarttool function with station (non-grid) data.\n'");
				errStr += theAreaMaskInfo.GetMaskText();
				errStr += ::GetDictionaryString("' ");
				errStr += ::GetDictionaryString("in line:");
				errStr += ::GetDictionaryString("\n");
				errStr += theAreaMaskInfo.GetOrigLineText();
				throw std::runtime_error(errStr);
			}
		}

		areaMask->Initialize(); // virtuaalinen initialisointi konstruktion jälkeen
		areaMask->SetCalculationOperationType(maskType);
		if(mustUsePressureInterpolation)
		{
			areaMask->UsePressureLevelInterpolation(true);
			areaMask->UsedPressureLevelValue(theAreaMaskInfo.GetLevel()->LevelValue());
			if(theAreaMaskInfo.GetLevel()->LevelType() == kFmiFlightLevel)
				const_cast<NFmiLevel*>(areaMask->Level())->SetIdent(static_cast<unsigned long>(kFmiFlightLevel));
		}
	}
	return areaMask;
}

boost::shared_ptr<NFmiAreaMask>  NFmiSmartToolModifier::CreateSoundingIndexFunctionAreaMask(const NFmiAreaMaskInfo &theAreaMaskInfo)
{
	// HUOM!! Tähän vaaditaan syvä data kopio!!!
	// JOS kyseessä on ehtolauseen muuttujasta, joka on editoitavaa dataa.
	bool mustUsePressureInterpolation = false; // tätäei käytetä tässä, mutta pakko laittaa metodin interfacen takia
	boost::shared_ptr<NFmiFastQueryInfo> info = CreateInfo(theAreaMaskInfo, mustUsePressureInterpolation);
	if(theAreaMaskInfo.GetUseDefaultProducer())
	{ // Pitää tehdä syvä kopio datasta, että datan muuttuminen ei vaikuta laskuihin.
		boost::shared_ptr<NFmiFastQueryInfo> tmp(dynamic_cast<NFmiFastQueryInfo*>(info->Clone()));
		info = tmp;
	}
	boost::shared_ptr<NFmiAreaMask> areaMask(new NFmiInfoAreaMaskSoundingIndex(info, theAreaMaskInfo.SoundingParameter()));
	return areaMask;
}

boost::shared_ptr<NFmiAreaMask> NFmiSmartToolModifier::CreateCalculatedAreaMask(const NFmiAreaMaskInfo &theAreaMaskInfo)
{
	boost::shared_ptr<NFmiAreaMask> areaMask;
	FmiParameterName parId = FmiParameterName(theAreaMaskInfo.GetDataIdent().GetParamIdent());
	if(parId == kFmiLatitude || parId == kFmiLongitude)
		areaMask = boost::shared_ptr<NFmiAreaMask>(new NFmiLatLonAreaMask(theAreaMaskInfo.GetDataIdent(), theAreaMaskInfo.GetMaskCondition()));
	else if(parId == kFmiElevationAngle)
		areaMask = boost::shared_ptr<NFmiAreaMask>(new NFmiElevationAngleAreaMask(theAreaMaskInfo.GetDataIdent(), theAreaMaskInfo.GetMaskCondition()));
	else if(parId == kFmiDay)
		areaMask = boost::shared_ptr<NFmiAreaMask>(new NFmiJulianDayAreaMask(theAreaMaskInfo.GetDataIdent(), theAreaMaskInfo.GetMaskCondition()));
	else if(parId == kFmiHour)
		areaMask = boost::shared_ptr<NFmiAreaMask>(new NFmiLocalHourAreaMask(theAreaMaskInfo.GetDataIdent(), theAreaMaskInfo.GetMaskCondition()));
	else if(parId == kFmiSecond)
		areaMask = boost::shared_ptr<NFmiAreaMask>(new NFmiUtcHourAreaMask(theAreaMaskInfo.GetDataIdent(), theAreaMaskInfo.GetMaskCondition()));
	else if(parId == kFmiForecastPeriod)
		areaMask = boost::shared_ptr<NFmiAreaMask>(new NFmiForecastHourAreaMask(itsInfoOrganizer->FindInfo(NFmiInfoData::kEditable), theAreaMaskInfo.GetDataIdent(), theAreaMaskInfo.GetMaskCondition()));
	else if(parId == kFmiDeltaTime)
		areaMask = boost::shared_ptr<NFmiAreaMask>(new NFmiTimeStepAreaMask(itsInfoOrganizer->FindInfo(NFmiInfoData::kEditable), theAreaMaskInfo.GetDataIdent(), theAreaMaskInfo.GetMaskCondition()));
	else if(parId == kFmiLastParameter)
		areaMask = boost::shared_ptr<NFmiAreaMask>(new NFmiGridSizeAreaMask(this->fMacroParamCalculation ? UsedMacroParamData() : itsInfoOrganizer->FindInfo(NFmiInfoData::kEditable), theAreaMaskInfo.GetDataIdent(), theAreaMaskInfo.GetMaskCondition(), true));
	else if(parId == kFmiLastParameter+1)
		areaMask = boost::shared_ptr<NFmiAreaMask>(new NFmiGridSizeAreaMask(this->fMacroParamCalculation ? UsedMacroParamData() : itsInfoOrganizer->FindInfo(NFmiInfoData::kEditable), theAreaMaskInfo.GetDataIdent(), theAreaMaskInfo.GetMaskCondition(), false));


	if(areaMask)
		return areaMask;

	throw runtime_error(::GetDictionaryString("SmartToolModifierErrorStrangeVariable"));
}

// Muista jos tulee päivityksiä, smanlainen funktio löytyy myös NFmiSmartToolCalculation-luokasta
boost::shared_ptr<NFmiDataModifier> NFmiSmartToolModifier::CreateIntegrationFuction(const NFmiAreaMaskInfo &theAreaMaskInfo)
{
	boost::shared_ptr<NFmiDataModifier> modifier;
	NFmiAreaMask::FunctionType func = theAreaMaskInfo.GetFunctionType();
	switch(func)
	{
	case NFmiAreaMask::Avg:
		modifier = boost::shared_ptr<NFmiDataModifier>(new NFmiDataModifierAvg());
		break;
	case NFmiAreaMask::Min:
		modifier = boost::shared_ptr<NFmiDataModifier>(new NFmiDataModifierMin());
		break;
	case NFmiAreaMask::Max:
		modifier = boost::shared_ptr<NFmiDataModifier>(new NFmiDataModifierMax());
		break;
	case NFmiAreaMask::Sum:
		modifier = boost::shared_ptr<NFmiDataModifier>(new NFmiDataModifierSum());
		break;
		// HUOM!!!! Tee WAvg-modifier myös, joka on peritty Avg-modifieristä ja tee joku kerroin juttu painotukseen.
	default:
		throw runtime_error(::GetDictionaryString("SmartToolModifierErrorStrangeIntegrationFunction"));
	}
	return modifier;
}

boost::shared_ptr<NFmiDataIterator> NFmiSmartToolModifier::CreateIterator(const NFmiAreaMaskInfo &theAreaMaskInfo, const boost::shared_ptr<NFmiFastQueryInfo> &theInfo)
{
	boost::shared_ptr<NFmiDataIterator> iterator;
	NFmiAreaMask::CalculationOperationType mType = theAreaMaskInfo.GetOperationType();
	switch(mType)
	{
		case NFmiAreaMask::FunctionAreaIntergration:
			// HUOM!! NFmiRelativeDataIterator:iin pitää tehdä joustavampi 'laatikon' säätö systeemi, että laatikko ei olisi aina keskitetty
			iterator = boost::shared_ptr<NFmiDataIterator>(new NFmiRelativeDataIterator(theInfo.get(),
													static_cast<long>(theAreaMaskInfo.GetOffsetPoint1().X()),
													static_cast<long>(theAreaMaskInfo.GetOffsetPoint1().Y()),
													0,
													static_cast<long>(theAreaMaskInfo.GetOffsetPoint2().X()),
													static_cast<long>(theAreaMaskInfo.GetOffsetPoint2().Y()),
													0));
			break;
		case NFmiAreaMask::FunctionTimeIntergration:
			{
				NFmiPoint p(theAreaMaskInfo.GetOffsetPoint1());
				iterator = boost::shared_ptr<NFmiDataIterator>(new NFmiRelativeTimeIntegrationIterator2(theInfo.get(),
																   static_cast<int>(p.Y() - p.X() + 1),
																   static_cast<int>(p.Y())));
				break;
			}
	default:
		throw runtime_error(::GetDictionaryString("SmartToolModifierErrorStrangeIteratorType"));
	}
	return iterator;
}

boost::shared_ptr<NFmiAreaMask> NFmiSmartToolModifier::CreateEndingAreaMask(void)
{
	boost::shared_ptr<NFmiAreaMask> areaMask(new NFmiCalculationSpecialCase());
	areaMask->SetCalculationOperationType(NFmiAreaMask::EndOfOperations);
	return areaMask;
}

static bool IsBetweenValues(double value, double value1, double value2)
{
	if(value >= value1 && value <= value2)
		return true;
	if(value >= value2 && value <= value1)
		return true;
	return false;
}

boost::shared_ptr<NFmiFastQueryInfo> NFmiSmartToolModifier::GetPossibleLevelInterpolatedInfo(const NFmiAreaMaskInfo &theAreaMaskInfo, bool &mustUsePressureInterpolation)
{
	boost::shared_ptr<NFmiFastQueryInfo> info;
	boost::shared_ptr<NFmiFastQueryInfo> possiblePressureLevelDataInfo;
	if(theAreaMaskInfo.GetLevel() != 0 && theAreaMaskInfo.GetLevel()->LevelType() != kFmiHybridLevel)
	{
		bool flightLevelWanted = theAreaMaskInfo.GetLevel()->LevelType() == kFmiFlightLevel;
		checkedVector<boost::shared_ptr<NFmiFastQueryInfo> > infoVector = itsInfoOrganizer->GetInfos(theAreaMaskInfo.GetDataIdent().GetProducer()->GetIdent());
		for(size_t i = 0; i<infoVector.size(); i++)
		{
			boost::shared_ptr<NFmiFastQueryInfo> tmpInfo = infoVector[i];
			if(flightLevelWanted ? (tmpInfo->HeightValueAvailable()) : (tmpInfo->PressureDataAvailable()))
			{
				if(tmpInfo->Param(static_cast<FmiParameterName>(theAreaMaskInfo.GetDataIdent().GetParamIdent())))
				{
					tmpInfo->FirstLevel();
					if(tmpInfo->Level()->GetIdent() == kFmiHybridLevel)
					{ // lähdetään tässä siitä että jos löytyy mallipinta-dataa, mikä sopii tarkoitukseen, se valitaan ensisijaisesti
						info = tmpInfo;
						break;
					}
					else if(tmpInfo->Level()->GetIdent() == kFmiPressureLevel)
					{
						if(flightLevelWanted)
							possiblePressureLevelDataInfo = tmpInfo;
						else
						{
							double levelValue1 = tmpInfo->Level()->LevelValue();
							tmpInfo->LastLevel();
							double levelValue2 = tmpInfo->Level()->LevelValue();
							if(::IsBetweenValues(theAreaMaskInfo.GetLevel()->LevelValue(), levelValue1, levelValue2))
							{
								possiblePressureLevelDataInfo = tmpInfo;
							}
						}
					}
				}
			}
		}
	}
	if(info == 0 && possiblePressureLevelDataInfo != 0) // jos ei löytynyt sopivaa mallipinta-dataa, mutta painepinta-dataa löytyi, otetaan se käyttöön
		info = possiblePressureLevelDataInfo;
	if(info)
	{
		// Tarkistetaan vielä haluttiinko vanhaa malliajodataa
		if(theAreaMaskInfo.ModelRunIndex() < 0)
		{
			checkedVector<boost::shared_ptr<NFmiFastQueryInfo> > infos = itsInfoOrganizer->GetInfos(info->DataFilePattern());
			if(infos.size())
				info = infos[0];
		}

		info = ::CreateShallowCopyOfHighestInfo(info);
		mustUsePressureInterpolation = true;
	}
	return info;
}

// tämä funktio on tehty siksi että voidaan hanskata z-parametri (poikkeus) yhtenäisellä tavalla
boost::shared_ptr<NFmiFastQueryInfo> NFmiSmartToolModifier::GetInfoFromOrganizer(const NFmiDataIdent& theIdent, const NFmiLevel* theLevel, NFmiInfoData::Type theType, bool fUseParIdOnly, bool fLevelData, int theModelRunIndex)
{
	boost::shared_ptr<NFmiFastQueryInfo> info = itsInfoOrganizer->Info(theIdent, theLevel, theType, fUseParIdOnly, fLevelData, theModelRunIndex);
	if(info == 0)
	{
		std::string parName(theIdent.GetParamName());
		NFmiStringTools::LowerCase(parName);
		if(parName == "z")
		{ // z-parametri on poikkeus (siis smarttool-kielen tekstinä annettu "z" -parametri), eli tätä yritetään hakea sekä kFmiGeomHeight:ista (id=3), joka on default, ja kFmiGeopHeight:ista (id=2)
			NFmiParam secondaryParam(*theIdent.GetParam());
			secondaryParam.SetIdent(kFmiGeopHeight);
			NFmiDataIdent secondaryDataIdent(secondaryParam, *theIdent.GetProducer());
			info = itsInfoOrganizer->Info(secondaryDataIdent, theLevel, theType, fUseParIdOnly, fLevelData, theModelRunIndex);
		}
	}
	return info;
}

boost::shared_ptr<NFmiFastQueryInfo> NFmiSmartToolModifier::CreateCopyOfAnalyzeInfo(const NFmiDataIdent& theDataIdent, const NFmiLevel* theLevel)
{
	boost::shared_ptr<NFmiFastQueryInfo> info = GetInfoFromOrganizer(theDataIdent, theLevel, NFmiInfoData::kAnalyzeData);
	if(info)
	{
		if(info->Param(static_cast<FmiParameterName>(theDataIdent.GetParamIdent())) && (theLevel == 0 || info->Level(*theLevel)))
			return ::CreateShallowCopyOfHighestInfo(info);
	}
	return info;
}

boost::shared_ptr<NFmiFastQueryInfo> NFmiSmartToolModifier::GetWantedAreaMaskData(const NFmiAreaMaskInfo &theAreaMaskInfo, bool fUseParIdOnly, NFmiInfoData::Type theOverRideDataType, FmiLevelType theOverRideLevelType)
{
	NFmiInfoData::Type usedDataType = theAreaMaskInfo.GetDataType();
	if(theOverRideDataType != NFmiInfoData::kNoDataType)
		usedDataType = theOverRideDataType;

	boost::shared_ptr<NFmiFastQueryInfo> info;
	if(theOverRideLevelType == kFmiNoLevelType)
		info = GetInfoFromOrganizer(theAreaMaskInfo.GetDataIdent(), theAreaMaskInfo.GetLevel(), usedDataType, fUseParIdOnly, fUseLevelData | fDoCrossSectionCalculation, theAreaMaskInfo.ModelRunIndex());
	else
	{
		if(theAreaMaskInfo.GetLevel()) // level voi olla 0-pointteri, joten se pitää tarkistaa
		{
			NFmiLevel aLevel(*theAreaMaskInfo.GetLevel());
			aLevel.SetIdent(theOverRideLevelType);
			info = GetInfoFromOrganizer(theAreaMaskInfo.GetDataIdent(), &aLevel, usedDataType, fUseParIdOnly, fUseLevelData, theAreaMaskInfo.ModelRunIndex());
		}
	}
	return ::CreateShallowCopyOfHighestInfo(info); // tehdään vielä 'kevyt' kopio löytyneestä datasta
}

boost::shared_ptr<NFmiFastQueryInfo> NFmiSmartToolModifier::CreateInfo(const NFmiAreaMaskInfo &theAreaMaskInfo, bool &mustUsePressureInterpolation)
{
	mustUsePressureInterpolation = false;
	boost::shared_ptr<NFmiFastQueryInfo> info;
	if(theAreaMaskInfo.GetDataType() == NFmiInfoData::kScriptVariableData)
		info = CreateScriptVariableInfo(theAreaMaskInfo.GetDataIdent());
	else if(theAreaMaskInfo.GetUseDefaultProducer() || theAreaMaskInfo.GetDataType() == NFmiInfoData::kCopyOfEdited)
	{
		if(theAreaMaskInfo.GetDataType() == NFmiInfoData::kMacroParam)
		{ // tämä macroParam data viritys on multi threaddaavaa serveriä varten, eli macroparam data pitää olla thread-kohtainen
			// ja se on aina annettu luodulle NFmiSmartToolModifier-luokan instansille erikseen.
			if(UsedMacroParamData())
				info = ::CreateShallowCopyOfHighestInfo(UsedMacroParamData());
			else
				throw runtime_error("NFmiSmartToolModifier::CreateInfo - error in program, no macroParam data available.");
		}
		else
                {
			info = GetWantedAreaMaskData(theAreaMaskInfo, true);
			if(info && itsModifiedLevel)
				info->Level(*itsModifiedLevel);
                }
		if(info == 0)
			info = GetPossibleLevelInterpolatedInfo(theAreaMaskInfo, mustUsePressureInterpolation);
	}
	else
	{
		if(fUseLevelData && theAreaMaskInfo.GetLevel() != 0) // jos pitää käyttää level dataa (SumZ ja MinH funktiot), ei saa antaa level infoa parametrin yhteydessä
			throw runtime_error(::GetDictionaryString("SmartToolModifierErrorParamNoLevel") + "\n" + theAreaMaskInfo.GetMaskText());
		if(fUseLevelData || fDoCrossSectionCalculation) // jos leveldata-flagi päällä, yritetään ensin, löytyykö hybridi dataa
			info = GetWantedAreaMaskData(theAreaMaskInfo, false, NFmiInfoData::kHybridData);
		if(info == 0)
			info = GetWantedAreaMaskData(theAreaMaskInfo, false);
		if(info == 0 && theAreaMaskInfo.GetDataType() == NFmiInfoData::kAnalyzeData) // analyysi datalle piti tehdä pika viritys tähän
			info = CreateCopyOfAnalyzeInfo(theAreaMaskInfo.GetDataIdent(), theAreaMaskInfo.GetLevel());
		if(info == 0)
			info = GetPossibleLevelInterpolatedInfo(theAreaMaskInfo, mustUsePressureInterpolation);
		if(info == 0 && theAreaMaskInfo.GetLevel() != 0) // kokeillaan vielä jos halutaan hybridi datan leveliä
			info = GetWantedAreaMaskData(theAreaMaskInfo, false, NFmiInfoData::kHybridData, kFmiHybridLevel);
		if(info == 0 && theAreaMaskInfo.GetLevel() != 0) // kokeillaan vielä jos halutaan 'height' (type 105) datan leveliä
			info = GetWantedAreaMaskData(theAreaMaskInfo, false, NFmiInfoData::kViewable, kFmiHeight);

		if(info == 0) // kokeillaan vielä model-help-dataa (esim. EPS-data, sounding-index-data jne)
			info = GetWantedAreaMaskData(theAreaMaskInfo, false, NFmiInfoData::kModelHelpData);
		if(info == 0) // kokeillaan vielä analyysi-dataa (esim. mesan, LAPS jne) HUOM! tämä on eri asia kuin edella oli analyysi-dataa. tässä on pyydetty PAR_PRODid yhdistelmällä analyysi-dataa
			info = GetWantedAreaMaskData(theAreaMaskInfo, false, NFmiInfoData::kAnalyzeData);
		if(info == 0) // kokeillaan vielä fraktiili-dataa (esim. EC:n fraktiili dataa jne)
			info = GetWantedAreaMaskData(theAreaMaskInfo, false, NFmiInfoData::kClimatologyData);

#ifdef FMI_SUPPORT_STATION_DATA_SMARTTOOL
		if(info == 0) // kokeillaan vielä havainto dataa (eli ne on yleensä asemadataa)
			info = GetWantedAreaMaskData(theAreaMaskInfo, false, NFmiInfoData::kObservations);
		if(info == 0) // kokeillaan vielä eri level vaihtoehtoja
			info = GetWantedAreaMaskData(theAreaMaskInfo, false, NFmiInfoData::kObservations, kFmiHeight);
		if(info == 0) // kokeillaan vielä luotaus datan leveltyyppi
			info = GetWantedAreaMaskData(theAreaMaskInfo, false, NFmiInfoData::kObservations, kFmiSoundingLevel);
		if(info == 0) // kokeillaan vielä yksittäisten tutkien dataa level moodissa
			info = GetWantedAreaMaskData(theAreaMaskInfo, false, NFmiInfoData::kSingleStationRadarData, kFmiPressureLevel);
		if(info == 0) // kokeillaan vielä yksittäisten tutkien dataa ilman level moodia (esim. vertikaali-funktioiden yhteydessä)
			info = GetWantedAreaMaskData(theAreaMaskInfo, false, NFmiInfoData::kSingleStationRadarData);

#endif // FMI_SUPPORT_STATION_DATA_SMARTTOOL


	}
	if(!info)
		throw runtime_error(::GetDictionaryString("SmartToolModifierErrorParamNotFound") + "\n" + theAreaMaskInfo.GetMaskText());
	info->LatLon(); // tämä on varmistus uusia multi-thread laskuja varten, että jokaisella infolla on olemassa latlon-cache, ennen kuin mennään eri säikeisiin hommiin
	return info;
}

void NFmiSmartToolModifier::GetParamValueLimits(const NFmiAreaMaskInfo &theAreaMaskInfo, float *theLowerLimit, float *theUpperLimit, bool *fCheckLimits)
{
    FmiParameterName parName = static_cast<FmiParameterName>(theAreaMaskInfo.GetDataIdent().GetParamIdent());
    // yhdistelmä parametreille ei ole rajoja, samoin par-id kFmiBadParameter, koska silloin on kyse erikoisfunktioista kuten grad tms.
    if(parName == kFmiTotalWindMS || parName == kFmiWeatherAndCloudiness || parName == kFmiBadParameter)
		*fCheckLimits = false;
	else
	{
		boost::shared_ptr<NFmiDrawParam> drawParam = itsInfoOrganizer->CreateDrawParam(theAreaMaskInfo.GetDataIdent(), theAreaMaskInfo.GetLevel(), theAreaMaskInfo.GetDataType());
		if(drawParam)
		{
			*theLowerLimit = static_cast<float>(drawParam->AbsoluteMinValue());
			*theUpperLimit = static_cast<float>(drawParam->AbsoluteMaxValue());
		}
	}
}

struct FindScriptVariable
{
	FindScriptVariable(int theParId):itsParId(theParId){}

	bool operator()(boost::shared_ptr<NFmiFastQueryInfo> &thePtr)
	{return itsParId == static_cast<int>(thePtr->Param().GetParamIdent());}

	int itsParId;
};

boost::shared_ptr<NFmiFastQueryInfo> NFmiSmartToolModifier::CreateScriptVariableInfo(const NFmiDataIdent &theDataIdent)
{
	boost::shared_ptr<NFmiFastQueryInfo> tmp = GetScriptVariableInfo(theDataIdent);
	if(tmp)
		return ::CreateShallowCopyOfHighestInfo(tmp);
	else // pitää vielä luoda kyseinen skripti-muuttuja, koska sitä käytetään nyt 1. kertaa
	{
		boost::shared_ptr<NFmiFastQueryInfo> tmp2 = CreateRealScriptVariableInfo(theDataIdent);
		if(tmp2)
		{
			itsScriptVariableInfos.push_back(tmp2);
			tmp = GetScriptVariableInfo(theDataIdent);
			if(tmp)
				return ::CreateShallowCopyOfHighestInfo(tmp);
		}
	}

	throw runtime_error(::GetDictionaryString("SmartToolModifierErrorStrange1") + " " + string(theDataIdent.GetParamName()) + " " + ::GetDictionaryString("SmartToolModifierErrorStrange2"));
}

boost::shared_ptr<NFmiFastQueryInfo> NFmiSmartToolModifier::GetScriptVariableInfo(const NFmiDataIdent &theDataIdent)
{
	checkedVector<boost::shared_ptr<NFmiFastQueryInfo> >::iterator it = std::find_if(itsScriptVariableInfos.begin(), itsScriptVariableInfos.end(), FindScriptVariable(theDataIdent.GetParamIdent()));
	if(it != itsScriptVariableInfos.end())
		return *it;
	return boost::shared_ptr<NFmiFastQueryInfo>();
}


void NFmiSmartToolModifier::ClearScriptVariableInfos(void)
{
	itsScriptVariableInfos.clear();
}

boost::shared_ptr<NFmiFastQueryInfo> NFmiSmartToolModifier::CreateRealScriptVariableInfo(const NFmiDataIdent &theDataIdent)
{
	boost::shared_ptr<NFmiFastQueryInfo> baseInfo = fMacroParamCalculation ? UsedMacroParamData() : itsInfoOrganizer->FindInfo(NFmiInfoData::kEditable);
	NFmiParamBag paramBag;
	paramBag.Add(theDataIdent);
	NFmiParamDescriptor paramDesc(paramBag);
	NFmiQueryInfo innerInfo(paramDesc, itsModifiedTimes ? *itsModifiedTimes : baseInfo->TimeDescriptor(), baseInfo->HPlaceDescriptor(), baseInfo->VPlaceDescriptor());
	NFmiQueryData *data = new NFmiQueryData(innerInfo);
	data->Init();
	boost::shared_ptr<NFmiFastQueryInfo> returnInfo(new NFmiSmartInfo(data, NFmiInfoData::kScriptVariableData, "", ""));
	return returnInfo;
}

NFmiParamBag NFmiSmartToolModifier::ModifiedParams(void)
{
	return itsSmartToolIntepreter->ModifiedParams();
}

const std::string& NFmiSmartToolModifier::GetStrippedMacroText(void) const
{
	return itsSmartToolIntepreter->GetStrippedMacroText();
}

boost::shared_ptr<NFmiFastQueryInfo> NFmiSmartToolModifier::UsedMacroParamData(void)
{
	if(fDoCrossSectionCalculation)
		return itsInfoOrganizer->CrossSectionMacroParamData();
	else
		return itsInfoOrganizer->MacroParamData();
}

void NFmiSmartToolModifier::ModifiedLevel(boost::shared_ptr<NFmiLevel> &theLevel)
{
	itsModifiedLevel = theLevel;
}

#ifdef FMI_SUPPORT_STATION_DATA_SMARTTOOL

void NFmiSmartToolModifier::SetGeneralDoc(NFmiEditMapGeneralDataDoc *theDoc)
{
	itsDoc = theDoc;
}

#endif // FMI_SUPPORT_STATION_DATA_SMARTTOOL
