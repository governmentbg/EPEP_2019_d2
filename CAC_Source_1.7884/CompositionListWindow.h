//{{TListWindow = TCompositionListWindow}}
class TCompositionListWindow : public TGroupListWindow
{
public:
	TCompositionListWindow(TWindow* parent, int resourceId);

	void SetCollege(char tCollege);
	TFineComposition *Composition() { return (TFineComposition *) Group(); }

	void Print();
	bool Active();

protected:
	char college;

	virtual TGroup *NewGroup() const;
	virtual bool EditGroup(TGroup *group);
};    //{{TCompositionListWindow}}
