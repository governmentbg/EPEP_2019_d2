//{{TDialog = TSubpoenaVariantDialog}}
class TSubpoenaVariantDialog : public TPostalDialog
{
public:
	TSubpoenaVariantDialog(TWindow *parent, TSubpoenaVariantGroup *variant, const char *tSubpoenaKinds, bool tAllowLinks);

protected:
	const char *subpoenaKinds;
	bool allowLinks;
	TGArray<TLinkWind> links;	// selection buffer
	const TLawsuit *lawsuit;	// 2016:243 for e-portal

	TSelectLinkedWindsListBox *singleSelect;
	TSelectLinkedWindsListBox *multpleSelect;
	TSelectLinkedWindsListBox *linkedWinds;	// current
	TStatic *subpoenaKindLabel;
	TCharListFace *subpoenaKind;
	TStatic *info;
	TStatic *linksLabel;
	// 2017:058 +related
	TStatic *subpoenaDateLabel;
	TDateFace *subpoenaDate;
	// 2017:213 +related
	TStatic *validDateLabel;
	TDateFace *validDate;
	bool validDateActive;

	const char *SubpoenaDateName();

	DEFINE_GROUP_TYPE(TSubpoenaVariantGroup, Variant)
	virtual bool IsValid();

//{{TSubpoenaKindDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TSubpoenaKindDialogVIRTUAL_END}}

//{{TSubpoenaVariantDialogRSP_TBL_BEGIN}}
	void SubpoenaKindLBNSelChange();
	void SubpoenaDateENUpdate();
	void OKBNClicked();
//{{TSubpoenaVariantDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TSubpoenaVariantDialog);
};    //{{TSubpoenaVariantDialog}}
