//{{TListWindow = TSurroundmentListWindow}}
class TSurroundmentListWindow : public TGroupListWindow
{
public:
	TSurroundmentListWindow(TWindow *parent, int resourceId, TSession *tSession, TDecision *tDecision,
		TGArray<TSurroundment> *surroundments);

	void Print();

protected:
	TSession *session;
	TDecision *decision;

	virtual bool EditGroup(TGroup *group);
	TGroup *NewGroup() const;
};    //{{TSurroundmentListWindow}}
