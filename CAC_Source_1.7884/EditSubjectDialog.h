//{{TDialog = TEditSubjectDialog}}
class TEditSubjectDialog : public TGroupDialog
{
public:
	TEditSubjectDialog(TWindow* parent, TSubject *tSubject, bool tEdit, TGArray<TArchiveIndex> *tIndexes);

protected:
	TSubject backup;
	TStringFace *name;
	TCharListNameFace *results;
	TCharListFace *attrib;
	TCharListNameFace *closeds;
	// 2005:356 LPR: generic add-or-edit
	bool edit;
	TStringFace *subject;
	TLongFace *judgeRep;
	TStatic *quickRepLabel;
	TLongFace *quickRep;
	TPushButton *eispp;
	TCheckFace *apcTmpl;
	TGArray<TArchiveIndex> *indexes;

	DEFINE_GROUP_TYPE(TSubject, Subject)
	virtual bool IsValid();

//{{TEditSubjectDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEditSubjectDialogVIRTUAL_END}}

//{{TEditSubjectDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
	void IndexesBNClicked();
	void RealitiesBNClicked();
	void EisppBNClicked();
	void EisppBNEnable(TCommandEnabler &tce);
//{{TEditSubjectDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TEditSubjectDialog);
};    //{{TEditSubjectDialog}}
