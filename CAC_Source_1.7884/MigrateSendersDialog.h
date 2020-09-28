class TMigrateSendersDialog : public TQuickDialog
{
public:
	TMigrateSendersDialog(TWindow* parent, FILE *tf, const char *ucnTypes, int resId = IDD_MIGRATE_SENDERS);

protected:
	FILE *f;
	TCharAliasFace *ucnType;
	TMigrateSendersListBox *sources;
	TGroupListBox *targets;

	TSender *Sender() { return TYPECHECK_PTR_CAST(TSender, sources->Group()); }
	TGroup *Target() { return TYPECHECK_PTR_CAST(TGroup, targets->Group()); }

	virtual bool Migrate(TSender *sender, TGroup *target) = 0;

	void DeleteBNEnable(TCommandEnabler &tce);
	void DeleteBNClicked();
	void MigrateBNEnable(TCommandEnabler &tce);
	void MigrateBNClicked();
DECLARE_RESPONSE_TABLE(TMigrateSendersDialog);
};
