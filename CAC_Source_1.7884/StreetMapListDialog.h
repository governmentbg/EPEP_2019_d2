//{{TDialog = TStreetMapListDialog}}
class TStreetMapListDialog : public TGroupListDialog
{
public:
	TStreetMapListDialog(TWindow* parent, TUser *tUser, int resId = IDD_STREET_MAP_LIST);

protected:
	TUser *user;

	TStringFace *name;
	TUCLPAliasFace *uclp;
	TStreetMapListWindow *streetMaps;

//{{TStreetMapListDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TStreetMapListDialogVIRTUAL_END}}

//{{TStreetMapListDialogRSP_TBL_BEGIN}}
	void UCLPCBNSelChange();
	void PrintBNClicked();
	void CopyBNClicked();
//{{TStreetMapListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TStreetMapListDialog);
};    //{{TStreetMapListDialog}}
