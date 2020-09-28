//{{TListBox = TWhatListBox}}
class TWhatListBox : public TGroupListBox
{
public:
	TWhatListBox(TWindow* parent, int resourceId);

	void SetLog(bool tLog);
	void GetMark(TIntegerArray *selected);

protected:
	bool log;

	virtual TGroup *NewGroup() const;
	virtual void Criteria(msql &m, TGroup *group);
};    //{{TWhatListBox}}
