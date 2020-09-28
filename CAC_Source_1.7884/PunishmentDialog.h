//{{TDialog = TPunishmentDialog}}
class TPunishmentDialog : public TGroupDialog
{
public:
	TPunishmentDialog(TWindow* parent, TDecision *decision, TAccusation *accusation, TLawsuit *lawsuit);

protected:
	DEFINE_GROUP_TYPE(TAccusation, Accusation)

//{{TPunishmentDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TPunishmentDialogVIRTUAL_END}}

//{{TPunishmentDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
//{{TPunishmentDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TPunishmentDialog);
};    //{{TPunishmentDialog}}
