#include "cac.h"  // 18.may.2001
//# include "all.h"

// Look for an old version of this dialog in versionArchices\732

DEFINE_RESPONSE_TABLE1(TPrintTimeBookDialog, TGroupDialog)
  EV_CBN_SELCHANGE(IDC_PRINT_TB_COLLEGE, lawTypeCBNSelChange),
  EV_BN_CLICKED(IDOK, OKBNClicked),
  EV_EN_KILLFOCUS(IDC_PRINT_TB_DATE_BEGIN, begDFocusKilled),
  EV_EN_KILLFOCUS(IDC_PRINT_TB_HEAPING_DATE, bkHeapingBegDFocusKilled),
END_RESPONSE_TABLE;

TPrintTimeBookDialog :: TPrintTimeBookDialog (TWindow * parent,
				 collCompBegDEndDBegN * aCollCompBegDEndDBegNPtr,
                         int resId) :
                         TGroupDialog (parent, NULL, resId),
                         pCollCompBegDEndDBegN (aCollCompBegDEndDBegNPtr),
                         pBkHeapingBegD (NULL),
                         pWantBook (NULL),
                         pWantSummary (NULL),
                         pWantJudgesSummary (NULL),
                         pWantResultsCheckup (NULL),
                         pJan2005Format (NULL),  // 08.feb.2005
                         pSplitIn2 (NULL)   // 08.feb.2005
 {
  pClg = new TTypeAliasFace (this, IDC_PRINT_TB_COLLEGE, "F_TYPE",
                             true, NULL) ;
  pBegD = new TDateFace (this, IDC_PRINT_TB_DATE_BEGIN, "F_DATE_BEGIN",
                         true) ;
  new TDateFaceUpDown(this, IDC_PRINT_TB_DATE_BEGIN_UPDN, pBegD);
  pEndD = new TDateFace (this, IDC_PRINT_TB_DATE_END, "F_DATE_END",
                         true) ;
  new TDateFaceUpDown(this, IDC_PRINT_TB_DATE_END_UPDN, pEndD);
  pBN = new TLongFace (this, IDC_PRINT_TB_NO_BEGIN, "F_NO_BEGIN",
                       SIZE_OF_NO) ;
# if INSTANCE
   {
    tb_1st_instance_laws = new TRadioButton (this, IDC_1ST_INSTANCE_LAWS);
#   if APPEAL
#   else  // of APPEAL
    tb_2nd_instance_laws = new TRadioButton (this, IDC_2ND_INSTANCE_LAWS);
    tb_all_2nd_instance_laws = new TRadioButton (this, IDC_ALL_2ND_INSTANCE_LAWS);
#   endif  // of APPEAL
    tb_3rd_instance_laws_pac = new TRadioButton (this, IDC_3RD_INSTANCE_LAWS_PAC);
   }
   {
    tb_print_tb_whole_day = new TRadioButton (this, IDC_PRINT_TB_WHOLE_DAY);
    tb_print_tb_morning = new TRadioButton (this, IDC_PRINT_TB_MORNING);
    tb_print_tb_afternoon = new TRadioButton (this, IDC_PRINT_TB_AFTERNOON);
   }
  pNoon = new TTimeFace (this, IDC_PRINT_TB_NOON, "NOON", false) ;
# endif  // of INSTANCE
  pComposit = new TLongAliasFace(this, IDC_PRINT_TB_COMPOSIT, "F_COMPOSIT", Composition, QUERY_COMPOSITIONS, ALIAS_EMPTY);
  pBkHeapingBegD = new TDateFace (this, IDC_PRINT_TB_HEAPING_DATE,
                                  "F_BK_DATE_BEGIN", true) ;
  pWantBook = new TCheckBox (this, IDC_PRINT_TB_WANT_BOOK, NULL) ;
  pWantSummary = new TCheckBox (this, IDC_PRINT_TB_WANT_SUMMARY, NULL) ;
  pWantJudgesSummary = new TCheckBox (this, IDC_PRINT_TB_WANT_JUDGE_SUMMARY,
                                      NULL) ;
  pWantResultsCheckup = new TCheckBox (this, IDC_PRINT_TB_WANT_RESULTS_SET,
                                       NULL) ;
  // 08.feb.2005 - begin
  pJan2005Format = new TCheckBox (this, IDC_PRINT_TB_NEW_FORMAT, NULL) ;
  pSplitIn2 = new TCheckBox (this, IDC_PRINT_TB_SPLIT_IN_2, NULL) ;
  // 08.feb.2005 - end

  // 20051206  rb_OrderBy ->
  {
#   if INSTANCE
#      if APPEAL      // appeal
	    tb_rba1 = new TRadioButton (this, IDC_RBA1);
	    tb_rba2 = new TRadioButton (this, IDC_RBA2);
#      else           // areal
	    tb_rb_1 = new TRadioButton (this, IDC_RB_1);
	    tb_rb_2 = new TRadioButton (this, IDC_RB_2);
#      endif
#   else              // regional
	 tb_rb_order_1 = new TRadioButton (this, IDC_RB_ORDER_1);
	 tb_rb_order_2 = new TRadioButton (this, IDC_RB_ORDER_2);
#   endif
  }
  // 20051206  rb_OrderBy <-

 }  // TPrintTimeBookDialog :: TPrintTimeBookDialog


