class TPrintJudgeCheckupDialog : public TScaleDialog
 {
  protected :
    begMYEndMYTypeCIU * pBMYEMYTCIU ;
    TMonthAliasFace * pBegMon ;
    TLongFace * pBegYear ;
    TMonthAliasFace * pEndMon ;
    TLongFace * pEndYear ;
    TCharListFace * pLawTypes ;
    TLongAliasFace *pComposit;
    RadioCluster checkupKinds ;
# if INSTANCE
#   if APPEAL
#   else  // of APPEAL
		TTimeFaceOldVal * pNoon ;  // 06.jun.2002
#   endif  // of APPEAL
# else  // of INSTANCE
    TSubjectListBox * pSubjects ;
    TGArray <TSubject> * pSubjSelectionsArr ;
# endif  // of INSTANCE
    void turnInternalUse () ;
    bool isValid () ;
  public:
    TPrintJudgeCheckupDialog (TWindow * parent,
				begMYEndMYTypeCIU * BMYEMYTCIUPtr,
# if INSTANCE
#   if APPEAL
				int resId =
				IDD_PRT_JUDGE_CHKUP_BK_APPEAL
#   else  // of APPEAL
				int resId = IDD_PRT_JUDGE_CHECKUP_BOOK
#   endif  // of APPEAL
# else
				int resId =
				IDD_PRT_JUDGE_CHKUP_BK_REGIONAL
# endif
			      ) ;
    virtual ~TPrintJudgeCheckupDialog () ;

//{{TPrintJudgeCheckupDialogVIRTUAL_BEGIN}}
  public:
    virtual void SetupWindow () ;
//{{TPrintJudgeCheckupDialogVIRTUAL_END}}

//{{TPrintJudgeCheckupDialogRSP_TBL_BEGIN}}
  protected:
    void OKBNClicked () ;
    void lawTypeCBNSelChange () ;
# if INSTANCE
#  if APPEAL
#  else  //   COURT_ADMIN, COURT_AREAL, COURT_MILITARY 
    void handleFaceGroupBoxMsg (uint notifyCode) ;
#  endif  // of APPEAL
# else  // of INSTANCE
    void handleFaceGroupBoxMsg (uint notifyCode) ;
# endif
//{{TPrintJudgeCheckupDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TPrintJudgeCheckupDialog);
 };    //{{TPrintJudgeCheckupDialog}}
