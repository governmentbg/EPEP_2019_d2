//{{TDialog = TGetMoveJudicialActDialog}}
class TGetMoveJudicialActDialog : public TGetGroupDialog
{
public:
	TGetMoveJudicialActDialog(TWindow* parent, TDecision &source, TSession &tSession);

protected:
	TSession &session;
	TCharAliasFace *sessionKind;
	TDateFace *sessionDate;
	TIndexFace *sessionIndex;
	mstr sesKinds;

	virtual bool IsValid();
	DEFINE_GROUP_TYPE(TDecision, Source)

//{{TGetMoveJudicialActDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TGetMoveJudicialActDialogVIRTUAL_END}}

//{{TGetMoveJudicialActDialogRSP_TBL_BEGIN}}
	void TypeCBNSelChange();
	void SessionKindCBNSelChange();
//{{TGetMoveJudicialActDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TGetMoveJudicialActDialog);
};    //{{TGetMoveJudicialActDialog}}
