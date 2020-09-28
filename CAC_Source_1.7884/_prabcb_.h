class TPrintAbcBookDialog : public TScaleDialog //200806M public TPrintDescBookDialog
 {
  protected :
    begNLEndNLYear * pBegNLEndNLYear ;
//    TLongFace * pBD, * pED, * pY ;

    // 200806M ->
    char * pSelCollege ;

    TTypeAliasFace * pType ;

    TLongFace * pBN, * pEN, * pY ;
    // 200806M <-

    TCheckBox * pJan2005Format ;  // 08.feb.2005

    TCitizenNameFace * pBL, * pEL ;

    TCheckBox * pWantCheckup ;           // Справка по букви

    TRadioButton * pWantBrokenPrint ;    // по букви
    TRadioButton * pWantPrintByPages ;   // по страници
    TRadioButton * pWantUnbrokenPrint ;  // без паузи 30.sep.2003
  public :
    TPrintAbcBookDialog (TWindow * parent,
				 begNLEndNLYear * aBegNLEndNLYearPtr,
				 char * selCollegePtr) ;
    virtual void SetupWindow () ;
  protected:
    void OKBNClicked () ;

  DECLARE_RESPONSE_TABLE (TPrintAbcBookDialog) ;
 } ;    //{{TPrintAbcBookDialog}}
