#if COURT_TYPE == COURT_AREAL
class TPrint634TLBookDialog : public TScaleDialog
 {
  protected :
    TCheckFace *html;
    TRCDKey_Array lawKeys ;
    TDateFace * pBegDate ;
    TDateFace * pEndDate ;
    TTypeAliasFace * pLawType ;
    TLongFace * pLawNo ;
    TLongFace * pLawYear ;
    TLongFace * pBegSerialNo ;
    void PrintLaws (const bool truncChain) ;
    void OKBNClicked () ;
    void ChainBNClicked () ;
    void SearchBNClicked () ;
    bool IsFilledOK () const ;
    void ExtractDialogData (Trade634DlgData & dd) const ;
  public:
    TPrint634TLBookDialog (TWindow * parent) ;
    virtual void SetupWindow () ;
  DECLARE_RESPONSE_TABLE (TPrint634TLBookDialog) ;
 } ;  // class TPrint634TLBookDialog
#endif  // AREAL
