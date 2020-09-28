//{{TListBox = THallListBox}}
class THallListBox : public TAliasListBox
{
public:
	THallListBox(TWindow* parent, int resourceId, long flags);

	void GetMarks(TIntegerArray *selected, TIntegerArray *unselected);
};    //{{THallListBox}}
