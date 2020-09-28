//{{TDialog = TSelectFirmsDialog}}
class TSelectFirmsDialog : public TGroupDialog
{
public:
	TSelectFirmsDialog(TWindow* parent, firm_action tAction, int resId = IDD_SELECT_FIRMS);

protected:
	TCharAliasFace *uxnType;
	TFirmNameFace *name;
	TBulStatFace *bulstat;
	TSelectUCNListBox *firms;

	TButton *ok;
	TButton *search;
	TButton *research;

	firm_action action;
	TFirm firm;

	bool Search(bool extra);
	void UpdateButtons(bool okay);

//{{TSelectFirmsDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TSelectFirmsDialogVIRTUAL_END}}

//{{TSelectFirmsDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
	void FirmsLBNSelChange();
	void SearchBNClicked();
	void AddressBNClicked();
	void AddressBNEnable(TCommandEnabler &tce);
	void PrintBNClicked();
	void ResearchBNClicked();
	void NameENUpdate();
//{{TSelectFirmsDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TSelectFirmsDialog);
};    //{{TSelectFirmsDialog}}
