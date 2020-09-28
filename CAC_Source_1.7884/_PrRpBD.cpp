#include "cac.h"

// 2008:295 LPR: added CORRUPT for REGIONAL

DEFINE_RESPONSE_TABLE1(TPrintReportBookDialog, TGroupDialog)
  EV_BN_CLICKED(IDOK, OKBNClicked),
  EV_LBN_SELCHANGE(IDC_PRINT_RB_LAW_TYPE, lawTypeCBNSelChange),
# if INSTANCE
#   if APPEAL  // COURT_APPEAL
#   else       // COURT_ADMIN and COURT_AREAL and COURT_MILITARY
  EV_CHILD_NOTIFY_ALL_CODES(IDC_PRINT_RB_FACE, handleFaceGroupBoxMsg),
  EV_CHILD_NOTIFY_ALL_CODES(IDC_REPORT_BY_INSTANCE, handleFaceGroupBoxMsg),
#   endif
# else         // COURT_REGIONAL
  EV_CHILD_NOTIFY_ALL_CODES(IDC_PRINT_RB_FACE, handleFaceGroupBoxMsg),
# endif
END_RESPONSE_TABLE;

// -----------------------------------------------------------------------------

void TPrintReportBookDialog :: makeCommonFields ()
 {
  pBegMon = new TMonthAliasFace (this, IDC_PRINT_RB_BEG_MONTH,
                                 "BEG_MONTH") ;
  pBegYear = new TLongFace (this, IDC_PRINT_RB_BEG_YEAR, "BEG_YEAR",
                            SIZE_OF_YEAR) ;
  pEndMon = new TMonthAliasFace (this, IDC_PRINT_RB_END_MONTH,
                                 "END_MONTH") ;
  pEndYear = new TLongFace (this, IDC_PRINT_RB_END_YEAR, "END_YEAR",
                            SIZE_OF_YEAR) ;
  pLawTypes = new TCharListFace (this, IDC_PRINT_RB_LAW_TYPE, "TYPES",
					   Type, QUERY_COLLEGES, SIZE_OF_COLLEGES) ;
  pComposit = new TLongAliasFace(this, IDC_PRINT_RB_COMPOSIT, "COMPOSIT", Composition, QUERY_COMPOSITIONS, ALIAS_EMPTY);
 }  // TPrintReportBookDialog :: makeCommonFields

// -----------------------------------------------------------------------------

#if COURT_TYPE == COURT_APPEAL
TPrintReportBookDialog :: TPrintReportBookDialog (TWindow * parent,
				  begMYEndMYTypeCIU * BMYEMYTCIUPtr,
				  int resId) :
    TGroupDialog (parent, NULL, resId),
    pBMYEMYTCIU (BMYEMYTCIUPtr), pBegMon (NULL), pBegYear (NULL),
    pEndMon (NULL), pEndYear (NULL), pLawTypes (NULL)
 {
  // INSERT>> Your constructor code here.
  makeCommonFields () ;
  // 20060327 ->
    TGroupBox * pFace = new TGroupBox (this, IDC_GROUPBOX4) ;

    reportKind.Add (new TRadioButton (this, IDC_PRINT_RB_INTERNAL_USE_A,
                                      pFace),
			    IDC_PRINT_RB_INTERNAL_USE_A) ;
    reportKind.Add (new TRadioButton (this, IDC_PRINT_RB_FACE_FOR_MINISTRY_A,
                                      pFace),
                      IDC_PRINT_RB_FACE_FOR_MINISTRY_A) ;
  // 20060327 <-
 }  // TPrintReportBookDialog :: TPrintReportBookDialog
#endif // COURT_APPEAL

#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
TPrintReportBookDialog :: TPrintReportBookDialog (TWindow * parent,
			  begMYEndMYTypeCIU * BMYEMYTCIUPtr,
			  int resId) :
    TGroupDialog (parent, NULL, resId),
    pBMYEMYTCIU (BMYEMYTCIUPtr), pBegMon (NULL), pBegYear (NULL),
    pEndMon (NULL), pEndYear (NULL), pLawTypes (NULL)