void TPrintTimeBookDialog :: adjustHeapingBegD ()
 {
  CDate d = pBegD -> Date () ;

  d.day = 1 ;
  pBkHeapingBegD -> SetDate (d) ;
 }  // TPrintTimeBookDialog :: adjustHeapingBegD

void TPrintTimeBookDialog :: SetupWindow ()
 {
  TGroupDialog :: SetupWindow () ;
   {
    pBegD -> SetDate (Today) ;
    pEndD -> SetDate (Today) ;
   }
  pBN -> SetL (0) ;
  pClg -> SetSelIndex (0) ;
  turnClSessCheckBox () ;
  adjustHeapingBegD () ;
  pWantBook -> SetCheck (BF_CHECKED) ;
  pWantSummary -> SetCheck (BF_CHECKED) ;
  pWantJudgesSummary -> SetCheck (BF_CHECKED) ;
  pWantResultsCheckup -> SetCheck (BF_UNCHECKED) ;  // 21.sep.2000
# if INSTANCE
  tb_1st_instance_laws->SetCheck(BF_CHECKED);
  tb_print_tb_whole_day->SetCheck(BF_CHECKED);
  pNoon -> SetTime (CTime (12, 30, 0)) ;
#   if APPEAL
  pComposit->SetXCrit("");
#   else  // of APPEAL
  if (PRINT_COMPOSITIONS)
    select_de1st_comp(pComposit);
  else
    pComposit->SetXCrit("");
#   endif  // of APPEAL
# else          // REGIONAL
  select_de1st_comp(pComposit);
# endif  // ALL
  if(default_split_wide_hyper_docs())  // 08.feb.2005 - begin
  {
	pSplitIn2->SetCheck(BF_CHECKED);
  }
  pJan2005Format->SetCheck(BF_CHECKED);  // 08.feb.2005 - end

  pSplitIn2->Show(SW_HIDE); //20050314

#   if INSTANCE
#      if APPEAL      // appeal
	   tb_rba1->SetCheck(BF_CHECKED) ;       // 20051206
#      else           // areal
	   tb_rb_1->SetCheck(BF_CHECKED) ;       // 20051206
#      endif
#   else              // regional
	 tb_rb_order_1->SetCheck(BF_CHECKED) ;   // 20051206
#   endif

 }  // TPrintTimeBookDialog :: SetupWindow

