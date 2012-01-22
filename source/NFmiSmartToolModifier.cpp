//**********************************************************
// C++ Class Name : NFmiSmartToolModifier
// ---------------------------------------------------------
//  Author         : pietarin
//  Creation Date  : 8.11. 2010
//**********************************************************
#ifdef _MSC_VER
#pragma warning(disable : 4786) // poistaa n kpl VC++ k��nt�j�n varoitusta
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
#include "NFmiQueryDataUtil.h"

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
		if((*it)->FirstVariableInfo()) // pit�isi l�yty� aina jotain!!!
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

void NFmiSmartToolCalculationBlock::Calculate_ver2(const NFmiCalculationParams &theCalculationParams)
{
	if(itsFirstCalculationSection)
		itsFirstCalculationSection->Calculate_ver2(theCalculationParams);

	if(itsIfAreaMaskSection && itsIfAreaMaskSection->IsMasked(theCalculationParams))
		itsIfCalculationBlocks->Calculate_ver2(theCalculationParams);
	else if(itsElseIfAreaMaskSection && itsElseIfAreaMaskSection->IsMasked(theCalculationParams))
		itsElseIfCalculationBlocks->Calculate_ver2(theCalculationParams);
	else if(itsElseCalculationBlocks)
		itsElseCalculationBlocks->Calculate_ver2(theCalculationParams);

	if(itsLastCalculationSection)
		itsLastCalculationSection->Calculate_ver2(theCalculationParams);
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
#ifdef FMI_SUPPORT_STATION_DATA_SMARTTOOL
,itsDoc(0)
#endif // FMI_SUPPORT_STATION_DATA_SMARTTOOL
{
}
NFmiSmartToolModifier::~NFmiSmartToolModifier(void)
{
	delete itsSmartToolIntepreter;
}
//--------------------------------------------------------
// InitSmartTool
//--------------------------------------------------------
// Tulkitsee macron, luo tavittavat systeemit, ett� makro voidaan suorittaa.
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
// HUOM!!!! editoitavaN DATAN QDatasta pit�� tehd� kopiot, muuten maskit eiv�t toimi
// kaikissa tilanteissa oikein!! KORJAA T�M�!!!!!
				areaMaskHandler->AddCalculation(CreateAreaMask(*maskInfos[i]));
			// loppuun lis�t��n viel� lopetus 'merkki'
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
		bool mustUsePressureInterpolation = false; // t�t�ei k�ytet� t�ss�, mutta pakko laittaa metodin interfacen takia
		calculation->SetResultInfo(CreateInfo(*theCalcInfo->GetResultDataInfo(), mustUsePressureInterpolation));
		if(calculation->GetResultInfo() && calculation->GetResultInfo()->Grid())
			itsWorkingGrid = boost::shared_ptr<MyGrid>(new MyGrid(*calculation->GetResultInfo()->Grid())); // t�m� ty�skentely alueen hila ja area otettava talteen

		float lowerLimit = kFloatMissing;
		float upperLimit = kFloatMissing;
		bool checkLimits = true; // yleens� parametreille k�ytetd��n min/max rajoja, mutta ei esim TotalWind tai W&C:lle
		GetParamValueLimits(*theCalcInfo->GetResultDataInfo(), &lowerLimit, &upperLimit, &checkLimits);
		calculation->SetLimits(lowerLimit, upperLimit, checkLimits);
		calculation->AllowMissingValueAssignment(theCalcInfo->AllowMissingValueAssignment());
		for(size_t i=0; i<size; i++)
		{
			if(areaMaskInfos[i] != 0)
				calculation->AddCalculation(CreateAreaMask(*areaMaskInfos[i])); // HUOM! T�H�N KAATUU JOSKUS, TUTKI ASIAA!!!!!
			else
			{
				string errStr("Error in application: NFmiSmartToolModifier::CreateCalculation - zero pointer error with calculation\n");
				errStr += theCalcInfo->GetCalculationText();
				errStr += "\nNotify application developer with this.";
				throw runtime_error(errStr);
			}
		}
		// loppuun lis�t��n viel� loputus 'merkki'
		calculation->AddCalculation(CreateEndingAreaMask());
		return calculation;
	}
	return calculation;
}

