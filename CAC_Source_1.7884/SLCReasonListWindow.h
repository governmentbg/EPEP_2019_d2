//{{TListWindow = TSLCReasonListWindow}}
class TSLCReasonListWindow : public TGroupListWindow
{
public:
	TSLCReasonListWindow(TWindow* parent, int resourceId);

	void SetSLCType(char tSLCType);
	void Print();

protected:
	char slcType;

	virtual bool AddGroup(TGroup *group);
	virtual bool EditGroup(TGroup *group);
	virtual bool AskDelete(TGroup *group);

	virtual TGroup *NewGroup() const;
	virtual void Criteria(msql &m, TGroup *group);
};    //{{TSLCReasonListWindow}}
