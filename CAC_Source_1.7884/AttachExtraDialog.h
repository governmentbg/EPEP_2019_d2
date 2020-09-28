//{{TDialog = TAttachExtraDialog}}
class TAttachExtraDialog : public TTroubleDialog
{
public:
	TAttachExtraDialog(TWindow* parent, TElectric *electric);

protected:
	TStringFace *descript;

	bool Move(int type);

	DEFINE_GROUP_TYPE(TElectric, Electric)
	virtual bool IsValid();

//{{TAttachExtraDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TAttachExtraDialogVIRTUAL_END}}

//{{TAttachExtraDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
//{{TAttachExtraDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TAttachExtraDialog);
};    //{{TAttachExtraDialog}}
