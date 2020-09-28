//{{TListWindow = TUserListWindow}}
class TUserListWindow : public TGroupListWindow
{
public:
	TUserListWindow(TWindow* parent, int resourceId);

	void SetPost(char tPost);
	TUser *User() { return (TUser *) Group(); }

	void Copy();
	void Right();
	void Region();
	//void Assign();	// 2016:026 LPR/LRQ: +related: block distrib
	void List(long active);  // 2018-07-10: IRQ: +related

protected:
	char post;

	virtual TGroup *NewGroup() const;
	virtual void Criteria(msql &m, TGroup *group);
	virtual bool AddGroup(TGroup *group);
	virtual bool EditGroup(TGroup *group);

//{{TUserListWindowRSP_TBL_BEGIN}}
	void EvLButtonDblClk(uint modKeys, TPoint& point);
//{{TUserListWindowRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TUserListWindow);
};    //{{TUserListWindow}}
