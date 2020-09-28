//{{TDialog = TEditExecListDialog}}
class TEditExecListDialog : public TGroupDialog
{
public:
	TEditExecListDialog(TWindow* parent, TExecList *execList, TGArray<TLinkWind> *xLinks, bool tEdit,
		int resId = IDD_EDIT_EXEC_LIST);

protected:
	bool edit;

	TExecListLinkListWindow *links;
	TExecListSumListWindow *sums;
	TLongFace *execListNo;
	bool sumsEnabled;

	DEFINE_GROUP_TYPE(TExecList, ExecList)
	virtual bool IsValid();

//{{TEditExecListDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEditExecListDialogVIRTUAL_END}}

//{{TEditExecListDialogRSP_TBL_BEGIN}}
	void AddressBNEnable(TCommandEnabler &tce);
	void AddressBNClicked();
	void AddBNEnable(TCommandEnabler &tce);
	void AddBNClicked();
	void EditBNEnable(TCommandEnabler &tce);
	void EditBNClicked();
	void DeleteBNEnable(TCommandEnabler &tce);
	void DeleteBNClicked();
	void OKBNEnable(TCommandEnabler &tce);
	void OKBNClicked();
	bool LaidsLVNItemChanging(TLwNotify &lwn);
	bool SumsLVNItemChanging(TLwNotify &lwn);
//{{TEditExecListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TEditExecListDialog);
};    //{{TEditExecListDialog}}
