#include "cac.h"  // 18.may.2001
//# include "all.h"

TypeErrorStrings :: TypeErrorStrings () :
    wrongArgument ("Невалидна стойност (празен низ) за аргумента"),
    noAccessibleTypes ("Няма достъпни видове дела за избор от "
                       "списъчно поле"),
    mayBeNoRights ("\n (може би нямате права за работа с нужния "
                   "вид/видове дела)")
 {
 }  // TypeErrorStrings :: TypeErrorStrings

TTypeAliasFace :: TTypeAliasFace (TWindow * parent,
					    int resourceId, const char * name,
					    const bool readOnly,
					    const char * const typesAllowed) :
			  TCharAliasFace (parent, resourceId, name, Type,
				    types)
 {
  build_ctypes(types, typesAllowed ? typesAllowed : TYPE_LAWSUITS, !readOnly);
 }  // TTypeAliasFace :: TTypeAliasFace


// --------------

TMonthAliasFace :: TMonthAliasFace (TWindow * parent, int resourceId,
						const char * name) :
			   TLongAliasFace (parent, resourceId, name, Month,
						 NULL, FLAG_NULL)
 {
 }  // TMonthAliasFace :: TMonthAliasFace

void TMonthAliasFace :: SetupWindow ()
 {
  TLongAliasFace :: SetupWindow () ;
  SetSelIndex (0) ;
 }  // TMonthAliasFace :: SetupWindow

RadioCluster :: RadioCluster () :
                   pGrpBox (NULL), oldSelection (-1)
 {
 }  // RadioCluster :: RadioCluster

RadioCluster :: ~RadioCluster ()
 {
 }  // RadioCluster :: ~RadioCluster

void RadioCluster :: SetGrpBox (TGroupBox * pGB)
 {
  pGrpBox = pGB ;
 }  // RadioCluster :: SetGrpBox

TGroupBox * RadioCluster :: GetGrpBox ()
{
  return pGrpBox ;
}

void RadioCluster :: Add (TRadioButton * pB, int idc)
 {
  if (! pB)
    return ;

  ButtonData * pBD = new ButtonData ;

  pBD -> idc = idc ;
  pBD -> enabled = true ;
  pBD -> pButton = pB ;
  cluster.Add (pBD) ;
 }  // RadioCluster :: Add

int RadioCluster :: GetSelection ()
 {
  int sel = -1 ;

  for (int i = 0 ; sel == -1 && i < cluster.Count () ; i ++)
    if ((cluster [i] -> pButton -> GetCheck () == BF_CHECKED)
         // && ( cluster [i] -> pButton -> IsWindowEnabled () ) // m2y 20070615
        )
      sel = i ;
  return sel ;
 }  // RadioCluster :: GetSelection

TRadioButton * RadioCluster :: GetSelectionP ()
 {
  const int sel = GetSelection () ;

  return sel != -1 ? cluster [sel] -> pButton : NULL ;
 }  // RadioCluster :: GetSelectionP

int RadioCluster :: GetSelectionIdc ()
 {
  const int sel = GetSelection () ;

  return sel != -1 ? cluster [sel] -> idc : -1 ;
 }  // RadioCluster :: GetSelectionP

void RadioCluster :: SetSelection (int sel)
 {
  for (int i = 0 ; i < cluster.Count () ; i ++)
    cluster [i] -> pButton -> SetCheck (i == sel ? BF_CHECKED :
                                                   BF_UNCHECKED) ;
 }  // RadioCluster :: SetSelection

void RadioCluster :: SetSelectionP (TRadioButton * pB)
 {
  if (pB == NULL)
    return ;

  for (int i = 0 ; i < cluster.Count () ; i ++)
   {
    TRadioButton * c = cluster [i] -> pButton ;

    c -> SetCheck (c == pB ? BF_CHECKED : BF_UNCHECKED) ;
   }
 }  // RadioCluster :: SetSelectionP

