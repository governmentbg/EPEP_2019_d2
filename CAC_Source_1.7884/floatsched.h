// ----- TExpireSchedGroup ---------------------------------------------------
class TExpireSchedGroup : public TDummyGroup
{
public:
	TExpireSchedGroup(const char *colleges);

	char types[SIZE_OF_TYPES];
	CDate date;
	TGSArray<TUserAliasGroup> judges;
};

// ----- query ---------------------------------------------------------------
void query_sched(TWindow *parent, TExpireSchedGroup *group);
void expire_sched(TWindow *parent);
void float_sched(TWindow *parent);
