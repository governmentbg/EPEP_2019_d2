#include "cac.h"

//{{TUnwrittenDialog Implementation}}

TUnwrittenDialog::TUnwrittenDialog(TWindow* parent, TUnwrittenGroup *group, const char *tWhat, int resId)
:
	TFloatSubjectDialog(parent, group, resId), what(tWhat)
{
	judges = new TUserListBox(this, IDC_UNWRITTEN_JUDGES, POST_VISIBLE_JUDGES);
	date = new TDateFace(this, IDC_UNWRITTEN_DATE, "F_DATE");
}

void TUnwrittenDialog::Action(bool execute)
{
	msql m;

	TFloatSubjectDialog::GetSubjects(UnwrittenGroup()->newSubjects, UnwrittenGroup()->oldSubjects);
	judges->GetMark(&UnwrittenGroup()->judges);
	UnwrittenGroup()->FormCriteria(m);

	if (execute)
	{
		mstr title(what);
		UnwrittenGroup()->Title(title);
		Query(str(m), str(title));
	}
	else
		MessageEx(m);
}
