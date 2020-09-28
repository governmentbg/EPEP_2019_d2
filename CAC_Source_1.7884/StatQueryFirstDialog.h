class TStatQueryFirstDialog : public TFloatDialog
{
public:
	TStatQueryFirstDialog(TWindow* parent, TStatQueryFirstGroup *group);

protected:
	TSenderListBox *senders;
	TDateFace *minDate;
	TDateFace *maxDate;

	DEFINE_GROUP_TYPE(TStatQueryFirstGroup, StatQueryFirstGroup)
	virtual bool IsValid();
	virtual void Action(bool execute);
};
