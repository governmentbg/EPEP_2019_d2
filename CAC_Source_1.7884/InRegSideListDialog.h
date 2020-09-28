//{{TDialog = TInRegSideListDialog}}
class TInRegSideListDialog : public TSideWindListDialog
{
public:
	TInRegSideListDialog(TWindow* parent, TInReg *inReg, TLawsuit *lawsuit, long sideMode);

protected:
	TButton *editor;
	TButton *appealer;
	sign eportalSendable;

	DEFINE_SIDES_TYPE(TInReg, InReg)
	virtual void Header2(mstr &m);

	virtual bool Primary(TSideWind *sideWind) const;  // appealer
	virtual void Accept();

//{{TInRegSideListDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TInRegSideListDialogVIRTUAL_END}}

//{{TInRegSideListDialogRSP_TBL_BEGIN}}
	void AppealerBNClicked();
	void MoneyBNClicked();
	void AppealerBNEnable(TCommandEnabler &tce);
	void AttachBNClicked();
//{{TInRegSideListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TInRegSideListDialog);
};    //{{TInRegSideListDialog}}
