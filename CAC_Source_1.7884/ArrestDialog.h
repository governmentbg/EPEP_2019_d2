//{{TDialog = TArrestDialog}}
class TArrestDialog : public TEditDialog
{
public:
	TArrestDialog(TWindow* parent, TLawsuit *lawsuit, TAccusation *accusation, bool edit, bool start);

protected:
	TCheckFace *old;
	TLongFace *accusationNo;
	TCharAliasFace *resolution;
	TDateFace *arrestDate;
	TStatic *dateLabel;
	TStatic *noLabel;
	TStatic *resolutionLabel;

	DEFINE_GROUP_TYPE(TAccusation, Accusation)

//{{TArrestDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TArrestDialogVIRTUAL_END}}

//{{TArrestDialogRSP_TBL_BEGIN}}
	void DeleteBNClicked();
	void DeleteBNEnable(TCommandEnabler &tce);
//{{TArrestDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TArrestDialog);
};    //{{TArrestDialog}}
