//{{TDialog = TEditArchiveIndexDialog}}
class TEditArchiveIndexDialog : public TEditDialog
{
public:
	TEditArchiveIndexDialog(TWindow* parent, TArchiveIndex *tArchiveIndex, bool edit, int resId = IDD_EDIT_ARCHIVE_INDEX);

protected:
	TStringFace *archiveIndex;

//{{TEditArchiveIndexDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEditArchiveIndexDialogVIRTUAL_END}}
};    //{{TEditArchiveIndexDialog}}
