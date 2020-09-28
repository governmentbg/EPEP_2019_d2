//{{TDialog = TInRegNewBaseDialog}}
class TInRegNewBaseDialog : public TEditIncomingDialog
{
public:
	TInRegNewBaseDialog(TWindow* parent, TInReg *inReg, const char *kinds, const char *surroundKinds,
		const char *sessionKinds, const TLawsuit *tLawsuit);

protected:
	TStringFace *text;
	TCharAliasFace *lawsuitType;
	TLongFace *lawsuitNo;
	TYearFace *lawsuitYear;
	TCharAliasFace *sessionKind;
	TDateFace *sessionDate;
	TCharAliasFace *decisionKind;
	TLongFace *decisionNo;		// 2005:042 LRQ: commout +all support

	TIndexFace *sessionIndex;
	TCharAliasFace *surroundKind;
	TLongFace *surroundNo;
	TYearFace *surroundYear;
	// 2016:083 +related
	TSenderAliasFace *receiver;
	const TLawsuit *lawsuit;
	TStatic *lawsuitTypeLabel;
	TStatic *lawsuitNoLabel;
	TStatic *lawsuitYearLabel;
	TStatic *collegeLabel;
	TPushButton *query;

	void SetReceivers();		// 2016:083 +related

	DEFINE_GROUP_TYPE(TInReg, InReg)

//{{TInRegNewBaseDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TInRegNewBaseDialogVIRTUAL_END}}

//{{TInRegNewBaseDialogRSP_TBL_BEGIN}}
	void QueryBNEnable(TCommandEnabler &tce);
	void QueryBNClicked();
//{{TInRegNewBaseDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TInRegNewBaseDialog);
};    //{{TInRegNewBaseDialog}}