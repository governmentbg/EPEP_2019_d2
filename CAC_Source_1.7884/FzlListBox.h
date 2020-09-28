//{{TListBox = TFzlListBox}}
class TFzlListBox : public TGroupListBox
{
public:
	TFzlListBox(TWindow* parent, int resourceId, TGroupArray *array);

protected:
	virtual TGroup *NewGroup() const { return new FZL; }
	virtual void GetGroupText(mstr &m, TGroup *group, int);
};    //{{TFzlListBox}}
