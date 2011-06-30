//**********************************************************
// C++ Class Name : NFmiSmartToolModifier
// ---------------------------------------------------------
//  Author         : pietarin
//  Creation Date  : 8.11. 2010
//**********************************************************
#ifdef _MSC_VER
#pragma warning(disable : 4786) // poistaa n kpl VC++ kääntäjän varoitusta
#endif

#include "NFmiSmartToolModifier.h"
#include "NFmiFastQueryInfo.h"
#include "NFmiSmartInfo.h"
#include "NFmiSmartToolIntepreter.h"
#include "NFmiSmartToolCalculation.h"
#include "NFmiSmartToolCalculationSection.h"
#include "NFmiSmartToolCalculationSectionInfo.h"
#include "NFmiSmartToolCalculationInfo.h"
#include "NFmiAreaMaskInfo.h"
#include "NFmiCalculationConstantValue.h"
#include "NFmiInfoAreaMask.h"
#include "NFmiInfoOrganizer.h"
#include "NFmiAreaMaskSectionInfo.h"
#include "NFmiCalculatedAreaMask.h"
#include "NFmiDataModifierClasses.h"
#include "NFmiRelativeDataIterator.h"
#include "NFmiRelativeTimeIntegrationIterator.h"
#include "NFmiDrawParam.h"
#include "NFmiMetEditorTypes.h"
#include "NFmiQueryData.h"
#include "NFmiInfoAreaMaskSoundingIndex.h"
#include "NFmiDictionaryFunction.h"

#include <stdexcept>

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

NFmiSmartToolCalculationBlockVector::NFmiSmartToolCalculationBlockVector(void)
:itsCalculationBlocks()
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
		(*it)->SetTime(theTime);
}

