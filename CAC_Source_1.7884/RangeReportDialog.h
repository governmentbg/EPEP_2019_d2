//{{TDialog = TRangeReportDialog}}
class TRangeReportDialog : public TGroupDialog
{
public:
	TRangeReportDialog(TWindow* parent, TRangeReportGroup *group, int resId = IDD_RANGE_REPORT);

protected:
	TLongAliasFace *minMonth;
	TYearFace *minYear;
	TLongAliasFace *maxMonth;
	TYearFace *maxYear;
	TRadioFace *instance1st;
	TRadioFace *instance2nd;
	TRadioFace *instanceAll;
	TCharListFace *types;
	TCheckFace *builtin;

	DEFINE_GROUP_TYPE(TRangeReportGroup, RRGroup)
	virtual bool IsValid();

//{{TRangeReportDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TRangeReportDialogVIRTUAL_END}}

//{{TRangeReportDialogRSP_TBL_BEGIN}}
	void OKBNEnable(TCommandEnabler &tce);
	void OKBNClicked();
//{{TRangeReportDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TRangeReportDialog);
};    //{{TRangeReportDialog}}
