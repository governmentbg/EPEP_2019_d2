//{{TListWindow = TSubpoenaListWindow}}
class TSubpoenaListWindow : public TDoubleListWindow
{
public:
	TSubpoenaListWindow(TWindow* parent, int resourceId, TRCDKeyContainer *tContainer, TSideWind *tSideWind);

	TSubpoena *Subpoena() { return (TSubpoena *) Group(); }
	void InsertGroup(TSubpoena *subpoena) { TDoubleListWindow::InsertGroup(new TDoubleSubpoena(subpoena)); }
	bool Delete(TSubpoenaVariantGroup *variant);
	bool ReturnGroup(char originKind, char secondKind);
	TSubpoena *FindLetter(const TSubpoena *subpoena);
	int Count() { return subpoenas.Count(); }
	void RedrawFocus() { TDoubleListWindow::RedrawFocus(); }

protected:
	TRCDKeyContainer *container;
	TSideWind *sideWind;
	TGSArray<TDoubleSubpoena> subpoenas;

	void ServeStickers(const TSubpoena *subpoena);

	virtual void DrawSecond(TListWindItem &lwi, TGroup *group);
};    //{{TSubpoenaListWindow}}
