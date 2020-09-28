class TPrintReportBookDialog : public TGroupDialog
 {
  protected :
    begMYEndMYTypeCIU * pBMYEMYTCIU ;
    TMonthAliasFace * pBegMon ;
    TLongFace * pBegYear ;
    TMonthAliasFace * pEndMon ;
    TLongFace * pEndYear ;
    TCharListFace * pLawTypes ;
    TLongAliasFace *pComposit;

    RadioCluster reportKind ;

#if COURT_TYPE == COURT_REGIONAL
    TLongFace *pAdmLowSubjRng;
    TLongFace *pAdmHighSubjRng;
#endif  // REGIONAL

#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
    RadioCluster lawsByInstance ;
#endif  // ADMIN || AREAL || MILITARY

#if COURT_TYPE == COURT_AREAL
	TRadioButton *rbFaceForMinistry;	// 2016:195 en/disable +related
#endif  // AREAL

    void makeCommonFields () ;
    void setCommonFields () ;
    void getCommonFields () ;
    void turnAdmRanges () ;
    void turnComposit () ;

#if COURT_TYPE == COURT_ADMIN
    void turnInstance () ;            // 20070809
#endif  // ADMIN

    bool isValid () ;

  public:
    TPrintReportBookDialog (TWindow* parent,
				    begMYEndMYTypeCIU * BMYEMYTCIUPtr,
				    int resId =
#if COURT_TYPE == COURT_APPEAL
							 IDD_PRINT_REPORT_BOOK_APPEAL
#endif  // APPEAL
#if COURT_TYPE == COURT_ADMIN
							 IDD_PRINT_REPORT_BOOK_ADMIN
#endif  // ADMIN
#if COURT_TYPE == COURT_AREAL
							 IDD_PRINT_REPORT_BOOK_AREAL
#endif  // AREAL
#if COURT_TYPE == COURT_MILITARY
#define IDD_PRINT_REPORT_BOOK_MILITARY IDD_PRINT_REPORT_BOOK_AREAL
							 IDD_PRINT_REPORT_BOOK_MILITARY
#endif  // MILITARY
#if COURT_TYPE == COURT_REGIONAL
							 IDD_PRINT_REPORT_BOOK
#endif  // REGIONAL
				    );
    virtual void SetupWindow () ;
  protected:
    void OKBNClicked();
    void lawTypeCBNSelChange () ;
    void initializeBNClicked () ;
    void handleFaceGroupBoxMsg (uint) ;
DECLARE_RESPONSE_TABLE(TPrintReportBookDialog);
 } ;  // class TPrintReportBookDialog
