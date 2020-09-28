//{{TDialog = TExecListListDialog}}
class TExecListListDialog : public TScaleDialog
{
public:
	TExecListListDialog(TWindow* parent, int resId = IDD_EXEC_LIST_LIST);

protected:
	TCharAliasFace *type;
	TLongFace *no;
	TLongFace *year;
	TDateFace *execListDate;
	TExecListListWindow *execLists;
	TButton *search;
	TButton *enter;
	TButton *local;
	int lastReceiverTypes;

	void SetSearch();

	virtual bool IsValid();

//{{TExecListListDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TExecListListDialogVIRTUAL_END}}

//{{TExecListListDialogRSP_TBL_BEGIN}}
	void SearchBNClicked();
	void EnterBNEnable(TCommandEnabler &tce);
	void EnterBNClicked();
	void LocalBNEnable(TCommandEnabler &tce);
	void LocalBNClicked();
	long EvFocusFace(TParam1 wParam, TParam2);
	bool ExecListsLVNItemChanging(TLwNotify &lwn);
//{{TExecListListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TExecListListDialog);
};    //{{TExecListListDialog}}
