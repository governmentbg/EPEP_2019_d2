DEFINE_RESPONSE_TABLE1(TPrintJudgeCheckupDialog, TScaleDialog)
  EV_BN_CLICKED(IDOK, OKBNClicked),
  EV_LBN_SELCHANGE(IDC_PRINT_RB_LAW_TYPE, lawTypeCBNSelChange),
# if INSTANCE
#   if APPEAL
#   else  // of APPEAL          // AREAL only
	EV_CHILD_NOTIFY_ALL_CODES(IDC_PRINT_RB_FACE, handleFaceGroupBoxMsg),
#   endif  // of APPEAL
# else  // of INSTANCE
	EV_CHILD_NOTIFY_ALL_CODES(IDC_PRINT_RB_FACE, handleFaceGroupBoxMsg),
# endif
END_RESPONSE_TABLE;


TPrintJudgeCheckupDialog :: TPrintJudgeCheckupDialog (TWindow * parent,
						      begMYEndMYTypeCIU *
							BMYEMYTCIUPtr,
						      int resId) :
				TScaleDialog (parent, resId),
			      pBMYEMYTCIU (BMYEMYTCIUPtr),
			      pBegMon (NULL), pBegYear (NULL),
			      pEndMon (NULL), pEndYear (NULL),
			      pLawTypes (NULL)
# if INSTANCE
# else
			      , pSubjects (NULL),
			      pSubjSelectionsArr (NULL)
# endif
 {
  pBegMon = new TMonthAliasFace (this, IDC_PRINT_RB_BEG_MONTH,
				 "BEG_MONTH") ;
  pBegYear = new TYearFace (this, IDC_PRINT_RB_BEG_YEAR, "BEG_YEAR") ;
  pEndMon = new TMonthAliasFace (this, IDC_PRINT_RB_END_MONTH,
				"END_MONTH") ;
  pEndYear = new TYearFace (this, IDC_PRINT_RB_END_YEAR, "END_YEAR") ;
  pLawTypes = new TCharListFace (this, IDC_PRINT_RB_LAW_TYPE, "TYPES",
				Type, QUERY_COLLEGES, SIZE_OF_COLLEGES);
  pComposit = new TLongAliasFace(this, IDC_PRINT_RB_COMPOSIT, "COMPOSIT", Composition, QUERY_COMPOSITIONS, ALIAS_EMPTY);

# if INSTANCE
#   if APPEAL
   {                                         // APPEAL COURT only
    // 2008:253 LPR +related
    TGroupBox * pCKGrpBox = new TGroupBox (this, IDC_PRINT_RB_FACE) ;

    checkupKinds.SetGrpBox (pCKGrpBox) ;
    checkupKinds.Add (new TRadioButton (this, IDC_PRINT_RB_INTERNAL_USE,
					pCKGrpBox),
					IDC_PRINT_RB_INTERNAL_USE) ;
    checkupKinds.Add (new TRadioButton (this, IDC_PRINT_RB_FOR_VSS,
					pCKGrpBox),
					IDC_PRINT_RB_FOR_VSS );
    checkupKinds.Add (new TRadioButton (this, IDC_PRINT_RB_FOR_VSS_APPEAL,
					pCKGrpBox),
					IDC_PRINT_RB_FOR_VSS_APPEAL) ;
   }
#   else  // of APPEAL                       // AREAL COURT only
   {                                         // AREAL COURT only
    TGroupBox * pCKGrpBox = new TGroupBox (this, IDC_PRINT_RB_FACE) ;

    checkupKinds.SetGrpBox (pCKGrpBox) ;
    checkupKinds.Add (new TRadioButton (this, IDC_PRINT_RB_INTERNAL_USE,
					pCKGrpBox),
					IDC_PRINT_RB_INTERNAL_USE) ;
    checkupKinds.Add (new TRadioButton (this, IDC_PRINT_RB_FACE_COMBINED,
					pCKGrpBox),
					IDC_PRINT_RB_FACE_COMBINED) ;
    checkupKinds.Add (new TRadioButton (this,
					IDC_PRINT_RB_FACE_FOR_MINISTRY,
					pCKGrpBox),
					IDC_PRINT_RB_FACE_FOR_MINISTRY) ;
    // 20060607 -->
    checkupKinds.Add (new TRadioButton (this,
					IDC_RB_FACE_INCOMING_EXTEND,
					pCKGrpBox),
					IDC_RB_FACE_INCOMING_EXTEND) ;
    // 20060607 <--
    checkupKinds.Add (new TRadioButton (this,
					IDC_PRT_JCHKUP_DECIDED_LAWS,
					pCKGrpBox),
					IDC_PRT_JCHKUP_DECIDED_LAWS) ;
    checkupKinds.Add (new TRadioButton (this,
					IDC_PRT_JCHKUP_SESS_DISTRIB,
					pCKGrpBox),
					IDC_PRT_JCHKUP_SESS_DISTRIB) ;
    // 2008:186 LPR
    checkupKinds.Add (new TRadioButton (this, IDC_PRINT_RB_FOR_VSS,
					pCKGrpBox),
					IDC_PRINT_RB_FOR_VSS );
    checkupKinds.Add (new TRadioButton (this, IDC_PRINT_RB_FOR_VSS_APPEAL,
					pCKGrpBox),
					IDC_PRINT_RB_FOR_VSS_APPEAL) ;
   }
   pNoon = new TTimeFaceOldVal (this, IDC_PRT_JCHKUP_NOON, "NOON") ;  // 07.jun.2002
#   endif  // of APPEAL
# else  // of INSTANCE
   {
    TGroupBox * pCKGrpBox = new TGroupBox (this, IDC_PRINT_RB_FACE) ;

    checkupKinds.SetGrpBox (pCKGrpBox) ;
    checkupKinds.Add (new TRadioButton (this, IDC_PRINT_RB_INTERNAL_USE,
					pCKGrpBox),
					IDC_PRINT_RB_INTERNAL_USE) ;
    checkupKinds.Add (new TRadioButton (this, IDC_PRINT_RB_FACE_COMBINED,
					pCKGrpBox),
					IDC_PRINT_RB_FACE_COMBINED) ;
    // 2008:183 LPR
    checkupKinds.Add (new TRadioButton (this, IDC_PRINT_RB_FOR_VSS,
					pCKGrpBox),
					IDC_PRINT_RB_FOR_VSS );
    checkupKinds.Add (new TRadioButton (this, IDC_PRINT_RB_FOR_VSS_APPEAL,
					pCKGrpBox),
					IDC_PRINT_RB_FOR_VSS_APPEAL) ;
   }
# endif  // of INSTANCE
# if ! INSTANCE
   {
    pSubjects = new TSubjectListBox (this, IDC_SUBJECT_LB, C2S[KIND_PRIVATE_PUNISHMENT_LAW]);
   }
  pSubjSelectionsArr = new TGArray <TSubject> () ;
# endif
 }  // TPrintJudgeCheckupDialog :: TPrintJudgeCheckupDialog


