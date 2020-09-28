//{{TDialog = TFirmLawsuitSideListDialog}}
class TFirmLawsuitSideListDialog : public TSideWindListDialog
{
public:
	TFirmLawsuitSideListDialog(TWindow* parent, TLawsuit *lawsuit, long sideMode);

protected:
	TButton *firm;

	DEFINE_SIDES_TYPE(TLawsuit, Lawsuit)

//{{TFirmLawsuitSideListDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
	virtual bool CanClose();
//{{TFirmLawsuitSideListDialogVIRTUAL_END}}

//{{TFirmLawsuitSideListDialogRSP_TBL_BEGIN}}
	void FirmBNClicked();
	void PrintBNClicked();
	void PrintBNEnable(TCommandEnabler &tce);
	void ListBNClicked();
	void CancelBNClicked();
	void EvClose();
//{{TFirmLawsuitSideListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TFirmLawsuitSideListDialog);
};    //{{TFirmLawsuitSideListDialog}}
