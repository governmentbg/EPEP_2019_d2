//{{TListBox = TNumapListFace}}
class TNumapListFace : public TCharArrayListFace
{
public:
	TNumapListFace(TWindow* parent, int resourceId, const char *name, int size);

	void Advanced(bool enable);

protected:
	virtual const char *S();
	virtual void SetS(const char *ts);

	virtual void Refresh();
};    //{{TNumapListFace}}
