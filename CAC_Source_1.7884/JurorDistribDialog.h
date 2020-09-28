//{{TDialog = TJurorDistribDialog}}
class TJurorDistribDialog : public TGroupDialog
{
public:
	TJurorDistribDialog(TWindow* parent, TJurorDistribution *tDistribution, TBaseSession *tSession,
		const TLawsuit *tLawsuit);

protected:
	TJurorDistribution *distribution;
	TBaseSession *session;
	const TLawsuit *lawsuit;
	bool locked;

	TStatic *header;
	TJurorAliasFace *jurors[JUROR_COUNT];
	TTwinButton *buttons[JUROR_COUNT];
	TStatic *labels[JUROR_COUNT];
	TCheckFace *pedagogs[JUROR_COUNT];
	TPushButton *cancel, *erase;

	DEFINE_GROUP_TYPE(TJurorDistrib, Distrib)
	virtual bool IsValid();

	void UpdateButton(int index);
	void Update();
	void MaybeLock();
	void Select(char oper, const char *ucn, int index, const char *text);
	void InitJuror(int index);
	void MaybeClear(int index);
	void ChangeJuror(int index);
	void ManualJuror(int index);

//{{TJurorDistribDialogVIRTUAL_BEGIN}}
	virtual bool CanClose();
	virtual void SetupWindow();
//{{TJurorDistribDialogVIRTUAL_END}}

//{{TJurorDistribDialogRSP_TBL_BEGIN}}
	void JurorXCBNSelChange();
	void ReserveCBNSelChange();
	void JurorChangeBNClicked();
	void JurorManualBNClicked();
	void ReJurorChangeBNClicked();
	void ReJurorManualBNClicked();
	void JurorXChangeBNClicked();
	void JurorXManualBNClicked();
	void ReserveChangeBNClicked();
	void ReserveManualBNClicked();
	void QueryBNClicked();
	void PrintBNClicked();
	void JurorPedagogBNClicked();
	void ReJurorPedagogBNClicked();
	void JurorXPedagogBNClicked();
	void ReservePedagogBNClicked();
	void DeleteBNClicked();
	void CancelBNClicked();
	void OKBNClicked();
	void EvClose();
//{{TJurorDistribDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TJurorDistribDialog);
};    //{{TJurorDistribDialog}}