//    ,pCorruptPrint(NULL)    // m2y
 {
  // INSERT>> Your constructor code here.
  makeCommonFields () ;
   {
    TGroupBox * pFace = new TGroupBox (this, IDC_PRINT_RB_FACE) ;

    reportKind.SetGrpBox (pFace) ;
    reportKind.Add (new TRadioButton (this, IDC_PRINT_RB_INTERNAL_USE,
				     pFace),
		    IDC_PRINT_RB_INTERNAL_USE) ;

    // 2008:304 LRQ: not for admin
#if COURT_TYPE == COURT_AREAL
    rbFaceForMinistry = new TRadioButton (this,
					IDC_PRINT_RB_FACE_FOR_MINISTRY,
					pFace);
    reportKind.Add (rbFaceForMinistry,
		    IDC_PRINT_RB_FACE_FOR_MINISTRY) ;
#endif	// AREAL
    reportKind.Add (new TRadioButton (this, IDC_PRINT_RB_FACE_FOR_BOSS,
						  pFace),
			  IDC_PRINT_RB_FACE_FOR_BOSS) ;

    // 20070604 -->
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	reportKind.Add (new TRadioButton (this, IDC_PRINT_RB_FACE_CORRUPT,
					   pFace), IDC_PRINT_RB_FACE_CORRUPT) ;
    //pCorruptPrint =

    // m2y    pCorruptPrint = new TRadioButton (this,
    //                                         IDC_PRINT_RB_FACE_CORRUPT,
    //                                         pFace) ;
    //    reportKind.Add ( pCorruptPrint ) ;
#endif  // AREAL || MILITARY
    // 20070604 <--

#if COURT_TYPE == COURT_ADMIN
	  reportKind.Add (new TRadioButton (this, IDC_PRINT_RB_FACE_FOR_VSS,
							 pFace), IDC_PRINT_RB_FACE_FOR_VSS) ;
#endif
   }
   {
    TGroupBox * pLawsByInst =
	new TGroupBox (this, IDC_REPORT_BY_INSTANCE) ;

    lawsByInstance.SetGrpBox (pLawsByInst) ;
    lawsByInstance.Add (new TRadioButton (this, IDC_1ST_INSTANCE_REPORT,
                                          pLawsByInst),
                        IDC_1ST_INSTANCE_REPORT) ;
    lawsByInstance.Add (new TRadioButton (this, IDC_2ND_INSTANCE_REPORT,
                                          pLawsByInst),
				IDC_2ND_INSTANCE_REPORT) ;
   }
 }  // TPrintReportBookDialog :: TPrintReportBookDialog
#endif  // ADMIN || AREAL || MILITARY


#if COURT_TYPE == COURT_REGIONAL
TPrintReportBookDialog :: TPrintReportBookDialog (TWindow * parent,
				  begMYEndMYTypeCIU * BMYEMYTCIUPtr,
				  int resId) :
    TGroupDialog (parent, NULL, resId),
    pBMYEMYTCIU (BMYEMYTCIUPtr), pBegMon (NULL), pBegYear (NULL),
    pEndMon (NULL), pEndYear (NULL),
    pLawTypes (NULL)
 {
  // INSERT>> Your constructor code here.
  makeCommonFields () ;
   {
    TGroupBox * pFace = new TGroupBox (this, IDC_PRINT_RB_FACE) ;

    reportKind.SetGrpBox (pFace) ;
    reportKind.Add (new TRadioButton (this, IDC_PRINT_RB_INTERNAL_USE,
                                     pFace),
                    IDC_PRINT_RB_INTERNAL_USE) ;
    reportKind.Add (new TRadioButton (this,
				      IDC_PRINT_RB_FACE_FOR_MINISTRY,
                                      pFace),
                    IDC_PRINT_RB_FACE_FOR_MINISTRY) ;
    reportKind.Add (new TRadioButton (this, IDC_PRINT_RB_FACE_COMBINED,
                                      pFace),
                    IDC_PRINT_RB_FACE_COMBINED) ;
	reportKind.Add (new TRadioButton (this, IDC_PRINT_RB_FACE_CORRUPT,
					   pFace), IDC_PRINT_RB_FACE_CORRUPT) ;
   }
  pAdmLowSubjRng = new TLongFace (this,
	   IDC_PRINT_RB_ADMIN_LOW_RANGE, "ADM_LOW_R", SIZE_OF_NO) ;
  pAdmHighSubjRng = new TLongFace (this,
       IDC_PRINT_RB_ADMIN_HIGH_RANGE, "ADM_HIGH_R", SIZE_OF_NO) ;
 }  // TPrintReportBookDialog :: TPrintReportBookDialog

#endif  // REGIONAL
// 03.may.2000 - end