// Laskee smarttool-systeemin avulla halutun poikkileikkauksen arvo-matriisin.
// T�m� on universaali metodi, joka hoitaa kaikki kolme tapausta: poikkileikkaus, aika-poikkileikkaus ja reitti-poikkileikkaus.
// Parametreina annetaa aina sopiva pisteet ja ajat, ett� saadaan eri efektit aikaan. Eli aika-poikkileikkauksessa
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

	// lasketaan l�pi yksitt�isi� arvoja kaikille halutuille pisteille
	for(size_t i = 0; i<sizeX; i++)
	{
		macroParamValue.itsLatlon = theLatlonPoints[i];
		macroParamValue.itsTime = thePointTimes[i];
		NFmiTimeBag timeBag(macroParamValue.itsTime, macroParamValue.itsTime, 60);
		NFmiTimeDescriptor times(macroParamValue.itsTime, timeBag);
		itsInfoOrganizer->CrossSectionMacroParamData()->SetTimeDescriptor(times); // asetetaan makroData-infon aikasysteemi currentin kartan kohtaan (feikki datassa vain yksi aika ja se pit�� s��t�� kohdalleen, ett� laskut onnistuvat)
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
	// oikeasti t�ss� ei modifioida mit��n, vaan palautetaan vain yksi arvo
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
// Suorittaa varsinaiset modifikaatiot. K�ytt�j� voi antaa parametrina rajoitetun ajan
// modifioinneille, jos theModifiedTimes on 0-pointteri, tehd��n operaatiot kaikille
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
		ClearScriptVariableInfos(); // lopuksi n�m� skripti-muuttujat tyhjennet��n
	}
	catch(...)
	{
		ClearScriptVariableInfos(); // lopuksi n�m� skripti-muuttujat tyhjennet��n
		fMacroRunnable = false;
		throw ;
	}
}

void NFmiSmartToolModifier::ModifyData_ver2(NFmiTimeDescriptor* theModifiedTimes, bool fSelectedLocationsOnly, bool isMacroParamCalculation)
{
	itsModifiedTimes = theModifiedTimes;
	fMacroParamCalculation = isMacroParamCalculation;
	fModifySelectedLocationsOnly = fSelectedLocationsOnly;
	try
	{
		checkedVector<NFmiSmartToolCalculationBlockInfo>& smartToolCalculationBlockInfos = itsSmartToolIntepreter->SmartToolCalculationBlocks();
		size_t size = smartToolCalculationBlockInfos.size();
		for(size_t i=0; i<size; i++)
		{
			NFmiSmartToolCalculationBlockInfo blockInfo = smartToolCalculationBlockInfos[i];
			boost::shared_ptr<NFmiSmartToolCalculationBlock> block = CreateCalculationBlock(blockInfo);
			if(block)
			{
				ModifyBlockData_ver2(block);
			}
		}
		ClearScriptVariableInfos(); // lopuksi n�m� skripti-muuttujat tyhjennet��n
	}
	catch(...)
	{
		ClearScriptVariableInfos(); // lopuksi n�m� skripti-muuttujat tyhjennet��n
		fMacroRunnable = false;
		throw ;
	}
}

// Kun intepreter on tulkinnut smarttool-tekstin, voidaan kysy�, onko kyseinen makro ns. macroParam-skripti eli sis�lt��k� se RESULT = ??? tapaista teksti�
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

