//**********************************************************
// C++ Class Name : NFmiSmartToolModifier 
// ---------------------------------------------------------
// Filetype: (SOURCE)
// Filepath: G:/siirto/marko/oc/NFmiSmartToolModifier.cpp 
// 
// 
// GDPro Properties 
// ---------------------------------------------------
//  - GD Symbol Type    : CLD_Class 
//  - GD Method         : UML ( 4.0 ) 
//  - GD System Name    : aSmartTools 
//  - GD View Type      : Class Diagram 
//  - GD View Name      : smarttools 1 
// ---------------------------------------------------  
//  Author         : pietarin 
//  Creation Date  : Thur - Jun 20, 2002 
// 
//  Change Log     : 
// 
//**********************************************************
#ifdef WIN32
#pragma warning(disable : 4786) // poistaa n kpl VC++ kääntäjän varoitusta
#endif


#include "NFmiSmartToolModifier.h"
#include "NFmiSmartToolIntepreter.h"
#include "NFmiSmartToolCalculation.h"
#include "NFmiSmartToolCalculationSection.h"
#include "NFmiSmartToolCalculationSectionInfo.h"
#include "NFmiSmartToolCalculationInfo.h"
#include "NFmiAreaMaskInfo.h"
#include "NFmiCalculationConstantValue.h"
#include "NFmiInfoAreaMask.h"
#include "NFmiSmartInfo.h"
#include "NFmiInfoOrganizer.h"
#include "NFmiAreaMaskSectionInfo.h"
#include "NFmiCalculatedAreaMask.h"
#include "NFmiDataModifierClasses.h"
#include "NFmiRelativeDataIterator.h"
#include "NFmiRelativeTimeIntegrationIterator.h"
#include "NFmiDrawParam.h"
#include "NFmiMetEditorTypes.h"
#include "NFmiQueryData.h"

#include <cassert>
#include <stdexcept>

