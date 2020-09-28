//{{TListBox = TEisDepartListBox}}
class TEisDepartListBox : public TFindAliasListBox
{
public:
	TEisDepartListBox(TWindow* parent, int resourceId, char tUCNType, const char *initialName);

	TEisDepart *Depart() { return (TEisDepart *) Group(); }
	void SetUCNType(char tUCNType);

protected:
	char ucnType;
};    //{{TEisDepartListBox}}
