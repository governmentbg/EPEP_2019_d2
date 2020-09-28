//{{TDialog = TLawsuitSideListDialog}}
class TLawsuitSideListDialog : public TOriginSideListDialog
{
public:
	TLawsuitSideListDialog(TWindow* parent, TLawsuit *lawsuit, TRequest *tRequest, long sideMode);

protected:
	TRequest *request;
	TButton *extraData;

	DEFINE_SIDES_TYPE(TLawsuit, Lawsuit)

	virtual void Accept();
	virtual void Reject();

//{{TLawsuitSideListDialogRSP_TBL_BEGIN}}
	void ExtraDataBNEnable(TCommandEnabler &tce);
	void ExtraDataBNClicked();
	void ListBNClicked();
//{{TLawsuitSideListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TLawsuitSideListDialog);
};    //{{TLawsuitSideListDialog}}
