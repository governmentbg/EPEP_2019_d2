//{{TComboBox = TGetContainerAliasFace}}
class TGetContainerAliasFace : public TCharAliasFace
{
public:
	TGetContainerAliasFace(TWindow* parent, int resourceId, const char *name, const char *tKindsMax);

	void SetType(char type);

protected:
	const char *kindsMax;
	char kinds[SIZE_OF_KINDS];
};    //{{TGetContainerAliasFace}}