using namespace std;
/*
#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/

NFmiSmartToolCalculationBlockVector::NFmiSmartToolCalculationBlockVector(void)
:itsCalculationBlocks()
{
}

NFmiSmartToolCalculationBlockVector::~NFmiSmartToolCalculationBlockVector(void)
{
	Clear();
}

void NFmiSmartToolCalculationBlockVector::Clear(void)
{ // tuhoaa datan deletoimalla!
	std::for_each(Begin(), End(), PointerDestroyer());
	itsCalculationBlocks.clear();
}

NFmiSmartInfo* NFmiSmartToolCalculationBlockVector::FirstVariableInfo(void)
{
	Iterator it = Begin();
	Iterator endIt = End();
	for( ; it != endIt; ++it)
		if((*it)->FirstVariableInfo()) // pitäisi löytyä aina jotain!!!
			return (*it)->FirstVariableInfo();
	return 0;
}

void NFmiSmartToolCalculationBlockVector::SetTime(const NFmiMetTime &theTime)
{
	Iterator it = Begin();
	Iterator endIt = End();
	for( ; it != endIt; ++it)
		(*it)->SetTime(theTime);
}

void NFmiSmartToolCalculationBlockVector::Calculate(const NFmiPoint &theLatlon, unsigned long theLocationIndex, const NFmiMetTime &theTime, int theTimeIndex)
{
	Iterator it = Begin();
	Iterator endIt = End();
	for( ; it != endIt; ++it)
		(*it)->Calculate(theLatlon, theLocationIndex, theTime, theTimeIndex);
}

void NFmiSmartToolCalculationBlockVector::Add(NFmiSmartToolCalculationBlock* theBlock)
{ // ottaa omistukseen theBlock:in!!
	itsCalculationBlocks.push_back(theBlock);
}

NFmiSmartToolCalculationBlock::NFmiSmartToolCalculationBlock(void)
:itsFirstCalculationSection(0)
,itsIfAreaMaskSection(0)
,itsIfCalculationBlocks(0)
,itsElseIfAreaMaskSection(0)
,itsElseIfCalculationBlocks(0)
,itsElseCalculationBlocks(0)
,itsLastCalculationSection(0)
{
}

NFmiSmartToolCalculationBlock::~NFmiSmartToolCalculationBlock(void)
{
	Clear();
}

void NFmiSmartToolCalculationBlock::Clear(void)
{
	if(itsFirstCalculationSection)
	{
		delete itsFirstCalculationSection;
		itsFirstCalculationSection = 0;
	}
	if(itsIfAreaMaskSection)
	{
		delete itsIfAreaMaskSection;
		itsIfAreaMaskSection = 0;
	}
	if(itsIfCalculationBlocks)
	{
		delete itsIfCalculationBlocks;
		itsIfCalculationBlocks = 0;
	}
	if(itsElseIfAreaMaskSection)
	{
		delete itsElseIfAreaMaskSection;
		itsElseIfAreaMaskSection = 0;
	}
	if(itsElseIfCalculationBlocks)
	{
		delete itsElseIfCalculationBlocks;
		itsElseIfCalculationBlocks = 0;
	}
	if(itsElseCalculationBlocks)
	{
		delete itsElseCalculationBlocks;
		itsElseCalculationBlocks = 0;
	}
	if(itsLastCalculationSection)
	{
		delete itsLastCalculationSection;
		itsLastCalculationSection = 0;
	}
}

NFmiSmartInfo* NFmiSmartToolCalculationBlock::FirstVariableInfo(void)
{
	NFmiSmartInfo* info = 0;
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

void NFmiSmartToolCalculationBlock::SetTime(const NFmiMetTime &theTime)
{
	if(itsFirstCalculationSection)
		itsFirstCalculationSection->SetTime(theTime);
	if(itsIfAreaMaskSection)
		itsIfAreaMaskSection->SetTime(theTime);
	if(itsIfCalculationBlocks)
		itsIfCalculationBlocks->SetTime(theTime);
	if(itsElseIfAreaMaskSection)
		itsElseIfAreaMaskSection->SetTime(theTime);
	if(itsElseIfCalculationBlocks)
		itsElseIfCalculationBlocks->SetTime(theTime);
	if(itsElseCalculationBlocks)
		itsElseCalculationBlocks->SetTime(theTime);
	if(itsLastCalculationSection)
		itsLastCalculationSection->SetTime(theTime);
}

void NFmiSmartToolCalculationBlock::Calculate(const NFmiPoint &theLatlon, unsigned long theLocationIndex, const NFmiMetTime &theTime, int theTimeIndex)
{
	if(itsFirstCalculationSection)
		itsFirstCalculationSection->Calculate(theLatlon, theLocationIndex, theTime, theTimeIndex);

	if(itsIfAreaMaskSection && itsIfAreaMaskSection->IsMasked(theLatlon, theLocationIndex, theTime, theTimeIndex))
		itsIfCalculationBlocks->Calculate(theLatlon, theLocationIndex, theTime, theTimeIndex);
	else if(itsElseIfAreaMaskSection && itsElseIfAreaMaskSection->IsMasked(theLatlon, theLocationIndex, theTime, theTimeIndex))
		itsElseIfCalculationBlocks->Calculate(theLatlon, theLocationIndex, theTime, theTimeIndex);
	else if(itsElseCalculationBlocks)
		itsElseCalculationBlocks->Calculate(theLatlon, theLocationIndex, theTime, theTimeIndex);

	if(itsLastCalculationSection)
		itsLastCalculationSection->Calculate(theLatlon, theLocationIndex, theTime, theTimeIndex);
}

//--------------------------------------------------------
// Constructor/Destructor 
//--------------------------------------------------------
NFmiSmartToolModifier::NFmiSmartToolModifier(NFmiInfoOrganizer* theInfoOrganizer)
:itsInfoOrganizer(theInfoOrganizer)
,itsSmartToolIntepreter(new NFmiSmartToolIntepreter(theInfoOrganizer))
//,itsFirstCalculationSection(0)
//,itsLastCalculationSection(0)
//,itsIfAreaMaskSection(0)
//,itsIfCalculationSection(0)
//,itsElseIfAreaMaskSection(0)
//,itsElseIfCalculationSection(0)
//,itsElseCalculationSection(0)
,fModifySelectedLocationsOnly(false)
,itsIncludeDirectory()
,itsModifiedTimes(0)
,fHeightFunctionFlag(false)
,fUseLevelData(false)
,itsCommaCounter(0)
,itsParethesisCounter(0)
{
	assert(itsInfoOrganizer);
}
NFmiSmartToolModifier::~NFmiSmartToolModifier(void)
{
	delete itsSmartToolIntepreter;
//	ClearCalculationModifiers();
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

//	InitializeCalculationModifiers();
}

NFmiSmartToolCalculationBlockVector* NFmiSmartToolModifier::CreateCalculationBlockVector(NFmiSmartToolCalculationBlockInfoVector* theBlockInfoVector)
{
	if(theBlockInfoVector && (!theBlockInfoVector->Empty()))
	{
		NFmiSmartToolCalculationBlockVector *vec = new NFmiSmartToolCalculationBlockVector;
		NFmiSmartToolCalculationBlockInfoVector::Iterator it = theBlockInfoVector->Begin();
		NFmiSmartToolCalculationBlockInfoVector::Iterator endIt = theBlockInfoVector->End();
		for( ; it != endIt; ++it)
			vec->Add(CreateCalculationBlock(*it));
		return vec;
	}
	return 0;
}

NFmiSmartToolCalculationBlock* NFmiSmartToolModifier::CreateCalculationBlock(NFmiSmartToolCalculationBlockInfo* theBlockInfo)
{
	NFmiSmartToolCalculationBlock *block = new NFmiSmartToolCalculationBlock;
	auto_ptr<NFmiSmartToolCalculationBlock> blockPtr(block); // poikkeus tapauksessa tuhoaa automaattrisesti otuksen

	block->itsFirstCalculationSection = CreateCalculationSection(theBlockInfo->itsFirstCalculationSectionInfo);
	block->itsIfAreaMaskSection = CreateConditionalSection(theBlockInfo->itsIfAreaMaskSectionInfo);
	if(block->itsIfAreaMaskSection)
	{
		block->itsIfCalculationBlocks = CreateCalculationBlockVector(theBlockInfo->itsIfCalculationBlockInfos);
		if(!block->itsIfCalculationBlocks)
		{
			string errorText("IF-lauseen perässä ei ollut lasku osiota?");
			throw runtime_error(errorText);
		}
		block->itsElseIfAreaMaskSection = CreateConditionalSection(theBlockInfo->itsElseIfAreaMaskSectionInfo);
		if(block->itsElseIfAreaMaskSection)
		{
			block->itsElseIfCalculationBlocks = CreateCalculationBlockVector(theBlockInfo->itsElseIfCalculationBlockInfos);
			if(!block->itsElseIfCalculationBlocks)
			{
				string errorText("ELSEIF-lauseen perässä ei ollut lasku osiota?");
				throw runtime_error(errorText);
			}
		}
		if(theBlockInfo->fElseSectionExist)
		{
			block->itsElseCalculationBlocks = CreateCalculationBlockVector(theBlockInfo->itsElseCalculationBlockInfos);
			if(!block->itsElseCalculationBlocks)
			{
				string errorText("ELSE-lauseen perässä ei ollut lasku osiota?");
				throw runtime_error(errorText);
			}
		}
	}
	block->itsLastCalculationSection = CreateCalculationSection(theBlockInfo->itsLastCalculationSectionInfo);
	blockPtr.release();
	return block;
}
/*
void NFmiSmartToolModifier::InitializeCalculationModifiers(NFmiSmartToolCalculationBlockInfo* theBlock)
{
	ClearCalculationModifiers();

	itsFirstCalculationSection = CreateCalculationSection(theBlock->itsFirstCalculationSectionInfo);
	itsIfAreaMaskSection = CreateConditionalSection(theBlock->itsIfAreaMaskSectionInfo);
	if(itsIfAreaMaskSection)
	{
		itsIfCalculationSection = CreateCalculationSection(theBlock->itsIfCalculationSectionInfo);
		if(!itsIfCalculationSection)
		{
			string errorText("IF-lauseen perässä ei ollut lasku osiota?");
			throw NFmiSmartToolModifier::Exception(errorText);
		}
		itsElseIfAreaMaskSection = CreateConditionalSection(theBlock->itsElseIfAreaMaskSectionInfo);
		if(itsElseIfAreaMaskSection)
		{
			itsElseIfCalculationSection = CreateCalculationSection(theBlock->itsElseIfCalculationSectionInfo);
			if(!itsElseIfCalculationSection)
			{
				string errorText("ELSEIF-lauseen perässä ei ollut lasku osiota?");
				throw NFmiSmartToolModifier::Exception(errorText);
			}
		}
		if(theBlock->fElseSectionExist)
		{
			itsElseCalculationSection = CreateCalculationSection(theBlock->itsElseCalculationSectionInfo);
			if(!itsElseCalculationSection)
			{
				string errorText("ELSE-lauseen perässä ei ollut lasku osiota?");
				throw NFmiSmartToolModifier::Exception(errorText);
			}
		}
	}
	itsLastCalculationSection = CreateCalculationSection(theBlock->itsLastCalculationSectionInfo);
}
*/

