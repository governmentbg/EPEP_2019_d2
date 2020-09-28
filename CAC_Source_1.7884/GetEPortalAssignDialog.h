//{{TDialog = TGetEPortalAssignDialog}}
class TGetEPortalAssignDialog : public TGetGroupDialog
{
public:
	TGetEPortalAssignDialog(TWindow* parent, TEPortAssign *tAssign, long tEPortalMode, const TSideWind *tSideWind);

protected:
	TEPortAssign *assign;
	long eportalMode;
	const TSideWind *sideWind;
	TLawsuit lawsuit;

	TCheckFace *eportalSubpoena;
	TCharAliasFace *involvement;

//{{TGetEPortalAssignDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TGetEPortalAssignDialogVIRTUAL_END}}

//{{TGetEPortalAssignDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
//{{TGetEPortalAssignDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TGetEPortalAssignDialog);
};    //{{TGetEPortalAssignDialog}}