bool TPrintTimeBookDialog :: isValid ()
 {
  if (pBkHeapingBegD -> Date () > pBegD -> Date ())
    return error("Некоректна дата на натрупване!") ;
#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
  const char lawType = pClg -> C () ;

#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
    const bool wantConcern = tb_3rd_instance_laws_pac->GetCheck() == BF_CHECKED;
    const bool want2nd = tb_2nd_instance_laws->GetCheck() == BF_CHECKED;
    const bool wantAll2nd = tb_all_2nd_instance_laws->GetCheck() == BF_CHECKED;
    if ((wantConcern &&
	  lawType != TYPE_PUNISHMENT_LAW && lawType != TYPE_ADMIN_LAW
	 ) ||
	 (want2nd && lawType == TYPE_ADMIN_LAW) ||
	 (want2nd && lawType == TYPE_FIRM_LAW) ||
	 (wantAll2nd && lawType != TYPE_PUNISHMENT_LAW)  // 02.jan.2001
	)
	return error("\"Вид дело\" и \"Дела по инстанции\" "
			 "не си съответствуват") ;
#endif  // AREAL || MILITARY

#if COURT_TYPE == COURT_ADMIN
    // 2008:320 LRQ: not any more
    //if( (lawType == TYPE_PUNISHMENT_LAW || lawType == TYPE_TRICKY_LAW)
    //     &&
    //    (tb_3rd_instance_laws_pac->GetCheck() != BF_CHECKED)
    //   )
    //   return error(this, "В Административен съд наказателните дела са само касационни.") ;
    if( (lawType == TYPE_CITIZEN_LAW)
	   &&
	  (tb_1st_instance_laws->GetCheck() != BF_CHECKED)
	 )
	 return error("В Административен съд гражданските дела са само първоинстанционни.") ;
#endif  // ADMIN

#endif  // ADMIN || AREAL || MILITARY
  return true ;
 }  // TPrintTimeBookDialog :: isValid

