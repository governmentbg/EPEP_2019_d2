//{{TDialog = TEditAnnounceDialog}}
class TEditAnnounceDialog : public TGroupDialog
{
public:
	TEditAnnounceDialog(TWindow* parent, TAnnounce *announce, bool tEdit, TGroupArray *tDeliverers, const char *kinds,
		bool tScan);

protected:
	bool edit;
	mstr courtUCN;
	TFixeiver fixeiver;
	TStringAliasFace *sender;
	TGroupArray *deliverers;
	TCharAliasFace *kind;
	TLongFace *no;
	TYearFace *year;
	TCharAliasFace *subpoenaKind;
	TDateFace *sessionDate;
	TLongFace *sideNo;
	TAreaAliasFace *area;
	TRegionAliasFace *region;
	TFindUCLPAliasFace *uclp;
	TFindStreetAliasFace *street;
	TPushButton *addStreet;
	TPushButton *editStreet;
	TStatic *noLabel;
	TStringFace *adrNo; 
	TStringFace *blockNo;			// 2010:035 IRQ: +related 
	TPersonAliasFace *deliverer;
	TStringFace *subpoenaText;
	TSubpoena *subpoena;
	TLongAliasFace *composition;		// 2010:048
	mstr cause;
	bool write;
	// 2012:069
	TStringFace *subNo;
	TStringFace *entrance;
	TStringFace *floor;
	TStringFace *apartment;
	// 2015:106
	sign scan;
	TDateFace *subpoenaDate;
	TStringFace *barcodeText;

	DEFINE_GROUP_TYPE(TAnnounce, Announce)
	virtual bool IsValid();
	void Assign();
	void SetComposition();

//{{TEditAnnounceDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEditAnnounceDialogVIRTUAL_END}}

//{{TEditAnnounceDialogRSP_TBL_BEGIN}}
	void AddStreetBNClicked();
	void EditStreetBNClicked();
	void StreetCBNDropdown();
	void SubpoenaKindCBNSelChange();
	void OKBNClicked();
	void StreetCBNSelChange();
	void AdrNoKillFocus();
	void BlockNoKillFocus();
	void UCLPCBNSelChange();
	void ConnectKindCBNSelChange();
	void NoKillFocus();
	void YearKillFocus();
	void KindCBNSelChange();
//{{TEditAnnounceDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TEditAnnounceDialog);
};    //{{TEditAnnounceDialog}}