void NFmiSmartToolModifier::ModifyBlockData_ver2(const boost::shared_ptr<NFmiSmartToolCalculationBlock> &theCalculationBlock)
{
	ModifyData2_ver2(theCalculationBlock->itsFirstCalculationSection);
	ModifyConditionalData_ver2(theCalculationBlock);
	ModifyData2_ver2(theCalculationBlock->itsLastCalculationSection);
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
			NFmiCalculationParams calculationParams;
			SetInfosMaskType(info);
			NFmiTimeDescriptor modifiedTimes(itsModifiedTimes ? *itsModifiedTimes : info->TimeDescriptor());
			for(modifiedTimes.Reset(); modifiedTimes.Next(); )
			{
				calculationParams.itsTime = modifiedTimes.Time();
				if(theMacroParamValue.fSetValue)
					calculationParams.itsTime = theMacroParamValue.itsTime;
				info->Time(calculationParams.itsTime); // asetetaan my�s t�m�, ett� saadaan oikea timeindex
				calculationParams.itsTimeIndex = info->TimeIndex();
				theCalculationBlock->itsIfAreaMaskSection->SetTime(calculationParams.itsTime); // yritet��n optimoida laskuja hieman kun mahdollista
				theCalculationBlock->itsIfCalculationBlocks->SetTime(calculationParams.itsTime); // yritet��n optimoida laskuja hieman kun mahdollista
				if(theCalculationBlock->itsElseIfAreaMaskSection && theCalculationBlock->itsElseIfCalculationBlocks)
				{
					theCalculationBlock->itsElseIfAreaMaskSection->SetTime(calculationParams.itsTime);
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
						info->Location(calculationParams.itsLatlon); // pit�� laittaa nearestlocation p��lle, ett� tuloksia voidaan my�hemmin hakea interpolaation avulla
					}
					calculationParams.itsLocationIndex = info->LocationIndex(); // t�m� locationindex juttu liittyy kai optimointiin, jota ei tehd� en��, pit�isik� poistaa
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
		catch(...)
		{
			throw ;
		}
	}
}

