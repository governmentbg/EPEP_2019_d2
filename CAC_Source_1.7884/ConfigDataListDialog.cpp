DEFINE_RESPONSE_TABLE1(TConfigDataListDialog, TGroupListDialog)
//{{TConfigDataListDialogRSP_TBL_BEGIN}}
	EV_CBN_SELCHANGE(IDC_CONFIG_DATA_LIST_SECTION, SectionCBNSelChange),
	EV_LVN_ITEMCHANGED(IDC_GROUP_LIST_WINDOW, ConfigsLVNItemChanged),
//{{TConfigDataListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TConfigDataListDialog Implementation}}

TConfigDataListDialog::TConfigDataListDialog(TWindow* parent, TConfigName *tConfigName, int resId)
:
	TGroupListDialog(parent, resId), configName(tConfigName)
{
	strcpy(sections, configName->Global() ? SECTION_GLOBALS : SECTION_LOCALS);

	if (COURT_AREA != COURT_VAROS)
		stremove(sections, SECTION_SMS);

	name = new TStringFace(this, IDC_CONFIG_DATA_LIST_CONFIG, "F_CONFIG", SIZE_OF_ALIAS);
	section = new TCharAliasFace(this, IDC_CONFIG_DATA_LIST_SECTION, "F_SECTION", Section, sections);
	configs = new TConfigDataListWindow(this, IDC_GROUP_LIST_WINDOW, configName);
	text1 = new TStatic(this, IDC_CONFIG_DATA_LIST_TEXT_1);
	text2 = new TStatic(this, IDC_CONFIG_DATA_LIST_TEXT_2);
	text3 = new TStatic(this, IDC_CONFIG_DATA_LIST_TEXT_3);
}

void TConfigDataListDialog::SetupWindow()
{
	TGroupListDialog::SetupWindow();
	name->SetS(configName->config);
	SectionCBNSelChange();
}

void TConfigDataListDialog::SectionCBNSelChange()
{
	configs->SetSection(section->C());
	ConfigsLVNItemChanged(TLwNotify());
}

void TConfigDataListDialog::ConfigsLVNItemChanged(TLwNotify &)
{
	TConfigData *configData = (TConfigData *) configs->Group();

	if (configData)
	{
		const TSettingData *settingData = TConfigure::Find(configData->section, configData->name);

		if (settingData)
		{
			text1->SetCaption(settingData->text1);
			text2->SetCaption(settingData->text2);
			text3->SetCaption(settingData->text3);
		}
		else
		{
			text1->SetCaption("Настройката не се поддържа.");
			text2->SetCaption("");
			text3->SetCaption("");
		}
	}
}
