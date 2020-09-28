//{{TDialog = TEditLawsuitDialog}}
class TEditLawsuitDialog : public TTroubleDialog
{
public:
	TEditLawsuitDialog(TWindow* parent, TLawsuit *lawsuit, TGArray<TConnect> *tConnects, const char *predefined);

protected:
	TCharAliasFace *kind;
	TSubjectAliasFace *subject;
	TDateFace *date;
	TDateFace *previousDate;
	TDateFace *receivedDate;
	TCheckFace *quick;
	TCheckFace *converted;
	TConnectListWindow *connects;
	long lawQuick;
	TCharAliasFace *college;
	TCheckFace *newGPK;
	TCheckFace *limited;
	TCheckFace *doublex;
	TEisNmrFace *eispp_nmr;
	TStatic *eispp_nmr_label;	// 2014:314 TRQ, hide only
	bool has_eispp_nmr;
	// 2010:266 LPR: for checks
	TUserAliasFace *judge;
	TUserAliasFace *reJudge;
	TCheckBox *children;
	TCheckBox *corrupt;
	TCheckFace *pt329a3; 

	DEFINE_GROUP_TYPE(TLawsuit, Lawsuit)
	virtual bool IsValid();
	virtual bool IsClean();

//{{TEditLawsuitDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEditLawsuitDialogVIRTUAL_END}}

//{{TEditLawsuitDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
	void QuickBNClicked();
	void AddBNClicked();
	void EditBNClicked();
	void EditBNEnable(TCommandEnabler &tce);
	void DeleteBNClicked();
	void DeleteBNEnable(TCommandEnabler &tce);
	bool ListLVNItemChanging(TLwNotify &lwn);
	void ImportanceBNClicked();
	void SubjectCBNSelChange();
//{{TEditLawsuitDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TEditLawsuitDialog);
};    //{{TEditLawsuitDialog}}
