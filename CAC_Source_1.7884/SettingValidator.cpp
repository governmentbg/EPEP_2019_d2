//{{TSettingValidator Implementation}}

TSettingValidator::TSettingValidator(TSettingAliasFace *tSetting)
:
	setting(tSetting)
{
}

void TSettingValidator::Error(TWindow* )
{
	error("Невалидна стойност на настройката.");
}

bool TSettingValidator::IsValid(const char far* str)
{
		const TSettingData *settingData = setting->Data();

		return strlen(str) <= (settingData->type == SETTING_INTEGER ? LENGTH_OF_CONFIG_INTEGER : settingData->max);
}

bool TSettingValidator::IsValidInput(char far* str, bool suppressFill)
{
	bool result = TValidator::IsValidInput(str,  suppressFill);

	if (result)
		result = IsValid(str);

	return result;
}
