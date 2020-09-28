//{{TListWindow = TConnectListWindow}}
class TConnectListWindow : public TGroupListWindow
{
public:
	TConnectListWindow(TWindow* parent, int resourceId, TGArray<TConnect> *array, const char *tPredefined,
		TEisNmrFace *tEisppNmr);

	const char *predefined;
	bool dirty;
	TEisNmrFace *eispp_nmr;		// 2012:296 LPR: +related: xferKind

	void SetIndocKind(char tIndocKind);		// indoc & decision kinds
	void AdjustJurisd();			// 2010:165 LPR/IRQ: FIX
	void SetLawKind(char tLawKind);
	char MainType();
	bool IsValid();

	TGArray<TConnect> *Connects() { return (TGArray<TConnect> *) Array(); }
	void ResetConnects(TGArray<TConnect> *connects);
	void UpdateConnects(TGArray<TConnect> &connects);
	bool MayEdit();
	bool MayDelete();

protected:
	char indocKind;
	char lawKind;

	virtual TGroup *NewGroup() const;
	virtual bool AddGroup(TGroup *group);
	virtual bool EditGroup(TGroup *group);
	virtual bool DeleteGroup(TGroup *);		// memory only -> true

//{{TConnectListWindowRSP_TBL_BEGIN}}
	void EvLButtonDblClk(uint modKeys, TPoint& point);
//{{TConnectListWindowRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TConnectListWindow);
};    //{{TConnectListWindow}}
