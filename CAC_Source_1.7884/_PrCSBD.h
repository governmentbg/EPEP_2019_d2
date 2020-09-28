class TPrintClosedSessBkDialog : public TGroupDialog {
  protected :
    collBegDEndDBegN * pBegDEndDBegN ;
    TCharAliasFace * pCol ;
    TDateFace * pBegD, * pEndD ;
    TLongFace * pBegN ;
    char * colCiphers ;
    bool loadColCiphersOK ;
    TCheckBox * pWantAllRes ;
    TCheckBox * pWantResultsCheckup ;  // 21.sep.2000
    TCheckBox * pJan2005Format ;  // 26.jan.2005
#if INSTANCE
#  if APPEAL
#  else  // of APPEAL
    //char * compositCiphers ;  // 04.jan.2005
    // TComposition thisComposit ;  // 04.jan.2005
    // int allCompositionsIndex ;  // 04.jan.2005
    // bool loadCompositCiphOK ;  // 04.jan.2005
    TLongAliasFace *pComposit;	// 2010:273 LPR: +related
#  endif  // of APPEAL
#else  // of INSTANCE
    // char * compositCiphers ;  // 04.jan.2005 - begin
    // TComposition thisComposit ;
    // int allCompositionsIndex ;
    // bool loadCompositCiphOK ;
    TLongAliasFace *pComposit;	// 2010:273 LPR: +related
#endif  // of INSTANCE
# if FOLLOW_LAWSUIT_TYPES
    TType thisType ;
# else
    TCollege thisCollege ;
# endif
# if INSTANCE
# else  // of INSTANCE
    TGroupBox * pSortGB ;
    TRadioButton * pNormalSort ;
    TRadioButton * pWantSortByDateReturned ;
    bool sortByDRetOldVal ;
# endif  // of INSTANCE
    //bool loadCompositCiphers () ;   // 04.jan.2005
    bool loadColCiphers () ;
  public:
    TPrintClosedSessBkDialog (TWindow * parent,
                              collBegDEndDBegN * colBegDEndDBegNPtr,
                              int resId =
# if INSTANCE
#    if APPEAL
						   IDD_PRINT_CLOSED_SESS_BOOK_APPEAL
#    else  // of APPEAL
						   IDD_PRINT_CLOSED_SESS_BOOK_AREAL
#    endif  // of APPEAL  // 10.apr.2000
# else  // of INSTANCE
                                             IDD_PRINT_CLOSED_SESS_BOOK
# endif  // of INSTANCE
                              ) ;
    virtual ~TPrintClosedSessBkDialog();

//{{TPrintClosedSessBkDialogVIRTUAL_BEGIN}}
  public:
    virtual void SetupWindow();
//{{TPrintClosedSessBkDialogVIRTUAL_END}}

//{{TPrintClosedSessBkDialogRSP_TBL_BEGIN}}
  protected:
    void OKBNClicked();
#if 0  // 30.nov.2004
    void turnWantSort () ;
#endif  // of 0
//{{TPrintClosedSessBkDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TPrintClosedSessBkDialog);
};    //{{TPrintClosedSessBkDialog}}
