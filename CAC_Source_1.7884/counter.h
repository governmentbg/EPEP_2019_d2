// ----- TCounterGroup -------------------------------------------------------
struct offield;

class TCounterGroup : public TDummyGroup
{
public:
	TCounterGroup(const char *tNone, const char *tOne, const char *tMany);
	void SetNames(const char *tNone, const char *tOne, const char *tMany);

	long count;
	long maximum;
	const char *none;
	const char *one;
	const char *many;

protected:
	TCounterGroup();
};
