//{{TDialog = TEditConfigDataDialog}}
class TEditConfigDataDialog : public TEditDialog
{
public:
	TEditConfigDataDialog(TWindow* parent, TConfigData *configData, TGSArray<TSetting> *settings, bool edit,
		int resId = IDD_EDIT_CONFIG_DATA);

protected:
	long previous;
	TSettingAliasFace *setting;
	TStringFace *value;
	TStatic *text1;
	TStatic *text2;
	TStatic *text3;

	DEFINE_GROUP_TYPE(TConfigData, ConfigData)
	bool IsValid();

//{{TEditConfigDataDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEditConfigDataDialogVIRTUAL_END}}

//{{TEditConfigDataDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
	void NameCBNSelChange();
//{{TEditConfigDataDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TEditConfigDataDialog);
};    //{{TEditConfigDataDialog}}