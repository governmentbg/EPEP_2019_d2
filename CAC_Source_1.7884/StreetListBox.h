//{{TListBox = TStreetListBox}}
class TStreetListBox : public TGroupListBox
{
public:
	TStreetListBox(TWindow* parent, int resourceId, long tUCLP);

	TStreet *Street() { return (TStreet *) Group(); }

protected:
	long uclp;

	virtual TGroup *NewGroup() const;
};    //{{TStreetListBox}}
