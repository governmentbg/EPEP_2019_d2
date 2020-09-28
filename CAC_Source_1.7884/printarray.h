bool print_apfish(TLawsuit *lawsuit, TSideWind *sideWind);
void print_halls(TPeriodGroup *period);
void print_subjects(TGSArray<TSubject> *subjects, char lawKind);
void print_old_subjects(char lawKind);	// 2016:062
void print_trade_sides(TGArray<TBaseWind> *sideWinds);
void print_compositions(TGArray<TFineComposition> *compositions, char college);
void print_realticles(TLongAliasArray *realticles, const char *what);
void print_dangers(TGArray<TDanger> *dangers);
