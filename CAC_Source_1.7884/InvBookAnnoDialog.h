class TInvBookAnnoDialog : public TFloatDialog
{
public:
	TInvBookAnnoDialog(TWindow* parent, TInvBookAnnoGroup *group);

protected:
	TSenderListBox *senders;
	TDateFace *minDate;
	TDateFace *maxDate;
	TCheckFace *serialNoColumn;
	TLongFace *autoSerialNo;  

	DEFINE_GROUP_TYPE(TInvBookAnnoGroup, InvBookAnnoGroup)
	virtual bool IsValid();
	virtual void Action(bool execute);

	void SerialNoColumnBNClicked();
DECLARE_RESPONSE_TABLE(TInvBookAnnoDialog);
};
