//{{TListBox = TMigrateSendersListBox}}
class TMigrateSendersListBox : public TSenderListBox
{
public:
	TMigrateSendersListBox(TWindow* parent, int resourceId, char ucnType);

	void RemoveGroup() { TSenderListBox::RemoveGroup(); }
};    //{{TMigrateSendersListBox}}
