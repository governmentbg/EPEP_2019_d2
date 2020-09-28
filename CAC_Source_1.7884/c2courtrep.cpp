#include "cac.h"

#if COURT_TYPE == COURT_APPEAL
#include "pccourtrep.cpp"
#endif  // APPEAL

#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
#include "accourtrep.cpp"
#endif  // AREAL || MILITARY
