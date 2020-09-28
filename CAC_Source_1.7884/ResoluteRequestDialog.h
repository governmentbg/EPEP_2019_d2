//{{TDialog = TResoluteRequestDialog}}
class TResoluteRequestDialog : public TTroubleDialog
{
public:
	TResoluteRequestDialog(TWindow* parent, TRequestResolution *resolution, TRequest *tRequest, bool edit);

protected:
	TDateFace *date;
	TPersonAliasFace *judge;	// 2003:183 LRQ: must not be empty
	// 2005:089 LPR: for kind display
	TRequest *request;
	TCharAliasFace *requestKind;
	TLongFace *eventNo;		// 2005:089 LRQ

	DEFINE_GROUP_TYPE(TRequestResolution, Resolution)
	virtual bool IsValid();

//{{TResoluteRequestDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TResoluteRequestDialogVIRTUAL_END}}

//{{TResoluteRequestDialogRSP_TBL_BEGIN}}
	void RightBNClicked();
//{{TResoluteRequestDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TResoluteRequestDialog);
};    //{{TResoluteRequestDialog}}
