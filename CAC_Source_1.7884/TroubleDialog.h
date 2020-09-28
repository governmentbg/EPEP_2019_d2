//{{TDialog = TTroubleDialog}}
class TTroubleDialog : public TGroupDialog
{
public:
	TTroubleDialog(TWindow* parent, TRCDKeyContainer *container, long tEdit, int type, int resId);

protected:
	enum { ATTACH_RIGHT = 0x02 };

	bool edit;
	TElectricTrouble trouble;
	CDate *electricDate;

	// 2009:198 or earlier; 2016:167 moved here from judicial act
	TCheckFace *publInet;
	// 2016:166..2016:169
	TCheckFace *publTreg;
	bool allowPublInet;
	bool checkPublInet;

	void PostMove();
	void SetPortreg(bool portreg) { trouble.SetPortreg(Container(), *electricDate, portreg, edit); }
	void SetupPublic(bool publres, bool portreg, const TLawsuit *lawsuit);
	void SetupPublic(bool portreg);

	DEFINE_GROUP_TYPE(TRCDKeyContainer, Container)
	virtual bool IsValid();

//{{TTroubleDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TTroubleDialogVIRTUAL_END}}

//{{TTroubleDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
	void AttachBNClicked();
	void SwapBNClicked();
	void PublInetBNClicked();
	void PublTregBNClicked();
//{{TTroubleDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TTroubleDialog);
};    //{{TTroubleDialog}}
