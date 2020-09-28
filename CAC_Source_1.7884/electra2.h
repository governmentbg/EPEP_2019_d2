struct TElectra
{
	const char *name;
	int command;
	const char *types;
	const char *kinds;
	const char *statuses;
	match_func match;
	const char *menus;
};

extern const TElectra electra_globals[];
extern const TElectra electra_locals[];
// 2017:011
extern const TElectra electra_global_killeds[];
extern const TElectra electra_local_killeds[];

void electra_action(TWindow *parent, int command, TRCDKeyContainer *container);
