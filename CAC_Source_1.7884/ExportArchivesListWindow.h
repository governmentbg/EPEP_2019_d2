//{{TListWindow = TExportArchivesListWindow}}
class TExportArchivesListWindow : public TGroupListWindow
{
public:
	TExportArchivesListWindow(TWindow* parent, int resourceId);

	TLawsuit *Lawsuit() { return (TLawsuit *) Group(); }
	int Count() { return GetVisualCount(); } 
	void Export();

protected:
	TLawsuit *LawsuitAt(int index) { return (TLawsuit *) GroupAt(index); }

	virtual TGroup *NewGroup() const;
	virtual bool AddGroup(TGroup *group);
	virtual bool DeleteGroup(TGroup *);
};    //{{TExportArchivesListWindow}}
