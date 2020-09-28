//{{TFloatRegisterActDialog Implementation}}

TFloatRegisterActDialog::TFloatRegisterActDialog(TWindow* parent, TFloatRegisterActGroup *group, int resId)
:
	TQueryRegisterActDialog(parent, group, resId)
{
	strcpy(regKinds, KIND_VISIBLE_ENDOCS);
	strcat(regKinds, KIND_ENDOC_RIDERS);
	kinds = new TCharListFace(this, IDC_FLOAT_REGISTER_ACT_KINDS, "F_KINDS", Kind, regKinds, SIZE_OF_KINDS);
}

void TFloatRegisterActDialog::Action(bool execute)
{
	msql m;

	judges->GetMark(&FloatRegisterActGroup()->judges);
	FloatRegisterActGroup()->FormCriteria(m);

	if (execute)
		TFloatRegisterActQuery(this, str(m)).Execute();
	else
		Message(m);
}
