//{{TDialog = TAlterUXNDialog}}
class TAlterUXNDialog : public TQuickDialog
{
public:
	TAlterUXNDialog(TWindow* parent, TUCNGroup *tSource, TUCNGroup *tTarget, const char *sources, const char *targets,
		int resId);

protected:
	TUCNGroup *source;
	TUCNGroup *target;

	TCharAliasFace *sourceType;
	TUCNFace *sourceCode;
	TCharAliasFace *targetType;
	TUCNFace *targetCode;

	virtual bool IsValid();
};    //{{TAlterUXNDialog}}
