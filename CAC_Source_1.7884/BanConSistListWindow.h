//{{TListWindow = TBanConSistListWindow}}
class TBanConSistListWindow : public TDoubleListWindow
{
public:
	TBanConSistListWindow(TWindow* parent, int resourceId, bool tActive);

	void Add();
	void Edit();
	void Delete();
	void Print() { print_banconsist(this, &banConSists); }

protected:
	bool active;
	bool dirty;		// reload cache
	TGSArray<TBanConSist> banConSists;

	void Add(TGroupListColumn *column) { TDoubleListWindow::Add(column); }
	virtual void DrawSecond(TListWindItem &lwi, TGroup *group);

//{{TBanConSistListWindowVIRTUAL_BEGIN}}
	virtual void SetupWindow();
	virtual void CleanupWindow();
//{{TBanConSistListWindowVIRTUAL_END}}

//{{TBanConSistListWindowRSP_TBL_BEGIN}}
	void EvLButtonDblClk(uint modKeys, TPoint& point);
//{{TBanConSistListWindowRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TBanConSistListWindow);
};    //{{TBanConSistListWindow}}
