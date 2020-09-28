//{{TDialog = TInformedListDialog}}
class TInformedListDialog : public TGroupListDialog
{
public:
	TInformedListDialog(TWindow* parent, int resId = IDD_INFORMED_LIST);

protected:
	TSenderAliasFace *informed;
	TInformedListWindow *informeds;

//{{TInformedListDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TInformedListDialogVIRTUAL_END}}

//{{TInformedListDialogRSP_TBL_BEGIN}}
	void InformedCBNSelChange();
	void ImportBNClicked();
	void PrintBNClicked();
//{{TInformedListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TInformedListDialog);
};    //{{TInformedListDialog}}
