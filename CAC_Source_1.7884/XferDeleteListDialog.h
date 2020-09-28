//{{TDialog = TXferDeleteListDialog}}
class TXferDeleteListDialog : public TGroupListDialog
{
public:
	TXferDeleteListDialog(TWindow* parent, const char *xData);
	virtual ~TXferDeleteListDialog();

protected:
	TLongAliasFace *court;
	TXferDeleteListWindow *documents;

//{{TXferDeleteListDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TXferDeleteListDialogVIRTUAL_END}}

//{{TXferDeleteListDialogRSP_TBL_BEGIN}}
	void CourtCBNSelChange();
	void PrintBNClicked();
//{{TXferDeleteListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TXferDeleteListDialog);
};    //{{TXferDeleteListDialog}}
