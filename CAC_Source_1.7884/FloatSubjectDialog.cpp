#include "cac.h"

DEFINE_RESPONSE_TABLE1(TFloatSubjectDialog, TFloatDialog)
//{{TFloatSubjectDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDC_FLOAT_SUBJECTS_PUSH_FULL, PushFullBNClicked),
	EV_BN_CLICKED(IDC_FLOAT_SUBJECTS_CHECK_OLD, CheckOldBNClicked),
//{{TFloatSubjectDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TFloatSubjectDialog Implementation}}

TFloatSubjectDialog::TFloatSubjectDialog(TWindow* parent, TFloatGroup *group, int resId)
	: TFloatDialog(parent, group, resId)
{
	pushFull = new TPushButton(this, IDC_FLOAT_SUBJECTS_PUSH_FULL);
	checkOld = new TCheckBox(this, IDC_FLOAT_SUBJECTS_CHECK_OLD);
	subjectsNew = new TSubjectListBox(this, IDC_FLOAT_SUBJECTS_NEW);
	subjectsOld = new TOldSubjectListBox(this, IDC_FLOAT_SUBJECTS_OLD);
}

void TFloatSubjectDialog::GetSubjects(TGArray<TSubject> &newSubjects, TGArray<TOldSubject> &oldSubjects)
{
	if (checkOld->GetCheck() == BF_CHECKED)
	{
		subjectsOld->GetMark(&oldSubjects);
		newSubjects.Flush();
	}
	else
	{
		oldSubjects.Flush();
		subjectsNew->GetMark(&newSubjects);
	}
}

void TFloatSubjectDialog::PushFullBNClicked()
{
	if (checkOld->GetCheck() == BF_CHECKED)
	{
		TGArray<TOldSubject> oldSubjects;
		subjectsOld->GetMark(&oldSubjects);

		if (select_old_subjects(this, &oldSubjects, subjectsOld->Kinds()))
			subjectsOld->SetMark(&oldSubjects);
	}
	else
	{
		TGArray<TSubject> subjects;
		subjectsNew->GetMark(&subjects);

		if (select_subjects(this, &subjects, subjectsNew->Kinds()))
			subjectsNew->SetMark(&subjects);
	}
}

void TFloatSubjectDialog::CheckOldBNClicked()
{
	bool tCheckOld = checkOld->GetCheck() == BF_CHECKED;

	subjectsNew->ShowWindow(tCheckOld ? SW_HIDE : SW_SHOW);
	subjectsNew->EnableWindow(!tCheckOld);
	subjectsOld->EnableWindow(tCheckOld);
	subjectsOld->ShowWindow(tCheckOld ? SW_SHOW : SW_HIDE);
}