//{{TDialog = TEditRequestDialog}}
class TEditRequestDialog : public TEditIncomingDialog
{
public:
	TEditRequestDialog(TWindow* parent, TRequest *request, char tStatus, int resId = IDD_REGISTER_REQUEST);
	virtual ~TEditRequestDialog();

protected:
	char type;
	TSubjectAliasFace *subject;
	TQuickFace *quick;
	TConnectListWindow *connects;
	char status;			// 2007:214 disable subject/connects
	TButton *add;
	// 2007:219 LRQ: display only; 2017:087 LPR: or choose
	TCharAliasFace *lawKind;
	const char *types;
	mstr lawKinds;			// 2017:087
	// 2012:291
	bool has_eispp_nmr;
	TEisNmrFace *eispp_nmr;
	TStatic *eispp_nmr_label;	// 2014:314 TRQ, hide only
	TGArray<TConnect> *tConnects;	// LPR: FIX: memory leak

	DEFINE_GROUP_TYPE(TRequest, Request)
	virtual bool IsValid();
	virtual bool IsClean();

//{{TEditRequestDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEditRequestDialogVIRTUAL_END}}

//{{TEditRequestDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
	void KindCBNSelChange();
	void QuickBNClicked();
	void AddBNClicked();
	void EditBNClicked();
	void EditBNEnable(TCommandEnabler &tce);
	void DeleteBNClicked();
	void DeleteBNEnable(TCommandEnabler &tce);
	bool ListLVNItemChanging(TLwNotify &lwn);
	void LoadBNClicked();
	void LawKindCBNSelChange();
//{{TEditRequestDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TEditRequestDialog);
};    //{{TEditRequestDialog}}
