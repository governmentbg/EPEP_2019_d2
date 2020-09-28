#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
//{{TDialog = TProveActionDialog}}
class TProveActionDialog : public TTroubleDialog
{
public:
	TProveActionDialog(TWindow* parent, TProve *tProve, TProveAction *proveAction, bool edit, int resId);

protected:
	TProve *prove;
	TDateFace *date;
	TCharAliasFace *resolution;
	TCharAliasFace *sessionKind;
	TDateFace *sessionDate;
	TCharAliasFace *decisionKind;
	TLongFace *outNo;
	TYearFace *outYear;
	TCheckFace *flagsInstance;	// 2006:013 LRQ
	// 2005:346 For checks/display
	TCharAliasFace *lawsuitType;
	TLongFace *lawsuitNo;
	TYearFace *lawsuitYear;
	TStatic *sessionKindLabel;
	TStatic *sessionDateLabel;
	TStatic *decisionKindLabel;
	TIndexFace *index;		// 2006:038 LPR

	DEFINE_GROUP_TYPE(TProveAction, ProveAction)
	virtual bool IsValid();

//{{TProveActionDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TProveActionDialogVIRTUAL_END}}

//{{TProveActionDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
	void ResolutionCBNSelChange();
	void SessionKindCBNSelChange();
//{{TProveActionDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TProveActionDialog);
};    //{{TProveActionDialog}}
#endif  // APPEAL || AREAL || MILITARY || REGIONAL
