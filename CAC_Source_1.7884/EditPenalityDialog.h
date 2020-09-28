//{{TDialog = TEditPenalityDialog}}
class TEditPenalityDialog : public TGroupDialog
{
public:
	TEditPenalityDialog(TWindow* parent, TExpense *expense, TLawsuit *, bool tEdit);

protected:
	bool edit;
	char deciments[SIZE_OF_ENDOC_KINDS];

	TCharAliasFace *sumState;
	TDateFace *paidDate;
	TCharAliasFace *rejectKind;
	TDateFace *rejectDate;
	TIndexFace *rejectIndex;
	TStringFace *paidNote;		// 2003:209 LRQ: +related
	// 2003:350 LRQ: reject by a higher court
	TLongAliasFace *penalityType;
	TStatic *rejectKindLabel;
	TStatic *rejectIndexLabel;
	TSenderAliasFace *rejectCourt;

	DEFINE_GROUP_TYPE(TExpense, Expense)
	virtual bool IsValid();

//{{TEditPenalityDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEditPenalityDialogVIRTUAL_END}}

//{{TEditPenalityDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
	void SumStateCBNSelChange();
	void RejectKindCBNSelChange();
	void PenalityTypeCBNSelChange();
	void PrintBNClicked();
//{{TEditPenalityDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TEditPenalityDialog);
};    //{{TEditPenalityDialog}}