TPrintJudgeCheckupDialog::~TPrintJudgeCheckupDialog()
{
# if ! INSTANCE
  delete pSubjSelectionsArr ;
# endif
}


void TPrintJudgeCheckupDialog::OKBNClicked()
{
  // INSERT>> Your code here.

  if (! isValid ())
    return ;
  pBMYEMYTCIU -> begMonth = pBegMon -> L() ;
  pBMYEMYTCIU -> endMonth = pEndMon -> L() ;
  pBMYEMYTCIU -> begYear = pBegYear -> L () ;
  pBMYEMYTCIU -> endYear = pEndYear -> L () ;
  ostrstream (pBMYEMYTCIU -> types, sizeof (pBMYEMYTCIU -> types))
    << pLawTypes -> S () << ends ;
  pBMYEMYTCIU -> types [sizeof (pBMYEMYTCIU -> types) - 1] = '\0' ;
  if (strlen (pBMYEMYTCIU -> types) < strlen (pLawTypes -> S ()))
    error(TYPES_BUFF_TOO_SHORT) ; // This
	// normally will never be displayed

# if INSTANCE
#   if APPEAL                             // APPEAL only
  pBMYEMYTCIU -> internalUse =
    checkupKinds.GetSelectionIdc () == IDC_PRINT_RB_INTERNAL_USE ;
  pBMYEMYTCIU -> composition = 0 ;
#   else  // of APPEAL                    // AREAL COURT only
  // 20060607 -->
  pBMYEMYTCIU -> incomingExtend =
    checkupKinds.GetSelectionIdc () == IDC_RB_FACE_INCOMING_EXTEND ;
  // 20060607 <--
  pBMYEMYTCIU -> internalUse =
    checkupKinds.GetSelectionIdc () == IDC_PRINT_RB_INTERNAL_USE ;
  pBMYEMYTCIU -> forBoss =
    checkupKinds.GetSelectionIdc () == IDC_PRINT_RB_FACE_FOR_MINISTRY ;
  pBMYEMYTCIU -> decidedLawsJudgeCheckup =
		checkupKinds.GetSelectionIdc () == IDC_PRT_JCHKUP_DECIDED_LAWS ;
	pBMYEMYTCIU -> sessionsDistribution =
		checkupKinds.GetSelectionIdc () == IDC_PRT_JCHKUP_SESS_DISTRIB ;
	pBMYEMYTCIU -> noon = pNoon -> Time () ;
	pBMYEMYTCIU -> composition = pComposit->L();
#   endif  // of APPEAL
# else  // of INSTANCE
                                       // REGIONAL COURT only
  pBMYEMYTCIU -> internalUse =
    checkupKinds.GetSelectionIdc () == IDC_PRINT_RB_INTERNAL_USE ;

  pBMYEMYTCIU -> composition = pComposit->L();

  if (checkupKinds.GetSelectionIdc () == IDC_PRINT_RB_FACE_COMBINED)
   {
    if (pSubjects->IsWindowEnabled())
	pSubjects -> GetMark (pSubjSelectionsArr) ;
    else
	pSubjSelectionsArr->Flush();

    const int items = pSubjSelectionsArr -> Count () ;

    for (int i = 0 ; i < items ; i ++)
	pBMYEMYTCIU ->
	  addToArr ((* ((* pSubjSelectionsArr) [i])).subject) ;
   }
# endif  // of INSTANCE

  // 2008:183 LPR; 2008:189 AREAL || REGIONAL -> all
  switch (checkupKinds.GetSelectionIdc())
  {
    case IDC_PRINT_RB_INTERNAL_USE : pBMYEMYTCIU->variant = JUDGE_REPORT_INTERNAL; break;
    case IDC_PRINT_RB_FOR_VSS : pBMYEMYTCIU->variant = JUDGE_REPORT_VSS; break;
    case IDC_PRINT_RB_FOR_VSS_APPEAL : pBMYEMYTCIU->variant = JUDGE_REPORT_VSS_APPEAL; break;
    case IDC_PRINT_RB_FACE_COMBINED : pBMYEMYTCIU->variant = JUDGE_REPORT_COMBINED; break;
    default : pBMYEMYTCIU->variant = JUDGE_REPORT_NULL;
  }

  CmOk () ;
}  // TPrintJudgeCheckupDialog::OKBNClicked

