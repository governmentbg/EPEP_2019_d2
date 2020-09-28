void float_announce(TWindow *parent);	// 2010:039

enum
{
	ORDER_BY_DKYN = ORDER_BY_USER,
	ORDER_BY_SDKYN,
	ORDER_BY_NMKYN,
	ORDER_BY_SDTYN,
	ORDER_BY_NMTYN
};

void load_announces_criteria(msql &m, const TGroup *group, TGroupArray &deliverers);
void load_announces(const TGroup *group, TGroupArray &announces, TGroupArray &deliverers);
extern const offield floatano_fields[];
