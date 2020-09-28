void discard_citizens(TWindow *parent);

struct TSubjectTKS
{
	char tork;
	long subject;

	bool operator<=(const TSubjectTKS &that);
};

void update_subjects(TWindow *parent);
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
void update_session_times(TWindow *parent);
#endif  // AREAL || MILITARY || REGIONAL
void update_mini(TWindow *parent);
#if COURT_TYPE == COURT_ADMIN
void export_csv(TWindow *parent);
#endif  // COURT_ADMIN
void update_intervals(TWindow *parent);
void update_requests(TWindow *parent);
void update_long_texts(TWindow *parent);
void update_log_kinds(TWindow *parent);
void update_eportals(TWindow *parent);
