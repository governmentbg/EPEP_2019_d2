DEFINE_RESPONSE_TABLE1(TSubjectRealityTypesDialog, TGroupDialog)
//{{TSubjectRealityTypesDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TSubjectRealityTypesDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TSubjectRealityTypesDialog Implementation}}

TSubjectRealityTypesDialog::TSubjectRealityTypesDialog(TWindow* parent, TSubject *subject, int resId)
:
	TGroupDialog(parent, subject, resId), charReasonType(ReasonType, TCharRealityType::CloneReason),
		charPenalityType(PenalityType, TCharRealityType::ClonePenality)
{
	new TCharListFace(this, IDC_SUBJECT_REALITY_TYPES_REASONS, subject->FNI("F_REASONS"), &charReasonType, NULL,
		SIZE_OF_REALITIES);
	new TCharListFace(this, IDC_SUBJECT_REALITY_TYPES_PENALITIES, subject->FNI("F_PENALITIES"), &charPenalityType, NULL,
		SIZE_OF_REALITIES);
}

void TSubjectRealityTypesDialog::OKBNClicked()
{
	if (IsValid())
	{
		*Subject() << this;
		CmOk();
	}
}
