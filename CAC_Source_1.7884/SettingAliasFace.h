//{{TComboBox = TSettingAliasFace}}
class TSettingAliasFace : public TStringAliasFace
{
public:
	TSettingAliasFace(TWindow* parent, int resourceId, const char *name, TGSArray<TSetting> *tArray, bool edit);

	const TSettingData *Data() { return ((TSetting *) Group())->Data; }
};    //{{TSettingAliasFace}}