// 03.may.2000 - begin
void TPrintReportBookDialog :: setCommonFields ()
 {
  pBegMon -> SetL (Today.month) ;
  pEndMon -> SetL (Today.month) ;
  pBegYear -> SetL (Today.year) ;
  pEndYear -> SetL (Today.year) ;
 }  // TPrintReportBookDialog :: setCommonFields

# if INSTANCE
#   if APPEAL                  // COURT_APPEAL -->
void TPrintReportBookDialog :: SetupWindow ()
 {
  TGroupDialog :: SetupWindow () ;

  setCommonFields () ;

  // 2018-02-06 LPR: was IDC_PRINT_RB_INTERNAL_USE_A /disabled/
  reportKind.SetSelectionIdc (IDC_PRINT_RB_FACE_FOR_MINISTRY_A) ;

  pComposit->SetXCrit("");
  turnAdmRanges () ;
 }  // TPrintReportBookDialog :: SetupWindow

#   else                       // COURT_AREAL   and   COURT_ADMIN -->
void TPrintReportBookDialog :: SetupWindow ()
 {
  TGroupDialog :: SetupWindow () ;

  setCommonFields () ;
  reportKind.SetSelectionIdc (IDC_PRINT_RB_INTERNAL_USE) ;
  if (!PRINT_COMPOSITIONS)
    pComposit->SetXCrit("");
  turnAdmRanges () ;
#if COURT_TYPE == COURT_ADMIN
  lawsByInstance.SetSelectionIdc (IDC_1ST_INSTANCE_REPORT) ;
#endif  // ADMIN
#if COURT_TYPE == COURT_AREAL
  if (COURT_CODE == COURT_VAROS)
	rbFaceForMinistry->EnableWindow(true);
  // 2017:117 AREAL only
  lawsByInstance.SetSelectionIdc (IDC_1ST_INSTANCE_REPORT) ;
#endif  // AREAL
  // 2017:117
#if COURT_TYPE == COURT_MILITARY
  lawsByInstance.DisableClusterIdc(CourtType == COURT_MILITARY ? IDC_1ST_INSTANCE_REPORT : IDC_2ND_INSTANCE_REPORT);
#endif  // COURT_MILITARY
 }  // TPrintReportBookDialog :: SetupWindow

#   endif  // APPEAL
# else  // INSTANCE            // COURT_REGIONAL -->
void TPrintReportBookDialog :: SetupWindow ()
 {
  TGroupDialog :: SetupWindow () ;

  setCommonFields () ;
  reportKind.SetSelectionIdc (IDC_PRINT_RB_INTERNAL_USE) ;
  turnAdmRanges () ;
  set_zero_all_comp(pComposit);
 }  // TPrintReportBookDialog :: SetupWindow

# endif  // INSTANCE
// 03.may.2000 - end

bool TPrintReportBookDialog :: isValid ()
 {
  if (! strlen (pLawTypes -> S ()))
    return error(NO_TYPE_CHOOSEN) ;
  if (strlen (pLawTypes -> S ()) > 1 &&
	strchr (pLawTypes -> S (), TYPE_PUNISHMENT_LAW))
    return error(CANNOT_UNIFY_PUNISHMENT) ;
# if INSTANCE
#   if APPEAL  // 03.may.2000  // COURT_APPEAL -->
#   else  // APPEAL            // COURT_AREAL   and   COURT_ADMIN -->
	if (lawsByInstance.GetSelectionIdc () != IDC_1ST_INSTANCE_REPORT &&
	  strchr (pLawTypes -> S (), TYPE_FIRM_LAW))
	return error(FIRM_LAWS_ARE_1ST_INSTANCE) ;
// 20070604 -->
#     if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	if( ((lawsByInstance.GetSelectionIdc () != IDC_1ST_INSTANCE_REPORT ) ||
	   !strchr (pLawTypes -> S (), TYPE_PUNISHMENT_LAW) ) &&
	  (reportKind.GetSelectionIdc () == IDC_PRINT_RB_FACE_CORRUPT )
	  // m2y (pCorruptPrint -> GetCheck () == BF_CHECKED)
	)
	return error("Отчет за корупционни престъпления може да бъде "
			   "изготвен само за наказателни първоинстанционни дела"
		     ) ;
