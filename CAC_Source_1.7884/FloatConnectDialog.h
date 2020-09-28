//{{TDialog = TFloatConnectDialog}}
class TFloatConnectDialog : public TFloatDialog
{
public:
	TFloatConnectDialog(TWindow* parent, TFloatConnectGroup *group, int resId = IDD_FLOAT_CONNECT);

protected:
	TTypeListFace *types;
	TLongFace *minNo;
	TLongFace *maxNo;
	TYearFace *minYear;
	TYearFace *maxYear;
	TCharListFace *connectTypes;
	TStatic *sendersLabel;
	TSenderListBox *senders;
	TCharListFace *generalKinds;
	TCharListFace *connectKinds;
	TConnectNoFace *connectNo;
	TCheckFace *exactNo;
	TDateFace *minDate;
	TDateFace *maxDate;
	TYearFace *minConnectYear;
	TYearFace *maxConnectYear;
	TDateFace *minFinished;
	TDateFace *maxFinished;
	TCheckFace *queryActs;
	TCharListNameFace *results;
	TStringFace *text;

	virtual void SetupWindow();
	DEFINE_GROUP_TYPE(TFloatConnectGroup, FloatConnectGroup);
	DECLARE_RESPONSE_TABLE(TFloatConnectDialog);
	void BClicked();
	void BChange();
	void QueryActsBNClicked();

	virtual bool IsValid();
	virtual void Action(bool execute);
};    //{{TFloatConnectDialog}}