NFmiSmartToolCalculation* NFmiSmartToolModifier::CreateConditionalSection(NFmiAreaMaskSectionInfo* theAreaMaskSectionInfo)
{
	NFmiSmartToolCalculation* areaMaskHandler = 0;
	if(theAreaMaskSectionInfo)
	{
		checkedVector<NFmiAreaMaskInfo*>& maskInfos = *theAreaMaskSectionInfo->GetAreaMaskInfoVector();
		int size = maskInfos.size();
		if(size)
		{
			areaMaskHandler = new NFmiSmartToolCalculation;
			auto_ptr<NFmiSmartToolCalculation> areaMaskHandlerPtr(areaMaskHandler);
			for(int i=0; i<size; i++)  
// HUOM!!!! editoitavaN DATAN QDatasta pitää tehdä kopiot, muuten maskit eivät toimi 
// kaikissa tilanteissa oikein!! KORJAA TÄMÄ!!!!!
				areaMaskHandler->AddCalculation(CreateAreaMask(*maskInfos[i]));
			// loppuun lisätään vielä lopetus 'merkki'
			areaMaskHandler->AddCalculation(CreateEndingAreaMask());

			return areaMaskHandlerPtr.release();
		}
	}
	return areaMaskHandler;
}

NFmiSmartToolCalculationSection* NFmiSmartToolModifier::CreateCalculationSection(NFmiSmartToolCalculationSectionInfo *theCalcSectionInfo)
{
	NFmiSmartToolCalculationSection *section = 0;
	if(theCalcSectionInfo)
	{
		checkedVector<NFmiSmartToolCalculationInfo*>& calcInfos = *theCalcSectionInfo->GetCalculationInfos();
		int size = calcInfos.size();
		if(size)
		{
			section = new NFmiSmartToolCalculationSection;
			auto_ptr<NFmiSmartToolCalculationSection> sectionPtr(section);
			for(int i=0; i<size; i++)
			{
				section->AddCalculations(CreateCalculation(calcInfos[i]));
			}
			return sectionPtr.release();
		}
	}
	return section;
}