#     endif  // AREAL || MILITARY
// 20070604 <--
#   endif  // APPEAL
# else  // INSTANCE            // COURT_REGIONAL
  if (! strchr (pLawTypes -> S (), TYPE_PUNISHMENT_LAW) &&
	(pAdmLowSubjRng -> L () || pAdmHighSubjRng -> L ()))
    return error("Ако \"Наказателно дело\" не е избрано, "
			 "границите на шифрите за АНД отчета трябва "
			 "да са нули") ;

	if(  !strchr (pLawTypes -> S (), TYPE_PUNISHMENT_LAW) &&
	  (reportKind.GetSelectionIdc () == IDC_PRINT_RB_FACE_CORRUPT )
	  // m2y (pCorruptPrint -> GetCheck () == BF_CHECKED)
	)
	return error("Отчет за корупционни престъпления може да бъде "
			   "изготвен само за наказателни първоинстанционни дела"
		     ) ;
  // 2017:046
  if ((pAdmLowSubjRng->L() && pAdmLowSubjRng->L() != SUBJECT_P_ADM_LOW_SUBJ_RNG) ||
	(pAdmHighSubjRng->L() && pAdmHighSubjRng->L() != SUBJECT_P_ADM_HIGH_SUBJ_RNG))
  {
	return error("Разрешено е изключване само на предмети от %d до %d, или генериране на пълен отчет.",
		SUBJECT_P_ADM_LOW_SUBJ_RNG, SUBJECT_P_ADM_HIGH_SUBJ_RNG);
  }
# endif  // INSTANCE
  // 2011:148 LPR: FIX
  if (!pBegYear->L() || !pEndYear->L())
	return error("Годините са задължителни.");
  return true ;
 }  // TPrintReportBookDialog :: isValid

// 03.may.2000 - begin
void TPrintReportBookDialog :: getCommonFields ()
 {
  pBMYEMYTCIU -> begMonth = pBegMon -> L() ;
  pBMYEMYTCIU -> endMonth = pEndMon -> L() ;
  pBMYEMYTCIU -> begYear = pBegYear -> L () ;
  pBMYEMYTCIU -> endYear = pEndYear -> L () ;
  ostrstream (pBMYEMYTCIU -> types, sizeof (pBMYEMYTCIU -> types))
    << pLawTypes -> S () << ends ;
  pBMYEMYTCIU -> types [sizeof (pBMYEMYTCIU -> types) - 1] = '\0' ;
  if (strlen (pBMYEMYTCIU -> types) < strlen (pLawTypes -> S ()))
    error(TYPES_BUFF_TOO_SHORT) ;  // This
    // normally will never be displayed
 }  // TPrintReportBookDialog :: getCommonFields

#if COURT_TYPE == COURT_APPEAL
void TPrintReportBookDialog :: OKBNClicked ()
 {
  // INSERT>> Your code here.

  if (! isValid ())
    return ;
  getCommonFields () ;
  pBMYEMYTCIU -> composition = 0 ;
  pBMYEMYTCIU -> firstInstLaws = false ;
  pBMYEMYTCIU -> secondInstLaws = false ;  // 20060609
  pBMYEMYTCIU -> forBoss = false ;

  pBMYEMYTCIU -> internalUse =
     reportKind.GetSelectionIdc () == IDC_PRINT_RB_INTERNAL_USE_A ;

  CmOk () ;
 }  // TPrintReportBookDialog::OKBNClicked
#endif  // APPEAL

#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
void TPrintReportBookDialog :: OKBNClicked ()
 {
  // INSERT>> Your code here.

  if (! isValid ())
    return ;
  getCommonFields () ;
  pBMYEMYTCIU -> composition = pComposit->L();
  pBMYEMYTCIU -> firstInstLaws =
    lawsByInstance.GetSelectionIdc () == IDC_1ST_INSTANCE_REPORT ;
  pBMYEMYTCIU -> internalUse =
    reportKind.GetSelectionIdc () == IDC_PRINT_RB_INTERNAL_USE ;
  pBMYEMYTCIU -> forBoss =
    reportKind.GetSelectionIdc () == IDC_PRINT_RB_FACE_FOR_BOSS ;
  // 20070604
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
  pBMYEMYTCIU -> forCorrupt =
    reportKind.GetSelectionIdc () == IDC_PRINT_RB_FACE_CORRUPT ;
    // 20080608  pCorruptPrint -> GetCheck () == BF_CHECKED ;
#endif  // AREAL || MILITARY

#if COURT_TYPE == COURT_ADMIN   // 20070809
  pBMYEMYTCIU -> forVSS =
    reportKind.GetSelectionIdc () == IDC_PRINT_RB_FACE_FOR_VSS ;
  // 20070608  pCorruptPrint -> GetCheck () == BF_CHECKED ;
  // 20070913                                                           // TYPE_PUNISHMENT_LAW
  //if(( pBMYEMYTCIU -> firstInstLaws ) && ( strchr(pBMYEMYTCIU -> types, TYPE_TRICKY_LAW )!=NULL ))
  //{   error("Наказателните дела са само касационни.");
  //    return;
  //}
  //if((! pBMYEMYTCIU -> firstInstLaws ) && ( strchr(pBMYEMYTCIU -> types, TYPE_CITIZEN_LAW )!=NULL ))
  //{   error("Гражданските дела са само първоинстанционни.");
  //    return;
  //}
#endif  // ADMIN


  CmOk () ;
 }  // TPrintReportBookDialog::OKBNClicked
