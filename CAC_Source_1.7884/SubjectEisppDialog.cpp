//{{TSubjectEisppDialog Implementation}}

TSubjectEisppDialog::TSubjectEisppDialog(TWindow* parent, TSubject *subject)
:
	TGroupDialog(parent, subject, IDD_SUBJECT_EISPP)
{
	vid = new TLongFace(this, IDC_SUBJECT_EISPP_VID, subject->FNI("F_VID") , SIZE_OF_SUBJECT_VID);
	new TLongSetFace(this, IDC_SUBJECT_EISPP_SBES, subject->FNI("F_SBES"), SIZE_OF_SUBJECT_SBES);
}

#if TESTVER
bool TSubjectEisppDialog::IsValid()
{
	bool isValid = TGroupDialog::IsValid();

	if (isValid)
	{
		if (vid->L() && !eispp_matches(Subject()->SaasD().kind, vid->L()))
			isValid = error("Подразбиращият се вид съдебно производство не съответства на вида на делото.");
	}

	return isValid;
}
#endif  // TESTVER