void TPrintJudgeCheckupDialog::SetupWindow()
{
  TScaleDialog::SetupWindow();

  pBegMon -> SetL (Today.month) ;
  pEndMon -> SetL (Today.month) ;
  pBegYear -> SetL (Today.year) ;
  pEndYear -> SetL (Today.year) ;
# if INSTANCE
#   if APPEAL
  checkupKinds.SetSelectionIdc (IDC_PRINT_RB_INTERNAL_USE) ;
  pComposit->SetXCrit("");
#   else  // of APPEAL
  if (!PRINT_COMPOSITIONS)
    pComposit->SetXCrit("");
	pNoon -> SetTime (CTime (12, 30, 0)) ;
	pNoon -> DisableFace ();  // Safe because of the next line
	checkupKinds.SetSelectionIdc (IDC_PRINT_RB_INTERNAL_USE) ;
#   endif  // of APPEAL
# else  // of INSTANCE
  set_zero_all_comp(pComposit);
  checkupKinds.SetSelectionIdc (IDC_PRINT_RB_INTERNAL_USE) ;
# endif  // of INSTANCE
  turnInternalUse () ;
}  //  TPrintJudgeCheckupDialog::SetupWindow

bool TPrintJudgeCheckupDialog :: isValid ()
 {
  if (! strlen (pLawTypes -> S ()))
    return error(NO_TYPE_CHOOSEN) ;
  if (strlen (pLawTypes -> S ()) > 1 &&
	strchr (pLawTypes -> S (), TYPE_PUNISHMENT_LAW))
    return error(CANNOT_UNIFY_PUNISHMENT) ;
# if INSTANCE
#   if APPEAL
#   else  // APPEAL
    // 2017:047 LPR: comout
//  if (strlen (pLawTypes -> S ()) > 1 &&
//	strchr (pLawTypes -> S (), TYPE_FIRM_LAW))
//    return error("Фирмените дела не могат да се комбинират "
//			 "с другите видове.") ;
  if (strchr (pLawTypes -> S (), TYPE_FIRM_LAW))
    if (checkupKinds.GetSelectionIdc () == IDC_PRINT_RB_FACE_FOR_MINISTRY)
	return error("Този вид справка не може да се генерира "
				   "за фирмени дела.") ;
#   endif  // APPEAL
# else  // INSTANCE
  if (checkupKinds.GetSelectionIdc () == IDC_PRINT_RB_FACE_COMBINED)
   {
    if (pSubjects->IsWindowEnabled())
	pSubjects -> GetMark (pSubjSelectionsArr) ;
    else
	pSubjSelectionsArr->Flush();
    if (pSubjSelectionsArr -> Count () == 0)
	return error("Няма избран предмет.") ;
   }
# endif
  // 2011:148 LPR: FIX
  if (!pBegYear->L() || !pEndYear->L())
	return error("Годините са задължителни.");
  return true ;
 }  //  TPrintJudgeCheckupDialog :: isValid

