//{{TDialog = TEditEISenderDialog}}
class TEditEISenderDialog : public TEditSenderDialog
{
public:
	TEditEISenderDialog(TWindow* parent, TSender *sender, bool edit, bool full, int resId = IDD_EDIT_EISENDER);

protected:
	TLongFace *eispp;
	TStringFace *depart;

	virtual bool IsValid();

//{{TEditEISenderDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEditEISenderDialogVIRTUAL_END}}

//{{TEditEISenderDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
	void EisppKillFocus();
//{{TEditEISenderDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TEditEISenderDialog);
};    //{{TEditEISenderDialog}}
