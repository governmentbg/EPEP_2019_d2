//{{TListWindow = TExecListLinkListWindow}}
class TExecListLinkListWindow : public TTryAddressSideListWindow
{
public:
	TExecListLinkListWindow(TWindow* parent, int resourceId, TGArray<TLinkWind> *xLinks);

protected:
	virtual void DrawSecond(TListWindItem &lwi, TGroup *group);
};    //{{TExecListLinkListWindow}}
