//{{TJurorLoadListBox Implementation}}

TJurorLoadListBox::TJurorLoadListBox(TWindow* parent, int resourceId)
:
	TKnownPersonListBox(parent, resourceId, INVOLVEMENT_JUROR)
{
}

void TJurorLoadListBox::GetCopy(TGroupArray *tArray)
{
	tArray->Flush();

	for (int index = 0; index < array->Count(); index++)
		tArray->AddCopy(GroupAt(index));
}
