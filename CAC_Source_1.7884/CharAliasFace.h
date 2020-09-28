//{{TComboBox = TCharAliasFace}}
class TCharAliasFace : public TAliasFace
{
public:
	TCharAliasFace(TWindow* parent, int resourceId, const char *name, TCharAliasGroup *alias, const char *xCrit,
		long flags = FLAG_NULL);

	char C();
	void SetC(char c);

protected:
	char CharAt(int index) { return ((TCharAliasGroup *) GroupAt(index))->ValueField()->C(); }
	virtual void Refresh();
};    //{{TCharAliasFace}}
