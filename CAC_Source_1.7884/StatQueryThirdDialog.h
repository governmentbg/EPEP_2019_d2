class TStatQueryThirdDialog : public TGroupListDialog
{
public:
	TStatQueryThirdDialog(TWindow* parent);

protected:
	TStatQueryListWindow *files;
	void LaunchBNClicked();
	void LaunchBNEnable(TCommandEnabler &tce);

DECLARE_RESPONSE_TABLE(TStatQueryThirdDialog);
};

