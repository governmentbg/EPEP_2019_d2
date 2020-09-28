//{{TDialog = TGetJurorDistribDialog}}
class TGetJurorDistribDialog : public TGetWhatExtern, public TQuickDialog
{
public:
	TGetJurorDistribDialog(TWindow* parent, TBaseSession **tSessptr, sign tEdit, const TExternSession *tSuggest,
		bool tForceExcess);

protected:
	TBaseSession **sessptr;
	const TExternSession *suggest;
	mstr kinds;

	TCharAliasFace *type;
	TLongFace *no;
	TYearFace *year;
	TCharAliasFace *kind;
	TDateFace *date;
	TStatic *timeLabel;
	TTimeFace *time;
	TCheckFace *external;

	bool External() { return external && external->GetCheck() == BF_CHECKED; }

	bool IsValidBasic();
	virtual bool IsValid();

//{{TGetJurorDistribDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TGetJurorDistribDialogVIRTUAL_END}}

//{{TGetJurorDistribDialogRSP_TBL_BEGIN}}
	void TypeCBNSelChange();
	void KindCBNSelChange();
	void OKBNClicked();
	void QueryBNEnable(TCommandEnabler &tce);
	void QueryBNClicked();
//{{TGetJurorDistribDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TGetJurorDistribDialog);
};    //{{TGetJurorDistribDialog}}
