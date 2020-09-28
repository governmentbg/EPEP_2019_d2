//{{TSettingAliasFace Implementation}}

TSettingAliasFace::TSettingAliasFace(TWindow* parent, int resourceId, const char *name, TGSArray<TSetting> *tArray, bool edit)
:
	TStringAliasFace(parent, resourceId, name, (*tArray)[0], NULL, edit ? FLAG_NULL : ALIAS_ENABLE)
{
	delete array;
	array = tArray;
	dynamic = loadData = false;
}
