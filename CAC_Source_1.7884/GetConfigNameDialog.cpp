//{{TGetConfigNameDialog Implementation}}

TGetConfigNameDialog::TGetConfigNameDialog(TWindow* parent, TConfigName *configName, const char *tTitle, const char *tInvalid, int resId)
	: TGroupDialog(parent, configName, resId), title(tTitle), invalid(tInvalid)
{
	strcpy(backup, configName->config);
	config = new THostNameFace(this, IDC_GET_CONFIG_NAME_CONFIG, "F_CONFIG", SIZE_OF_ALIAS);
}

bool TGetConfigNameDialog::IsValid()
{
	bool isValid = TGroupDialog::IsValid();

	if (isValid)
		if (!strcmp(config->S(), backup))
			isValid = error(invalid);

	return isValid;
}

void TGetConfigNameDialog::SetupWindow()
{
	TGroupDialog::SetupWindow();
	SetCaption(title);
}
