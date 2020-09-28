class TMigrateNIASendersDialog : public TMigrateSendersDialog
{
public:
	TMigrateNIASendersDialog(TWindow* parent, FILE *tf, int resId = IDD_MIGRATE_SENDERS);

protected:
	virtual bool Migrate(TSender *sender, TGroup *target);
};