void NFmiSmartToolCalculationBlockVector::Calculate(const NFmiPoint &theLatlon, unsigned long theLocationIndex, const NFmiMetTime &theTime, int theTimeIndex, NFmiMacroParamValue &theMacroParamValue)
{
	Iterator it = Begin();
	Iterator endIt = End();
	for( ; it != endIt; ++it)
		(*it)->Calculate(theLatlon, theLocationIndex, theTime, theTimeIndex, theMacroParamValue);
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

void NFmiSmartToolCalculationBlock::Calculate(const NFmiPoint &theLatlon, unsigned long theLocationIndex, const NFmiMetTime &theTime, int theTimeIndex, NFmiMacroParamValue &theMacroParamValue)
{
	if(itsFirstCalculationSection)
		itsFirstCalculationSection->Calculate(theLatlon, theLocationIndex, theTime, theTimeIndex, theMacroParamValue);

	if(itsIfAreaMaskSection && itsIfAreaMaskSection->IsMasked(theLatlon, theLocationIndex, theTime, theTimeIndex))
		itsIfCalculationBlocks->Calculate(theLatlon, theLocationIndex, theTime, theTimeIndex, theMacroParamValue);
	else if(itsElseIfAreaMaskSection && itsElseIfAreaMaskSection->IsMasked(theLatlon, theLocationIndex, theTime, theTimeIndex))
		itsElseIfCalculationBlocks->Calculate(theLatlon, theLocationIndex, theTime, theTimeIndex, theMacroParamValue);
	else if(itsElseCalculationBlocks)
		itsElseCalculationBlocks->Calculate(theLatlon, theLocationIndex, theTime, theTimeIndex, theMacroParamValue);

	if(itsLastCalculationSection)
		itsLastCalculationSection->Calculate(theLatlon, theLocationIndex, theTime, theTimeIndex, theMacroParamValue);
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
{
}
NFmiSmartToolModifier::~NFmiSmartToolModifier(void)
{
	delete itsSmartToolIntepreter;
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
	ModifyData(&times, false, true, theMacroParamValue);
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
void NFmiSmartToolModifier::ModifyData(NFmiTimeDescriptor* theModifiedTimes, bool fSelectedLocationsOnly, bool isMacroParamCalculation)
{
	NFmiMacroParamValue macroParamValue;
	ModifyData(theModifiedTimes, fSelectedLocationsOnly, isMacroParamCalculation, macroParamValue);
}

void NFmiSmartToolModifier::ModifyData(NFmiTimeDescriptor* theModifiedTimes, bool fSelectedLocationsOnly, bool isMacroParamCalculation, NFmiMacroParamValue &theMacroParamValue)
{
	itsModifiedTimes = theModifiedTimes;
	fMacroParamCalculation = isMacroParamCalculation;
	fModifySelectedLocationsOnly = fSelectedLocationsOnly;
	try
	{
		// Seed the random-number generator with current time so that
		// the numbers will be different every time we run.
		srand( static_cast<unsigned int>(time( NULL ))); // mahd. satunnais funktion käytön takia, pitää 'sekoittaa' random generaattori

		checkedVector<NFmiSmartToolCalculationBlockInfo>& smartToolCalculationBlockInfos = itsSmartToolIntepreter->SmartToolCalculationBlocks();
		size_t size = smartToolCalculationBlockInfos.size();
		for(size_t i=0; i<size; i++)
		{
			NFmiSmartToolCalculationBlockInfo blockInfo = smartToolCalculationBlockInfos[i];
			boost::shared_ptr<NFmiSmartToolCalculationBlock> block = CreateCalculationBlock(blockInfo);
			if(block)
			{
				ModifyBlockData(block, theMacroParamValue);
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

void NFmiSmartToolModifier::ModifyBlockData(const boost::shared_ptr<NFmiSmartToolCalculationBlock> &theCalculationBlock, NFmiMacroParamValue &theMacroParamValue)
{
	ModifyData2(theCalculationBlock->itsFirstCalculationSection, theMacroParamValue);
	ModifyConditionalData(theCalculationBlock, theMacroParamValue);
	ModifyData2(theCalculationBlock->itsLastCalculationSection, theMacroParamValue);
}

void NFmiSmartToolModifier::ModifyConditionalData(const boost::shared_ptr<NFmiSmartToolCalculationBlock> &theCalculationBlock, NFmiMacroParamValue &theMacroParamValue)
{
	if(theCalculationBlock->itsIfAreaMaskSection && theCalculationBlock->itsIfCalculationBlocks)
	{
		if(theCalculationBlock->FirstVariableInfo() == 0)
			throw runtime_error(::GetDictionaryString("SmartToolModifierErrorUnknownProblem"));
		boost::shared_ptr<NFmiFastQueryInfo> info(dynamic_cast<NFmiFastQueryInfo*>(theCalculationBlock->FirstVariableInfo()->Clone()));

		try
		{
			SetInfosMaskType(info);
			NFmiTimeDescriptor modifiedTimes(itsModifiedTimes ? *itsModifiedTimes : info->TimeDescriptor());
			for(modifiedTimes.Reset(); modifiedTimes.Next(); )
			{
				NFmiMetTime time1(modifiedTimes.Time());
				if(theMacroParamValue.fSetValue)
					time1 = theMacroParamValue.itsTime;
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
					if(theMacroParamValue.fSetValue)
					{
						latlon = theMacroParamValue.itsLatlon;
						info->Location(latlon); // pitää laittaa nearestlocation päälle, että tuloksia voidaan myöhemmin hakea interpolaation avulla
					}
					unsigned long locationIndex = info->LocationIndex(); // tämä locationindex juttu liittyy kai optimointiin, jota ei tehdä enää, pitäisikö poistaa
					int timeIndex = info->TimeIndex();
					if(theCalculationBlock->itsIfAreaMaskSection->IsMasked(latlon, locationIndex, time1, timeIndex))
						theCalculationBlock->itsIfCalculationBlocks->Calculate(latlon, locationIndex, time1, timeIndex, theMacroParamValue);
					else if(theCalculationBlock->itsElseIfAreaMaskSection && theCalculationBlock->itsElseIfCalculationBlocks && theCalculationBlock->itsElseIfAreaMaskSection->IsMasked(latlon, locationIndex, time1, timeIndex))
					{
						theCalculationBlock->itsElseIfCalculationBlocks->Calculate(latlon, locationIndex, time1, timeIndex, theMacroParamValue);
					}
					else if(theCalculationBlock->itsElseCalculationBlocks)
						theCalculationBlock->itsElseCalculationBlocks->Calculate(latlon, locationIndex, time1, timeIndex, theMacroParamValue);
					if(theMacroParamValue.fSetValue)
					{
						return ; // eli jos oli yhden pisteen laskusta kyse, lopetetaan loppi heti
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

void NFmiSmartToolModifier::ModifyData2(boost::shared_ptr<NFmiSmartToolCalculationSection> &theCalculationSection, NFmiMacroParamValue &theMacroParamValue)
{
	if(theCalculationSection && theCalculationSection->FirstVariableInfo())
	{
		boost::shared_ptr<NFmiFastQueryInfo> info(dynamic_cast<NFmiFastQueryInfo*>(theCalculationSection->FirstVariableInfo()->Clone()));
		if(info == 0)
			return ;
		try
		{
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
					NFmiMetTime time1(modifiedTimes.Time());
					if(theMacroParamValue.fSetValue)
						time1 = theMacroParamValue.itsTime;
					if(info->Time(time1)) // asetetaan myös tämä, että saadaan oikea timeindex
					{
						theCalculationSection->SetTime(time1); // yritetään optimoida laskuja hieman kun mahdollista
						for(info->ResetLocation(); info->NextLocation(); )
						{
							NFmiPoint latlon(info->LatLon());
							if(theMacroParamValue.fSetValue)
							{
								latlon = theMacroParamValue.itsLatlon;
								info->Location(latlon); // pitää laittaa nearestlocation päälle, että tuloksia voidaan myöhemmin hakea interpolaation avulla
							}
							// TUON LOCATIONINDEX jutun voisi kai poistaa, kun kyseistä optimointi juttua ei kai enää käytetä
							theCalculationSection->GetCalculations()[i]->Calculate(latlon, info->LocationIndex(), time1, info->TimeIndex(), theMacroParamValue);

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

boost::shared_ptr<NFmiAreaMask> NFmiSmartToolModifier::CreatePeekFunctionAreaMask(const NFmiAreaMaskInfo &theAreaMaskInfo, bool fMustUsePressureInterpolation)
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

boost::shared_ptr<NFmiAreaMask> NFmiSmartToolModifier::CreateMetFunctionAreaMask(const NFmiAreaMaskInfo &theAreaMaskInfo, bool fMustUsePressureInterpolation)
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
	NFmiAreaMask::FunctionType funcType = theAreaMaskInfo.GetFunctionType();
	if(funcType == NFmiAreaMask::Grad)
		areaMask = boost::shared_ptr<NFmiAreaMask>(new NFmiInfoAreaMaskGrad(theAreaMaskInfo.GetMaskCondition(), NFmiAreaMask::kInfo, theAreaMaskInfo.GetDataType(), info, NFmiAreaMask::kNoValue));

	return areaMask;
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
			boost::shared_ptr<NFmiDataModifier> modifier = CreateIntegrationFuction(theAreaMaskInfo);
			boost::shared_ptr<NFmiDataIterator> iterator = CreateIterator(theAreaMaskInfo, info);
			areaMask = boost::shared_ptr<NFmiAreaMask>(new NFmiCalculationIntegrationFuction(iterator, modifier, NFmiAreaMask::kInfo, theAreaMaskInfo.GetDataType(), info));
			areaMask->SetFunctionType(theAreaMaskInfo.GetFunctionType());
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
			if(flightLevelWanted ? (tmpInfo->HeightValueAvailable()) : (tmpInfo->PressureValueAvailable() || tmpInfo->PressureLevelDataAvailable()))
			{
				if(tmpInfo->Param(static_cast<FmiParameterName>(theAreaMaskInfo.GetDataIdent().GetParamIdent())))
				{
					tmpInfo->FirstLevel();
					if(tmpInfo->Level()->GetIdent() == kFmiHybridLevel)
					{ // lähdetään tässä siitä että jos löytyy mallipinta-dataa, mikä sopii tarkoitukseen, se valitaan ensisijaisesti
						info = ::CreateShallowCopyOfHighestInfo(tmpInfo);
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
		info = ::CreateShallowCopyOfHighestInfo(possiblePressureLevelDataInfo);
	if(info)
		mustUsePressureInterpolation = true;
	return info;
}

boost::shared_ptr<NFmiFastQueryInfo> NFmiSmartToolModifier::CreateCopyOfAnalyzeInfo(const NFmiDataIdent& theDataIdent, const NFmiLevel* theLevel)
{
	boost::shared_ptr<NFmiFastQueryInfo> info = itsInfoOrganizer->Info(theDataIdent, theLevel, NFmiInfoData::kAnalyzeData);
	if(info)
	{
		if(info->Param(static_cast<FmiParameterName>(theDataIdent.GetParamIdent())) && (theLevel == 0 || info->Level(*theLevel)))
			return ::CreateShallowCopyOfHighestInfo(info);
	}
	return info;
}

boost::shared_ptr<NFmiFastQueryInfo> NFmiSmartToolModifier::CreateInfo(const NFmiAreaMaskInfo &theAreaMaskInfo, bool &mustUsePressureInterpolation)
{
	mustUsePressureInterpolation = false;
	boost::shared_ptr<NFmiFastQueryInfo> info;
	if(theAreaMaskInfo.GetDataType() == NFmiInfoData::kScriptVariableData)
		info = CreateScriptVariableInfo(theAreaMaskInfo.GetDataIdent());
	else if(theAreaMaskInfo.GetUseDefaultProducer() || theAreaMaskInfo.GetDataType() == NFmiInfoData::kCopyOfEdited)
	{
		NFmiInfoData::Type dataType = theAreaMaskInfo.GetDataType();
		if(fDoCrossSectionCalculation && dataType == NFmiInfoData::kMacroParam)
			dataType = NFmiInfoData::kCrossSectionMacroParam;
		if(theAreaMaskInfo.GetDataType() == NFmiInfoData::kMacroParam)
		{ // tämä macroParam data viritys on multi threaddaavaa serveriä varten, eli macroparam data pitää olla thread-kohtainen
			// ja se on aina annettu luodulle NFmiSmartToolModifier-luokan instansille erikseen.
			if(UsedMacroParamData())
				info = ::CreateShallowCopyOfHighestInfo(UsedMacroParamData());
			else
				throw runtime_error("NFmiSmartToolModifier::CreateInfo - error in program, no macroParam data available.");
		}
		else
			info = ::CreateShallowCopyOfHighestInfo(itsInfoOrganizer->Info(theAreaMaskInfo.GetDataIdent(), theAreaMaskInfo.GetLevel(), dataType, true, fUseLevelData));
		if(info == 0)
			info = GetPossibleLevelInterpolatedInfo(theAreaMaskInfo, mustUsePressureInterpolation);
	}
	else
	{
		if(fUseLevelData && theAreaMaskInfo.GetLevel() != 0) // jos pitää käyttää level dataa (SumZ ja MinH funktiot), ei saa antaa level infoa parametrin yhteydessä
			throw runtime_error(::GetDictionaryString("SmartToolModifierErrorParamNoLevel") + "\n" + theAreaMaskInfo.GetMaskText());
		if(fUseLevelData || fDoCrossSectionCalculation) // jos leveldata-flagi päällä, yritetään ensin, löytyykö hybridi dataa
			info = ::CreateShallowCopyOfHighestInfo(itsInfoOrganizer->Info(theAreaMaskInfo.GetDataIdent(), theAreaMaskInfo.GetLevel(), NFmiInfoData::kHybridData, false, fUseLevelData | fDoCrossSectionCalculation)); // tähän pieni hybrid-koukku, jos haluttiin level dataa
		if(info == 0)
		{
			info = ::CreateShallowCopyOfHighestInfo(itsInfoOrganizer->Info(theAreaMaskInfo.GetDataIdent(), theAreaMaskInfo.GetLevel(), theAreaMaskInfo.GetDataType(), false, fUseLevelData | fDoCrossSectionCalculation));
		}
		if(info == 0 && theAreaMaskInfo.GetDataType() == NFmiInfoData::kAnalyzeData) // analyysi datalle piti tehdä pika viritys tähän
			info = CreateCopyOfAnalyzeInfo(theAreaMaskInfo.GetDataIdent(), theAreaMaskInfo.GetLevel());
		if(info == 0)
			info = GetPossibleLevelInterpolatedInfo(theAreaMaskInfo, mustUsePressureInterpolation);
		if(info == 0 && theAreaMaskInfo.GetLevel() != 0) // kokeillaan vielä jos halutaan hybridi datan leveliä
		{
			NFmiLevel aLevel(*theAreaMaskInfo.GetLevel());
			aLevel.SetIdent(kFmiHybridLevel);
			info = ::CreateShallowCopyOfHighestInfo(itsInfoOrganizer->Info(theAreaMaskInfo.GetDataIdent(), &aLevel, NFmiInfoData::kHybridData, false, fUseLevelData));
		}
		if(info == 0 && theAreaMaskInfo.GetLevel() != 0) // kokeillaan vielä jos halutaan 'height' (type 105) datan leveliä
		{
			NFmiLevel aLevel(*theAreaMaskInfo.GetLevel());
			aLevel.SetIdent(kFmiHeight);
			info = ::CreateShallowCopyOfHighestInfo(itsInfoOrganizer->Info(theAreaMaskInfo.GetDataIdent(), &aLevel, NFmiInfoData::kViewable, false, fUseLevelData));
		}
	}
	if(!info)
		throw runtime_error(::GetDictionaryString("SmartToolModifierErrorParamNotFound") + "\n" + theAreaMaskInfo.GetMaskText());
	return info;
}

void NFmiSmartToolModifier::GetParamValueLimits(const NFmiAreaMaskInfo &theAreaMaskInfo, float *theLowerLimit, float *theUpperLimit, bool *fCheckLimits)
{
	if(theAreaMaskInfo.GetDataIdent().GetParamIdent() == kFmiTotalWindMS || theAreaMaskInfo.GetDataIdent().GetParamIdent() == kFmiWeatherAndCloudiness)
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

