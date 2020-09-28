//{{TComboBox = TAccountAliasFace}}
class TAccountAliasFace : public TStringAliasFace
{
public:
	TAccountAliasFace(TWindow* parent, int resourceId, const char *name, const char ucnType, long flags = FLAG_NULL);

	TAccount *Account() { return TYPECHECK_PTR_CAST(TAccount, Group()); }
};    //{{TAccountAliasFace}}
