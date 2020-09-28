//{{TDialog = TRequestSideListDialog}}
class TRequestSideListDialog : public TOriginSideListDialog
{
public:
	TRequestSideListDialog(TWindow* parent, TRequest *request, long sideMode);

protected:
	TButton *load;

	DEFINE_SIDES_TYPE(TRequest, Request)

	virtual void Accept();
	virtual void Reject();

//{{TRequestSideListDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TRequestSideListDialogVIRTUAL_END}}

//{{TRequestSideListDialogRSP_TBL_BEGIN}}
	void LoadBNClicked();
	void AttachBNClicked();
//{{TRequestSideListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TRequestSideListDialog);
};    //{{TRequestSideListDialog}}
