//{{TDialog = TAdditionalTextDialog}}
class TAdditionalTextDialog : public TGroupDialog
{
public:
	TAdditionalTextDialog(TWindow* parent, TPrintSubpoena *printsub, TRCDKeyContainer *tContainer, int resId = IDD_ADDITIONAL_TEXT);

protected:
	TPrintSubpoena *printsub;
	TRCDKeyContainer *container;

	TTranscriptComboBox *transcript;
	TStringFace *additionalText;
	TStringFace *name;
	TComboBox *appeal;
	TComboBox *interval;
	TComboBox *before;
	TComboBox *from;
	int noappeal;
	int nocheck;
	TStatic *topLabel;
	TAliasFace *accomply;

	DEFINE_GROUP_TYPE(TPrintSubpoena, PrintSub)
	bool AppealActive();

//{{TAdditionalTextDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TAdditionalTextDialogVIRTUAL_END}}

//{{TAdditionalTextDialogRSP_TBL_BEGIN}}
	void AppealCBNSelChange();
	void OKBNClicked();
	void EditBNClicked();
//{{TAdditionalTextDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TAdditionalTextDialog);
};    //{{TAdditionalTextDialog}}
