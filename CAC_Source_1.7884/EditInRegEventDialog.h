//{{TDialog = TEditInRegEventDialog}}
class TEditInRegEventDialog : public TTroubleDialog
{
public:
	TEditInRegEventDialog(TWindow* parent, TInReg *tInReg, TInRegEvent *inRegEvent, bool edit, TSideWind &tSideWind);

protected:
	TInReg *inReg;
	TDateFace *date;
	TCharAliasFace *inRegKind;	// 2005:318 display
	TLongFace *eventNo;		// 2010:260 en/disable
	TDateFace *returned;		// 2010:260
	TPersonAliasFace *judge;	// 2010:270 check for empty
	TCharAliasFace *result;		// 2016:252 for e-portal
	TIndexFace *index;		// 2017:156 greasy hack
	TSideWind &sideWind;		// 2018-10-26: LPR: +related
	TStatic *judgeLabel;		// 2018-11-26: PRQ: +related

	bool IsEPortalAllowed();

	DEFINE_GROUP_TYPE(TInRegEvent, InRegEvent)
	virtual bool IsValid();

//{{TEditInRegEventDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEditInRegEventDialogVIRTUAL_END}}

//{{TEditInRegEventDialogRSP_TBL_BEGIN}}
	void RightBNClicked();
	void OKBNClicked();
//{{TEditInRegEventDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TEditInRegEventDialog);
};    //{{TEditInRegEventDialog}}
