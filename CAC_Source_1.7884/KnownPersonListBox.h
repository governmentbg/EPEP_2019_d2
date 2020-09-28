//{{TListBox = TKnownPersonListBox}}
class TKnownPersonListBox : public TGroupListBox
{
public:
	TKnownPersonListBox(TWindow* parent, int resourceId, char tInvolvement);

protected:
	char involvement;

	virtual TGroup *NewGroup() const;
};    //{{TKnownPersonListBox}}
