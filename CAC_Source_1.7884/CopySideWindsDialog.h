//{{TDialog = TCopySideWindsDialog}}
class TCopySideWindsDialog : public TGroupDialog
{
public:
	TCopySideWindsDialog(TWindow* parent, TRCDKeyContainer *source, TRCDKeyContainer *tTarget, const char *types);

protected:
	TRCDKeyContainer *target;

	TCharAliasFace *type;
	TCharAliasFace *kind;
	TDateFace *date;
	TIndexFace *index;

	TCharAliasFace *reType;
	TLongFace *reNo;
	TLongFace *reYear;
	TCharAliasFace *reKind;
	TDateFace *reDate;
	TIndexFace *reIndex;

	DEFINE_GROUP_TYPE(TRCDKeyContainer, Source)
	virtual bool IsValid();

//{{TCopySideWindsDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TCopySideWindsDialogVIRTUAL_END}}

//{{TCopySideWindsDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
	void TypeCBNSelChange();
	void KindCBNSelChange();
	void ReTypeCBNSelChange();
	void ReKindCBNSelChange();
//{{TCopySideWindsDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TCopySideWindsDialog);
};    //{{TCopySideWindsDialog}}
