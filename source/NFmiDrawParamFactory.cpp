// ======================================================================
/*!
 *
 * C++ Class Name : NFmiDrawParamFactory 
 * ---------------------------------------------------------
 * Filetype: (SOURCE)
 * Filepath: D:/projekti/GDPro/GDTemp/NFmiDrawParamFactory.cpp 
 * 
 * 
 * GDPro Properties 
 * ---------------------------------------------------
 *  - GD Symbol Type    : CLD_Class 
 *  - GD Method         : UML ( 2.1.4 ) 
 *  - GD System Name    : Met-editor Plan 2 
 *  - GD View Type      : Class Diagram 
 *  - GD View Name      : Markon ehdotus 
 * ---------------------------------------------------  
 *  Author         : pietarin 
 *  Creation Date  : Thur - Jan 28, 1999 
 * 
 * 
 *  Description: 
 * 
 *  Change Log: 
 * 1999.08.26/Marko	Laitoin DrawParam:in (oikeastaan DataParam tarvitsee) 
 *					luontiin mukaan tiedon levelist‰.
 * 
 */
// ======================================================================

//#ifndef UNIX
//  #include "stdafx.h" // DEBUG_NEW
//#endif

#include "NFmiDrawParamFactory.h"
#include "NFmiSmartInfo.h"
#include "NFmiValueString.h"
#include "NFmiDrawParam.h"
#include <assert.h>
/*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/

static void InitDrawParam(NFmiDrawParam* theDrawParam, const NFmiString &theFileName, bool createDrawParamFileIfNotExist)
{
	if(theDrawParam)
	{
		if(!theDrawParam->Init(theFileName))
		{
			if(createDrawParamFileIfNotExist)
			{
				if(!theDrawParam->StoreData(theFileName))
				{
					// tiedostoa ei voitu luoda, mit‰ pit‰isi tehd‰?
				}
			}
		}
	}
}

//--------------------------------------------------------
// NFmiDrawParamFactory(void) 
//--------------------------------------------------------
NFmiDrawParamFactory::NFmiDrawParamFactory(bool createDrawParamFileIfNotExist) 
:itsLoadDirectory("")
,fCreateDrawParamFileIfNotExist(createDrawParamFileIfNotExist)
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
NFmiDrawParam* NFmiDrawParamFactory::CreateDrawParam (const NFmiDataIdent& theIdent
													 ,const NFmiLevel* theLevel)

//  T‰ss‰ metodissa valitaan s‰‰parametrin theParam perusteella piirtoa
//  varten sopiva drawParam. Kostruktorin NFmiDrawParam vaatima pointteri
//  NFmiDataParam* dataParam saadaan attribuutilta itsDataParamFactory.
//  Huomaa, ett‰ palautettava drawParam = new NFmiDrawParam, joten drawParam   
//  pit‰‰ muistaa tuhota ulkopuolella.

{

//	NFmiDrawParam* drawParam = new NFmiDrawParam(theInfo, theIdent, 1); // 1 = priority
// 7.1.2002/Marko Muutin dataidentin alustuksen niin, ett‰ se otetaan annetusta
// infosta, jolloin se on aina oikein. Info on aina asetettu halutun parametrin 
// kohdalle, kun se tulee t‰nne.
	NFmiDrawParam* drawParam = new NFmiDrawParam(theIdent, theLevel ? *theLevel : NFmiLevel(), 1); // 1 = priority

	if(drawParam)
	{
		NFmiString fileName = CreateFileName(drawParam);
		InitDrawParam(drawParam, fileName, fCreateDrawParamFileIfNotExist);
	}
	return drawParam;
}

// luodaan drawparam crossSectionDataa varten. Huom k‰ytetyt tiedostonimet
// poikkeavat muista drawparamien tiedostonimist‰.
// Eli esim. DrawParam_4_CrossSection.dpa
NFmiDrawParam * NFmiDrawParamFactory::CreateCrossSectionDrawParam(const NFmiDataIdent& theIdent)
{
	NFmiDrawParam* drawParam = new NFmiDrawParam(theIdent, NFmiLevel(), 1); // 1 = priority
	if(drawParam)
	{
		NFmiString fileName = CreateFileName(drawParam, true);
		InitDrawParam(drawParam, fileName, fCreateDrawParamFileIfNotExist);
	}
	return drawParam;
}

NFmiDrawParam* NFmiDrawParamFactory::CreateEmptyInfoDrawParam(const NFmiDataIdent& theIdent)
{
	NFmiDrawParam* drawParam = new NFmiDrawParam;

	if(drawParam)
	{
		drawParam->Param(theIdent);
		NFmiString fileName = CreateFileName(drawParam);
		InitDrawParam(drawParam, fileName, fCreateDrawParamFileIfNotExist);
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
NFmiString NFmiDrawParamFactory::CreateFileName(NFmiDrawParam* drawParam, bool fCrossSectionCase)
{
	NFmiString fileName(itsLoadDirectory);
	if(itsLoadDirectory.GetLen() > 0)
		fileName += "/";  // laitetaan varmuuden vuoksi keno, jos asetetun polun lopussa ei ole kenoa
	fileName += "DrawParam_";

	if(drawParam)
	{
		int paramId = drawParam->Param().GetParam()->GetIdent();
		NFmiValueString idStr(paramId, "%d");
		fileName += idStr;
		if(fCrossSectionCase)
			fileName += "_CrossSection";
		else
		{
			if(drawParam && drawParam->Level().LevelType() == kFmiPressureLevel)
			{ // jos leveleit‰ on useita, niill‰ on omat tiedostot
				fileName += "_level_";
				NFmiLevel& level = drawParam->Level();
				int levelTypeId = level.LevelTypeId();
				NFmiValueString levelTypeIdStr(levelTypeId, "%d");
				fileName += levelTypeIdStr;
				fileName += "_";
				int levelValue = level.LevelValue();
				NFmiValueString levelValueStr(levelValue, "%d");
				fileName += levelValueStr;
			}
			else if(drawParam && drawParam->Level().LevelType() == kFmiHybridLevel)
			{ // hybrideill‰ on ain yksi piirtotapa
				fileName += "_hybrid";
			}
			else if(drawParam && drawParam->Level().LevelType() == 50) // t‰m‰ on luotaus dataa
			{ // luotaus parametreilla on ain yksi piirtotapa kartalla
				fileName += "_temp";
			}
			else
			{
				// normaali parametrille ei taida olla tiedosto nimess‰ mit‰‰n ekstraa
			}
		}
		fileName +=".dpa";
	}
	return fileName;
}
