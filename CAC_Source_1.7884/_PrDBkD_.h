class TPrintDescBookDialog : public TGroupDialog  // 03.nov.2000
 {
  public :
    TPrintDescBookDialog (TWindow * parent,
                          begNEndNYear * aBegNEndNYearPtr,
                          char * selCollegePtr,
                          int resId = IDD_PRINT_DESC_BOOK) ;
//{{TPrintDescBookDialogVIRTUAL_BEGIN}}
    virtual void SetupWindow () ;
//{{TPrintDescBookDialogVIRTUAL_END}}
//{{TPrintDescBookDialogRSP_TBL_BEGIN}}
  protected :
    begNEndNYear * pBegNEndNYear ;

    char * pSelCollege ;

    TTypeAliasFace * pType ;

    RadioCluster rb_SelectCrit ;

    TLongFace * pBN, * pEN, * pY ;

    TDateFace * pBegD ;
    TDateFace * pEndD ;

    TCheckBox * pJan2005Format ;  // 08.feb.2005
    TCheckBox * pSplitIn2 ;  // 08.feb.2005
  protected:
    void HandleGroupBoxMsg (uint) ;              // 20080605
    bool isValid () ;                            // 20080605
    void OKBNClicked () ;

//{{TPrintDescBookDialogRSP_TBL_END}}

  DECLARE_RESPONSE_TABLE (TPrintDescBookDialog) ;
 } ;    //{{TPrintDescBookDialog}}