void NFmiSmartToolModifier::ModifyConditionalData_ver2(const boost::shared_ptr<NFmiSmartToolCalculationBlock> &theCalculationBlock)
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
				calculationParams.itsTime = modifiedTimes.Time();
				info->Time(calculationParams.itsTime); // asetetaan my�s t�m�, ett� saadaan oikea timeindex
				calculationParams.itsTimeIndex = info->TimeIndex();
				theCalculationBlock->itsIfAreaMaskSection->SetTime(calculationParams.itsTime); // yritet��n optimoida laskuja hieman kun mahdollista
				theCalculationBlock->itsIfCalculationBlocks->SetTime(calculationParams.itsTime); // yritet��n optimoida laskuja hieman kun mahdollista
				if(theCalculationBlock->itsElseIfAreaMaskSection && theCalculationBlock->itsElseIfCalculationBlocks)
				{
					theCalculationBlock->itsElseIfAreaMaskSection->SetTime(calculationParams.itsTime);
					theCalculationBlock->itsElseIfCalculationBlocks->SetTime(calculationParams.itsTime);
				}
				if(theCalculationBlock->itsElseCalculationBlocks)
					theCalculationBlock->itsElseCalculationBlocks->SetTime(calculationParams.itsTime);

				for(info->ResetLocation(); info->NextLocation(); )
				{
					calculationParams.itsLatlon = info->LatLon();
					calculationParams.itsLocationIndex = info->LocationIndex(); // t�m� locationindex juttu liittyy kai optimointiin, jota ei tehd� en��, pit�isik� poistaa
					if(theCalculationBlock->itsIfAreaMaskSection->IsMasked(calculationParams))
						theCalculationBlock->itsIfCalculationBlocks->Calculate_ver2(calculationParams);
					else if(theCalculationBlock->itsElseIfAreaMaskSection && theCalculationBlock->itsElseIfCalculationBlocks && theCalculationBlock->itsElseIfAreaMaskSection->IsMasked(calculationParams))
					{
						theCalculationBlock->itsElseIfCalculationBlocks->Calculate_ver2(calculationParams);
					}
					else if(theCalculationBlock->itsElseCalculationBlocks)
						theCalculationBlock->itsElseCalculationBlocks->Calculate_ver2(calculationParams);
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
		{ // skripti muuttujalle pit�� asettaa sama valittujen pisteiden maski kuin on editoidulla datalla
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
			NFmiCalculationParams calculationParams;
			SetInfosMaskType(info);
			NFmiTimeDescriptor modifiedTimes(itsModifiedTimes ? *itsModifiedTimes : info->TimeDescriptor());

			// Muutin lasku systeemin suoritusta, koska tuli ongelmia mm. muuttujien kanssa, kun niit� k�ytettiin samassa calculationSectionissa
			// CalculationSection = lasku rivej� per�kk�in esim.
			// T = T + 1
			// P = P + 1
			// jne. ilman IF-lauseita
			// ENNEN laskettiin t�ll�inen sectio siten ett� k�ytiin l�pi koko sectio samalla paikalla ja ajalla ja sitten siirryttiin eteenp�in.
			// NYT lasketaan aina yksi laskurivi l�pi kaikkien aikojen ja paikkojen, ja sitten siirryt��n seuraavalle lasku riville.
			size_t size = theCalculationSection->GetCalculations().size();
			for(size_t i=0; i<size; i++)
			{
				for(modifiedTimes.Reset(); modifiedTimes.Next(); )
				{
					calculationParams.itsTime = modifiedTimes.Time();
					if(theMacroParamValue.fSetValue)
						calculationParams.itsTime = theMacroParamValue.itsTime;
					if(info->Time(calculationParams.itsTime)) // asetetaan my�s t�m�, ett� saadaan oikea timeindex
					{
						theCalculationSection->SetTime(calculationParams.itsTime); // yritet��n optimoida laskuja hieman kun mahdollista
						for(info->ResetLocation(); info->NextLocation(); )
						{
							calculationParams.itsLatlon = info->LatLon();
							if(theMacroParamValue.fSetValue)
							{
								calculationParams.itsLatlon = theMacroParamValue.itsLatlon;
								info->Location(calculationParams.itsLatlon); // pit�� laittaa nearestlocation p��lle, ett� tuloksia voidaan my�hemmin hakea interpolaation avulla
							}
							calculationParams.itsLocationIndex = info->LocationIndex();
							// TUON LOCATIONINDEX jutun voisi kai poistaa, kun kyseist� optimointi juttua ei kai en�� k�ytet�
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

void NFmiSmartToolModifier::ModifyData2_ver2(boost::shared_ptr<NFmiSmartToolCalculationSection> &theCalculationSection)
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

			// Muutin lasku systeemin suoritusta, koska tuli ongelmia mm. muuttujien kanssa, kun niit� k�ytettiin samassa calculationSectionissa
			// CalculationSection = lasku rivej� per�kk�in esim.
			// T = T + 1
			// P = P + 1
			// jne. ilman IF-lauseita
			// ENNEN laskettiin t�ll�inen sectio siten ett� k�ytiin l�pi koko sectio samalla paikalla ja ajalla ja sitten siirryttiin eteenp�in.
			// NYT lasketaan aina yksi laskurivi l�pi kaikkien aikojen ja paikkojen, ja sitten siirryt��n seuraavalle lasku riville.
			size_t size = theCalculationSection->GetCalculations().size();
			for(size_t i=0; i<size; i++)
			{
				for(modifiedTimes.Reset(); modifiedTimes.Next(); )
				{
					calculationParams.itsTime = modifiedTimes.Time();
					if(info->Time(calculationParams.itsTime)) // asetetaan my�s t�m�, ett� saadaan oikea timeindex
					{
						theCalculationSection->SetTime(calculationParams.itsTime); // yritet��n optimoida laskuja hieman kun mahdollista
						for(info->ResetLocation(); info->NextLocation(); )
						{
							calculationParams.itsLatlon = info->LatLon();
							calculationParams.itsLocationIndex = info->LocationIndex();
							// TUON LOCATIONINDEX jutun voisi kai poistaa, kun kyseist� optimointi juttua ei kai en�� k�ytet�
							theCalculationSection->GetCalculations()[i]->Calculate_ver2(calculationParams);
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

boost::shared_ptr<NFmiAreaMask> NFmiSmartToolModifier::CreatePeekFunctionAreaMask(const NFmiAreaMaskInfo &theAreaMaskInfo, bool &fMustUsePressureInterpolation)
{
	boost::shared_ptr<NFmiAreaMask> areaMask;
	// HUOM!! T�h�n vaaditaan syv� data kopio!!!
	// JOS kyseess� on ehtolauseen muuttujasta, joka on editoitavaa dataa.
	boost::shared_ptr<NFmiFastQueryInfo> info = CreateInfo(theAreaMaskInfo, fMustUsePressureInterpolation);
	if(theAreaMaskInfo.GetUseDefaultProducer())
	{ // Pit�� tehd� syv� kopio datasta, ett� datan muuttuminen ei vaikuta laskuihin.
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
	// HUOM!! T�h�n vaaditaan syv� data kopio!!!
	// JOS kyseess� on ehtolauseen muuttujasta, joka on editoitavaa dataa.
	boost::shared_ptr<NFmiFastQueryInfo> info = CreateInfo(theAreaMaskInfo, fMustUsePressureInterpolation);
	if(info == 0)
		DoErrorExceptionForMetFunction(theAreaMaskInfo, ::GetDictionaryString("Can't find wanted parameter for given function"), ::GetDictionaryString("with line"));

	if(theAreaMaskInfo.GetUseDefaultProducer())
	{ // Pit�� tehd� syv� kopio datasta, ett� datan muuttuminen ei vaikuta laskuihin.
		boost::shared_ptr<NFmiFastQueryInfo> tmp(dynamic_cast<NFmiFastQueryInfo*>(info->Clone()));
		info = tmp;
	}
	NFmiAreaMask::FunctionType funcType = theAreaMaskInfo.GetFunctionType();
	if(funcType == NFmiAreaMask::Grad || funcType == NFmiAreaMask::Divergence)
	{
		areaMask = boost::shared_ptr<NFmiAreaMask>(new NFmiInfoAreaMaskGrad(theAreaMaskInfo.GetMaskCondition(), NFmiAreaMask::kInfo, theAreaMaskInfo.GetDataType(), info, NFmiAreaMask::kNoValue));
		if(funcType == NFmiAreaMask::Divergence)
			dynamic_cast<NFmiInfoAreaMaskGrad*>(areaMask.get())->CalculateDivergence(true);
	}
	else if(funcType == NFmiAreaMask::Adv)
	{
		boost::shared_ptr<NFmiFastQueryInfo> infoUwind = CreateInfo(theAreaMaskInfo, fMustUsePressureInterpolation, kFmiWindUMS);
		boost::shared_ptr<NFmiFastQueryInfo> infoVwind = CreateInfo(theAreaMaskInfo, fMustUsePressureInterpolation, kFmiWindVMS);
		if(infoUwind && infoVwind)
			areaMask = boost::shared_ptr<NFmiAreaMask>(new NFmiInfoAreaMaskAdvection(theAreaMaskInfo.GetMaskCondition(), NFmiAreaMask::kInfo, theAreaMaskInfo.GetDataType(), info, infoUwind, infoVwind, NFmiAreaMask::kNoValue));
		else
			DoErrorExceptionForMetFunction(theAreaMaskInfo, ::GetDictionaryString("Can't find u- or -v wind components for wanted parameter in given function"), ::GetDictionaryString("with line"));
	}
	else if(funcType == NFmiAreaMask::Lap)
		areaMask = boost::shared_ptr<NFmiAreaMask>(new NFmiInfoAreaMaskLaplace(theAreaMaskInfo.GetMaskCondition(), NFmiAreaMask::kInfo, theAreaMaskInfo.GetDataType(), info, NFmiAreaMask::kNoValue));
	else if(funcType == NFmiAreaMask::Rot)
	{
		if(theAreaMaskInfo.GetDataIdent().GetParamIdent() == kFmiTotalWindMS)
			areaMask = boost::shared_ptr<NFmiAreaMask>(new NFmiInfoAreaMaskRotor(theAreaMaskInfo.GetMaskCondition(), NFmiAreaMask::kInfo, theAreaMaskInfo.GetDataType(), info, NFmiAreaMask::kNoValue));
		else
			DoErrorExceptionForMetFunction(theAreaMaskInfo, ::GetDictionaryString("Only usable param with rot-function in wind (=par19)"), ::GetDictionaryString("in the line"));
	}
	else
		DoErrorExceptionForMetFunction(theAreaMaskInfo, ::GetDictionaryString("SmartMet program error with Met-function"), ::GetDictionaryString("error with line"));

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
			// HUOM!! T�h�n vaaditaan syv� data kopio!!!
			// JOS kyseess� on ehtolauseen muuttujasta, joka on editoitavaa dataa.
			boost::shared_ptr<NFmiFastQueryInfo> info = CreateInfo(theAreaMaskInfo, mustUsePressureInterpolation);
			// HUOM!!! pit�isik� t�h�n laittaa joku debug-info raportti, jos level on reset-tilassa

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
			// HUOM!! T�h�n vaaditaan syv� data kopio!!!
			// JOS kyseess� on ehtolauseen muuttujasta, joka on editoitavaa dataa.
			boost::shared_ptr<NFmiFastQueryInfo> info = CreateInfo(theAreaMaskInfo, mustUsePressureInterpolation);
			if(theAreaMaskInfo.GetUseDefaultProducer())
			{ // Pit�� tehd� syv� kopio datasta, ett� datan muuttuminen ei vaikuta laskuihin.
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
				{  // kun pilkku-laskuri tuli t�yteen
					fUseLevelData = true; // on aika ruveta k�ytt�m��n level-dataa infoissa
					itsParethesisCounter = 1; // lis�ksi ruvetaan mets�st�m��n sulkuja,
											  // ett� tiedet��n milloin funktio ja level datan k�ytt� loppuu
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
			{ // jos funktio oli SumZ tai MinH tyyppinen, laitetaan seuraavat jutut 'p��lle'
				fHeightFunctionFlag = true;
				fUseLevelData = false;
				itsCommaCounter = 0;
			}
			break;
			}
		case NFmiAreaMask::VertFunctionStart:
			{
			fUseLevelData = true;
			boost::shared_ptr<NFmiFastQueryInfo> info = CreateInfo(theAreaMaskInfo, mustUsePressureInterpolation);
			areaMask = boost::shared_ptr<NFmiAreaMask>(new NFmiInfoAreaMaskVertFunc(theAreaMaskInfo.GetMaskCondition(), NFmiAreaMask::kInfo, info->DataType(), info, theAreaMaskInfo.GetFunctionType(), theAreaMaskInfo.GetSecondaryFunctionType(), theAreaMaskInfo.FunctionArgumentCount()));
			fUseLevelData = false; // en tied� pit��k� t�m� laittaa takaisin falseksi, mutta laitan varmuuden vuoksi
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
		{ // jos oli info dataa ja viel� asemadatasta, tarkistetaan ett� kyse oli viel� infoData-tyypist�, muuten oli virheellinen lauseke
#ifdef FMI_SUPPORT_STATION_DATA_SMARTTOOL
			if(maskType == NFmiAreaMask::InfoVariable)
			{
				NFmiStation2GridMask *station2GridMask = new NFmiStation2GridMask(areaMask->MaskType(), areaMask->GetDataType(), areaMask->Info());
				station2GridMask->SetGriddingHelpers(itsWorkingGrid->itsArea, itsDoc, NFmiPoint(itsWorkingGrid->itsNX, itsWorkingGrid->itsNY));
				areaMask = boost::shared_ptr<NFmiAreaMask>(station2GridMask);
				if(areaMask->Info()->LevelType() == kFmiSoundingLevel) 
				{ // Luotaus data on poikkeus, jonka haluttu painepinta level pit�� asettaa t�ss� erikseen.
					// Lis�ksi levelType pit�� vaihtaa pressuresta kFmiSoundingLevel!
					NFmiLevel soundingLevel(kFmiSoundingLevel, theAreaMaskInfo.GetLevel()->GetName(), theAreaMaskInfo.GetLevel()->LevelValue());
					areaMask->Level(soundingLevel);
				}
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

		areaMask->Initialize(); // virtuaalinen initialisointi konstruktion j�lkeen
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
	// HUOM!! T�h�n vaaditaan syv� data kopio!!!
	// JOS kyseess� on ehtolauseen muuttujasta, joka on editoitavaa dataa.
	bool mustUsePressureInterpolation = false; // t�t�ei k�ytet� t�ss�, mutta pakko laittaa metodin interfacen takia
	boost::shared_ptr<NFmiFastQueryInfo> info = CreateInfo(theAreaMaskInfo, mustUsePressureInterpolation);
	if(theAreaMaskInfo.GetUseDefaultProducer())
	{ // Pit�� tehd� syv� kopio datasta, ett� datan muuttuminen ei vaikuta laskuihin.
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

// Muista jos tulee p�ivityksi�, smanlainen funktio l�ytyy my�s NFmiSmartToolCalculation-luokasta
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
		// HUOM!!!! Tee WAvg-modifier my�s, joka on peritty Avg-modifierist� ja tee joku kerroin juttu painotukseen.
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
			// HUOM!! NFmiRelativeDataIterator:iin pit�� tehd� joustavampi 'laatikon' s��t� systeemi, ett� laatikko ei olisi aina keskitetty
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
					{ // l�hdet��n t�ss� siit� ett� jos l�ytyy mallipinta-dataa, mik� sopii tarkoitukseen, se valitaan ensisijaisesti
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
	if(info == 0 && possiblePressureLevelDataInfo != 0) // jos ei l�ytynyt sopivaa mallipinta-dataa, mutta painepinta-dataa l�ytyi, otetaan se k�ytt��n
		info = possiblePressureLevelDataInfo;
	if(info)
	{
		// Tarkistetaan viel� haluttiinko vanhaa malliajodataa
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

boost::shared_ptr<NFmiFastQueryInfo> NFmiSmartToolModifier::GetWantedAreaMaskData(const NFmiAreaMaskInfo &theAreaMaskInfo, bool fUseParIdOnly, NFmiInfoData::Type theOverRideDataType, FmiLevelType theOverRideLevelType)
{
	NFmiInfoData::Type usedDataType = theAreaMaskInfo.GetDataType();
	if(theOverRideDataType != NFmiInfoData::kNoDataType)
		usedDataType = theOverRideDataType;

	boost::shared_ptr<NFmiFastQueryInfo> info;
	if(theOverRideLevelType == kFmiNoLevelType)
		info = itsInfoOrganizer->Info(theAreaMaskInfo.GetDataIdent(), theAreaMaskInfo.GetLevel(), usedDataType, fUseParIdOnly, fUseLevelData | fDoCrossSectionCalculation, theAreaMaskInfo.ModelRunIndex());
	else
	{
		if(theAreaMaskInfo.GetLevel()) // level voi olla 0-pointteri, joten se pit�� tarkistaa
		{
			NFmiLevel aLevel(*theAreaMaskInfo.GetLevel());
			aLevel.SetIdent(theOverRideLevelType);
			info = itsInfoOrganizer->Info(theAreaMaskInfo.GetDataIdent(), &aLevel, usedDataType, fUseParIdOnly, fUseLevelData, theAreaMaskInfo.ModelRunIndex());
		}
	}
	return ::CreateShallowCopyOfHighestInfo(info); // tehd��n viel� 'kevyt' kopio l�ytyneest� datasta
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
		{ // t�m� macroParam data viritys on multi threaddaavaa serveri� varten, eli macroparam data pit�� olla thread-kohtainen
			// ja se on aina annettu luodulle NFmiSmartToolModifier-luokan instansille erikseen.
			if(UsedMacroParamData())
				info = ::CreateShallowCopyOfHighestInfo(UsedMacroParamData());
			else
				throw runtime_error("NFmiSmartToolModifier::CreateInfo - error in program, no macroParam data available.");
		}
		else
			info = GetWantedAreaMaskData(theAreaMaskInfo, true);
		if(info == 0)
			info = GetPossibleLevelInterpolatedInfo(theAreaMaskInfo, mustUsePressureInterpolation);
	}
	else
	{
		if(fUseLevelData && theAreaMaskInfo.GetLevel() != 0) // jos pit�� k�ytt�� level dataa (SumZ ja MinH funktiot), ei saa antaa level infoa parametrin yhteydess�
			throw runtime_error(::GetDictionaryString("SmartToolModifierErrorParamNoLevel") + "\n" + theAreaMaskInfo.GetMaskText());
		if(fUseLevelData || fDoCrossSectionCalculation) // jos leveldata-flagi p��ll�, yritet��n ensin, l�ytyyk� hybridi dataa
			info = GetWantedAreaMaskData(theAreaMaskInfo, false, NFmiInfoData::kHybridData);
		if(info == 0)
			info = GetWantedAreaMaskData(theAreaMaskInfo, false);
		if(info == 0 && theAreaMaskInfo.GetDataType() == NFmiInfoData::kAnalyzeData) // analyysi datalle piti tehd� pika viritys t�h�n
			info = CreateCopyOfAnalyzeInfo(theAreaMaskInfo.GetDataIdent(), theAreaMaskInfo.GetLevel());
		if(info == 0)
			info = GetPossibleLevelInterpolatedInfo(theAreaMaskInfo, mustUsePressureInterpolation);
		if(info == 0 && theAreaMaskInfo.GetLevel() != 0) // kokeillaan viel� jos halutaan hybridi datan leveli�
			info = GetWantedAreaMaskData(theAreaMaskInfo, false, NFmiInfoData::kHybridData, kFmiHybridLevel);
		if(info == 0 && theAreaMaskInfo.GetLevel() != 0) // kokeillaan viel� jos halutaan 'height' (type 105) datan leveli�
			info = GetWantedAreaMaskData(theAreaMaskInfo, false, NFmiInfoData::kViewable, kFmiHeight);

		if(info == 0) // kokeillaan viel� model-help-dataa (esim. EPS-data, sounding-index-data jne)
			info = GetWantedAreaMaskData(theAreaMaskInfo, false, NFmiInfoData::kModelHelpData);
		if(info == 0) // kokeillaan viel� analyysi-dataa (esim. mesan, LAPS jne) HUOM! t�m� on eri asia kuin edella oli analyysi-dataa. t�ss� on pyydetty PAR_PRODid yhdistelm�ll� analyysi-dataa
			info = GetWantedAreaMaskData(theAreaMaskInfo, false, NFmiInfoData::kAnalyzeData);
		if(info == 0) // kokeillaan viel� fraktiili-dataa (esim. EC:n fraktiili dataa jne)
			info = GetWantedAreaMaskData(theAreaMaskInfo, false, NFmiInfoData::kClimatologyData);

#ifdef FMI_SUPPORT_STATION_DATA_SMARTTOOL
		if(info == 0) // kokeillaan viel� havainto dataa (eli ne on yleens� asemadataa)
			info = GetWantedAreaMaskData(theAreaMaskInfo, false, NFmiInfoData::kObservations);
		if(info == 0) // kokeillaan viel� eri level vaihtoehtoja
			info = GetWantedAreaMaskData(theAreaMaskInfo, false, NFmiInfoData::kObservations, kFmiHeight);
		if(info == 0) // kokeillaan viel� luotaus datan leveltyyppi
			info = GetWantedAreaMaskData(theAreaMaskInfo, false, NFmiInfoData::kObservations, kFmiSoundingLevel);
		if(info == 0) // kokeillaan viel� yksitt�isten tutkien dataa level moodissa
			info = GetWantedAreaMaskData(theAreaMaskInfo, false, NFmiInfoData::kSingleStationRadarData, kFmiPressureLevel);
		if(info == 0) // kokeillaan viel� yksitt�isten tutkien dataa ilman level moodia (esim. vertikaali-funktioiden yhteydess�)
			info = GetWantedAreaMaskData(theAreaMaskInfo, false, NFmiInfoData::kSingleStationRadarData);

#endif // FMI_SUPPORT_STATION_DATA_SMARTTOOL


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
	else // pit�� viel� luoda kyseinen skripti-muuttuja, koska sit� k�ytet��n nyt 1. kertaa
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

#ifdef FMI_SUPPORT_STATION_DATA_SMARTTOOL

void NFmiSmartToolModifier::SetGeneralDoc(NFmiEditMapGeneralDataDoc *theDoc)
{
	itsDoc = theDoc;
}

#endif // FMI_SUPPORT_STATION_DATA_SMARTTOOL
