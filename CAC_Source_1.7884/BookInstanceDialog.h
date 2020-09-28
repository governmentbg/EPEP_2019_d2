class TBookInstanceDialog : public TBookDialog
{
public:
	TBookInstanceDialog(TWindow* parent, TBookGroup *group, TCharAliasGroup *type, int resId, bool aliasFace = false);

protected:
	TStatic *labelCaption;
	TCheckFace *instance_1st;
	TCheckFace *instance_2nd;

	virtual bool IsValid();
	virtual void SetupWindow();
};
