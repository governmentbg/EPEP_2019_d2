//{{TListBox = TSelectExecListListBox}}
class TSelectExecListListBox : public TCloneListBox
{
public:
	TSelectExecListListBox(TWindow* parent, int resourceId, TGArray<TExecList> *execLists);

	bool CheckCollects();
};    //{{TSelectExecListListBox}}
