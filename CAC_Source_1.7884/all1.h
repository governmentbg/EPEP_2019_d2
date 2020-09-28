#include "const1.h"
#include "utility1.h"
#include "BookInstanceDialog.h"

// ----------- BOOK -----------------
void appealed_book(TWindow *parent);
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
void execute_book(TWindow *parent);
void print_one_judgement (TWindow *parent, TJudgement *judgement);
void prove_book(TWindow *parent);
#endif  // APPEAL || AREAL || MILITARY || REGIONAL
#if COURT_TYPE == COURT_AREAL
void slow_book(TWindow *parent);
#endif  // AREAL
void indoc_book(TWindow *parent);
void request_book(TWindow *parent);
void archive_book(TWindow *parent);
#if COURT_TYPE == COURT_REGIONAL
void divorce_book(TWindow *parent);
#endif
void closed_session_book(TWindow * parent);
void abc_book(TWindow *parent);
void expense_book(TWindow *parent);
void inreg_book(TWindow *parent);
void time_book(TWindow *parent);
#if COURT_TYPE == COURT_AREAL
void print_book634(TWindow *parent, const CDate &minDate, const CDate &maxDate, char type, long noLaw, long yearLaw,
	long startNo);
#endif  // AREAL
// -----------FLOAT -----------------
void float_index(TWindow *parent);
void float_senders(TWindow * parent);
void float_publication(TWindow * parent);
void float_hall(TWindow * parent);
// ----------- PRINT -----------------
void print_zzdn();

// ----------- OTHERS -----------------
void session_list(TWindow *parent);
void first_set(TWindow *parent);
void delete_lawsuit(const char *expr, bool fail);
void subpoenas(CDate minDate, CDate maxDate, char kind);
void book_251(TWindow *parent);
void desc_book(TWindow *parent);
void decision_register(TWindow *parent);
void invDeliver_book(TWindow *parent);
void invSubpoena_book(TWindow *parent);
#include "InvBookAnno.h"
void invAnno_book(TWindow *parent);
void run_tests_func(TWindow *parent);
void print_street_map(const char *ucn);
void float_web(TWindow *parent);
void query_web_html(TWindow *parent);
void join_equal_streets(TWindow *parent);
