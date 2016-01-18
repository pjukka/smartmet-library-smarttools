//**********************************************************
// C++ Class Name : NFmiUndoableMultiLevelMask
// ---------------------------------------------------------
// Filetype: (SOURCE)
// Filepath: d:/projekti/GDPro/GDTemp/NFmiUndoableMultiLevelMask.cpp
//
//
// GDPro Properties
// ---------------------------------------------------
//  - GD Symbol Type    : CLD_Class
//  - GD Method         : UML ( 2.1.4 )
//  - GD System Name    : Met-editor Plan 3
//  - GD View Type      : Class Diagram
//  - GD View Name      : undoable multilevelmask
// ---------------------------------------------------
//  Author         : pietarin
//  Creation Date  : Tues - Jun 22, 1999
//
//
//  Description:
//   Halusin tehd‰ NFmiMultiLevelMask:iin undo/redo
//   mahdollisuuden. Tein NFmiMultiLevelMask-
//   smartpointerin, jolla on multilevelmask ja
//   lis‰ksi undo/redo-lista, jossa on multilevelmask-pointereita.
//
//
//  Change Log:
//
//**********************************************************
#include "NFmiUndoableMultiLevelMask.h"
#include "NFmiMultiLevelMask.h"
#include <stdexcept>

//--------------------------------------------------------
// Constructor/Destructor
//--------------------------------------------------------
NFmiUndoableMultiLevelMask::NFmiUndoableMultiLevelMask(unsigned long theSize)
    : itsMultiLevelMask(0),
      itsUndoList(),
      itsMaxUndoLevel(0),
      itsMaxRedoLevel(0),
      itsCurrentUndoLevel(0),
      itsCurrentRedoLevel(0)
{
  itsMultiLevelMask = new NFmiMultiLevelMask(theSize);
}
NFmiUndoableMultiLevelMask::NFmiUndoableMultiLevelMask(const NFmiUndoableMultiLevelMask& theMask)
    : itsMultiLevelMask(
          theMask.itsMultiLevelMask ? new NFmiMultiLevelMask(*theMask.itsMultiLevelMask) : 0),
      itsUndoList()  // undolistaa ei kopioida!!!!!
      ,
      itsMaxUndoLevel(0)  // eik‰ mit‰‰n undo/redo juttuihin liittyv‰‰!!!!!
      ,
      itsMaxRedoLevel(0),
      itsCurrentUndoLevel(0),
      itsCurrentRedoLevel(0)
{
}
NFmiUndoableMultiLevelMask& NFmiUndoableMultiLevelMask::operator=(
    const NFmiUndoableMultiLevelMask& theMask)
{
  delete itsMultiLevelMask;
  itsMultiLevelMask = 0;
  itsMultiLevelMask =
      theMask.itsMultiLevelMask ? new NFmiMultiLevelMask(*theMask.itsMultiLevelMask) : 0;
  //	itsUndoList // undolistaa ei kopioida!!!!!
  itsMaxUndoLevel = 0;  // eik‰ mit‰‰n undo/redo juttuihin liittyv‰‰!!!!!
  itsMaxRedoLevel = 0;
  itsCurrentUndoLevel = 0;
  itsCurrentRedoLevel = 0;

  return *this;
}
NFmiUndoableMultiLevelMask::~NFmiUndoableMultiLevelMask(void)
{
  delete itsMultiLevelMask;
  itsUndoList.Clear(true);
}
//--------------------------------------------------------
// SnapShotData
//--------------------------------------------------------
bool NFmiUndoableMultiLevelMask::SnapShotData(void)
{
  if (!itsMultiLevelMask || itsMaxUndoLevel <= 0) return false;

  if (itsCurrentUndoLevel == itsMaxUndoLevel - 1) RearrangeUndoTable();

  itsCurrentUndoLevel++;
  NFmiPtrList<NFmiMultiLevelMask>::Iterator it = itsUndoList.Index(itsCurrentUndoLevel);
  if (it.CurrentPtr() == 0) return false;  // ei pit‰isi menn‰ t‰h‰n, exceptionin paikka!
  it.Current() = *itsMultiLevelMask;
  itsCurrentRedoLevel = itsCurrentUndoLevel;
  itsMaxRedoLevel = itsCurrentRedoLevel;

  return true;
}
//--------------------------------------------------------
// RearrangeUndoTable
//--------------------------------------------------------

