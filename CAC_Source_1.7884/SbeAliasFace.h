//{{TComboBox = TSbeAliasFace}}
class TSbeAliasFace : public TEisAliasFace
{
public:
	TSbeAliasFace(TWindow* parent, int resourceId, const char *name, TGArray<SBE> *tExclude, NPR *npr,
		TIntegerArray *tInclude);

	virtual void Refresh();

protected:
	TGArray<SBE> *exclude;
	int fzlCount;
	TIntegerArray *include;
};    //{{TSbeAliasFace}}
