//{{TDialog = TExportCSVDialog}}
class TExportCSVDialog : public TGroupDialog
{
public:
	TExportCSVDialog(TWindow* parent, TExportCSVGroup *group, int resId = IDD_EXPORT_CSV);

protected:
	TDateFace *minRequestDate;
	TDateFace *maxRequestDate;
	TCheckFace *exportRequests;
	TDateFace *minLawsuitDate;
	TDateFace *maxLawsuitDate;
	TCheckFace *exportLawsuits;

	virtual bool IsValid();

//{{TExportCSVDialogRSP_TBL_BEGIN}}
	void OKBNEnable(TCommandEnabler &tce);
//{{TExportCSVDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TExportCSVDialog);
};    //{{TExportCSVDialog}}
