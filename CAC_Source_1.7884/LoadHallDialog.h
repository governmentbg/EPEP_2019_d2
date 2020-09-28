//{{TDialog = TLoadHallDialog}}
class TLoadHallDialog : public TGroupDialog
{
public:
	TLoadHallDialog(TWindow* parent, TLoadHall *loadHall, int resId);

protected:
	struct TLoadHallSet
	{
		TCharAliasFace *college;
		TLongAliasFace *composition;
		TCheckFace *reserved;
		TStringFace *text;
		TLoadData *origin;
	} bef, aft;

	TEdit *hall;	// 2011:353 LPR: to avoid cache

	DEFINE_GROUP_TYPE(TLoadHall, LoadHall)
	void ReservedBNClicked(TLoadHallSet *hs);
	void CollegeCBNEvent(TLoadHallSet *hs);

//{{TJudicialActDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TJudicialActDialogVIRTUAL_END}}

//{{TLoadHallDialogRSP_TBL_BEGIN}}
	void BefReservedBNClicked();
	void AftReservedBNClicked();
	void BefCollegeCBNSelChange();
	void AftCollegeCBNSelChange();
//{{TLoadHallDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TLoadHallDialog);
};    //{{TLoadHallDialog}}
