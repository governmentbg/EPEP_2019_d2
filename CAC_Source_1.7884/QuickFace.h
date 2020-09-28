//{{TCheckBox = TQuickFace}}
class TQuickFace : public TCheckFace
{
public:
	TQuickFace(TWindow* parent, int resourceId, const char *name, TGroupBox* group = 0);

	long L() const;
	void SetL(long l);

	void SetKind(char tKind);
	void AlterText();
};    //{{TQuickFace}}
