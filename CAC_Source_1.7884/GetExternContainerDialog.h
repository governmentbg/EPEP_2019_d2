//{{TDialog = TGetExternContainerDialog}}
class TGetExternContainerDialog : public TGetGroupDialog
{
public:
	TGetExternContainerDialog(TWindow* parent, TRCDKeyGroup **grouptr, match_func match, const TExternSession *tSuggest,
		bool tForceExcess = false, int getWhat = 0, int resId = IDD_GET_EXTERN_CONTAINER);

protected:
	const TExternSession *suggest;
	bool forceExcess;
	TCheckFace *external;
	TDummyContainer tGroup;
	TExternLawsuit lawsuit;
	bool getExternLawsuit;

	DEFINE_GROUP_TYPE(TExternContainer, Container)

//{{TGetExternContainerDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TGetExternContainerDialogVIRTUAL_END}}

//{{TGetExternContainerDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
	void ExternalBNClicked();
//{{TGetExternContainerDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TGetExternContainerDialog);
};    //{{TGetExternContainerDialog}}
