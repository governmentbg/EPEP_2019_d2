//{{TDialog = TNewRequestDialog}}
class TNewRequestDialog : public TEditIncomingDialog
{
public:
	TNewRequestDialog(TWindow* parent, TRequest *request, const char *kinds, char type);

protected:
	TSubjectAliasFace *subject;	// 2003:052
	TQuickFace *quick;		// 2003:261 New quick; 2006:072 newest
	TConnectListWindow *connects;
	// 2007:219 LRQ: display only; 2017:087 LPR: or choose
	TCharAliasFace *lawKind;
	mstr lawKinds;			// 2017:087
	const char *types;		// 2017:084; 2017:087 type -> types
	TButton *load;
	TEisNmrFace *eispp_nmr;		// 2012:289
	TStatic *eispp_nmr_label;	// 2014:314 TRQ, hide only

	DEFINE_GROUP_TYPE(TRequest, Request)
	virtual bool IsValid();

//{{TNewRequestDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TNewRequestDialogVIRTUAL_END}}

//{{TNewRequestDialogRSP_TBL_BEGIN}}
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
//{{TNewRequestDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TNewRequestDialog);
};    //{{TNewRequestDialog}}
