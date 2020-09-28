//{{TDialog = TBanConSistListDialog}}
class TBanConSistListDialog : public TQuickDialog
{
public:
	TBanConSistListDialog(TWindow* parent, TText *tField, int resId);

protected:
	TText *field;
	TBanConSistListWindow *banConSists;

	void Action(int command);

//{{TBanConSistListDialogRSP_TBL_BEGIN}}
	void AddBNClicked();
	void AddBNEnable(TCommandEnabler &tce);
	void EditBNClicked();
	void EditBNEnable(TCommandEnabler &tce);
	void DeleteBNClicked();
	void DeleteBNEnable(TCommandEnabler &tce);
	void PrintBNClicked();
	void OKBNClicked();
	void EnterBNClicked();
	void ExecBNClicked();
	bool BanConSistLVNItemChanging(TLwNotify &lwn);
//{{TBanConSistListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TBanConSistListDialog);
};    //{{TBanConSistListDialog}}