void TPrintJudgeCheckupDialog :: turnInternalUse ()
 {
# if INSTANCE
#   if APPEAL
#   else  // of APPEAL
  const char * const lT = pLawTypes -> S () ;

  if (! lT [0])
   {
    if (checkupKinds.GrpBoxIsEnabled ())
      checkupKinds.DisableClusterIdc (IDC_PRINT_RB_INTERNAL_USE) ;
   }
  else
    if (checkupKinds.GrpBoxIsEnabled () == false)
      checkupKinds.EnableCluster () ;
#   endif  // of APPEAL  // 29.may.2000
# else  // of INSTANCE
  // 2008:183 LPR/LRQ: only (en)disable "Мярка за неотклонение"
  // 2008:186 LPR: for all courts; 2009:012 FIX: REGIONAL only!
  if (strchr (pLawTypes -> S (), TYPE_PUNISHMENT_LAW))
   {
    checkupKinds.EnableItemIdc (IDC_PRINT_RB_FACE_COMBINED);
    //if (checkupKinds.GrpBoxIsEnabled () == false)
    //  checkupKinds.EnableCluster () ;
   }
  else
  {
    if (checkupKinds.GetSelectionIdc () == IDC_PRINT_RB_FACE_COMBINED)
	checkupKinds.SetSelectionIdc (IDC_PRINT_RB_INTERNAL_USE);
    checkupKinds.EnableItemIdc (IDC_PRINT_RB_FACE_COMBINED, false);
    //if (checkupKinds.GrpBoxIsEnabled ())
    //  checkupKinds.DisableClusterIdc (IDC_PRINT_RB_INTERNAL_USE) ;
  }
# endif  // of INSTANCE
 }  // TPrintJudgeCheckupDialog :: turnInternalUse

void TPrintJudgeCheckupDialog :: lawTypeCBNSelChange ()
{
  turnInternalUse () ;
}  // TPrintJudgeCheckupDialog :: lawTypeCBNSelChange

# if INSTANCE
#   if APPEAL
#   else  // of APPEAL
void TPrintJudgeCheckupDialog :: handleFaceGroupBoxMsg (uint)
{  // 06.jun.2002
  if (checkupKinds.GetSelectionIdc () == IDC_PRT_JCHKUP_SESS_DISTRIB)
  {
    pNoon -> EnableFace () ;
  }
  else
  {
    pNoon -> DisableFace () ;
  }
}  // TPrintJudgeCheckupDialog :: handleFaceGroupBoxMsg

#   endif  // of APPEAL
# else // of INSTANCE
void TPrintJudgeCheckupDialog :: handleFaceGroupBoxMsg (uint)
 {
  if (checkupKinds.GetSelectionIdc () == IDC_PRINT_RB_FACE_COMBINED)
   {
    pSubjects -> EnableWindow (true) ;
   }
  else
   {
    pSubjects -> EnableWindow (false) ;
   }
 }  // TPrintJudgeCheckupDialog :: handleFaceGroupBoxMsg

# endif
