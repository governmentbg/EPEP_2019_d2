// ----- TFloatGroup ---------------------------------------------------------
enum
{
	ORDER_BY_NULL	= 0,
	ORDER_BY_TKYN	= 1,
	ORDER_BY_TYN	= 2,
	ORDER_BY_TSYN	= 3,
	ORDER_BY_KYN	= 4,
	ORDER_BY_JTYN	= 5,
	ORDER_BY_DTYN	= 6,
	ORDER_BY_COUNT	= 7,
	ORDER_BY_USER	= 20
};

class TFloatGroup : public TGroup
{
public:
	TFloatGroup(const char *name, const char *tFields = NULL);

	virtual void FormCriteria(msql &m) = 0;	// FloatCriteria() after TGroup rewrite

	const char *fields;
	long order;

protected:
	void BeginCriteria(msql &m) { m.Begin(Name, fields); }
	void EndCriteria(msql &m, const char *prefix = NULL);
};
