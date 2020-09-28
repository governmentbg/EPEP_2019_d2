//{{TDialog = TGetSessionResolutionDialog}}
class TGetSessionResolutionDialog : public TGetGroupDialog
{
public:
	TGetSessionResolutionDialog(TWindow* parent, TSession *session, const char *statuses, TRCDKeyContainer **tResolutionPtr,
		const char *tResolutions, const char *tLabel, bool tSessionSelected);

protected:
	TRCDKeyContainer **resolutionPtr;	// 2005:140 LRQ: unset text
	const char *resolutions;
	const char *label;
	bool sessionSelected;

	TGetContainerAliasFace *resolutionKind;
	TDateFace *resolutionDate;
	TIndexFace *resolutionIndex;

	virtual bool IsValid();

//{{TGetSessionResolutionDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TGetSessionResolutionDialogVIRTUAL_END}}

//{{TGetSessionResolutionDialogRSP_TBL_BEGIN}}
	void TypeCBNSelChange();
	void ResolutionKindCBNSelChange();
	void OKBNClicked();
//{{TGetSessionResolutionDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TGetSessionResolutionDialog);
};    //{{TGetSessionResolutionDialog}}
