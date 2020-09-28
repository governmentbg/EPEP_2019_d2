//{{TListBox = TStringListBox}}
class TStringListBox : public TGroupListBox
{
public:
	TStringListBox(TWindow* parent, int resourceId);
	virtual ~TStringListBox();

	virtual int AddString(const char far* str);
	virtual int InsertString(const char far *str, int index);
	virtual int DeleteString(int index);

protected:
	virtual TGroup *NewGroup() const;
};    //{{TStringListBox}}
