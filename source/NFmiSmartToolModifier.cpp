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
#include "NFmiCalculationChangeFactorArray.h"
#include "NFmiDataModifierClasses.h"
#include "NFmiRelativeDataIterator.h"
#include "NFmiRelativeTimeIntegrationIterator.h"
#include "NFmiDrawParam.h"
#include "NFmiMetEditorTypes.h"

#include <cassert>

using namespace std;
//--------------------------------------------------------
// Constructor/Destructor 
//--------------------------------------------------------
NFmiSmartToolModifier::NFmiSmartToolModifier(NFmiInfoOrganizer* theInfoOrganizer)
:itsInfoOrganizer(theInfoOrganizer)
,itsSmartToolIntepreter(new NFmiSmartToolIntepreter)
,itsFirstCalculationSection(0)
,itsLastCalculationSection(0)
,itsIfAreaMaskSection(0)
,itsIfCalculationSection(0)
//,itsElseIfAreaMaskSectionVector()
//,itsElseIfCalculationSectionVector()
,itsElseIfAreaMaskSection(0)
,itsElseIfCalculationSection(0)
//,itsElseAreaMaskSection(0)
,itsElseCalculationSection(0)
,fModifySelectedLocationsOnly(false)
{
	assert(itsInfoOrganizer);
}
NFmiSmartToolModifier::~NFmiSmartToolModifier(void)
{
	delete itsSmartToolIntepreter;
	ClearCalculationModifiers();
}
//--------------------------------------------------------
// InitSmartTool 
//--------------------------------------------------------
// Tulkitsee macron, luo tavittavat systeemit, että makro voidaan suorittaa.
void NFmiSmartToolModifier::InitSmartTool(const std::string &theSmartToolText)  throw (NFmiSmartToolModifier::Exception)
{
	fMacroRunnable = true;
	itsErrorText = "";
	try
	{
		itsSmartToolIntepreter->Interpret(theSmartToolText);
	}
	catch(NFmiSmartToolIntepreter::Exception excep)
	{
		fMacroRunnable = false;
		itsErrorText = excep.What();
		throw  NFmiSmartToolModifier::Exception(itsErrorText);
	}

//	InitializeCalculationModifiers();
}

void NFmiSmartToolModifier::InitializeCalculationModifiers(NFmiSmartToolCalculationBlock* theBlock)
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

NFmiSmartToolCalculation* NFmiSmartToolModifier::CreateConditionalSection(NFmiAreaMaskSectionInfo* theAreaMaskSectionInfo)
{
	NFmiSmartToolCalculation* areaMaskHandler = 0;
	if(theAreaMaskSectionInfo)
	{
		std::vector<NFmiAreaMaskInfo*>& maskInfos = *theAreaMaskSectionInfo->GetAreaMaskInfoVector();
		int size = maskInfos.size();
		if(size)
		{
			areaMaskHandler = new NFmiSmartToolCalculation;
			for(int i=0; i<size; i++)  
// HUOM!!!! editoitavaN DATAN QDatasta pitää tehdä kopiot, muuten maskit eivät toimi 
// kaikissa tilanteissa oikein!! KORJAA TÄMÄ!!!!!
				areaMaskHandler->AddCalculation(CreateAreaMask(*maskInfos[i]));
			// loppuun lisätään vielä lopetus 'merkki'
			areaMaskHandler->AddCalculation(CreateEndingAreaMask());
		}
	}
	return areaMaskHandler;
}

