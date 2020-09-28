class TInvBookDeliverDialog : public TFloatDialog
{
public:
	TInvBookDeliverDialog(TWindow* parent, TInvBookDeliverGroup *group);

protected:
	TPersonAliasFace *deliverer;
	TDateFace *minDate;
	TDateFace *maxDate;
	TCheckFace *serialNo;
	TLongFace *autoSerialNo;

	virtual bool IsValid();
	virtual void SetupWindow();
	virtual void Action(bool execute);

	void SerialNoBNClicked();
DECLARE_RESPONSE_TABLE(TInvBookDeliverDialog);
};
