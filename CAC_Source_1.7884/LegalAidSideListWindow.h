//{{TListWindow = TLegalAidSideListWindow}}
class TLegalAidSideListWindow : public TTryAddressSideListWindow
{
public:
	TLegalAidSideListWindow(TWindow* parent, int resourceId, TGArray<TLegalSide> *laids);

	void Lawer(bool set);

protected:
	virtual void DrawSecond(TListWindItem &lwi, TGroup *group);
};    //{{TLegalAidSideListWindow}}