NFmiSmartToolCalculationSection* NFmiSmartToolModifier::CreateCalculationSection(NFmiSmartToolCalculationSectionInfo *theCalcSectionInfo)
{
	NFmiSmartToolCalculationSection *section = 0;
	if(theCalcSectionInfo)
	{
		std::vector<NFmiSmartToolCalculationInfo*>& calcInfos = *theCalcSectionInfo->GetCalculationInfos();
		int size = calcInfos.size();
		if(size)
		{
			section = new NFmiSmartToolCalculationSection;
			for(int i=0; i<size; i++)
				section->AddCalculations(CreateCalculation(calcInfos[i]));
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
		std::vector<NFmiAreaMaskInfo*>& areaMaskInfos = *theCalcInfo->GetCalculationOperandInfoVector();
//		std::vector<NFmiSmartToolCalculation::FmiCalculationOperators>& operators = *theCalcInfo->GetOperationVector();
		calculation = new NFmiSmartToolCalculation;
		calculation->SetCalculationText(theCalcInfo->GetCalculationText());
		calculation->SetResultInfo(CreateInfo(*theCalcInfo->GetResultDataInfo()));
		float lowerLimit;
		float upperLimit;
		GetParamValueLimits(*theCalcInfo->GetResultDataInfo(), &lowerLimit, &upperLimit);
		calculation->SetLimits(lowerLimit, upperLimit);
		for(int i=0; i<size; i++)
//			calculation->AddCalculation(CreateAreaMask(*areaMaskInfos[i]), operators[i]);
			calculation->AddCalculation(CreateAreaMask(*areaMaskInfos[i]));
		// loppuun lisätään vielä loputus 'merkki'
		calculation->AddCalculation(CreateEndingAreaMask());
	}
	return calculation;
}

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

//--------------------------------------------------------
// ModifyData 
//--------------------------------------------------------
// Suorittaa varsinaiset modifikaatiot. Käyttäjä voi antaa parametrina rajoitetun ajan
// modifioinneille, jos theModifiedTimes on 0-pointteri, tehdään operaatiot kaikille
// datan ajoille.
void NFmiSmartToolModifier::ModifyData(NFmiTimeDescriptor* theModifiedTimes, const std::vector<double> &theModificationFactors, bool fSelectedLocationsOnly) throw (NFmiSmartToolModifier::Exception)
{
	fModifySelectedLocationsOnly = fSelectedLocationsOnly;
	try
	{
		// Seed the random-number generator with current time so that
		// the numbers will be different every time we run.
		srand( static_cast<unsigned int>(time( NULL ))); // mahd. satunnais funktion käytön takia, pitää 'sekoittaa' random generaattori

		itsModificationFactors = theModificationFactors; // huom! tässä tehdään kopio

		std::vector<NFmiSmartToolCalculationBlock>& smartToolCalculationBlocks = itsSmartToolIntepreter->SmartToolCalculationBlocks();
		int size = smartToolCalculationBlocks.size();
		for(int i=0; i<size; i++)
		{
			NFmiSmartToolCalculationBlock block = smartToolCalculationBlocks[i];
			InitializeCalculationModifiers(&block);

			ModifyData2(theModifiedTimes, itsFirstCalculationSection);
			ModifyConditionalData(theModifiedTimes);
			ModifyData2(theModifiedTimes, itsLastCalculationSection);
		}
	}
	catch(NFmiSmartToolCalculation::Exception excep)
	{
		fMacroRunnable = false;
		itsErrorText = excep.What();
		throw  NFmiSmartToolModifier::Exception(itsErrorText);
	}
}

static void ModifyConditionalData(bool modifySelectedOnly, NFmiTimeDescriptor *theModifiedTimes, std::vector<double> &theModificationFactors
								 ,NFmiSmartToolCalculation* theIfAreaMaskSection, NFmiSmartToolCalculationSection* theIfCalculationSection
								 ,NFmiSmartToolCalculation* theElseIfAreaMaskSection, NFmiSmartToolCalculationSection* theElseIfCalculationSection
								 ,NFmiSmartToolCalculationSection* theElseCalculationSection)
{
	if(theIfAreaMaskSection && theIfCalculationSection && theIfCalculationSection->FirstVariableInfo())
	{
		theIfAreaMaskSection->SetModificationFactors(&theModificationFactors); // maskissakin voi olla tmf:iä
		theIfCalculationSection->SetModificationFactors(&theModificationFactors);
		if(theElseIfAreaMaskSection && theElseIfCalculationSection)
		{
			theElseIfAreaMaskSection->SetModificationFactors(&theModificationFactors);
			theElseIfCalculationSection->SetModificationFactors(&theModificationFactors);
		}
		if(theElseCalculationSection)
			theElseCalculationSection->SetModificationFactors(&theModificationFactors);
//		NFmiFastQueryInfo info(*theIfCalculationSection->FirstVariableInfo());
		NFmiSmartInfo *info = theIfCalculationSection->FirstVariableInfo()->Clone();
		std::auto_ptr<NFmiSmartInfo> infoPtr(info);

		try
		{
			if(modifySelectedOnly)
				info->MaskType(NFmiMetEditorTypes::kFmiSelectionMask);
			else
				info->MaskType(NFmiMetEditorTypes::kFmiNoMask);
			NFmiTimeDescriptor modifiedTimes(theModifiedTimes ? *theModifiedTimes : info->TimeDescriptor());
			for(modifiedTimes.Reset(); modifiedTimes.Next(); )
			{
				NFmiMetTime time1(modifiedTimes.Time());
				info->Time(time1); // asetetaan myös tämä, että saadaan oikea timeindex
				theIfAreaMaskSection->SetTime(time1); // yritetään optimoida laskuja hieman kun mahdollista
				theIfCalculationSection->SetTime(time1); // yritetään optimoida laskuja hieman kun mahdollista
				if(theElseIfAreaMaskSection && theElseIfCalculationSection)
				{
					theElseIfAreaMaskSection->SetTime(time1);
					theElseIfCalculationSection->SetTime(time1);
				}
				if(theElseCalculationSection)
					theElseCalculationSection->SetTime(time1);
				for(info->ResetLocation(); info->NextLocation(); )
				{
					NFmiPoint latlon(info->LatLon());
					unsigned long locationIndex = info->LocationIndex();
					int timeIndex = info->TimeIndex();
					if(theIfAreaMaskSection->IsMasked(latlon, locationIndex, time1, timeIndex))
						theIfCalculationSection->Calculate(latlon, locationIndex, time1, timeIndex);
					else if(theElseIfAreaMaskSection && theElseIfCalculationSection && theElseIfAreaMaskSection->IsMasked(latlon, locationIndex, time1, timeIndex))
					{
						theElseIfCalculationSection->Calculate(latlon, locationIndex, time1, timeIndex);
					}
					else if(theElseCalculationSection)
						theElseCalculationSection->Calculate(latlon, locationIndex, time1, timeIndex);
				}
			}
		}
		catch(NFmiSmartToolIntepreter::Exception excep)
		{
			info->DestroyData();
			throw excep;
		}
		info->DestroyData();
	}
}

// Miten tähän sapluunaan saa ujutettua myös elseif ja else haarat järkevästi??????
// Tee ensin kuitenkin vain IF-haara testausta varten.
// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX Tästä jatkuu!!!!!!!
//void NFmiSmartToolModifier::ModifyConditionalData(NFmiTimeDescriptor *theModifiedTimes, NFmiSmartToolCalculation *theMaskCondition, NFmiSmartToolCalculationSection *theConditionalCalculations)
void NFmiSmartToolModifier::ModifyConditionalData(NFmiTimeDescriptor *theModifiedTimes)
{
	::ModifyConditionalData(fModifySelectedLocationsOnly, theModifiedTimes, itsModificationFactors, itsIfAreaMaskSection, itsIfCalculationSection, itsElseIfAreaMaskSection, itsElseIfCalculationSection, itsElseCalculationSection);
}

void NFmiSmartToolModifier::ModifyData2(NFmiTimeDescriptor* theModifiedTimes, NFmiSmartToolCalculationSection* theCalculationSection)
{
	if(theCalculationSection && theCalculationSection->FirstVariableInfo())
	{
		theCalculationSection->SetModificationFactors(&itsModificationFactors);
//		NFmiFastQueryInfo info(*theCalculationSection->FirstVariableInfo());
		NFmiSmartInfo *info = theCalculationSection->FirstVariableInfo()->Clone();
		std::auto_ptr<NFmiSmartInfo> infoPtr(info);
		try
		{
			if(fModifySelectedLocationsOnly)
				info->MaskType(NFmiMetEditorTypes::kFmiSelectionMask);
			else
				info->MaskType(NFmiMetEditorTypes::kFmiNoMask);
			NFmiTimeDescriptor modifiedTimes(theModifiedTimes ? *theModifiedTimes : info->TimeDescriptor());
			for(modifiedTimes.Reset(); modifiedTimes.Next(); )
			{
				NFmiMetTime time1(modifiedTimes.Time());
				info->Time(time1); // asetetaan myös tämä, että saadaan oikea timeindex
				theCalculationSection->SetTime(time1); // yritetään optimoida laskuja hieman kun mahdollista
				for(info->ResetLocation(); info->NextLocation(); )
				{
					theCalculationSection->Calculate(info->LatLon(), info->LocationIndex(), time1, info->TimeIndex());
				}
			}
		}
		catch(NFmiSmartToolIntepreter::Exception excep)
		{
			info->DestroyData();
			throw excep;
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
NFmiAreaMask* NFmiSmartToolModifier::CreateAreaMask(const NFmiAreaMaskInfo &theAreaMaskInfo) throw (NFmiSmartToolModifier::Exception)
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
			areaMask = new NFmiInfoAreaMask(theAreaMaskInfo.GetMaskCondition(), NFmiAreaMask::kInfo, theAreaMaskInfo.GetDataType(), info, true);
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
		case NFmiAreaMask::ModifyFactor:
			{
			areaMask = new NFmiCalculationChangeFactorArray;
			break;
			}
		case NFmiAreaMask::Operator:
		case NFmiAreaMask::StartParenthesis:
		case NFmiAreaMask::EndParenthesis:
			{
			areaMask = new NFmiCalculationSpecialCase(theAreaMaskInfo.GetCalculationOperator());
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
			break;
			}
		default:
			throw NFmiSmartToolModifier::Exception("Outo data tyyppi yritettäessä tehdä laskuja.");
	}
	areaMask->SetCalculationOperationType(maskType);

	return areaMask;
}

NFmiAreaMask* NFmiSmartToolModifier::CreateCalculatedAreaMask(const NFmiAreaMaskInfo &theAreaMaskInfo) throw (NFmiSmartToolModifier::Exception)
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

	if(areaMask)
		return areaMask;

	throw NFmiSmartToolModifier::Exception(string("Outo laskettava muuttuja/data tyyppi (ohjelmointi vika?)."));
}

NFmiDataModifier* NFmiSmartToolModifier::CreateIntegrationFuction(const NFmiAreaMaskInfo &theAreaMaskInfo) throw (NFmiSmartToolModifier::Exception)
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
		throw NFmiSmartToolModifier::Exception("Outo integraatio-funktio tyyppi yritettäessä tehdä laskuja.");
	}
	return modifier;
}

