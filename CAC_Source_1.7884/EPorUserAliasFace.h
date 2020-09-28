//{{TComboBox = TEPorUserAliasFace}}
class TEPorUserAliasFace : public TUserAliasFace
{
public:
	TEPorUserAliasFace(TWindow* parent, int resourceId, const char *name, long flags);

protected:
	virtual void LoadData(const char *s);
};    //{{TEPorUserAliasFace}}
