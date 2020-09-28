DEFINE_RESPONSE_TABLE1(TMigrateEisppSendersDialog, TMigrateSendersDialog)
	EV_BN_CLICKED(IDSEARCH, SearchBNClicked),
	EV_CBN_SELCHANGE(IDC_MIGRATE_SENDERS_UCN_TYPE, SenderTypeCBNSelChange),
END_RESPONSE_TABLE;

TMigrateEisppSendersDialog::TMigrateEisppSendersDialog(TWindow* parent, FILE *tf, int resId)
	: TMigrateSendersDialog(parent, tf, EISPP_SENDERS, resId)
{
	name = new TStringFace(this, IDC_MIGRATE_EISPP_SENDERS_NAME, "F_NAME", SIZE_OF_EISPP_NAME);
	sources = new TMigrateEisppSendersListBox(this, IDC_MIGRATE_SENDERS_SOURCES, *EISPP_SENDERS);
	targets = new TEisDepartListBox(this, IDC_MIGRATE_SENDERS_TARGETS, *EISPP_SENDERS, "");
}

bool TMigrateEisppSendersDialog::Migrate(TSender *source, TGroup *target)
{
	return migrate_eispp_sender(source, TYPECHECK_PTR_CAST(TEisDepart, target));
}

void TMigrateEisppSendersDialog::SearchBNClicked()
{
	Targets()->Search(name->S(), false);
}

void TMigrateEisppSendersDialog::SenderTypeCBNSelChange()
{
	Sources()->SetUCNType(ucnType->C());
	Targets()->SetUCNType(ucnType->C());
}
