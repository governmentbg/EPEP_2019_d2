//{{TComboBox = TCompositionAliasFace}}
class TCompositionAliasFace : public TLongAliasFace
{
public:
	TCompositionAliasFace(TWindow* parent, int resourceId, const char *name, const char *compositions, long flags = FLAG_NULL);

	void SetSender(char ucnType, const char *ucn);

protected:
	long court;		// default = self

	virtual void InsertVisual(int index);
};    //{{TCompositionAliasFace}}
