//{{TDialog = TEditSurrouendocDialog}}
class TEditSurrouendocDialog : public TTroubleDialog
{
public:
	TEditSurrouendocDialog(TWindow* parent, TSurroundment *surrouendoc, const TSession *tSession, TLawsuit *tLawsuit);

protected:
	const TSession *session;
	TCharAliasFace *decisionKind;
	TLongFace *eventNo;
	TDateFace *decisionDate;
	TCharAliasFace *result;
	TCharListFace *accomplys;
	TCharAliasFace *debtorStatus;
	TLawsuit *lawsuit;	// 2016:273 non-const for print_decision_judgement()

	TBackupIndex backupIndex;
	void Commit();

	DEFINE_GROUP_TYPE(TSurroundment, Surrouendoc)

//{{TEditSurrouendocDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEditSurrouendocDialogVIRTUAL_END}}

//{{TEditSurrouendocDialogRSP_TBL_BEGIN}}
	void DecisionKindCBNSelChange();
	void AccomplysLBNSelChange();
	void OKBNClicked();
	void RightBNClicked();
	void PrintBNClicked();
//{{TEditSurrouendocDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TEditSurrouendocDialog);
};    //{{TEditSurrouendocDialog}}
