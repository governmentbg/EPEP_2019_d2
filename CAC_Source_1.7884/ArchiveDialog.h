//{{TDialog = TArchiveDialog}}
class TArchiveDialog : public TGroupDialog
{
public:
	TArchiveDialog(TWindow* parent, TArchive *archive, TExternContainer *tLawsuit, bool tEdit, int resId = IDD_ARCHIVE);

protected:
	bool edit;
	TDateFace *archiveDate;			// 2001:347 checks; 2005:342 disabling
	TLongFace *archiveNo;			// 2005:342 disabling
	// 2014:056 archive indexes
	TExternContainer *lawsuit;
	TLongFace *keep;
	TArchiveIndexAliasFace *archiveIndex;
	bool hasKeep;				// 2014:072 IRQ: always preserve keep 

	DEFINE_GROUP_TYPE(TArchive, Archive)
	virtual bool IsValid();

//{{TArchiveDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TArchiveDialogVIRTUAL_END}}

//{{TArchiveDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
	void ArchiveIndexCBNSelChange();
//{{TArchiveDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TArchiveDialog);
};    //{{TArchiveDialog}}
