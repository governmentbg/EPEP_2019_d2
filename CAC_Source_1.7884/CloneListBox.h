//{{TListBox = TCloneListBox}}
class TCloneListBox : public TGroupListBox
{
public:
	TCloneListBox(TWindow* parent, int resourceId, TGroupArray *array, TGroup *(*tClone)());

protected:
	TGroup *(*clone)();

	virtual TGroup *NewGroup() const;
};    //{{TCloneListBox}}
