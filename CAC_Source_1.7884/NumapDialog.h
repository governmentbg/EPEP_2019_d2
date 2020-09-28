//{{TDialog = TNumapDialog}}
class TNumapDialog : public TGroupDialog
{
public:
	TNumapDialog(TWindow* parent, TStreetMap *streetMap, int resId = IDD_NUMAP);

protected:
	TUCLPAliasFace *uclp;
	TEdit *street;
	TNumapListFace *numbers;

	DEFINE_GROUP_TYPE(TStreetMap, StreetMap)

//{{TNumapDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TNumapDialogVIRTUAL_END}}

//{{TNumapDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
	void AddBNClicked();
	void DeleteBNClicked();
//{{TNumapDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TNumapDialog);
};    //{{TNumapDialog}}
