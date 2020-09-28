//{{TDialog = TSurroundSideListDialog}}
class TSurroundSideListDialog : public TSideWindListDialog
{
public:
	TSurroundSideListDialog(TWindow* parent, TSurround *surround, TRCDKeyContainer *tSurrounded, long sideMode);
	virtual ~TSurroundSideListDialog();

protected:
	TRCDKeyContainer *surrounded;
	TButton *appealer;
	TLawsuit *lawsuit;

	DEFINE_SIDES_TYPE(TSurround, Surround)
	virtual void Header2(mstr &m);

	virtual bool Primary(TSideWind *sideWind) const;  // appealer
	virtual void Accept();

//{{TSurroundSideListDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TSurroundSideListDialogVIRTUAL_END}}

//{{TSurroundSideListDialogRSP_TBL_BEGIN}}
	void AppealerBNClicked();
	void MoneyBNClicked();
	void AttachBNClicked();
//{{TSurroundSideListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TSurroundSideListDialog);
};    //{{TSurroundSideListDialog}}