NFmiDataIterator* NFmiSmartToolModifier::CreateIterator(const NFmiAreaMaskInfo &theAreaMaskInfo, NFmiSmartInfo* theInfo) throw (NFmiSmartToolModifier::Exception)
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
																   static_cast<int>(p.X()));
				break;
			}
	default:
		throw NFmiSmartToolModifier::Exception("Outo iteraattori-tyyppi yritettäessä tehdä laskuja.");
	}
	return iterator;
}

NFmiAreaMask* NFmiSmartToolModifier::CreateEndingAreaMask(void)
{
	NFmiAreaMask *areaMask = new NFmiCalculationSpecialCase;
	areaMask->SetCalculationOperationType(NFmiAreaMask::EndOfOperations);
	return areaMask;
}

NFmiSmartInfo* NFmiSmartToolModifier::CreateInfo(const NFmiAreaMaskInfo &theAreaMaskInfo) throw (NFmiSmartToolModifier::Exception)
{
	NFmiSmartInfo* info = 0;
	if(theAreaMaskInfo.GetUseDefaultProducer() || theAreaMaskInfo.GetDataType() == NFmiInfoData::kCopyOfEdited)
		info = itsInfoOrganizer->CreateShallowCopyInfo(FmiParameterName(theAreaMaskInfo.GetDataIdent().GetParamIdent()), theAreaMaskInfo.GetLevel(), theAreaMaskInfo.GetDataType());
	else
		info = itsInfoOrganizer->CreateShallowCopyInfo(theAreaMaskInfo.GetDataIdent(), theAreaMaskInfo.GetLevel(), theAreaMaskInfo.GetDataType());
	if(!info)
		throw NFmiSmartToolModifier::Exception("Haluttua parametria ei löytynyt tietokannasta.\n" + theAreaMaskInfo.GetMaskText());
	return info;
}

void NFmiSmartToolModifier::GetParamValueLimits(const NFmiAreaMaskInfo &theAreaMaskInfo, float *theLowerLimit, float *theUpperLimit) throw (NFmiSmartToolModifier::Exception)
{
	NFmiDrawParam* drawParam = itsInfoOrganizer->CreateEmptyInfoDrawParam(FmiParameterName(theAreaMaskInfo.GetDataIdent().GetParamIdent()));
	if(drawParam)
	{
		*theLowerLimit = drawParam->AbsoluteMinValue();
		*theUpperLimit = drawParam->AbsoluteMaxValue();
		delete drawParam;
	}
	else
		throw NFmiSmartToolModifier::Exception("Parametrin min ja max rajoja ei saatu.");
}

