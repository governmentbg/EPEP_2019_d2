//{{TDialog = TNewLawsuitDialog}}
class TNewLawsuitDialog : public TTroubleDialog
{
public:
	TNewLawsuitDialog(TWindow* parent, TLawsuit *lawsuit, TGArray<TConnect> *tConnects, const char *tLawKinds,
		const char *sourceKinds, const char *predefined, TRequest *tRequest);

protected:
	constant lawKinds;
	TRequest *request;	// 2012:076 URQ: for lawable check only
	bool has_eispp_nmr;	// 2012:307 FIX: may come from request 

	TCharAliasFace *kind;
	TCharAliasFace *sourceKind;
	TSubjectAliasFace *subject;
	TLongAliasFace *composition;
	TCharAliasFace *college;
	TPersonAliasFace *judge;
	TDateFace *date;
	TDateFace *previousDate;
	TDateFace *receivedDate;
	TQuickFace *quick;
	TConnectListWindow *connects;
	TCheckFace *newGPK;
	bool newGPKChanged;
	TCheckFace *limited;
	TCheckFace *doublex;
	TEisNmrFace *eispp_nmr;
	TStatic *eispp_nmr_label;	// 2014:314 TRQ: hide only
	bool assigned;			// 2014:349 LRQ: distrib
	TLongFace *no;			// 2015:278
	TCheckFace *children;		// 2016:105
	TCheckFace *corrupt;		// 2017:219 +related
	TCheckFace *pt329a3;		// 2018-05-28 +related

	void TypeChanged();

	DEFINE_GROUP_TYPE(TLawsuit, Lawsuit)
	virtual bool IsValid();

//{{TNewLawsuitDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TNewLawsuitDialogVIRTUAL_END}}

//{{TNewLawsuitDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
	void KindCBNSelChange();
	void SourceKindCBNSelChange();
	void CompositionCBNSelChange();
	void QuickBNClicked();
	void AddBNClicked();
	void EditBNClicked();
	void EditBNEnable(TCommandEnabler &tce);
	void DeleteBNClicked();
	void DeleteBNEnable(TCommandEnabler &tce);
	bool ListLVNItemChanging(TLwNotify &lwn);
	void ImportanceBNClicked();
	void SubjectCBNSelChange();
	void CollegeCBNSelChange();
	void JudgeCBNSelChange();
	void DateENUpdate();
	void NewGPKBNClicked();
//{{TNewLawsuitDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TNewLawsuitDialog);
};    //{{TNewLawsuitDialog}}