void RadioCluster :: SetSelectionIdc (int idc)
 {
  for (int i = 0 ; i < cluster.Count () ; i ++)
   {
    ButtonData * pBD = cluster [i] ;

    pBD -> pButton -> SetCheck (pBD -> idc == idc ? BF_CHECKED :
                                                    BF_UNCHECKED) ;
   }
 }  // RadioCluster :: SetSelectionIdc

void RadioCluster :: DisableCluster (int sel)
 {
  oldSelection = GetSelection () ;
  for (int i = 0 ; i < cluster.Count () ; i ++)
   {
    ButtonData * d = cluster [i] ;

    d -> pButton -> SetCheck (i == sel ? BF_CHECKED : BF_UNCHECKED) ;
    d -> enabled = d -> pButton -> IsWindowEnabled () ;
    d -> pButton -> EnableWindow (false) ;
   }
  if (pGrpBox)
    pGrpBox -> EnableWindow (false) ;
 }  // RadioCluster :: DisableCluster

void RadioCluster :: DisableClusterP (TRadioButton * pB)
 {
  oldSelection = GetSelection () ;
  for (int i = 0 ; i < cluster.Count () ; i ++)
   {
    ButtonData * d = cluster [i] ;

    d -> pButton -> SetCheck (pB && d -> pButton == pB ? BF_CHECKED :
                                                         BF_UNCHECKED) ;
    d -> enabled = d -> pButton -> IsWindowEnabled () ;
    d -> pButton -> EnableWindow (false) ;
   }
  if (pGrpBox)
    pGrpBox -> EnableWindow (false) ;
 }  // RadioCluster :: DisableClusterP

void RadioCluster :: DisableClusterIdc (int idc)
 {
  oldSelection = GetSelection () ;
  for (int i = 0 ; i < cluster.Count () ; i ++)
   {
    ButtonData * d = cluster [i] ;

    d -> pButton -> SetCheck (d -> idc == idc ? BF_CHECKED :
                                                BF_UNCHECKED) ;
    d -> enabled = d -> pButton -> IsWindowEnabled () ;
    d -> pButton -> EnableWindow (false) ;
   }
  if (pGrpBox)
    pGrpBox -> EnableWindow (false) ;
 }  // RadioCluster :: DisableClusterIdc

void RadioCluster :: EnableCluster ()
 {
  for (int i = 0 ; i < cluster.Count () ; i ++)
    cluster [i]  -> pButton -> EnableWindow (true) ;
  if (pGrpBox)
    pGrpBox -> EnableWindow (true) ;
  SetSelection (oldSelection) ;  
 }  // RadioCluster :: EnableCluster

void RadioCluster :: EnableItem (int item, bool enable)
 {
  for (int i = 0 ; i < cluster.Count () ; i ++)
    // m2y if (i == item)
    if (cluster [i] -> idc == item) 
      cluster [i] -> pButton -> EnableWindow (enable) ;
 }  // RadioCluster :: EnableItem

void RadioCluster :: EnableItemP (TRadioButton * pB, bool enable)
 {
  if (pB == NULL)
    return ;
  for (int i = 0 ; i < cluster.Count () ; i ++)
   {
    TRadioButton * b = cluster [i] -> pButton ;

    if (b == pB)
      b -> EnableWindow (enable) ;
   }
 }  // RadioCluster :: EnableItemP

void RadioCluster :: EnableItemIdc (int idc, bool enable)
 {
  for (int i = 0 ; i < cluster.Count () ; i ++)
   {
    ButtonData * d = cluster [i] ;

    if (d -> idc == idc)
      d -> pButton -> EnableWindow (enable) ;
   }
 }  // RadioCluster :: EnableItemIdc

bool RadioCluster :: GrpBoxIsEnabled ()
 {
  return pGrpBox ? pGrpBox -> IsWindowEnabled () : false ;
 }  // RadioCluster :: GrpBoxIsEnabled

TKind * KindPtr :: KPtr ()
 {
  return & k ;
 }  // KindPtr :: KPtr

