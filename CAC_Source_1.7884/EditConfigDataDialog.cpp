DEFINE_RESPONSE_TABLE1(TEditConfigDataDialog, TEditDialog)
//{{TEditConfigDataDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_CBN_SELCHANGE(IDC_EDIT_CONFIG_DATA_NAME, NameCBNSelChange),
//{{TEditConfigDataDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TEditConfigDataDialog Implementation}}

TEditConfigDataDialog::TEditConfigDataDialog(TWindow* parent, TConfigData *configData, TGSArray<TSetting> *settings,
	bool edit, int resId)
:
	TEditDialog(parent, configData, edit, resId), previous(atoi(configData->value))
{
	new TStringFace(this, IDC_EDIT_CONFIG_DATA_CONFIG, "F_CONFIG", SIZE_OF_ALIAS);
	new TStringFace(this, IDC_EDIT_CONFIG_DATA_SECTION, "F_SECTION", SIZE_OF_ALIAS);
	setting = new TSettingAliasFace(this, IDC_EDIT_CONFIG_DATA_NAME, "F_NAME", settings, edit);
	value = new TSettingFace(this, IDC_EDIT_CONFIG_DATA_VALUE, "F_VALUE", setting);
	text1 = new TStatic(this, IDC_EDIT_CONFIG_DATA_TEXT_1);
	text2 = new TStatic(this, IDC_EDIT_CONFIG_DATA_TEXT_2);
	text3 = new TStatic(this, IDC_EDIT_CONFIG_DATA_TEXT_3);
}

bool TEditConfigDataDialog::IsValid()
{
	bool isValid = TEditDialog::IsValid();

	if (isValid)
	{
		const TSettingData *settingData = setting->Data();
		// TValidator::IsValid() is executed frequently causing slowdown
		const char *invalid = TConfigure::Invalid(settingData, value->S());
		long tValue = atoi(value->S());

		if (invalid)
			isValid = error("%s.", invalid);
		else if (ConfigData()->IsEPortal(setting->S()) && tValue == 1)
			isValid = warn("Тази настройка не би трябвало да е 1.\n%s", Q_CONTINUE);
		else if (settingData->min < 0 && settingData->max == 0 && tValue == 0)
			isValid = ask("Тази настройка не би трябвало да е 0.\n%s", Q_CONTINUE);
	}

	return isValid;
}

void TEditConfigDataDialog::SetupWindow()
{
	TEditDialog::SetupWindow();

	mstr m;

	if (ConfigData()->Global())
		m.printf("%s НА ГЛОБАЛНА НАСТРОЙКА", edit ? "РЕДАКТИРАНЕ" : "ДОБАВЯНЕ");
	else
		m.printf("%s на локална настройка", edit ? "Редактиране" : "Добавяне");

	SetCaption(str(m));
	NameCBNSelChange();
}

void TEditConfigDataDialog::OKBNClicked()
{
	if (IsValid())
	{
	#if 20180328
		if (edit || ConfigData()->AskEdit(value->S(), setting->Data()))
	#else  // 20180328
		if (edit || ConfigData()->AskEdit(setting->Data()->flags))
	#endif  // 20180328
		{
			// 2018-10-18: was TEditDialog::OKBNClicked()
			*Group() << this;

			if (Group()->Save(edit))
			{
				if (ConfigData()->IsEPortal())
				{
					mstr m("на настройка EPORTAL");

					if (edit)
					{
						m.printf(" %ld -> %s, %s", previous, value->S(), ConfigData()->Scope());
						log(LOG_EDIT, 0, str(m));
					}
					else
					{
						m.printf(" %s, %s", value->S(), ConfigData()->Scope());
						log(LOG_ADD, 0, str(m));
					}
				}

				CmOk();
			}
		}
	}
}

void TEditConfigDataDialog::NameCBNSelChange()
{
	const TSettingData *settingData = setting->Data();

	text1->SetCaption(settingData->text1);
	text2->SetCaption(settingData->text2);
	text3->SetCaption(settingData->text3);

	if (!edit)
	{
		*ConfigData() << this;
		TConfigure::GetGlobal(ConfigData()->section, ConfigData()->name, ConfigData()->value);
		*ConfigData() >> this;
	}
}