void TPrintTimeBookDialog :: OKBNClicked ()
 {
  if (isValid () == false)
    return ;
  ostrstream (pCollCompBegDEndDBegN -> colleges,
		  sizeof (pCollCompBegDEndDBegN -> colleges))
    << pClg -> C () << ends ;
  (pCollCompBegDEndDBegN -> colleges)
    [sizeof (pCollCompBegDEndDBegN -> colleges) - 1] = '\0' ;
  pCollCompBegDEndDBegN -> composit = pComposit->L();
  pCollCompBegDEndDBegN -> begD = pBegD -> Date () ;
  pCollCompBegDEndDBegN -> endD = pEndD -> Date () ;
  pCollCompBegDEndDBegN -> begN = pBN -> L () ;
# if INSTANCE
#   if APPEAL
  pCollCompBegDEndDBegN -> firstInstLaws = false ;
  pCollCompBegDEndDBegN -> puniAdmConcernLaws = false ;
  if (TYPE_PUNISHMENT_LAW == pCollCompBegDEndDBegN -> colleges [0])
   {
    const bool keepingMethodBook = tb_3rd_instance_laws_pac->GetCheck() == BF_CHECKED;
    TSubject subj ;  // 16.jun.2004, 15.jul.2004 uncommented
//    char * s = new char [1024] ;  // 16.jun.2004, 15.jul.2004 uncommented, 20.jul.2004

    pCollCompBegDEndDBegN -> keepingMethodTimeBook = keepingMethodBook ;
// 20.jul.2004 - begin
//    subj.type = pCollCompBegDEndDBegN -> colleges [0] ;  // 16.jun.2004-begin,
//                                                   // 15.jul.2004 uncommented
//    subj.FloatCriteria (s) ;
//
//    TQuery q (s) ;
//
//    while (q.Read ())
//     {
//      subj << q ;
//      if (subj.type == pCollCompBegDEndDBegN -> colleges [0] &&
//          keepingMethodBook == (SUBJECT_9002 == subj.subject ||
//                                SUBJECT_9003 == subj.subject
//                               )
//         )
//        pCollCompBegDEndDBegN -> addToArr (subj.subject) ;
//     }
//    delete [] s ;  // 16.jun.2004 - end, 15.jul.2004 uncommented
// 20.jul.2004 - end
   }
#   else  // of APPEAL
  pCollCompBegDEndDBegN -> firstInstLaws = tb_1st_instance_laws->GetCheck() == BF_CHECKED;
  pCollCompBegDEndDBegN -> puniAdmConcernLaws = tb_3rd_instance_laws_pac->GetCheck() == BF_CHECKED;
  pCollCompBegDEndDBegN -> all2ndPuniLaws = tb_all_2nd_instance_laws->GetCheck() == BF_CHECKED;
// ---------------------------- CHANGE !!! ----------------------------
  if (pCollCompBegDEndDBegN -> puniAdmConcernLaws &&
      pClg -> C () == TYPE_ADMIN_LAW)
    pCollCompBegDEndDBegN -> puniAdmConcernLaws = false ;
// --------------------------------------------------------------------
#   endif  // of APPEAL  // 31.mar.2000
  pCollCompBegDEndDBegN -> noon = pNoon -> Time () ;
  if (tb_print_tb_whole_day->GetCheck() == BF_CHECKED)
    pCollCompBegDEndDBegN -> whichPartOfDay = WholeDay ;
  else if (tb_print_tb_morning->GetCheck() == BF_CHECKED)
    pCollCompBegDEndDBegN -> whichPartOfDay = Morning ;
  else
    pCollCompBegDEndDBegN -> whichPartOfDay = Afternoon ;
# endif  // of INSTANCE
  pCollCompBegDEndDBegN -> heapingStartDate =
    pBkHeapingBegD -> Date () ;
  pCollCompBegDEndDBegN -> wantBook =
    (pWantBook -> GetCheck () == BF_CHECKED) ;
  pCollCompBegDEndDBegN -> wantSummary =
    (pWantSummary -> GetCheck () == BF_CHECKED) ;
  pCollCompBegDEndDBegN -> wantJudgesSummary =
    (pWantJudgesSummary -> GetCheck () == BF_CHECKED) ;
  pCollCompBegDEndDBegN -> wantResultsCheckup =
    pWantResultsCheckup -> GetCheck () == BF_CHECKED ;
  // 08.feb.2004 - begin
  pCollCompBegDEndDBegN -> jan2005Format =
    pJan2005Format -> GetCheck () == BF_CHECKED ;
  pCollCompBegDEndDBegN -> splitIn2 = pSplitIn2 -> GetCheck () == BF_CHECKED ;
  // 08.feb.2004 - end
#   if INSTANCE
#      if APPEAL      // appeal
	   pCollCompBegDEndDBegN -> orderBy1 = tb_rba1->GetCheck() == BF_CHECKED;
#      else           // areal
	   pCollCompBegDEndDBegN -> orderBy1 = tb_rb_1->GetCheck() == BF_CHECKED;
#      endif
#   else              // regional
	pCollCompBegDEndDBegN -> orderBy1 = tb_rb_order_1->GetCheck() == BF_CHECKED;
#   endif

  CmOk () ;
 }  // TPrintTimeBookDialog :: OKBNClicked

void TPrintTimeBookDialog :: turnClSessCheckBox ()
 {
# if INSTANCE
#   if APPEAL
  bool enable = pClg -> C () == TYPE_PUNISHMENT_LAW;

  tb_1st_instance_laws->EnableWindow(enable);
  tb_3rd_instance_laws_pac->EnableWindow(enable);
#   endif  // of APPEAL
# endif  // of INSTANCE
 }  // TPrintTimeBookDialog::turnClSessCheckBox

void TPrintTimeBookDialog::lawTypeCBNSelChange()
 {
  turnClSessCheckBox () ;
 }  // TPrintTimeBookDialog::lawTypeCBNSelChange

void TPrintTimeBookDialog :: begDFocusKilled ()
 {
  adjustHeapingBegD () ;
 }  // TPrintTimeBookDialog :: begDFocusKilled

void TPrintTimeBookDialog :: bkHeapingBegDFocusKilled ()
 {
  if (pBkHeapingBegD -> Date () > pBegD -> Date ())
    if (ask ("Тази дата е по-голяма от началната и ще бъде"
             " приравнена към нея. Ще я коригирате ли?"))
      pBkHeapingBegD -> SetFocus () ;
    else
      pBkHeapingBegD -> SetDate (pBegD -> Date ()) ;
 }  // TPrintTimeBookDialog :: bkHeapingBegDFocusKilled

// 20.sep.2000 - end
//
