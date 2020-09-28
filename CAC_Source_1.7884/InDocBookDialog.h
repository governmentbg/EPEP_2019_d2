class TInDocBookDialog : public TBookInstanceDialog
{
public:
	TInDocBookDialog(TWindow *parent, TInDocBookGroup *group);

protected:
	TCheckFace *inRequest;
	TCheckFace *inSurround;
	TCheckFace *inInReg;

	virtual bool IsValid();
	virtual void Action(bool execute);
};