DefaultLawKinds :: DefaultLawKinds (const char * const lKinds)
    // The lKinds parameter added on 29.jan.2002
 {
  scSprintf (sizeof (lawKinds), lawKinds, "%s",
             lKinds ? lKinds : KIND_LAWSUITS) ;
   {
    char tmp [sizeof (lawKinds)] ;
    const size_t lKL = strlen (lawKinds) ;
    ostrstream oss (tmp, sizeof (tmp)) ;

    for (size_t i = 0 ; i < lKL ; i ++)
     {
      const char * dComps = Default -> colleges ;  // The very brainy
        // debugger cannot identify the Default variable (may be I have
        // not debug info in the file where this variable is defined).
      const char t = kind2type (lawKinds [i]) ;

      if (strchr (dComps, t))
        oss << lawKinds [i] ;
     }
    oss << ends ;
    tmp [sizeof (tmp) - 1] = '\0' ;
    intersection (lawKinds, tmp) ;
   }
 }  // DefaultLawKinds :: DefaultLawKinds

char * DefaultLawKinds :: LawKinds ()
 {
  return lawKinds ;
 }  // DefaultLawKinds :: DefaultLawKinds

TLawKindsListFace :: TLawKindsListFace (TWindow * parent, int resourceId,
						    const char * name,
						    const bool followRights,
					  const char * const lKinds) :
			     KindPtr (), DefaultLawKinds (lKinds),
			     TypeErrorStrings (),
			     TCharListFace (parent, resourceId, name, KPtr (),
				     followRights ? LawKinds () : KIND_LAWSUITS,
						  SIZE_OF_KINDS),
			     accordingToRights (followRights)
 {
  if (lKinds)
    if (lKinds [0] == '\0')
      error ("%s\n%s::%s",
             WRONG_ARGS_PASSED_TO,
             typeid (* this).name (), typeid (* this).name ()) ;
 }  // TLawKindsListFace :: TLawKindsListFace

TLawKindsListFace :: ~TLawKindsListFace ()
 {
  Destroy (IDCANCEL) ;
 }  // TLawKindsListFace :: ~TLawKindsListFace

void TLawKindsListFace :: SetupWindow ()
 {
  if (accordingToRights)
    if (! (lawKinds [0]))
      fatal ("TLawKindsListFace- %s%s.", noAccessibleTypes, mayBeNoRights) ;
  TCharListFace :: SetupWindow () ;
 }  // TLawKindsListFace :: SetupWindow

TDateFaceOldVal :: TDateFaceOldVal (TWindow * parent, int resourceId,
                                    const char * name) :
                     TDateFace (parent, resourceId, name, false)
 {
  clear_CDate (oldVal) ;
 }  // TDateFaceOldVal :: TDateFaceOldVal

TDateFaceOldVal :: ~TDateFaceOldVal ()
 {
  Destroy (IDCANCEL) ;  // 10.dec.2001
 }  // TDateFaceOldVal :: ~TDateFaceOldVal

void TDateFaceOldVal :: DisableFace ()
 {
  if (IsWindowEnabled ())
   {
    oldVal = Date () ;
    SetDate (CDate ()) ;
    EnableWindow (false) ;
   }
 }  // TDateFaceOldVal :: DisableFace

void TDateFaceOldVal :: EnableFace ()
 {
  if (IsWindowEnabled () == false)
	 {
		SetDate (oldVal) ;
		EnableWindow (true) ;
	 }
 }  // TDateFaceOldVal :: EnableFace

TTimeFaceOldVal :: TTimeFaceOldVal (TWindow * parent, int resourceId, const char * name)
	: TTimeFace (parent, resourceId, name, false)
 {
	oldVal.hour = oldVal.minute = oldVal.second = '\0' ;
 }  // TTimeFaceOldVal :: TTimeFaceOldVal

