//{{TDialog = TResolutionSideListDialog}}
class TResolutionSideListDialog : public TSideWindListDialog
{
public:
	TResolutionSideListDialog(TWindow* parent, TRequestResolution *resolution, TRequest *request, long sideMode);

protected:
	TButton *messages;
	TRequest *request;	// 2005:081 LRQ: for Header1() only

	DEFINE_SIDES_TYPE(TRequestResolution, Resolution)
	virtual void Header1(mstr &m);
	virtual void Header2(mstr &m);

//{{TResolutionSideListDialogRSP_TBL_BEGIN}}
	void MessagesBNClicked();
	void ListBNClicked();		// 2004:006 BRW: LRQ addition
	void RightBNClicked();		// 2004:229 LRQ
//{{TResolutionSideListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TResolutionSideListDialog);
};    //{{TResolutionSideListDialog}}
