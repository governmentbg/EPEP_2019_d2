class TInvBookSubpoenaDialog : public TFloatDialog
{
public:
	TInvBookSubpoenaDialog(TWindow* parent, TInvBookSubpoenaGroup *group);

protected:

	char types[SIZE_OF_TYPES];
	TCharAliasFace *type;

	TStatic *dateLabel;
	TDateFace *minDate;
	TDateFace *maxDate;
	TCharAutoListFace *compositions;
	TRadioFace *bySubpoenaDate;
	TRadioFace *bySubpoenaReceived;
	TUserListBox *deliverers;
	TCheckFace *newFormat;
	TLongFace *autogen;

	DEFINE_GROUP_TYPE(TInvBookSubpoenaGroup, InvBookSubpoenaGroup);
	virtual bool IsValid();
	virtual void Action(bool execute);

	void RadioBNClicked();
	void TypeBNSelChange();
	void NewFormatBNClicked();
DECLARE_RESPONSE_TABLE(TInvBookSubpoenaDialog);
};