//{{TMigrateSendersListBox Implementation}}

TMigrateSendersListBox::TMigrateSendersListBox(TWindow* parent, int resourceId, char ucnType)
:
	TSenderListBox(parent, resourceId, C2S[ucnType])
{
}
