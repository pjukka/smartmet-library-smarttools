//**********************************************************
// C++ Class Name : NFmiSmartToolModifier 
// ---------------------------------------------------------
// Filetype: (HEADER)
// Filepath: G:/siirto/marko/oc/NFmiSmartToolModifier.h 
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
// T‰m‰ luokka hoitaa koko smarttool-toiminnan. Sill‰ on tietokanta,
// tulkki, ja erilaisia maski/operaatio generaattoreita, joiden avulla
// laskut suoritetaan.
//**********************************************************
#ifndef  NFMISMARTTOOLMODIFIER_H
#define  NFMISMARTTOOLMODIFIER_H

#include "NFmiAreaMask.h"
#include <string>
#include <vector>

class NFmiInfoOrganizer;
class NFmiSmartToolIntepreter;
class NFmiTimeDescriptor;
class NFmiAreaMaskInfo;
class NFmiSmartToolCalculationSection;
class NFmiSmartToolCalculationSectionInfo;
class NFmiSmartToolCalculation;
class NFmiSmartToolCalculationInfo;
class NFmiSmartInfo;
class NFmiAreaMaskSectionInfo;
class NFmiDataModifier;
class NFmiDataIterator;

class NFmiSmartToolModifier 
{
public:
	class Exception
	{
	public:
		Exception(const std::string &theText):itsText(theText){}
		const std::string& What(void){return itsText;}
	private:
		const std::string itsText;
	};

	void InitSmartTool(const std::string &theSmartToolText) throw (NFmiSmartToolModifier::Exception);
	void ModifyData(NFmiTimeDescriptor* theModifiedTimes, const std::vector<double> &theModificationFactors, bool fSelectedLocationsOnly) throw (NFmiSmartToolModifier::Exception);
	NFmiSmartToolModifier(NFmiInfoOrganizer* theInfoOrganizer);
	~NFmiSmartToolModifier(void);

	bool IsMacroRunnable(void) const {return fMacroRunnable;}
	const std::string& GetErrorText(void) const {return itsErrorText;}

private:
//	void ModifyConditionalData(NFmiTimeDescriptor *theModifiedTimes, NFmiSmartToolCalculation *theMaskCondition, NFmiSmartToolCalculationSection *theConditionalCalculations);
	void GetParamValueLimits(const NFmiAreaMaskInfo &theAreaMaskInfo, float *theLowerLimit, float *theUpperLimit) throw (NFmiSmartToolModifier::Exception);
	NFmiDataModifier* CreateIntegrationFuction(const NFmiAreaMaskInfo &theAreaMaskInfo) throw (NFmiSmartToolModifier::Exception);
	NFmiDataIterator* CreateIterator(const NFmiAreaMaskInfo &theAreaMaskInfo, NFmiSmartInfo* theInfo) throw (NFmiSmartToolModifier::Exception);
	void ModifyConditionalData(NFmiTimeDescriptor *theModifiedTimes);
	void ModifyData2(NFmiTimeDescriptor* theModifiedTimes, NFmiSmartToolCalculationSection* theCalculationSection);
	void InitializeCalculationModifiers(void);
	void ClearCalculationModifiers(void);
	NFmiAreaMask* CreateAreaMask(const NFmiAreaMaskInfo &theInfo) throw (NFmiSmartToolModifier::Exception);
	NFmiAreaMask* CreateEndingAreaMask(void);
	NFmiSmartInfo* CreateInfo(const NFmiAreaMaskInfo &theAreaMaskInfo) throw (NFmiSmartToolModifier::Exception);
	void CreateCalculationModifiers(void);
	void CreateFirstCalculationSection(void);
	NFmiSmartToolCalculationSection* CreateCalculationSection(NFmiSmartToolCalculationSectionInfo *theCalcSectionInfo);
	NFmiSmartToolCalculation* CreateCalculation(NFmiSmartToolCalculationInfo *theCalcInfo);
	NFmiSmartToolCalculation* CreateConditionalSection(NFmiAreaMaskSectionInfo* theAreaMaskSectionInfo);

	NFmiInfoOrganizer *itsInfoOrganizer; // eli database, ei omista ei tuhoa
	NFmiSmartToolIntepreter *itsSmartToolIntepreter; // omistaa, tuhoaa
	bool fMacroRunnable;
	std::string itsErrorText;

	NFmiSmartToolCalculationSection* itsFirstCalculationSection;
	NFmiSmartToolCalculationSection* itsLastCalculationSection;
	NFmiSmartToolCalculation* itsIfAreaMaskSection;
	NFmiSmartToolCalculationSection* itsIfCalculationSection;
//	std::vector<NFmiSmartToolCalculation*> itsElseIfAreaMaskSectionVector;
//	std::vector<NFmiSmartToolCalculationSection*> itsElseIfCalculationSectionVector;
	NFmiSmartToolCalculation* itsElseIfAreaMaskSection;
	NFmiSmartToolCalculationSection* itsElseIfCalculationSection;
//	NFmiSmartToolCalculation* itsElseAreaMaskSection; // else:ss‰ ei voi olla tietenk‰‰n ehtoja, joten t‰t‰ ei tarvita
	NFmiSmartToolCalculationSection* itsElseCalculationSection;

	std::vector<double> itsModificationFactors; // mahdolliset aikasarja muokkaus kertoimet (ei omista, ei tuhoa)
	bool fModifySelectedLocationsOnly;
};
#endif
