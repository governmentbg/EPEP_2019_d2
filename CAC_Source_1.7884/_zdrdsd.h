class TZdravkovaDistributionDialog : public T2ChkBoxesDialog
{
  public:
    TZdravkovaDistributionDialog(TWindow* parent, const char* const title,
      const char * const nameGrp,           
      const char* const name1, const char* const name2,
      const char* const dateLabel, bool& firstChecked, bool& secondChecked,
      CDate& lDate_, bool& wantSubjRanges_,
	int resId = IDD_ZDRAVKOVA_DISTRIBUTION);
    virtual void SetupWindow();
  protected:  // Methods
    void OKBNClicked();
  protected:  // Data
    TDateFace *pLastDate;
    CDate& lDate;
    TStatic* pLDateStatic;
    const char* const dLabelText;
    TCheckBox* pUseSubjRanges;
    bool& wantSubjRanges;
DECLARE_RESPONSE_TABLE(TZdravkovaDistributionDialog);
};  // TZdravkovaDistributionDialog
