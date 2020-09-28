//{{TComboBox = TNIATDepAliasFace}}
class TNIATDepAliasFace : public TSenderAliasFace
{
public:
	TNIATDepAliasFace(TWindow* parent, int resourceId, const char *name, long flags = FLAG_NULL,
		TGArray<TReceiver> *receivers = NULL);

	void SelectElement(const char *ucn, char ucnType);
};    //{{TNIATDepAliasFace}}