//   Kun Snapshot:illa menn‰‰n listan yli, j‰rjestet‰‰n
//   lista niin, ett‰ siirret‰‰n listan ensimm‰inen
//   alkio viimeiseksi ja asetetaan listan osoittimille
//   uudet arvot. Eli jos undo-level olisi 5 ja
//   tehd‰‰n kuudes per‰kk‰inen Snapshot, pit‰‰
//   uudelle snapshotille tehd‰ tilaa listan alusta.
//
void NFmiUndoableMultiLevelMask::RearrangeUndoTable(void)
{
  NFmiPtrList<NFmiMultiLevelMask>::Iterator it = itsUndoList.Index(1);  // 1 = 1. paikka listassa
  if (it.CurrentPtr() == 0) return;  // ei pit‰isi menn‰ t‰h‰n, exceptionin paikka!

  NFmiMultiLevelMask* tempMask = it.CurrentPtr();
  it.Remove(false);  // false = ei tuhoa dataa
  itsUndoList.AddEnd(tempMask);

  itsCurrentUndoLevel--;
  itsCurrentRedoLevel--;
}
//--------------------------------------------------------
// Undo
//--------------------------------------------------------
bool NFmiUndoableMultiLevelMask::Undo(void)
{
  if (itsCurrentUndoLevel < 1)
    return false;
  else
    return true;
}
//--------------------------------------------------------
// Redo
//--------------------------------------------------------
bool NFmiUndoableMultiLevelMask::Redo(void)
{
  if (itsCurrentRedoLevel == itsCurrentUndoLevel ||
      (itsCurrentRedoLevel == itsCurrentUndoLevel + 1))
    return false;
  else
    return true;
}

//--------------------------------------------------------
// UndoData
//--------------------------------------------------------
bool NFmiUndoableMultiLevelMask::UndoData(void)
{
  if (!itsMultiLevelMask || itsCurrentUndoLevel < 1) return false;
  if (itsCurrentUndoLevel == itsCurrentRedoLevel)
  {
    SnapShotData();         // "Ottaa kuvan" undo-toimintoa edelt‰neest‰ tilanteesta,
    itsCurrentUndoLevel--;  // jos siihen halutaankin myˆhemmin palata redo:lla.
  }

  NFmiPtrList<NFmiMultiLevelMask>::Iterator it = itsUndoList.Index(itsCurrentUndoLevel);
  if (it.CurrentPtr() == 0) return false;  // ei pit‰isi menn‰ t‰h‰n, exceptionin paikka!
  *itsMultiLevelMask = *it.CurrentPtr();
  itsCurrentUndoLevel--;
  itsCurrentRedoLevel = itsCurrentUndoLevel + 2;
  return true;
}
//--------------------------------------------------------
// RedoData
//--------------------------------------------------------
bool NFmiUndoableMultiLevelMask::RedoData(void)
{
  if (itsCurrentUndoLevel == itsCurrentRedoLevel ||
      (itsCurrentUndoLevel + 1 == itsCurrentRedoLevel))
    return false;
  else
  {
    NFmiPtrList<NFmiMultiLevelMask>::Iterator it = itsUndoList.Index(itsCurrentRedoLevel);
    if (it.CurrentPtr() == 0) return false;  // ei pit‰isi menn‰ t‰h‰n, exceptionin paikka!
    *itsMultiLevelMask = *it.CurrentPtr();

    itsCurrentUndoLevel++;
    if (itsCurrentRedoLevel + 1 <= itsMaxRedoLevel) itsCurrentRedoLevel++;
  }
  return true;
}
//--------------------------------------------------------
// UndoLevel
//--------------------------------------------------------
void NFmiUndoableMultiLevelMask::UndoLevel(int theNewUndoLevel)
{  // jos undo leveli‰ vaihdetaan kesken kaiken, tuhotaan vanha undo-tieto

  // undolistaan laitetaan level+1:lle tilaa, koska kun tehd‰‰n esim. 5 kertaa muutos,
  // ja sitten tehd‰‰n undo, pit‰‰ ottaa talteen nykyhetkest‰ 6.s 'kuva', ett‰ voidaan
  // palata takaisin nykyhetkeen (ja silti voidaan tehd‰ 5 kertaa undo per‰kk‰in).
  itsMaxUndoLevel = theNewUndoLevel + 1;
  itsMaxRedoLevel = 0;      // ??
  itsCurrentUndoLevel = 0;  // ??
  itsCurrentRedoLevel = 0;  // ??

  itsUndoList.Clear(true);
  for (int i = 0; i < itsMaxUndoLevel; i++)
    itsUndoList.AddEnd(new NFmiMultiLevelMask(itsMultiLevelMask->MaskSize()));
}

// T‰m‰ asettaa halutun maskin, mutta ei tee undo/redo valmisteluja tai muuta.
bool NFmiUndoableMultiLevelMask::Mask(const NFmiBitMask& theMask, unsigned long theMaskType)
{
  if (itsMultiLevelMask) return itsMultiLevelMask->Mask(theMask, theMaskType);

  return false;
}

const NFmiBitMask& NFmiUndoableMultiLevelMask::Mask(unsigned long theMaskType) const
{
  if (itsMultiLevelMask) return itsMultiLevelMask->Mask(theMaskType);

  throw std::runtime_error("Error in application - NFmiUndoableMultiLevelMask::Mask has no mask.");
}
