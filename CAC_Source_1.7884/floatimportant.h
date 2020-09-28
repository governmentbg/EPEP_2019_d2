class TFloatImportantGroup : public TFloatGroup
{
public:
	TFloatImportantGroup();

	virtual void FormCriteria(msql &m);

	char types[SIZE_OF_TYPES];
	CDate start;
	CDate final;
};

void query_important(TWindow *parent, TFloatImportantGroup *group);
void float_important(TWindow *parent);
