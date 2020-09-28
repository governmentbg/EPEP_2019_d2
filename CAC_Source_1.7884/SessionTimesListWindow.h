//{{TListWindow = TSessionTimesListWindow}}
class TSessionTimesListWindow : public TGroupListWindow
{
public:
	TSessionTimesListWindow(TWindow* parent, int resourceId, TGSArray<TSessionTimes> *times, bool *tEdited);

	bool IsValid(mstr &m);

protected:
	const TSession *session;
	bool *edited;

	virtual TGroup *NewGroup() const;
	virtual bool EditGroup(TGroup *group);
};    //{{TSessionTimesListWindow}}
