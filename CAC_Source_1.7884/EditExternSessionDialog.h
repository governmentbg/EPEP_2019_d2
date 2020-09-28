//{{TDialog = TEditExternSessionDialog}}
class TEditExternSessionDialog : public TSessionTimesDialog
{
public:
	TEditExternSessionDialog(TWindow* parent, TExternSession *session, bool edit, const char *kinds,
		const char *getFields, const char *results, int resId);

protected:
	bool unlockKeys;
	TCharAliasFace *type;
	TLongFace *no;
	TYearFace *year;
	TCharAliasFace *kind;
	TDateFace *date;

	virtual bool SingleDate() const;

	DEFINE_GROUP_TYPE(TExternSession, Session)
	virtual bool IsValid();

//{{TEditExternSessionDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEditExternSessionDialogVIRTUAL_END}}

//{{TEditExternSessionRSP_TBL_BEGIN}}
	void KindCBNSelChange();
	void TypeCBNSelChange();
	void OKBNClicked();
	void DateENUpdate();
	void ResultCBNSelChange();
//{{TEditExternSessionRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TEditExternSessionDialog);
};    //{{TEditExternSessionDialog}}
