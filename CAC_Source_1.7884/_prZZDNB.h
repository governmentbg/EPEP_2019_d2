# if ! INSTANCE
class TPrintBookZZDNDialog : public TGroupDialog
 {
  public :
    TPrintBookZZDNDialog (TWindow * parent,
                          ZZDN_Dlg_data * aZZDN_Dlg_dataPtr,
                          int resId = IDD_PRINT_BOOK_ZZDN) ;
    virtual void SetupWindow () ;

  protected :
    ZZDN_Dlg_data * pZZDN_Dlg_data ;

    TTypeAliasFace * pClg ;

    RadioCluster rb_SelectCrit ;

    TLongFace * pFromNo ;
    TLongFace * pToNo ;
    TLongFace * pYear ;

    TDateFace * pBegD ;
    TDateFace * pEndD ;

    TLongFace * pBN ;

  protected:
    void HandleByNoOrDateGroupBoxMsg(uint);
    void OKBNClicked () ;
    bool isValid () ;
    void HandleGroupBoxMsg (uint) ;

    DECLARE_RESPONSE_TABLE (TPrintBookZZDNDialog) ;
 } ;    //{{TPrintBookZZDNDialog}}
# endif
