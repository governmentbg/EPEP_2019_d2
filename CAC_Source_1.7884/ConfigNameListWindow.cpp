//{{TConfigNameListWindow Implementation}}

TConfigNameListWindow::TConfigNameListWindow(TWindow* parent, int resourceId)
	: TGroupListWindow(parent, resourceId, NULL, 220)
{
	Add(
		new TGroupListColumn("Име", "F_CONFIG", 80),
		new TGroupListColumn("Дата", "F_DATE", 48),
		new TGroupListColumn("Версия", config_version, 48),
		new TGroupListColumn("Протокол", config_protocol, 32),
		NULL
	);

	sortcol = 1;	// 2011:007 FIX: was missing
}

void TConfigNameListWindow::Open()
{
	TConfigDataListDialog(this, ConfigName()).Execute();
}

void TConfigNameListWindow::Copy()
{
	TConfigName *configName = (TConfigName *) NewGroup();

	if (get_config_name(this, configName, "Копиране на конфигурация", "Името е същото."))
	{
		if (configName->Try())
		{
			if (ask("Съществува конфигурация с такова име. Желаете ли да продължите?"))
				copy_config(ConfigName(), configName);
		}
		else if (configName->Insert())
		{
			copy_config(ConfigName(), configName);
			SetVisualIndex(InsertGroup(configName));
			UpdateVisual();
			configName = NULL;
		}
	}

	delete configName;
}

TGroup *TConfigNameListWindow::NewGroup() const
{
	return new TConfigName(NULL);
}

bool TConfigNameListWindow::AddGroup(TGroup *group)
{
	return get_config_name(this, (TConfigName *) group, "Добавяне на конфигурация", "Името е задължително.") && group->Insert();
}

bool TConfigNameListWindow::EditGroup(TGroup *group)
{
	TConfigName *configName = (TConfigName *) group;

	if (get_config_name(this, configName, "Преименуване на конфигурация", "Името е същото.") && configName->Insert())
		if (copy_config(ConfigName(), configName) && ConfigName()->Delete(true))
			return true;

	return false;
}

bool TConfigNameListWindow::AskDelete(TGroup *)
{
	return ask("Желаете ли да изтриете избраната конфигурация?");
}

void TConfigNameListWindow::SetupWindow()
{
	TGroupListWindow::SetupWindow();

	for (int i = 0; i < array->Count(); i++)
		if (!strcmp(CONFIG_NAME_LOCAL, ((TConfigName *) GroupAt(i))->config))
			SetVisualIndex(i);
}
