class TChooseRelationDialog : public TScaleDialog
{
public:
	TChooseRelationDialog(TWindow *parent, TSideWind *tOrdinary, TSideWind *tIndirect, int resId = IDD_CHOOSE_RELATION);

protected:
	TSideWind *ordinary;
	TSideWind *indirect;

	TButton *consentOfIndirect;
	TButton *indirectAsAny;
	TButton *indirectAsAny1;
	TButton *indirectAsAny2;	// 20080221
	TButton *indirectAsAny3;	// 20080221
	TButton *normalByIndirect;	// 2016:216 +related
	TButton *asWithInvolvement;	// 2016:216 +related

//{{TChooseRelationDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TChooseRelationDialogVIRTUAL_END}}

//{{TChooseRelationDialogRSP_TBL_BEGIN}}
	void IndirectAsAnyBNClicked();
	void IndirectAsAnyBNClicked_1();
	void IndirectAsAnyBNClicked_2();
	void IndirectAsAnyBNClicked_3();
	void ConsentOfIndirectBNClicked();
	void NormalByIndirectBNClicked();
	void AsWithInvolvementBNClicked();
//{{TChooseRelationDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TChooseRelationDialog);
};    //{{TChooeRelationDialog}}
