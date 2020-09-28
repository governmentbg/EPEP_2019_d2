//{{TComboBox = TVSSIndexAliasFace}}
class TVSSIndexAliasFace : public TStringAliasFace
{
public:
	TVSSIndexAliasFace(TWindow* parent, int resourceId, const char *name);

	void SetCrit(char tCourtType, const char *tTypes, char tKind, const CDate &tDate);

protected:
	char courtType;
	const char *types;
	char kind;
	CDate date;

	virtual void Refresh();
};    //{{TVSSIndexAliasFace}}