TTimeFaceOldVal :: ~TTimeFaceOldVal ()
 {
	Destroy (IDCANCEL) ;  // 10.dec.2001
 }  // TTimeFaceOldVal :: ~TTimeFaceOldVal

void TTimeFaceOldVal :: DisableFace ()
 {
	if (IsWindowEnabled ())
	 {
		oldVal = Time () ;
		SetTime (CTime ()) ;
		EnableWindow (false) ;
	 }
 }  // TTimeFaceOldVal :: DisableFace

void TTimeFaceOldVal :: EnableFace ()
 {
	if (IsWindowEnabled () == false)
	 {
		SetTime (oldVal) ;
		EnableWindow (true) ;
	 }
 }  // TTimeFaceOldVal :: EnableFace

DEFINE_RESPONSE_TABLE1 (TCatchPasteEdit, TEdit)
	EV_COMMAND (CM_EDITPASTE, PasteRequested),
END_RESPONSE_TABLE ;

TCatchPasteEdit :: TCatchPasteEdit (TWindow * parent, int resourceId,
                                    uint textLimit) :
                     TEdit (parent, resourceId, textLimit)
 {
 }  // TCatchPasteEdit :: TCatchPasteEdit

TCatchPasteEdit :: ~TCatchPasteEdit ()
 {
  Destroy (IDCANCEL) ;  // 10.dec.2001
 }  // TCatchPasteEdit :: ~TCatchPasteEdit

void TCatchPasteEdit :: PasteRequested ()
 {
  TEdit :: CmEditPaste () ;
  if ((uint) GetTextLimit () + 1 == TextLimit)
    error ("Текстът, който се опитахте да вмъкнете е прекалено "
	   "дълъг и не беше възприет (частично или изцяло).") ;
 }  // TCatchPasteEdit :: PasteRequested

TYearFaceOldVal::TYearFaceOldVal(TWindow* parent, int resourceId,
				 const char *name) :
				 TYearFace(parent, resourceId, name),
		   oldVal(0)
{
}  // TYearFaceOldVal::TYearFaceOldVal

TYearFaceOldVal :: ~TYearFaceOldVal ()
 {
  Destroy (IDCANCEL) ;
 }  // TYearFaceOldVal :: ~TYearFaceOldVal

void TYearFaceOldVal :: DisableFace  ()
 {
  if (IsWindowEnabled ())
   {
    oldVal = L () ;
    SetL (0) ;
    EnableWindow (false) ;
   }
 }  // TYearFaceOldVal :: DisableFace

void TYearFaceOldVal :: EnableFace  ()
 {
  if (IsWindowEnabled () == false)
   {
    EnableWindow (true) ;
    SetL (oldVal) ;
   }
 }  // TYearFaceOldVal :: EnableFace

TStringFaceOldVal::TStringFaceOldVal(TWindow* parent, int resourceId,
	const char *name, const uint textLimit) :
	TStringFace(parent, resourceId, name, TxtLimit(textLimit)),
    oldValSz(TxtLimit(textLimit)), oldVal(new char[TxtLimit(textLimit)])
{

}  // TStringFaceOldVal::TStringFaceOldVal

TStringFaceOldVal::~TStringFaceOldVal()
{
	delete[] oldVal;
}  // TStringFaceOldVal::~TStringFaceOldVal

uint TStringFaceOldVal::TxtLimit(uint textLimit)
{
	return textLimit ? textLimit + 1 : textLimit;
}  // TStringFaceOldVal::TxtLimit

void TStringFaceOldVal::DisableFace()
{
	if(IsWindowEnabled())
  {
    ostrstream(oldVal, oldValSz) << S() << ends;
    oldVal [oldValSz - 1] = '\0';
    SetS("");
    EnableWindow(false);
  }
}  // TStringFaceOldVal::DisableFace

void TStringFaceOldVal::EnableFace()
{
	if(IsWindowEnabled() == false)
  {
    EnableWindow(true);
    SetS(oldVal);
  }
}  // TStringFaceOldVal::EnableFace
