//{{TComboBox = TJurorAliasFace}}
class TJurorAliasFace : public TKnownAliasFace
{
public:
	TJurorAliasFace(TWindow* parent, int resourceId, const char *name, const char *ucn0, const char *ucn1, long flags);
	virtual ~TJurorAliasFace();

	void Include(const char *ucn);	// 2010:211 LPR/URQ: include already existing juror
	void SetUCN(const char *ucn0);

protected:
	const char *ucns[3];	// 2010:270 LPR: 4 -> 3, using ALIAS_EMPTY for empty element

	virtual void Refresh();
};    //{{TJurorAliasFace}}
