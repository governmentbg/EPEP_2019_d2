class TRequestBookDialog : public TBookInstanceDialog
{
public:
	TRequestBookDialog(TWindow* parent, TRequestBookGroup *group);

protected:
	virtual bool IsValid();
	virtual void Action(bool execute);
};
