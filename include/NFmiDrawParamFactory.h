//**********************************************************
// C++ Class Name : NFmiDrawParamFactory 
// ---------------------------------------------------------
// Filetype: (HEADER)
// Filepath: D:/projekti/GDPro/GDTemp/NFmiDrawParamFactory.h 
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
//  Creation Date  : Thur - Jan 28, 1999 
// 
// 
//  Description: 
// 
//  Change Log: 
// 1999.08.26/Marko	Laitoin DrawParam:in (oikeastaan DataParam tarvitsee) 
//					luontiin mukaan tiedon levelistš.
// 
//**********************************************************
#ifndef  NFMIDRAWPARAMFACTORY_H
#define  NFMIDRAWPARAMFACTORY_H

#include "NFmiParameterName.h"
#include "NFmiGlobals.h"
#include "NFmiString.h"

class NFmiDrawParam;
class NFmiSmartInfo;
class NFmiLevel;
class NFmiDataIdent;

class NFmiDrawParamFactory 
{

public:
    NFmiDrawParamFactory(void);
    ~NFmiDrawParamFactory(void);
	NFmiDrawParam * CreateDrawParam( NFmiSmartInfo* theInfo
                                   , const NFmiDataIdent& theIdent
								   , bool& fSubParam 
								   , const NFmiLevel* theLevel);
	NFmiDrawParam * CreateEmptyInfoDrawParam(const NFmiDataIdent& theIdent);
	bool Init();
	const NFmiString& LoadDirectory(void) const {return itsLoadDirectory;};
	void LoadDirectory(const NFmiString& newValue){itsLoadDirectory = newValue;};

private:
	NFmiString CreateFileName(NFmiDrawParam* drawParam);
	NFmiString itsLoadDirectory;
};

#endif
