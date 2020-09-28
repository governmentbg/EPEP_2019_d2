//{{TListBox = TSelectSurroundsListBox}}
class TSelectSurroundsListBox : public TGroupListBox
{
public:
	TSelectSurroundsListBox(TWindow* parent, int resourceId, TGArray<TSurround> *possibles);

	void SetMark(TGArray<TSurround> *surrounds);

protected:
	TSurround *SurroundAt(int index) { return (TSurround *) GroupAt(index); }
	virtual int MarkedsMax();

	virtual TGroup *NewGroup() const;
	virtual void GetGroupText(mstr &m, TGroup *group, int column);
};    //{{TSelectSurroundsListBox}}
