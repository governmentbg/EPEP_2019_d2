class TCombineCorruptListWindow : public TGroupListWindow
{
public:
	TCombineCorruptListWindow(TWindow* parent, int resourceId, TCombineCorruptGroup *tGroup);

	const TCombineCorruptGroup *Group() { return group; }

protected:
	TCombineCorruptGroup *group;

	virtual TGroup *NewGroup() const;
	virtual bool AddGroup(TGroup *tGroup);
};
