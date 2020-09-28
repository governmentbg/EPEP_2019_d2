//{{TDialog = TOutRegSendBaseDialog}}
class TOutRegSendBaseDialog : public TTroubleDialog
{
public:
	TOutRegSendBaseDialog(TWindow* parent, TOutReg *outReg, TInReg *tInReg, const char *kinds, TConnect **tConnects,
		bool edit, int resId);

protected:
	TCharAliasFace *kind;
	TLongFace *no;
	TDateFace *date;
	TStringFace *text;
	TCharAliasFace *lawsuitType; 
	TLongFace *lawsuitNo;
	TYearFace *lawsuitYear;
	TButton *print;			// 2007:256 LRQ: to reprint outreg
	TCharAliasFace *college;	// 2008:072 LRQ: enable if !lawsuit
	TStatic *vssIndexLabel;
	TVSSIndexAliasFace *vssIndex;
	TInReg *inReg;
	TStringAliasFace *receiver;
	// 2015:161 for vss indexes
	TConnect **connects;
	bool initialized;
	// 2017:243
	TCheckBox *buttons[IDC_POSTAL_INDEX_COUNT];
	TCheckFace *deliveryNotice;

	DEFINE_GROUP_TYPE(TOutReg, OutReg)
	virtual bool IsValid();

//{{TOutRegSendBaseDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TOutRegSendBaseDialogVIRTUAL_END}}

//{{TOutRegSendBaseDialogRSP_TBL_BEGIN}}
	void DateENUpdate();
	void PostalBNClicked();
//{{TOutRegSendBaseDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TOutRegSendBaseDialog);
};    //{{TOutRegSendBaseDialog}}
