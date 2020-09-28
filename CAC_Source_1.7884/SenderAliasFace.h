//{{TComboBox = TSenderAliasFace}}
class TSenderAliasFace : public TStringAliasFace
{
public:
	TSenderAliasFace(TWindow* parent, int resourceId, const char *name, const char ucnType, long flags = FLAG_NULL,
		TGArray<TReceiver> *receivers = NULL);

	char UCNType();
};    //{{TSenderAliasFace}}
