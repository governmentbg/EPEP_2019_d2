//{{TDialog = TSurrouendocListDialog}}
class TSurrouendocListDialog : public TGroupListDialog
{
public:
	TSurrouendocListDialog(TWindow* parent, TSession *tSession, TGArray<TSurroundment> *tSurrouendocs, TLawsuit *tLawsuit,
		bool tAppealing);

protected:
	TSession *session;
	TSurrouendocListWindow *surrouendocs;
	TButton *edit;
	// 2016:175
	TLawsuit *lawsuit;
	bool appealing;
	TButton *erase;
	TButton *appeal;

//{{TSurrouendocListDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TSurrouendocListDialogVIRTUAL_END}}

//{{TSurrouendocListDialogRSP_TBL_BEGIN}}
	void DeleteBNEnable(TCommandEnabler &tce);
	void DeleteBNClicked();
	void AppealBNEnable(TCommandEnabler &tce);
	void AppealBNClicked();
	bool ListLVNItemChanging(TLwNotify &lwn);
//{{TSurrouendocListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TSurrouendocListDialog);
};    //{{TSurrouendocListDialog}}
