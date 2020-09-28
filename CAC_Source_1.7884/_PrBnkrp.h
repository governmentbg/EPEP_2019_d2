class TPrintBankruptcyWideDialog : public TScaleDialog
 {
  protected :
    TCheckBox * pWantNoncompleted ;
    TCheckBox * pWantCompleted ;
    TDateFaceOldVal * pBegCompletD ;
    TDateFaceOldVal * pEndCompletD ;
    TDateFaceOldVal * pBegLawD ;
    TDateFaceOldVal * pEndLawD ;
    TCheckBox * pWantExtraTitle ;
    TStringFaceOldVal * pYourOutNo ;
    BankruptcyWideDlgData & rDialogData ;
    uint oldExtraTilleStatus;  // 10.jun.2003

    virtual void ExtractData () ;  // 10.jun.2003 virtual
    virtual bool ValidData () ;  // 10.jun.2003 virtual
    void OKBNClicked () ;
    void CompletedBNClicked () ;
    void NoncompletedBNClicked () ;
    void ExtraTitleBNClicked () ;

  public :
    TPrintBankruptcyWideDialog (TWindow * parent,
                                BankruptcyWideDlgData & rDlgData,
                                int resId = IDD_BANKRUPTCY_WIDE) ;
    virtual void SetupWindow () ;
DECLARE_RESPONSE_TABLE (TPrintBankruptcyWideDialog) ;
 } ; // TPrintBankruptcyWideDialog

class TPrintBankruptcyWideWebDialog : public TPrintBankruptcyWideDialog
{
  public:  // Construction / destruction
    TPrintBankruptcyWideWebDialog(TWindow* parent,
	BankruptcyWideDlgData& rDlgData, int resId = IDD_BANKRUPTCY_WIDE_WEB);
  public:  // Methods
    virtual void SetupWindow();
  protected:  // Methods
    void ProcessCheckboxes();
    void EnableCompletedDates(bool enable);
    void EnableNoncompletedDates(bool enable);
    void EnableCompletedPath(bool enable);
    void EnableNoncompletedPath(bool enable);
    virtual void ExtractData();
    virtual bool ValidData();
    void TransferPaths(TTransferDirection direction);
  protected:  // Data
    char oldCompletedPath[SIZE_OF_PATH];
    char oldNoncompletedPath[SIZE_OF_PATH];
  private:
    // 2009:064 LPR: eliminating GetDlgItemPtr
    TCheckBox *bw_noncompleted_web;
    TCheckBox *bw_completed_web;
    TEdit *bw_noncompleted_web_path;
    TEdit *bw_completed_web_path;
DECLARE_RESPONSE_TABLE (TPrintBankruptcyWideWebDialog) ;
};  // class TPrintBankruptcyWideWebDialog
