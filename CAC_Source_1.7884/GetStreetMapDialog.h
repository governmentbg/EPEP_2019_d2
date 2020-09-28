//{{TDialog = TGetStreetMapDialog}}
class TGetStreetMapDialog : public TQuickDialog
{
public:
	TGetStreetMapDialog(TWindow* parent, TGArray<TStreetMap> *tSelect, const char *exclude);

protected:
	TGArray<TStreetMap> *select;
	TUserAliasFace *user;
	TStreetMapListBox *streetMap;

//{{TGetStreetMapDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TGetStreetMapDialogVIRTUAL_END}}

//{{TGetStreetMapDialogRSP_TBL_BEGIN}}
	void OKBNEnable(TCommandEnabler &tce);
	void OKBNClicked();
	void UserCBNSelChange();
	void SelectAllBNEnable(TCommandEnabler &tce);
	void SelectAllBNClicked();
//{{TGetStreetMapDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TGetStreetMapDialog);
};    //{{TGetStreetMapDialog}}
