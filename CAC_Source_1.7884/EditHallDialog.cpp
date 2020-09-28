//{{TEditHallDialog Implementation}}

TEditHallDialog::TEditHallDialog(TWindow* parent, THall *tHall, bool edit)
:
	TEditDialog(parent, tHall, edit, IDD_EDIT_HALL)
{
	hall = new TLongFace(this, IDC_EDIT_HALL_HALL, "F_HALL_LONG", SIZE_OF_HALL);
	new TStringFace(this, IDC_EDIT_HALL_NAME, "F_NAME", SIZE_OF_NAME);
	new TStringFace(this, IDC_EDIT_HALL_ALIAS, "F_ALIAS", SIZE_OF_HALL_NAME);
	new TStringFace(this, IDC_EDIT_HALL_ADDRESS, "F_ADDRESS", SIZE_OF_HALL_ADDRESS);
	new TCheckFace(this, IDC_EDIT_HALL_ACTIVE, "F_FLAGS_ACTIVE");
}

void TEditHallDialog::SetupWindow()
{
	TGroupDialog::SetupWindow();

	SetCaption(edit ? "Редактиране на зала" : "Добавяне на зала");
	hall->EnableWindow(!edit);
}
