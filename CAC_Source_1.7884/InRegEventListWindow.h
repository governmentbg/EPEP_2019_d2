//{{TListWindow = TInRegEventListWindow}}
class TInRegEventListWindow : public TGroupListWindow
{
public:
	TInRegEventListWindow(TWindow* parent, int resourceId, TOutReg *tOutReg, TInReg *tInReg, bool tSave);

	void GetEvents(TGArray<TInRegEvent> *events);
	const TGArray<TInRegEvent> *Events() { return (const TGArray<TInRegEvent> *) array; }

protected:
	TOutReg *outReg;
	TInReg *inReg;
	bool save;

	virtual TGroup *NewGroup() const;
	virtual void Criteria(msql &m, TGroup *group);

	virtual bool AddGroup(TGroup *group);
	virtual bool DeleteGroup(TGroup *group);
};    //{{TInRegEventListWindow}}