#endif  // ADMIN || AREAL || MILITARY


#if COURT_TYPE == COURT_REGIONAL
void TPrintReportBookDialog :: OKBNClicked ()
 {
  // INSERT>> Your code here.

  if (! isValid ())
    return ;
  getCommonFields () ;
  pBMYEMYTCIU -> internalUse =
    reportKind.GetSelectionIdc () == IDC_PRINT_RB_INTERNAL_USE ;
  pBMYEMYTCIU -> internalUseCombined =
    reportKind.GetSelectionIdc () == IDC_PRINT_RB_FACE_COMBINED ;
  pBMYEMYTCIU -> composition = pComposit->L();
  pBMYEMYTCIU -> admLowSubjRange = pAdmLowSubjRng -> L () ;
  pBMYEMYTCIU -> admHighSubjRange = pAdmHighSubjRng -> L () ;
  pBMYEMYTCIU -> forCorrupt =
    reportKind.GetSelectionIdc () == IDC_PRINT_RB_FACE_CORRUPT ;
    // 20080608  pCorruptPrint -> GetCheck () == BF_CHECKED ;
  CmOk () ;
 }  // TPrintReportBookDialog::OKBNClicked
#endif  // REGIONAL


// 03.may.2000 - end
void TPrintReportBookDialog::turnAdmRanges ()
 {
#if COURT_TYPE == COURT_REGIONAL
  if (strchr (pLawTypes -> S (), TYPE_PUNISHMENT_LAW))
   {
    pAdmLowSubjRng->EnableWindow(true);
    pAdmLowSubjRng->SetL(SUBJECT_P_ADM_LOW_SUBJ_RNG);
    pAdmHighSubjRng->EnableWindow(true);
    pAdmHighSubjRng->SetL(SUBJECT_P_ADM_HIGH_SUBJ_RNG);
    reportKind.EnableItem( IDC_PRINT_RB_FACE_CORRUPT, true );
   }
  else
   {
    pAdmLowSubjRng->EnableWindow(false);
    pAdmLowSubjRng->SetL(0);
    pAdmHighSubjRng->EnableWindow(false);
    pAdmHighSubjRng->SetL(0);
	 if ( reportKind.GetSelectionIdc () == IDC_PRINT_RB_FACE_CORRUPT )
	    reportKind.SetSelectionIdc ( IDC_PRINT_RB_INTERNAL_USE ) ;
       reportKind.EnableItem( IDC_PRINT_RB_FACE_CORRUPT, false );
   }
#endif  // REGIONAL

#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
  if ( (strchr (pLawTypes -> S (), TYPE_PUNISHMENT_LAW)) &&
	 (lawsByInstance.GetSelectionIdc () == IDC_1ST_INSTANCE_REPORT)
     )
     // m2y  pCorruptPrint ->  EnableWindow (true) ;
    reportKind.EnableItem( IDC_PRINT_RB_FACE_CORRUPT, true );
  else
    {
	 // m2y  pCorruptPrint ->  EnableWindow (false) ;
	 if ( reportKind.GetSelectionIdc () == IDC_PRINT_RB_FACE_CORRUPT )
	    reportKind.SetSelectionIdc ( IDC_PRINT_RB_INTERNAL_USE ) ;
	 reportKind.EnableItem( IDC_PRINT_RB_FACE_CORRUPT, false );
    }
#endif  // AREAL || MILITARY

 }  // TPrintReportBookDialog::turnAdmRanges

