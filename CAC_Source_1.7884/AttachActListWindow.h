//{{TListWindow = TAttachActListWindow}}
class TAttachActListWindow : public TGroupListWindow
{
public:
	TAttachActListWindow(TWindow* parent, int resourceId, TGArray<TIndexContainer> *array);

	void Attach();

protected:
	virtual TGroup *NewGroup() const { return NULL; }
};    //{{TAttachActListWindow}}
