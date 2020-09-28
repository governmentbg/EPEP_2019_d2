TMigrateNIASendersDialog::TMigrateNIASendersDialog(TWindow* parent, FILE *tf, int resId)
	: TMigrateSendersDialog(parent, tf, C2S[UCN_NIA_TDEP], resId)
{
	sources = new TMigrateNIASendersListBox(this, IDC_MIGRATE_SENDERS_SOURCES, true);
	targets = new TMigrateNIASendersListBox(this, IDC_MIGRATE_SENDERS_TARGETS, false);
}

bool TMigrateNIASendersDialog::Migrate(TSender *source, TGroup *target)
{
	return migrate_nia_sender(source, TYPECHECK_PTR_CAST(TSender, target));
}
