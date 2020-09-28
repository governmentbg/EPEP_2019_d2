//{{TDialog = TEditJudicialDialog}}
class TEditJudicialDialog : public TEditNamedDialog
{
public:
	TEditJudicialDialog(TWindow* parent, TNamedGroup *ucnGroup, bool edit, bool full, bool tStat, int resId);

protected:
	bool stat;
	TBulStatFace *bulstat;
	TStringAliasFace *country;

	virtual bool IsValid();
};    //{{TEditJudicialDialog}}