NFmiSmartToolCalculation* NFmiSmartToolModifier::CreateCalculation(NFmiSmartToolCalculationInfo *theCalcInfo)
{
	assert(theCalcInfo);
	NFmiSmartToolCalculation* calculation = 0;
	int size = theCalcInfo->GetCalculationOperandInfoVector()->size();
	if(size)
	{
		checkedVector<NFmiAreaMaskInfo*>& areaMaskInfos = *theCalcInfo->GetCalculationOperandInfoVector();
		calculation = new NFmiSmartToolCalculation;
		auto_ptr<NFmiSmartToolCalculation> calculationPtr(calculation);
		calculation->SetCalculationText(theCalcInfo->GetCalculationText());
		calculation->SetResultInfo(CreateInfo(*theCalcInfo->GetResultDataInfo()));
		float lowerLimit;
		float upperLimit;
		GetParamValueLimits(*theCalcInfo->GetResultDataInfo(), &lowerLimit, &upperLimit);
		calculation->SetLimits(lowerLimit, upperLimit);
		calculation->AllowMissingValueAssignment(theCalcInfo->AllowMissingValueAssignment());
		for(int i=0; i<size; i++)
//			calculation->AddCalculation(CreateAreaMask(*areaMaskInfos[i]), operators[i]);
			calculation->AddCalculation(CreateAreaMask(*areaMaskInfos[i]));
		// loppuun lisätään vielä loputus 'merkki'
		calculation->AddCalculation(CreateEndingAreaMask());
		return calculationPtr.release();
	}
	return calculation;
}
/*
void NFmiSmartToolModifier::ClearCalculationModifiers(void)
{
	delete itsFirstCalculationSection;
	itsFirstCalculationSection = 0;
	delete itsLastCalculationSection;
	itsLastCalculationSection = 0;
	delete itsIfAreaMaskSection;
	itsIfAreaMaskSection = 0;
	delete itsIfCalculationSection;
	itsIfCalculationSection = 0;
//	itsElseIfAreaMaskSectionVector.clear(); // vuotavat, käytä PointerDestructoria
//	itsElseIfCalculationSectionVector.clear();
	delete itsElseIfAreaMaskSection;
	itsElseIfAreaMaskSection = 0;
	delete itsElseIfCalculationSection;
	itsElseIfCalculationSection = 0;
//	delete itsElseAreaMaskSection;
//	itsElseAreaMaskSection = 0;
	delete itsElseCalculationSection;
	itsElseCalculationSection = 0;
}
*/
//--------------------------------------------------------
// ModifyData 
//--------------------------------------------------------
// Suorittaa varsinaiset modifikaatiot. Käyttäjä voi antaa parametrina rajoitetun ajan
// modifioinneille, jos theModifiedTimes on 0-pointteri, tehdään operaatiot kaikille
// datan ajoille.
void NFmiSmartToolModifier::ModifyData(NFmiTimeDescriptor* theModifiedTimes, bool fSelectedLocationsOnly)
{
	itsModifiedTimes = theModifiedTimes;
	fModifySelectedLocationsOnly = fSelectedLocationsOnly;
	try
	{
		// Seed the random-number generator with current time so that
		// the numbers will be different every time we run.
		srand( static_cast<unsigned int>(time( NULL ))); // mahd. satunnais funktion käytön takia, pitää 'sekoittaa' random generaattori

		checkedVector<NFmiSmartToolCalculationBlockInfo>& smartToolCalculationBlockInfos = itsSmartToolIntepreter->SmartToolCalculationBlocks();
		int size = smartToolCalculationBlockInfos.size();
		for(int i=0; i<size; i++)
		{
			NFmiSmartToolCalculationBlockInfo blockInfo = smartToolCalculationBlockInfos[i];
			NFmiSmartToolCalculationBlock* block = CreateCalculationBlock(&blockInfo);
			auto_ptr<NFmiSmartToolCalculationBlock> blockPtr(block); // tuhoaa block-otuksen automaattisesti
			if(block)
			{
				ModifyBlockData(block);
//			InitializeCalculationModifiers(&blockInfo);
			}
//			ModifyData2(theModifiedTimes, itsFirstCalculationSection);
//			ModifyConditionalData(theModifiedTimes);
//			ModifyData2(theModifiedTimes, itsLastCalculationSection);
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

void NFmiSmartToolModifier::ModifyBlockData(NFmiSmartToolCalculationBlock *theCalculationBlock)
{
	ModifyData2(theCalculationBlock->itsFirstCalculationSection);
	ModifyConditionalData(theCalculationBlock);
	ModifyData2(theCalculationBlock->itsLastCalculationSection);
}

void NFmiSmartToolModifier::ModifyConditionalData(NFmiSmartToolCalculationBlock *theCalculationBlock)
{
	if(theCalculationBlock->itsIfAreaMaskSection && theCalculationBlock->itsIfCalculationBlocks)
	{
		if(theCalculationBlock->FirstVariableInfo() == 0)
			throw runtime_error("Tuntematon virhe skriptissä.");
		NFmiSmartInfo *info = theCalculationBlock->FirstVariableInfo()->Clone();
		std::auto_ptr<NFmiSmartInfo> infoPtr(info);

		try
		{
			if(this->fModifySelectedLocationsOnly)
				info->MaskType(NFmiMetEditorTypes::kFmiSelectionMask);
			else
				info->MaskType(NFmiMetEditorTypes::kFmiNoMask);
			NFmiTimeDescriptor modifiedTimes(itsModifiedTimes ? *itsModifiedTimes : info->TimeDescriptor());
			for(modifiedTimes.Reset(); modifiedTimes.Next(); )
			{
				NFmiMetTime time1(modifiedTimes.Time());
				info->Time(time1); // asetetaan myös tämä, että saadaan oikea timeindex
				theCalculationBlock->itsIfAreaMaskSection->SetTime(time1); // yritetään optimoida laskuja hieman kun mahdollista
				theCalculationBlock->itsIfCalculationBlocks->SetTime(time1); // yritetään optimoida laskuja hieman kun mahdollista
				if(theCalculationBlock->itsElseIfAreaMaskSection && theCalculationBlock->itsElseIfCalculationBlocks)
				{
					theCalculationBlock->itsElseIfAreaMaskSection->SetTime(time1);
					theCalculationBlock->itsElseIfCalculationBlocks->SetTime(time1);
				}
				if(theCalculationBlock->itsElseCalculationBlocks)
					theCalculationBlock->itsElseCalculationBlocks->SetTime(time1);

				for(info->ResetLocation(); info->NextLocation(); )
				{
					NFmiPoint latlon(info->LatLon());
					unsigned long locationIndex = info->LocationIndex();
					int timeIndex = info->TimeIndex();
					if(theCalculationBlock->itsIfAreaMaskSection->IsMasked(latlon, locationIndex, time1, timeIndex))
						theCalculationBlock->itsIfCalculationBlocks->Calculate(latlon, locationIndex, time1, timeIndex);
					else if(theCalculationBlock->itsElseIfAreaMaskSection && theCalculationBlock->itsElseIfCalculationBlocks && theCalculationBlock->itsElseIfAreaMaskSection->IsMasked(latlon, locationIndex, time1, timeIndex))
					{
						theCalculationBlock->itsElseIfCalculationBlocks->Calculate(latlon, locationIndex, time1, timeIndex);
					}
					else if(theCalculationBlock->itsElseCalculationBlocks)
						theCalculationBlock->itsElseCalculationBlocks->Calculate(latlon, locationIndex, time1, timeIndex);
				}
			}
		}
		catch(...)
		{
			info->DestroyData();
			throw ;
		}
		info->DestroyData();
	}
}

void NFmiSmartToolModifier::ModifyData2(NFmiSmartToolCalculationSection* theCalculationSection)
{
	if(theCalculationSection && theCalculationSection->FirstVariableInfo())
	{
//		NFmiFastQueryInfo info(*theCalculationSection->FirstVariableInfo());
		NFmiSmartInfo *info = theCalculationSection->FirstVariableInfo()->Clone();
		std::auto_ptr<NFmiSmartInfo> infoPtr(info);
		try
		{
			if(fModifySelectedLocationsOnly)
				info->MaskType(NFmiMetEditorTypes::kFmiSelectionMask);
			else
				info->MaskType(NFmiMetEditorTypes::kFmiNoMask);
			NFmiTimeDescriptor modifiedTimes(itsModifiedTimes ? *itsModifiedTimes : info->TimeDescriptor());

			// Muutin lasku systeemin suoritusta, koska tuli ongelmia mm. muuttujien kanssa, kun niitä käytettiin samassa calculationSectionissa
			// CalculationSection = lasku rivejä peräkkäin esim.
			// T = T + 1
			// P = P + 1
			// jne. ilman IF-lauseita
			// ENNEN laskettiin tälläinen sectio siten että käytiin läpi koko sectio samalla paikalla ja ajalla ja sitten siirryttiin eteenpäin.
			// NYT lasketaan aina yksi laskurivi läpi kaikkien aikojen ja paikkojen, ja sitten siirrytään seuraavalle lasku riville.
			int size = theCalculationSection->GetCalculations()->size();
			for(int i=0; i<size; i++)
			{
				for(modifiedTimes.Reset(); modifiedTimes.Next(); )
				{
					NFmiMetTime time1(modifiedTimes.Time());
					if(info->Time(time1)) // asetetaan myös tämä, että saadaan oikea timeindex
					{
						theCalculationSection->SetTime(time1); // yritetään optimoida laskuja hieman kun mahdollista
						for(info->ResetLocation(); info->NextLocation(); )
						{
//							theCalculationSection->Calculate(info->LatLon(), info->LocationIndex(), time1, info->TimeIndex());
							(*theCalculationSection->GetCalculations())[i]->Calculate(info->LatLon(), info->LocationIndex(), time1, info->TimeIndex());
						}
					}
				}

			}

		}
		catch(...)
		{
			info->DestroyData();
			throw ;
		}
		info->DestroyData();
	}
}

// pakko oli vääntää tämä konvertteri, koska nuo datatyyppi jutut ovat karanneet käsistä
/*
static NFmiAreaMask::DataType ConvertType(NFmiInfoData::Type theType)
{
	switch (theType)
	{
	case kFmiDataTypeEditable:
		return NFmiAreaMask::kEditable;
	case kFmiDataTypeStationary:
		return NFmiAreaMask::kStationary;
	default:
		return NFmiAreaMask::kNoDataType;
	}
}
*/
//--------------------------------------------------------
// CreateAreaMask 
//--------------------------------------------------------
// HUOM!! Ei osaa hoitaa kuin editoitavia datoja!!!!
// HUOM!! Vuotaa exception tilanteissa.
NFmiAreaMask* NFmiSmartToolModifier::CreateAreaMask(const NFmiAreaMaskInfo &theAreaMaskInfo)
{
	NFmiAreaMask::CalculationOperationType maskType = theAreaMaskInfo.GetOperationType();
	NFmiAreaMask* areaMask = 0;

	switch(maskType)
	{
		case NFmiAreaMask::InfoVariable:
			{
			// HUOM!! Tähän vaaditaan syvä data kopio!!!
			// JOS kyseessä on ehtolauseen muuttujasta, joka on editoitavaa dataa. 
			NFmiSmartInfo* info = CreateInfo(theAreaMaskInfo);
//			areaMask = new NFmiInfoAreaMask(theAreaMaskInfo.GetMaskCondition(), NFmiAreaMask::kInfo, theAreaMaskInfo.GetDataType(), info, true);
			areaMask = new NFmiInfoAreaMask(theAreaMaskInfo.GetMaskCondition(), NFmiAreaMask::kInfo, info->DataType(), info, true);
			break;
			}
		case NFmiAreaMask::RampFunction:
			{
			NFmiInfoData::Type type = theAreaMaskInfo.GetDataType();
			if(type != NFmiInfoData::kCalculatedValue)
			{
				NFmiSmartInfo* info = CreateInfo(theAreaMaskInfo);
				areaMask = new NFmiCalculationRampFuction(theAreaMaskInfo.GetMaskCondition(), NFmiAreaMask::kInfo, theAreaMaskInfo.GetDataType(), info, true);
			}
			else
			{
				NFmiAreaMask *areaMask2 = CreateCalculatedAreaMask(theAreaMaskInfo);
				areaMask = new NFmiCalculationRampFuctionWithAreaMask(theAreaMaskInfo.GetMaskCondition(), NFmiAreaMask::kInfo, theAreaMaskInfo.GetDataType(), areaMask2, true);
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
			NFmiSmartInfo* info = CreateInfo(theAreaMaskInfo);
			bool deepCopyCreated = false;
			if(theAreaMaskInfo.GetUseDefaultProducer())
			{ // Pitää tehdä syvä kopio datasta, että datan muuttuminen ei vaikuta laskuihin.
				deepCopyCreated = true;
				NFmiSmartInfo* tmp = info->Clone();
				delete info;
				info = tmp;
			}
			NFmiDataModifier *modifier = CreateIntegrationFuction(theAreaMaskInfo);
			NFmiDataIterator *iterator = CreateIterator(theAreaMaskInfo, info);
			areaMask = new NFmiCalculationIntegrationFuction(iterator, modifier, NFmiAreaMask::kInfo, theAreaMaskInfo.GetDataType(), info, true, deepCopyCreated);
			areaMask->SetFunctionType(theAreaMaskInfo.GetFunctionType());
			if(fUseLevelData)
				itsParethesisCounter++;
			break;
			}
		case NFmiAreaMask::FunctionPeekXY:
			{
			// HUOM!! Tähän vaaditaan syvä data kopio!!!
			// JOS kyseessä on ehtolauseen muuttujasta, joka on editoitavaa dataa. 
			NFmiSmartInfo* info = CreateInfo(theAreaMaskInfo);
			bool deepCopyCreated = false;
			if(theAreaMaskInfo.GetUseDefaultProducer())
			{ // Pitää tehdä syvä kopio datasta, että datan muuttuminen ei vaikuta laskuihin.
				deepCopyCreated = true;
				NFmiSmartInfo* tmp = info->Clone();
				delete info;
				info = tmp;
			}
			areaMask = new NFmiInfoAreaMaskPeekXY(theAreaMaskInfo.GetMaskCondition(), NFmiAreaMask::kInfo, info->DataType(), info, 
				static_cast<int>(theAreaMaskInfo.GetOffsetPoint1().X()), static_cast<int>(theAreaMaskInfo.GetOffsetPoint1().Y()), true, NFmiAreaMask::kNoValue, deepCopyCreated);
			if(fUseLevelData)
				itsParethesisCounter++;
			break;
			}
		case NFmiAreaMask::CalculatedVariable:
			{
			areaMask = CreateCalculatedAreaMask(theAreaMaskInfo);
			break;
			}
		case NFmiAreaMask::Constant:
			{
			areaMask = new NFmiCalculationConstantValue(theAreaMaskInfo.GetMaskCondition().LowerLimit());
			break;
			}
		case NFmiAreaMask::Operator:
			{
			areaMask = new NFmiCalculationSpecialCase(theAreaMaskInfo.GetCalculationOperator());
			break;
			}
		case NFmiAreaMask::StartParenthesis:
			{
			areaMask = new NFmiCalculationSpecialCase(theAreaMaskInfo.GetCalculationOperator());
			if(fUseLevelData)
				itsParethesisCounter++;
			break;
			}
		case NFmiAreaMask::EndParenthesis:
			{
			areaMask = new NFmiCalculationSpecialCase(theAreaMaskInfo.GetCalculationOperator());
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
			areaMask = new NFmiCalculationSpecialCase(theAreaMaskInfo.GetCalculationOperator());
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
			areaMask = new NFmiCalculationSpecialCase;
			areaMask->Condition(theAreaMaskInfo.GetMaskCondition());
			break;
			}
		case NFmiAreaMask::BinaryOperatorType:
			{
			areaMask = new NFmiCalculationSpecialCase;
			areaMask->PostBinaryOperator(theAreaMaskInfo.GetBinaryOperator());
			break;
			}
		case NFmiAreaMask::MathFunctionStart:
			{
			areaMask = new NFmiCalculationSpecialCase;
			areaMask->SetMathFunctionType(theAreaMaskInfo.GetMathFunctionType());
			if(fUseLevelData)
				itsParethesisCounter++;
			break;
			}
		case NFmiAreaMask::ThreeArgumentFunctionStart:
			{
			areaMask = new NFmiCalculationSpecialCase;
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
		case NFmiAreaMask::DeltaZFunction:
			{
			areaMask = new NFmiCalculationDeltaZValue;
			break;
			}
		default:
			throw runtime_error("Outo data tyyppi yritettäessä tehdä laskuja.");
	}
	areaMask->SetCalculationOperationType(maskType);

	return areaMask;
}

NFmiAreaMask* NFmiSmartToolModifier::CreateCalculatedAreaMask(const NFmiAreaMaskInfo &theAreaMaskInfo)
{
	NFmiAreaMask* areaMask = 0;
	FmiParameterName parId = FmiParameterName(theAreaMaskInfo.GetDataIdent().GetParamIdent());
	if(parId == kFmiLatitude || parId == kFmiLongitude)
		areaMask = new NFmiLatLonAreaMask(theAreaMaskInfo.GetDataIdent(), theAreaMaskInfo.GetMaskCondition());
	else if(parId == kFmiElevationAngle)
		areaMask = new NFmiElevationAngleAreaMask(theAreaMaskInfo.GetDataIdent(), theAreaMaskInfo.GetMaskCondition());
	else if(parId == kFmiDay)
		areaMask = new NFmiJulianDayAreaMask(theAreaMaskInfo.GetDataIdent(), theAreaMaskInfo.GetMaskCondition());
	else if(parId == kFmiHour)
		areaMask = new NFmiLocalHourAreaMask(theAreaMaskInfo.GetDataIdent(), theAreaMaskInfo.GetMaskCondition());
	else if(parId == kFmiForecastPeriod)
		areaMask = new NFmiForecastHourAreaMask(itsInfoOrganizer->EditedInfo(), theAreaMaskInfo.GetDataIdent(), theAreaMaskInfo.GetMaskCondition());
	else if(parId == kFmiDeltaTime)
		areaMask = new NFmiTimeStepAreaMask(itsInfoOrganizer->EditedInfo(), theAreaMaskInfo.GetDataIdent(), theAreaMaskInfo.GetMaskCondition());
	

	if(areaMask)
		return areaMask;

	throw runtime_error(string("Outo laskettava muuttuja/data tyyppi (ohjelmointi vika?)."));
}

// Muista jos tulee päivityksiä, smanlainen funktio löytyy myös NFmiSmartToolCalculation-luokasta
NFmiDataModifier* NFmiSmartToolModifier::CreateIntegrationFuction(const NFmiAreaMaskInfo &theAreaMaskInfo)
{
	NFmiDataModifier* modifier = 0;
	NFmiAreaMask::FunctionType func = theAreaMaskInfo.GetFunctionType();
	switch(func)
	{
	case NFmiAreaMask::Avg:
		modifier = new NFmiDataModifierAvg;
		break;
	case NFmiAreaMask::Min:
		modifier = new NFmiDataModifierMin;
		break;
	case NFmiAreaMask::Max:
		modifier = new NFmiDataModifierMax;
		break;
	case NFmiAreaMask::Sum:
		modifier = new NFmiDataModifierSum;
		break;
		// HUOM!!!! Tee WAvg-modifier myös, joka on peritty Avg-modifieristä ja tee joku kerroin juttu painotukseen.
	default:
		throw runtime_error("Outo integraatio-funktio tyyppi yritettäessä tehdä laskuja.");
	}
	return modifier;
}

NFmiDataIterator* NFmiSmartToolModifier::CreateIterator(const NFmiAreaMaskInfo &theAreaMaskInfo, NFmiSmartInfo* theInfo)
{
	NFmiDataIterator* iterator = 0;
	NFmiAreaMask::CalculationOperationType mType = theAreaMaskInfo.GetOperationType();
	switch(mType)
	{
		case NFmiAreaMask::FunctionAreaIntergration:
			// HUOM!! NFmiRelativeDataIterator:iin pitää tehdä joustavampi 'laatikon' säätö systeemi, että laatikko ei olisi aina keskitetty
			iterator = new NFmiRelativeDataIterator(theInfo,
													static_cast<long>(theAreaMaskInfo.GetOffsetPoint1().X()),
													static_cast<long>(theAreaMaskInfo.GetOffsetPoint1().Y()),
													0,
													static_cast<long>(theAreaMaskInfo.GetOffsetPoint2().X()),
													static_cast<long>(theAreaMaskInfo.GetOffsetPoint2().Y()),
													0);
			break;
		case NFmiAreaMask::FunctionTimeIntergration:
			{
				NFmiPoint p(theAreaMaskInfo.GetOffsetPoint1());
				iterator = new NFmiRelativeTimeIntegrationIterator(theInfo,
																   static_cast<int>(p.Y() - p.X() + 1),
																   static_cast<int>(p.Y()));
				break;
			}
	default:
		throw runtime_error("Outo iteraattori-tyyppi yritettäessä tehdä laskuja.");
	}
	return iterator;
}

NFmiAreaMask* NFmiSmartToolModifier::CreateEndingAreaMask(void)
{
	NFmiAreaMask *areaMask = new NFmiCalculationSpecialCase;
	areaMask->SetCalculationOperationType(NFmiAreaMask::EndOfOperations);
	return areaMask;
}

NFmiSmartInfo* NFmiSmartToolModifier::CreateInfo(const NFmiAreaMaskInfo &theAreaMaskInfo)
{
	NFmiSmartInfo* info = 0;
	if(theAreaMaskInfo.GetDataType() == NFmiInfoData::kScriptVariableData)
		info = CreateScriptVariableInfo(theAreaMaskInfo.GetDataIdent());
	else if(theAreaMaskInfo.GetUseDefaultProducer() || theAreaMaskInfo.GetDataType() == NFmiInfoData::kCopyOfEdited)
		info = itsInfoOrganizer->CreateShallowCopyInfo(theAreaMaskInfo.GetDataIdent(), theAreaMaskInfo.GetLevel(), theAreaMaskInfo.GetDataType(), true, fUseLevelData);
	else
	{
		if(fUseLevelData && theAreaMaskInfo.GetLevel() != 0) // jos pitää käyttää level dataa (SumZ ja MinH funktiot), ei saa antaa level infoa parametrin yhteydessä
			throw runtime_error("Nyt pitää antaa parametri ilman level tietoa.\n" + theAreaMaskInfo.GetMaskText());
		if(fUseLevelData) // jos leveldata-flagi päällä, yritetään ensin, löytyykö hybridi dataa
			info = itsInfoOrganizer->CreateShallowCopyInfo(theAreaMaskInfo.GetDataIdent(), theAreaMaskInfo.GetLevel(), NFmiInfoData::kHybridData, false, fUseLevelData); // tähän pieni hybrid-koukku, jos haluttiin level dataa
		if(info == 0)
			info = itsInfoOrganizer->CreateShallowCopyInfo(theAreaMaskInfo.GetDataIdent(), theAreaMaskInfo.GetLevel(), theAreaMaskInfo.GetDataType(), false, fUseLevelData);
	}
	if(!info)
		throw runtime_error("Haluttua parametria ei löytynyt tietokannasta.\n" + theAreaMaskInfo.GetMaskText());
	return info;
}

void NFmiSmartToolModifier::GetParamValueLimits(const NFmiAreaMaskInfo &theAreaMaskInfo, float *theLowerLimit, float *theUpperLimit)
{
	NFmiDrawParam* drawParam = itsInfoOrganizer->CreateDrawParam(theAreaMaskInfo.GetDataIdent(), theAreaMaskInfo.GetLevel(), theAreaMaskInfo.GetDataType());
	if(drawParam)
	{
		*theLowerLimit = static_cast<float>(drawParam->AbsoluteMinValue());
		*theUpperLimit = static_cast<float>(drawParam->AbsoluteMaxValue());
		delete drawParam;
	}
	else
		throw runtime_error("Parametrin min ja max rajoja ei saatu.");
}

struct FindScriptVariable
{
	FindScriptVariable(int theParId):itsParId(theParId){}

	bool operator()(NFmiSmartInfo* thePtr)
	{return itsParId == static_cast<int>(thePtr->Param().GetParamIdent());}

	int itsParId;
};

NFmiSmartInfo* NFmiSmartToolModifier::CreateScriptVariableInfo(const NFmiDataIdent &theDataIdent)
{
	NFmiSmartInfo* tmp = GetScriptVariableInfo(theDataIdent);
	if(tmp)
		return new NFmiSmartInfo(*tmp);
	else // pitää vielä luoda kyseinen skripti-muuttuja, koska sitä käytetään nyt 1. kertaa
	{
		NFmiSmartInfo* tmp2 = CreateRealScriptVariableInfo(theDataIdent);
		if(tmp2)
		{
			itsScriptVariableInfos.push_back(tmp2);
			tmp = GetScriptVariableInfo(theDataIdent);
			if(tmp)
				return new NFmiSmartInfo(*tmp);
		}
	}

	throw runtime_error(string("Jotain vikaa NFmiSmartToolModifier:issa kun skripti-muuttujan ") + string(theDataIdent.GetParamName()) + string(" luominen ei onnistu."));
}

NFmiSmartInfo* NFmiSmartToolModifier::GetScriptVariableInfo(const NFmiDataIdent &theDataIdent)
{
	checkedVector<NFmiSmartInfo*>::iterator it = std::find_if(itsScriptVariableInfos.begin(), itsScriptVariableInfos.end(), FindScriptVariable(theDataIdent.GetParamIdent()));
	if(it != itsScriptVariableInfos.end())
		return *it;
	return 0;
}


struct SmartInfoDataDestroyer
{
	void operator()(NFmiSmartInfo* thePtr)
	{thePtr->DestroyData();}
};

void NFmiSmartToolModifier::ClearScriptVariableInfos(void)
{
	std::for_each(itsScriptVariableInfos .begin(), itsScriptVariableInfos .end(), SmartInfoDataDestroyer());
	std::for_each(itsScriptVariableInfos .begin(), itsScriptVariableInfos .end(), PointerDestroyer());
	itsScriptVariableInfos.clear();
}

NFmiSmartInfo* NFmiSmartToolModifier::CreateRealScriptVariableInfo(const NFmiDataIdent &theDataIdent)
{
	NFmiSmartInfo* editedInfo = itsInfoOrganizer->EditedInfo();
	NFmiParamBag paramBag;
	paramBag.Add(theDataIdent);
	NFmiParamDescriptor paramDesc(paramBag);
	NFmiQueryInfo innerInfo(paramDesc, itsModifiedTimes ? *itsModifiedTimes : editedInfo->TimeDescriptor(), editedInfo->HPlaceDescriptor(), editedInfo->VPlaceDescriptor());
	NFmiQueryData *data = new NFmiQueryData(innerInfo);
	data->Init();
	NFmiQueryInfo info(data);
	info.First();
	NFmiSmartInfo *returnInfo = new NFmiSmartInfo(info, data);
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
