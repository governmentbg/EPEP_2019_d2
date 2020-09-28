//{{TEdit = TNameFace}}
class TNameFace : public TStringFace
{
public:
	TNameFace(TWindow* parent, int resourceId, const char *name, uint textLimit);

	const char *S();	// 2011:083 FIX: compress, for name searches
};    //{{TNameFace}}
