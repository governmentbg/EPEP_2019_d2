//{{TListWindow = TCadrListWindow}}
class TCadrListWindow : public TGroupListWindow
{
public:
	TCadrListWindow(TWindow* parent, int resourceId, const long &tCodeValue);

	virtual void Activate();
	bool InitialRefresh() { return initialRefresh; }

protected:
	const long &codeValue;

//{{TCadrListWindowRSP_TBL_BEGIN}}
protected:
	void EvKeyDown(uint key, uint repeatCount, uint flags);
//{{CadrTListWindowRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TCadrListWindow);
};    //{{TCadrListWindow}}
