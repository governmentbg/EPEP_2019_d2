DEFINE_RESPONSE_TABLE1(TGetEquateCriteriaDialog, TGroupDialog)
//{{TGetEquateCriteriaDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TGetEquateCriteriaDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TGetEquateCriteriaDialog Implementation}}

TGetEquateCriteriaDialog::TGetEquateCriteriaDialog(TWindow* parent, TEquateGroup *group, int resId)
:
	TGroupDialog(parent, group, resId)
{
	kinds = new TCharListFace(this, IDC_GET_EQUATE_CRITERIA_KINDS, "F_KINDS", Kind, KIND_EQUATE_LAWS, SIZE_OF_LAW_KINDS);
}

void TGetEquateCriteriaDialog::OKBNClicked()
{
	if (*kinds->S() || ask("Всички видове дела?"))
	{
		if (!*kinds->S())
			kinds->SetS(KIND_EQUATE_LAWS);
		*Group() << this;
		CmOk();
	}
}
