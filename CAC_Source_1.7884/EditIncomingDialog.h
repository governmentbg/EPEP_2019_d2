//{{TDialog = TEditIncomingDialog}}
class TEditIncomingDialog : public TTroubleDialog
{
public:
	TEditIncomingDialog(TWindow* parent, TRCDKeyContainer *incoming, const char *kinds, const char *colleges,
		bool edit, int resId, long collegeFlags = FLAG_NULL);

protected:
	TCharAliasFace *kind;
	TCharAliasFace *college;
	TLongFace *no;
	TDateFace *date;
	TTimeFace *time;
	TStringFace *direction;		// 2010:048

	bool TEditIncomingDialog::CheckDirection();

	DEFINE_GROUP_TYPE(TRCDKeyContainer, Incoming)
	virtual bool IsValid();

//{{TEditIncomingDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEditIncomingDialogVIRTUAL_END}}

//{{TEditIncomingDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
	void DateENUpdate();
//{{TEditIncomingDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TEditIncomingDialog);
};    //{{TEditIncomingDialog}}
