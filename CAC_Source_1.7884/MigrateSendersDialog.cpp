DEFINE_RESPONSE_TABLE1(TMigrateSendersDialog, TQuickDialog)
	EV_COMMAND_ENABLE(IDDELETE, DeleteBNEnable),
	EV_BN_CLICKED(IDDELETE, DeleteBNClicked),
	EV_COMMAND_ENABLE(IDCONVERT, MigrateBNEnable),
	EV_BN_CLICKED(IDCONVERT, MigrateBNClicked),
END_RESPONSE_TABLE;

TMigrateSendersDialog::TMigrateSendersDialog(TWindow* parent, FILE *tf, const char *ucnTypes, int resId)
	: TQuickDialog(parent, resId), f(tf)
{
	ucnType = new TCharAliasFace(this, IDC_MIGRATE_SENDERS_UCN_TYPE, "F_UCN_TYPE", UCNType, ucnTypes);
}

void TMigrateSendersDialog::DeleteBNEnable(TCommandEnabler &tce)
{
	tce.Enable(sources->Group());
}

void TMigrateSendersDialog::DeleteBNClicked()
{
	if (ask("Желаете ли да изтриете %s?", Sender()->name))
		delete_sender(Sender(), f);
}

void TMigrateSendersDialog::MigrateBNEnable(TCommandEnabler &tce)
{
	tce.Enable(sources->Group() && targets->Group());
}

void TMigrateSendersDialog::MigrateBNClicked()
{
	constant targetName = ((TString *) Target()->Find("F_NAME"))->S();

	if (ask("Желаете ли да актуализирате %s като %s?", Sender()->name, targetName))
	{
		TWaitWindow wait(this, WAIT);

		fprintf(f, "%s -> %s: ", Sender()->name, targetName);

		if (Migrate(Sender(), Target()))
		{
			sources->RemoveGroup();
			fputs("прекодиран\n", f);
		}
		else
			fputs("грешка\n", f);
	}
}
