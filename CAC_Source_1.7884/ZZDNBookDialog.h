class TZZDNBookDialog : public TBookDialog
{
public:
	TZZDNBookDialog(TWindow* parent, TZZDNBookGroup *group);

protected:
	TLongFace *autogen;

	virtual void SetupWindow();
};
