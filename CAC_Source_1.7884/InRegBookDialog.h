class TInRegBookDialog : public TBookDialog
{
public:
	TInRegBookDialog(TWindow* parent, TInRegBookGroup *group);

protected:
	virtual void Action(bool execute);
};
