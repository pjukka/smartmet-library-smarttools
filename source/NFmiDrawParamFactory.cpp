//**********************************************************
// C++ Class Name : NFmiDrawParamFactory 
// ---------------------------------------------------------
// Filetype: (SOURCE)
// Filepath: D:/projekti/GDPro/GDTemp/NFmiDrawParamFactory.cpp 
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
//					luontiin mukaan tiedon levelist‰.
// 
//**********************************************************
#include "NFmiDrawParamFactory.h"
#include "NFmiSmartInfo.h"
#include "NFmiValueString.h"
#include "NFmiDrawParam.h"
#include <assert.h>

//--------------------------------------------------------
// NFmiDrawParamFactory(void) 
//--------------------------------------------------------
NFmiDrawParamFactory::NFmiDrawParamFactory(void) 
:itsWorkingDirectory("")
{
}
//--------------------------------------------------------
// ~FmiDrawParamFactory(void) 
//--------------------------------------------------------
NFmiDrawParamFactory::~NFmiDrawParamFactory(void)
{
}

//--------------------------------------------------------
// DrawParam 
//--------------------------------------------------------
NFmiDrawParam* NFmiDrawParamFactory::CreateDrawParam ( NFmiSmartInfo* theInfo 
													 , const NFmiDataIdent& theIdent
													 , bool& fSubParam
													 , const NFmiLevel* theLevel
													 , bool theIsToolMasterAvailable)

//  T‰ss‰ metodissa valitaan s‰‰parametrin theParam perusteella piirtoa
//  varten sopiva drawParam. Kostruktorin NFmiDrawParam vaatima pointteri
//  NFmiDataParam* dataParam saadaan attribuutilta itsDataParamFactory.
//  Huomaa, ett‰ palautettava drawParam = new NFmiDrawParam, joten drawParam   
//  pit‰‰ muistaa tuhota ulkopuolella.

{
	if(!theInfo)
		return 0;

	fToolMasterAvailable = theIsToolMasterAvailable;
	theInfo->Param(theIdent);
	if(theLevel)
		theInfo->Level(*theLevel);
	else
		theInfo->FirstLevel();

//	NFmiDrawParam* drawParam = new NFmiDrawParam(theInfo, theIdent, 1); // 1 = priority
// 7.1.2002/Marko Muutin dataidentin alustuksen niin, ett‰ se otetaan annetusta
// infosta, jolloin se on aina oikein. Info on aina asetettu halutun parametrin 
// kohdalle, kun se tulee t‰nne.
	NFmiDrawParam* drawParam = new NFmiDrawParam(theInfo, theInfo->Param(), 1); // 1 = priority

	if(drawParam)
	{
		NFmiString fileName = CreateFileName(drawParam);
		if(!drawParam->Init(fileName))
			if(!drawParam->StoreData(fileName))
			  {
				// tiedostoa ei voitu luoda, mit‰ pit‰isi tehd‰?
			  }
	}
	return drawParam;
}

NFmiDrawParam* NFmiDrawParamFactory::CreateEmptyInfoDrawParam(const NFmiDataIdent& theIdent, bool theIsToolMasterAvailable)
{
	fToolMasterAvailable = theIsToolMasterAvailable;
	NFmiDrawParam* drawParam = new NFmiDrawParam;

	if(drawParam)
	{
		drawParam->Param(theIdent);
		NFmiString fileName = CreateFileName(drawParam);
		if(!drawParam->Init(fileName))
			if(!drawParam->StoreData(fileName))
			  {
				// tiedostoa ei voitu luoda, mit‰ pit‰isi tehd‰?
			  }
	}
	return drawParam;
}

//--------------------------------------------------------
// Init 
//--------------------------------------------------------
bool NFmiDrawParamFactory::Init()
{
 	return true;
}

//--------------------------------------------------------
// CreateFileName, private 
//--------------------------------------------------------
NFmiString NFmiDrawParamFactory::CreateFileName(NFmiDrawParam* drawParam)
{
	NFmiString fileName(itsWorkingDirectory);
//#ifdef FMI_USE_TOOLMASTER
//	if(fToolMasterAvailable)
		fileName += "\\TMDrawParams\\DrawParam_";
//#else
//	else
//		fileName += "\\DrawParams\\DrawParam_";
//#endif
//	assert(drawParam->Param().GetParam()); // testaa, onko parametri asetettu kohdalleen
	if(drawParam)
	{
		int paramId = drawParam->Param().GetParam()->GetIdent();
		NFmiValueString idStr(paramId, "%d");
		fileName += idStr;
		if(drawParam && drawParam->Info() && drawParam->Info()->SizeLevels() > 1)
		{ // jos leveleit‰ on useita, niill‰ on omat tiedostot
			fileName += "_level_";
			const NFmiLevel* level = drawParam->Info()->Level();
			int levelTypeId = 0;
			if(level)
				levelTypeId = level->LevelTypeId();
			NFmiValueString levelTypeIdStr(levelTypeId, "%d");
			fileName += levelTypeIdStr;
			fileName += "_";
			int levelValue = level->LevelValue();
			NFmiValueString levelValueStr(levelValue, "%d");
			fileName += levelValueStr;
		}
		else
		{
			// normaali parametrille ei taida olla tiedosto nimess‰ mit‰‰n ekstraa
		}
		fileName +=".dpa";
	}
	return fileName;
}
