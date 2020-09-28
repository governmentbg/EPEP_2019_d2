//{{TDialog = TGetDateDialog}}
class TGetDateDialog : public TGroupDialog
{
public:
	TGetDateDialog(TWindow* parent, TGroup *group, char *tTitle = NULL, bool mandatory = true, bool tFuture = false);

protected:
	char *title;
	TStatic *label;
	TDateFace *date;
	bool future;		// 2005:088 LRQ: required by hall load

	virtual bool IsValid();

//{{TGetDateDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TGetDateDialogVIRTUAL_END}}

//{{TGetDateDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
//{{TGetDateDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TGetDateDialog);
};    //{{TGetDateDialog}}
