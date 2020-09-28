//{{TDialog = TGetGroupDialog}}
class TGetGroupDialog : protected TGetTypesKinds, public TGroupDialog
{
public:
	TGetGroupDialog(TWindow* parent, TRCDKeyGroup *group, TRCDKeyGroup **tGrouptr, const char *tTypes, const char *tKinds, const char *tStatuses, match_func tMatch = match_null, int getWhat = 0, int resId = 0);

protected:
	TRCDKeyGroup **grouptr;
	const char *statuses;
	match_func match;

	TStatic *typeLabel;
	TCharAliasFace *type;
	TLongFace *no;
	TLongFace *year;
	TStatic *kindLabel;
	TGetContainerAliasFace *kind;
	TStatic *dateLabel;
	TDateFace *date;
	TStatic *timeLabel;
	TTimeFace *time;
	TPushButton *query;	// en/disable if type empty

	// 2009:136 LPR: repeater
	static TRCDKey last_key;
	static int last_func;

	DEFINE_GROUP_TYPE(TRCDKeyGroup, Group)
	bool IsValidBasic();
	virtual bool IsValid();
	TRCDKeyGroup *OKLowLevel(const char tKind, TRCDKeyGroup *tGroup);

//{{TGetGroupDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TGetGroupDialogVIRTUAL_END}}

//{{TGetGroupDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
	void QueryBNClicked();
	void TypeCBNSelChange();
	void KindCBNSelChange();
//{{TGetGroupDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TGetGroupDialog);
};    //{{TGetGroupDialog}}
