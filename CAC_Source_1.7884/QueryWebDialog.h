//{{TDialog = TQueryWebDialog}}
class TQueryWebDialog : public TPeriodDialog
{
public:
	TQueryWebDialog(TWindow* parent, TQueryWebGroup *webGroup, int resId = IDD_QUERY_WEB);

protected:
	TCharListFace *kinds;
	TRadioFace *orderByTYN;
	TRadioFace *orderByDTYN;
	TStringFace *name;
	TCheckFace *filtered;
	TCheckFace *attached;
	TCheckFace *text;
	TCheckFace *limited;
	TRadioFace *forced;
	TCheckFace *important;		// 2010:119 +related
	TCheckFace *appeal;
	TRadioFace *acquit;
	TRadioFace *reinvest;
	// 2018-11-09
	TRadioFace *protocol;
	TStatic *minDateLabel;
	TStatic *maxDateLabel;

	enum
	{
		CB_IMPORTANT,
		CB_FILTERED,
		CB_ATTACHED,
		CB_TEXT,
		CB_APPEAL,
		CB_LIMITED,
		CB_COUNT
	};

	DEFINE_GROUP_TYPE(TQueryWebGroup, WebGroup)
	virtual bool IsValid();

//{{TQueryWebDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TQueryWebDialogVIRTUAL_END}}

//{{TQueryWebDialogRSP_TBL_BEGIN}}
	void VariantBNClicked();
//{{TQueryWebDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TQueryWebDialog);
};    //{{TQueryWebDialog}}
