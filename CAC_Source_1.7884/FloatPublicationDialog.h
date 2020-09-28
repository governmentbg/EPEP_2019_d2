//{{TDialog = TFloatDialog}}
class TFloatPublicationDialog : public TGroupDialog
{
public:
	TFloatPublicationDialog(TWindow* parent, TFloatPublicationGroup *group, int resId = IDD_FLOAT_PUBLICATION);

protected:
	TCharAutoListFace *types;
	TDateFace *minDate;
	TDateFace *maxDate;
	TStringFace *fileName;

	DEFINE_GROUP_TYPE(TFloatPublicationGroup, FloatPublicationGroup)
	virtual bool IsValid();

//{{TFloatDialogRSP_TBL_BEGIN}}
	void SaveBNClicked();
//{{TFloatDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TFloatPublicationDialog);
};  //{{TFloatDialog}}
