//{{TDialog = TQueryArchiveDialog}}
class TQueryArchiveDialog : public TGroupDialog
{
public:
	TQueryArchiveDialog(TWindow* parent, int resId);

protected:
	TArchive archive;
	char types[SIZE_OF_TYPES];
	TLongFace *no;
	TLongFace *year;

//{{TQueryArchiveDialogVIRTUAL_BEGIN}}
	virtual bool IsValid();
//{{TQueryArchiveDialogVIRTUAL_END}}

//{{TQueryArchiveDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
//{{TQueryArchiveDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TQueryArchiveDialog);
};    //{{TQueryArchiveDialog}}
