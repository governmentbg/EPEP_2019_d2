//{{TDialog = TInRegEventSideListDialog}}
class TInRegEventSideListDialog : public TSideWindListDialog
{
public:
	TInRegEventSideListDialog(TWindow* parent, TInRegEvent *inRegEvent, TInReg *inReg, long sideMode);
	virtual ~TInRegEventSideListDialog();

protected:
	TInReg *inReg;				// 2004:035 LRQ: for sched
	sign eportalSendable;

	DEFINE_SIDES_TYPE(TInRegEvent, InRegEvent)
	virtual void Header1(mstr &m);	// 2005:319 LPR: new event: inreg data
	virtual void Header2(mstr &m);

//{{TInRegEventSideListDialogRSP_TBL_BEGIN}}
	void MessagesBNClicked();
	void ListBNClicked();
	void RightBNClicked();
	void SchedBNClicked();
//{{TInRegEventSideListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TInRegEventSideListDialog);
};    //{{TInEventRegDialog}}
