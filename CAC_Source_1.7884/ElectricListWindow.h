//{{TListWindow = TElectricListWindow}}
class TElectricListWindow : public TGroupListWindow, public TElectricCollect
{
public:
	TElectricListWindow(TElectricListDialog *parent, int resourceId, TRCDKeyContainer *container);
	virtual ~TElectricListWindow();

	TRCDKeyContainer *Container() { return (TRCDKeyContainer *) Group(); }
	TRCDKeyContainer *ElementAt(int index) { return (TRCDKeyContainer *) GroupAt(index); }
	void Signal(int sigtype, const TRCDKeyContainer *container, const void *data);

	void ElectricList();
	void ElectricBlue();
	void ElectricSave();
	void ElectricQuery();
	void ElectricEispp();

	void ElectraMenu(TRCDKeyContainer *container, const TElectra *electras, int x, int y, uint align);

protected:
	TRCDKeyContainer *primary;
	mstr cause;
	bool write;

	int sigmain;
	TGArray<TRCDKeyContainer> garbage;
	bool select;
	TRCDKeyContainer *renumber;

	bool Dispose(const TRCDKeyContainer *filter);
	bool UpdateFields(const TRCDKeyContainer *filter, const char *fields);
	void CheckAccess();

	void ElectraDevice(TPoint &point);
	void ElectraButton(TPoint &point, bool action);

	void Save(FILE *f, TPeriodGroup *period, const char *dir);
	virtual void AddItem(TRCDKeyContainer *container);

	virtual TGroup *NewGroup() const;
	virtual void Refresh();

//{{TElectricListWindowRSP_TBL_BEGIN}}
protected:
	void EvRButtonUp(uint modKeys, TPoint &point);
	void EvRButtonDown(uint modKeys, TPoint &point);
	void EvKeyUp(uint key, uint repeatCount, uint flags);
	void EvKeyDown(uint key, uint repeatCount, uint flags);
	void EvSysKeyUp(uint key, uint repeatCount, uint flags);
	void EvSysKeyDown(uint key, uint repeatCount, uint flags);
//{{TElectricListWindowRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TElectricListWindow);
};    //{{TElectricListWindow}}
