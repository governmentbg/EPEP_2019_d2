#include "cac.h"  // 18.may.2001
//# include "all.h"


DEFINE_RESPONSE_TABLE1(TPrintClosedSessBkDialog, TGroupDialog)
//{{TPrintClosedSessBkDialogRSP_TBL_BEGIN}}
  EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TPrintClosedSessBkDialogRSP_TBL_END}}
END_RESPONSE_TABLE;


//{{TPrintClosedSessBkDialog Implementation}}


bool TPrintClosedSessBkDialog :: loadColCiphers ()
 {
  if ((colCiphers =
# if FOLLOW_LAWSUIT_TYPES
       loadTypeCiphers ()
# else
       loadCollegeCiphers ()
# endif
       ) != NULL)
   {
    return (strlen (colCiphers) != 0) ;
   }
  else
    return false ;
 }  // TPrintClosedSessBkDialog :: loadColCiphers

TPrintClosedSessBkDialog :: TPrintClosedSessBkDialog (TWindow * parent,
				    collBegDEndDBegN * colBegDEndDBegNPtr,
                            int resId) :
    TGroupDialog(parent, NULL, resId),
    pBegDEndDBegN (colBegDEndDBegNPtr),
    pCol (NULL), pBegD (NULL), pEndD (NULL), pBegN (NULL)
# if INSTANCE
#   if APPEAL  // APPEAL
    , pWantAllRes (NULL)
#   else       // AREAL
    , pWantAllRes (NULL), pWantResultsCheckup (NULL)
#   endif
# else         // REGIONAL
    , pWantAllRes (NULL), pNormalSort (NULL),
    pWantSortByDateReturned (NULL), sortByDRetOldVal
                     (true)  // because of citizen - like laws
# endif        // ALL
 {
  if ((loadColCiphersOK = loadColCiphers ()) == true)
    pCol = new TCharAliasFace (this, IDC_PRINT_CSB_COLLEGE,
# if FOLLOW_LAWSUIT_TYPES
                               "F_TYPE", & thisType,
# else
					 "F_COLLEGE", & thisCollege,
# endif
                               colCiphers) ;
  pBegD = new TDateFace (this, IDC_PRINT_CSB_BEG_DATE, "F_DATE_BEGIN",
                         true) ;
  pEndD = new TDateFace (this, IDC_PRINT_CSB_END_DATE, "F_DATE_END",
                         true) ;
  pBegN = new TLongFace (this, IDC_PRINT_CSB_BEG_NUMBER, "F_NO_BEGIN",
                         SIZE_OF_NO) ;
  pWantAllRes = new TCheckBox (this, IDC_PRINT_CSB_WANT_ALL_RES,
                               NULL) ;
  pWantResultsCheckup = new TCheckBox (this,
                                       IDC_PRINT_CSB_WANT_RESULTS_SET,
                                       NULL) ;
  pJan2005Format = new TCheckBox (this, IDC_PRINT_CSB_NEW_FORMAT,
                                  NULL) ;  // 26.jan.2005
# if INSTANCE
#   if APPEAL
#   else      // AREAL
  pComposit = new TLongAliasFace(this, IDC_PRINT_CSB_COMPOSITION, "COMPOSIT", Composition, PRINT_COMPOSITIONS ? QUERY_COMPOSITIONS : "", ALIAS_EMPTY);
#   endif
# else        // REGIONAL
  pComposit = new TLongAliasFace(this, IDC_PRINT_CSB_COMPOSITION, "COMPOSIT", Composition, PRINT_COMPOSITIONS ? QUERY_COMPOSITIONS : "", ALIAS_EMPTY);
  pSortGB = new TGroupBox (this, IDC_LAWS_BY_INSTANCE) ;
  pNormalSort = new TRadioButton (this, IDC_PRINT_CSB_ORDER_BY_DATE,
					    pSortGB) ;
  pWantSortByDateReturned =
    new TRadioButton (this, IDC_PRINT_CSB_ORDER_BY_DATE_RET, pSortGB) ;
# endif
 }  // TPrintClosedSessBkDialog :: TPrintClosedSessBkDialog


TPrintClosedSessBkDialog :: ~TPrintClosedSessBkDialog ()
 {
    delete [] colCiphers ;
 }  // TPrintClosedSessBkDialog :: ~TPrintClosedSessBkDialog


void TPrintClosedSessBkDialog :: SetupWindow ()
 {
  TGroupDialog::SetupWindow();

  if (loadColCiphersOK)
   {
     {
	pBegD -> SetDate (Today) ;
	pEndD -> SetDate (Today) ;
     }
    pCol -> SetSelIndex (0) ;
    pBegN -> SetL (1) ;
    pWantAllRes -> SetCheck (BF_CHECKED) ;
# if INSTANCE
#   if APPEAL
#   else      // AREAL
    select_de1st_comp(pComposit);
#   endif
# else         // REGIONAL
    pNormalSort ->
	SetCheck (sortByDRetOldVal ? BF_UNCHECKED : BF_CHECKED) ;
    pWantSortByDateReturned ->
	SetCheck (sortByDRetOldVal ? BF_CHECKED : BF_UNCHECKED) ;
    // 04.jan.2005 - end
    if (PRINT_COMPOSITIONS)
	set_zero_all_comp(pComposit);
# endif        // ALL
    pWantResultsCheckup -> SetCheck (BF_UNCHECKED) ;  // 21.sep.2000

    //20050329 in comment-> pJan2005Format -> SetCheck (pBegDEndDBegN -> jan2005Format) ; // 26.jan.2005
    pJan2005Format->SetCheck(BF_CHECKED);  // 08.feb.2005 - end
   }
 }  // TPrintClosedSessBkDialog :: SetupWindow


void TPrintClosedSessBkDialog :: OKBNClicked ()
 {
  if (loadColCiphersOK)
   {
    pBegDEndDBegN -> college = pCol -> C () ;
    pBegDEndDBegN -> begD = pBegD -> Date () ;
    pBegDEndDBegN -> endD = pEndD -> Date () ;
    pBegDEndDBegN -> begN = pBegN -> L () ;
    pBegDEndDBegN -> wantAllResults =
      (pWantAllRes -> GetCheck () == BF_CHECKED) ;

# if INSTANCE
#   if APPEAL   // APPEAL
    pBegDEndDBegN -> wantSortByDateReturned = true ;
#   else        // AREAL
    pBegDEndDBegN -> wantSortByDateReturned = true ;
    //pBegDEndDBegN -> composition = pComposit -> LongVal () ;
    pBegDEndDBegN -> composition = pComposit->L() ;
#   endif
# else          // REGIONAL
    pBegDEndDBegN -> wantSortByDateReturned =
	(pWantSortByDateReturned -> GetCheck () == BF_CHECKED) ;
  //pBegDEndDBegN -> composition = pComposit -> LongVal () ;  // 04.jan.2005-end
  pBegDEndDBegN -> composition = pComposit->L();
# endif
                // ALL
    pBegDEndDBegN -> jan2005Format =
      (pJan2005Format -> GetCheck () == BF_CHECKED) ; // 26.jan.2005

    pBegDEndDBegN -> wantResultsCheckup =
      pWantResultsCheckup -> GetCheck () == BF_CHECKED ;
    CmOk () ;
   }
  else
    CmCancel () ;
 }  // TPrintClosedSessBkDialog :: OKBNClicked
