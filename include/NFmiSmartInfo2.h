#pragma once

// T‰m‰ luokka tulee nykyisen NFmiSmartInfo-luokan tilalle.
// Uusi luokka on tarkoitettu vain SmartMetissa olevan 
// editoitavan datan k‰sittely. Eli t‰‰ll‰ on tietoa mm.
// valitusta maskista (selction, display jne.).
// Undo/Redo toiminnot ja Harmonisaattorin vastaava 
// kirjanpito.
// Luokka tehd‰‰n uusiksi siksi, ett‰ sit‰ yksinkertaistetaan
// ja ett‰ esim. undo/redo datojen omistus ja tuhoaminen tehd‰‰n
// automaattisesti smart-pointtereilla eik‰ niin kuin nykyisin
// erillisen DestroyData-funktion avulla.
// TODO: keksi parempi nimi, toi poista lopuksi NFmiSmartInfo
// -luokka ja laita t‰m‰ sen nimiseksi.

#include "NFmiOwnerInfo.h"

class NFmiModifiableQDatasBookKeeping;
class NFmiHarmonizerBookKeepingData;

class NFmiSmartInfo2 : public NFmiOwnerInfo
{
public:
	NFmiSmartInfo2(const NFmiOwnerInfo &theInfo); // matala kopio, eli jaettu data
	NFmiSmartInfo2(const NFmiSmartInfo2 &theInfo); // matala kopio, eli jaettu data
	NFmiSmartInfo2(NFmiQueryData *theOwnedData, NFmiInfoData::Type theDataType, const std::string &theDataFileName, const std::string &theDataFilePattern); // ottaa datan omistukseensa emossa
	~NFmiSmartInfo2(void);

	NFmiSmartInfo2& operator=(const NFmiSmartInfo2 &theInfo); // matala kopio, eli jaettu data
	NFmiSmartInfo2* Clone(void) const; // syv‰ kopio, eli kloonille luodaan oma queryData sen omistukseen
										// TODO: katso pit‰‰kˆ metodin nimi muuttaa, koska emoissa Clone on 
										// virtuaali funktio, jossa eri paluu-luokka.

	bool NextLocation (void);

	bool SnapShotData(const std::string& theAction, const NFmiHarmonizerBookKeepingData &theCurrentHarmonizerBookKeepingData);
	bool Undo(void);
	bool Redo(void);
	bool UndoData(const NFmiHarmonizerBookKeepingData &theHarmonizerBookKeepingData);
	bool RedoData(void);
	void UndoLevel(long theDepth);

	bool LocationSelectionSnapShot(void);	// ota maskit talteen
	bool LocationSelectionUndo(void);		// kysyy onko undo mahdollinen
	bool LocationSelectionRedo(void);		// kysyy onko redo mahdollinen
	bool LocationSelectionUndoData(void);	// suorittaa todellisen undon
	bool LocationSelectionRedoData(void);	// suorittaa todellisen redon
	void LocationSelectionUndoLevel(int theNewUndoLevel); // undolevel asetetaan t‰ll‰
	bool LoadedFromFile(void);
	void LoadedFromFile(bool loadedFromFile);
	bool IsDirty(void) const;
	void Dirty(bool newState);
	const NFmiHarmonizerBookKeepingData* CurrentHarmonizerBookKeepingData(void) const; // palauttaa nyt k‰ytˆss‰ olevan harmonisaattori parambagin

	int MaskedCount(unsigned long theMaskType, unsigned long theIndex, const NFmiRect& theSearchArea);
	void InverseMask(unsigned long theMaskType);
	void MaskAllLocations (const bool& newState, unsigned long theMaskType);
	unsigned long LocationMaskedCount(unsigned long theMaskType);
	bool Mask(const NFmiBitMask& theMask, unsigned long theMaskType);
	const NFmiBitMask& Mask(unsigned long theMaskType) const;
	void MaskLocation (const bool& newState, unsigned long theMaskType);
	void MaskType(unsigned long theMaskType);
	unsigned long MaskType(void);
protected:
	void CopyClonedDatas(const NFmiSmartInfo2 &theOther);

	boost::shared_ptr<NFmiModifiableQDatasBookKeeping> itsQDataBookKeepingPtr;
private:
	NFmiSmartInfo2(void); // ei toteuteta tyhj‰‰ konstruktoria
};
