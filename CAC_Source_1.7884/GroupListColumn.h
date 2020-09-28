class TGroupListColumn : public TObject
{
public:
	TGroupListColumn(const char *tTitle, const char *name, int tWidth, int tFormat = LVCFMT_LEFT);
	TGroupListColumn(const char *tTitle, void (*tPrint)(mstr &m, TGroup *group), int tWidth, int tFormat = LVCFMT_LEFT);

	void Print(mstr &m, TGroup *group);

	const char *title;
	int width;
	int format;
	bool search;
	bool resize;

protected:
	void Init();
	void (*print)(mstr &m, TGroup *group);
};
