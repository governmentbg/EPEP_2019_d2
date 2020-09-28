//{{TSettingFace Implementation}}

TSettingFace::TSettingFace(TWindow* parent, int resourceId, const char *name, TSettingAliasFace *setting)
:
	TStringFace(parent, resourceId, name, SIZE_OF_CONFIG_VALUE)
{
	SetValidator(new TSettingValidator(setting));
}
