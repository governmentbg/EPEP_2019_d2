class TMigrateEisppSendersDialog : public TMigrateSendersDialog
{
public:
	TMigrateEisppSendersDialog(TWindow* parent, FILE *tf, int resId = IDD_MIGRATE_EISPP_SENDERS);

protected:
	TStringFace *name;

	TEisDepart *Depart() { return TYPECHECK_PTR_CAST(TEisDepart, targets->Group()); }
	TMigrateEisppSendersListBox *Sources() { return TYPECHECK_PTR_CAST(TMigrateEisppSendersListBox, sources); }
	TEisDepartListBox *Targets() { return TYPECHECK_PTR_CAST(TEisDepartListBox, targets); }

	virtual bool Migrate(TSender *sender, TGroup *target);

	void SearchBNClicked();
	void SenderTypeCBNSelChange();
DECLARE_RESPONSE_TABLE(TMigrateEisppSendersDialog);
};
