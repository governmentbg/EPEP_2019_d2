//{{TListWindow = TSurrouendocListWindow}}
class TSurrouendocListWindow : public TGroupListWindow
{
public:
	TSurrouendocListWindow(TWindow *parent, int resourceId, TSession *tSession, TGArray<TSurroundment> *surrouendocs,
		TLawsuit *tLawsuit);

	void Clear();
	void Appeal();

protected:
	TSession *session;
	TLawsuit *lawsuit;
	bool electricMap[SURROUNDMENTS_MAX + 1];

	virtual bool EditGroup(TGroup *group);
	TGroup *NewGroup() const;
};    //{{TSurrouendocListWindow}}
