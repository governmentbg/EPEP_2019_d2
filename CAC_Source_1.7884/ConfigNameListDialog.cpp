DEFINE_RESPONSE_TABLE1(TConfigNameListDialog, TGroupListDialog)
//{{TConfigNameListDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDEDIT, EditBNClicked),
	EV_BN_CLICKED(IDCOPY, CopyBNClicked),
	EV_BN_CLICKED(IDRENAME, RenameBNClicked),
	EV_COMMAND_ENABLE(IDDELETE, LocalEnable),
	EV_COMMAND_ENABLE(IDCOPY, SelectEnable),
	EV_COMMAND_ENABLE(IDRENAME, LocalEnable),
//{{TConfigNameListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TConfigNameListDialog Implementation}}

TConfigNameListDialog::TConfigNameListDialog(TWindow* parent, int resId)
	: TGroupListDialog(parent, resId)
{
	configs = new TConfigNameListWindow(this, IDC_GROUP_LIST_WINDOW);
}

void TConfigNameListDialog::LocalEnable(TCommandEnabler &tce)
{
	TConfigName *configName = configs->ConfigName();
	tce.Enable(configName && strcmp(configName->config, CONFIG_NAME_GLOBAL));
}

void TConfigNameListDialog::EditBNClicked()
{
	configs->Open();
}

void TConfigNameListDialog::CopyBNClicked()
{
	configs->Copy();
}

void TConfigNameListDialog::RenameBNClicked()
{
	configs->Edit();
}
