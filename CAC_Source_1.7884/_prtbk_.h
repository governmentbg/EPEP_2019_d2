class TPrintTimeBookDialog : public TGroupDialog
 {
  protected :
    collCompBegDEndDBegN * pCollCompBegDEndDBegN ;
    TTypeAliasFace * pClg ;
    TLongAliasFace *pComposit;
    TDateFace * pBegD ;
    TDateFace * pEndD ;
    TLongFace * pBN ;
    void turnClSessCheckBox () ;
# if INSTANCE
    TTimeFace * pNoon ;
# endif  // of INSTANCE
    TDateFace * pBkHeapingBegD ;
    TCheckBox * pWantBook ;
    TCheckBox * pWantSummary ;
    TCheckBox * pWantJudgesSummary ;
    TCheckBox * pWantResultsCheckup ;
    TCheckBox * pJan2005Format ;  // 08.feb.2005
    TCheckBox * pSplitIn2 ;  // 08.feb.2005
    void adjustHeapingBegD () ;
    bool isValid () ;
  public :
    TPrintTimeBookDialog (TWindow * parent,
				  collCompBegDEndDBegN * aCollCompBegDEndDBegNPtr,
				  int resId = IDD_PRINT_TIME_BOOK) ;
    virtual void SetupWindow () ;
  protected:
    void OKBNClicked () ;
    void lawTypeCBNSelChange () ;
    void begDFocusKilled () ;
    void bkHeapingBegDFocusKilled () ;
  private:
#if INSTANCE
    TRadioButton *tb_1st_instance_laws;
#   if APPEAL
#   else  // of APPEAL
    TRadioButton *tb_2nd_instance_laws;
    TRadioButton *tb_all_2nd_instance_laws;
#endif
    TRadioButton *tb_3rd_instance_laws_pac;
    TRadioButton *tb_print_tb_whole_day;
    TRadioButton *tb_print_tb_morning;
    TRadioButton *tb_print_tb_afternoon;
#endif  // INSTANCE
#if INSTANCE
#   if APPEAL      // appeal
    TRadioButton *tb_rba1;
    TRadioButton *tb_rba2;
#   else           // areal
    TRadioButton *tb_rb_1;
    TRadioButton *tb_rb_2;
#   endif
#else
    TRadioButton *tb_rb_order_1;
    TRadioButton *tb_rb_order_2;
#endif

  DECLARE_RESPONSE_TABLE (TPrintTimeBookDialog) ;
 } ;    //{{TPrintTimeBookDialog}}
