# if INSTANCE
#   if APPEAL
#   else  // APPEAL
class TPrintDescBookArealDialog : public TPrintDescBookDialog
 {
  protected :
//    bool want1stLawsOldCheck ;  // 21.nov.2001
//    bool want2ndLawsOldCheck ;  // 21.nov.2001 
    TCheckBox * pWant1stInstLaws ;
    TCheckBox * pWant2ndInstLaws ;
    TGroupBox * pPseudoLawsByInst ;
  public :
    TPrintDescBookArealDialog (TWindow * parent,
                          begNEndNYear * aBegNEndNYearPtr,
                          char * selCollegePtr,
                          int resId = IDD_PRINT_DESK_BOOK_AREAL) ;
    virtual void SetupWindow () ;
  protected:
    void OKBNClicked () ;
//    void lawTypeCBNSelChange () ;  // 21.nov.2001 - begin
//  protected :
//    void setLawsByInstance () ;
//    void turnLawsByInstance () ;  // 21.nov.2001 - end

  DECLARE_RESPONSE_TABLE (TPrintDescBookArealDialog) ;
 } ;    //{{TPrintDescBookArealDialog}}

#   endif  // APPEAL
# endif  // INSTANCE