void TPrintReportBookDialog::lawTypeCBNSelChange()
 {
  turnAdmRanges () ;

 }  // TPrintReportBookDialog::lawTypeCBNSelChange

void TPrintReportBookDialog::turnComposit ()
 {
#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
  if (   ( reportKind.GetSelectionIdc () == IDC_PRINT_RB_INTERNAL_USE )
	     // 20070809  -->
#          if (COURT_TYPE == COURT_ADMIN)
	     && (reportKind.GetSelectionIdc () != IDC_PRINT_RB_FACE_FOR_VSS )
#          endif
	     // 20070809  <--
     )
   {
    if (PRINT_COMPOSITIONS)
	pComposit->SetXCrit(QUERY_COMPOSITIONS);
   }
  else
    pComposit->SetXCrit("");
#endif  // ADMIN || AREAL || MILITARY

#if COURT_TYPE == COURT_REGIONAL
  if ((reportKind.GetSelectionIdc () == IDC_PRINT_RB_INTERNAL_USE ||
	 reportKind.GetSelectionIdc () == IDC_PRINT_RB_FACE_COMBINED
	)
     )
    pComposit->SetXCrit(QUERY_COMPOSITIONS);
  else
    pComposit->SetXCrit("");
#endif  // REGIONAL

// 20070605  -->
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
  if ( (strchr (pLawTypes -> S (), TYPE_PUNISHMENT_LAW)) &&
	 (lawsByInstance.GetSelectionIdc () == IDC_1ST_INSTANCE_REPORT)
     )
     // m2y pCorruptPrint ->  EnableWindow (true) ;
     reportKind.EnableItem( IDC_PRINT_RB_FACE_CORRUPT, true );
  else
    {
	 // m2y pCorruptPrint ->  EnableWindow (false) ;

	 if ( reportKind.GetSelectionIdc () == IDC_PRINT_RB_FACE_CORRUPT )
	  reportKind.SetSelectionIdc ( IDC_PRINT_RB_INTERNAL_USE ) ;
	 reportKind.EnableItem( IDC_PRINT_RB_FACE_CORRUPT, false );
    }
  // 20070910
  if ( reportKind.GetSelectionIdc () == IDC_PRINT_RB_FACE_CORRUPT )
    lawsByInstance.EnableItem( IDC_2ND_INSTANCE_REPORT, false );
  else  // 2017:117 check CourtType 
    lawsByInstance.EnableItem( IDC_2ND_INSTANCE_REPORT, CourtType != COURT_MILITARY );

#endif  // AREAL || MILITARY
// 20070605  <--

#if COURT_TYPE == COURT_REGIONAL
  if ( (strchr (pLawTypes -> S (), TYPE_PUNISHMENT_LAW)) )
     // m2y pCorruptPrint ->  EnableWindow (true) ;
     reportKind.EnableItem( IDC_PRINT_RB_FACE_CORRUPT, true );
  else
    {
	 // m2y pCorruptPrint ->  EnableWindow (false) ;

	 if ( reportKind.GetSelectionIdc () == IDC_PRINT_RB_FACE_CORRUPT )
	  reportKind.SetSelectionIdc ( IDC_PRINT_RB_INTERNAL_USE ) ;
	 reportKind.EnableItem( IDC_PRINT_RB_FACE_CORRUPT, false );
    }
#endif	// REGIONAL

 }  // TPrintReportBookDialog::turnComposit

#if COURT_TYPE == COURT_ADMIN              // 20070809
void TPrintReportBookDialog::turnInstance ()
 {
  if (reportKind.GetSelectionIdc () == IDC_PRINT_RB_FACE_FOR_VSS )
  {
    lawsByInstance.EnableItem (IDC_1ST_INSTANCE_REPORT, false);
    lawsByInstance.EnableItem (IDC_2ND_INSTANCE_REPORT, false);
  }
  else
  {
    lawsByInstance.EnableItem (IDC_1ST_INSTANCE_REPORT, true);
    lawsByInstance.EnableItem (IDC_2ND_INSTANCE_REPORT, true);
  }
 }  // TPrintReportBookDialog::turnInstance
#endif  // ADMIN


void TPrintReportBookDialog :: handleFaceGroupBoxMsg (uint)
 {
  turnComposit () ;
# if COURT_TYPE == COURT_ADMIN
  turnInstance () ;
# endif
 }  // TPrintReportBookDialog :: handleFaceGroupBoxMsg
