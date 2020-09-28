//{{TDialog = TXferWebListDialog}}
class TXferWebListDialog : public TGroupListDialog
{
public:
	TXferWebListDialog(TWindow* parent, TXferWebGroup *group);

protected:
	TDateFace *minXferDate;
	TDateFace *maxXferDate;
	TXferWebListWindow *xferWebs;
	TButton *search, *query;	

	void SetSearch();

//{{TXferWebListDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TXferWebListDialogVIRTUAL_END}}

//{{TXferWebListDialogRSP_TBL_BEGIN}}
	void SearchBNClicked();
	void QueryBNClicked();
	void ViewBNClicked();
	void PrintBNClicked();
	void DeleteBNEnable(TCommandEnabler &tce);
	long EvFocusFace(TParam1 wParam, TParam2);
//{{TXferWebListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TXferWebListDialog);
};  //{{TXferWebListDialog}}
