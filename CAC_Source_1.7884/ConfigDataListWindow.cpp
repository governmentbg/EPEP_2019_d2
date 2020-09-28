//{{TConfigDataListWindow Implementation}}

TConfigDataListWindow::TConfigDataListWindow(TWindow* parent, int resourceId, TConfigName *tConfigName)
:
	TGroupListWindow(parent, resourceId, new TGArray<TConfigData>, 304), configName(tConfigName), section('\0')
{
	Add(new TGroupListColumn("Настройка", group_print, 292));
	initialRefresh = false;
	sortcol = 1;	// 2015:033 LPR
}

void TConfigDataListWindow::SetSection(char tSection)
{
	if (section != tSection)
	{
		section = tSection;
		Refresh();
	}
}

TGroup *TConfigDataListWindow::NewGroup() const
{
	TConfigData *configData = new TConfigData(configName->config);
	strcpy(configData->section, Section->Seek(section));
	return configData;
}

bool TConfigDataListWindow::AddGroup(TGroup *group)
{
	return edit_config_data(this, (TConfigData *) group, (TGArray<TConfigData> *) array);
}

bool TConfigDataListWindow::EditGroup(TGroup *group)
{
	TConfigData *configData = (TConfigData *) group;
	const TSettingData *settingData = TConfigure::Find(configData->section, configData->name);
	return configData->AskEdit(settingData->flags) && edit_config_data(this, configData, NULL);
}

bool TConfigDataListWindow::AskDelete(TGroup *group)
{
	return ask("Желаете ли да изтриете избраната %s настройка?", ((const TConfigData *) group)->Scope());
}

bool TConfigDataListWindow::DeleteGroup(TGroup *group)
{
	TConfigData *configData = (TConfigData *) group;
	bool wasEPortal = configData->IsEPortal();
	mstr m("на настройка EPORTAL");

	if (wasEPortal)
		m.printf(" %s, %s", configData->value, configData->Scope());

	if (TGroupListWindow::DeleteGroup(group))
	{
		if (wasEPortal)
			log(LOG_ADD, 0, str(m));

		return true;
	}

	return false;
}
