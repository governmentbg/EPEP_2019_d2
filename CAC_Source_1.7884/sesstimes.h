void edit_session_prices(TWindow *parent);
bool check_session_times(const TTimeFace *start, const TTimeFace *entime, int oldCount);
bool edit_session_times(TWindow *parent, TGSArray<TSessionTimes> &times);
const char *session_times_complect(bool optional, const CTime &startime, const CTime &entime);
const char *session_times_invalid(bool internal, const CDate &stdate, const CTime &startime, const CTime &entime);
const char *session_times_invalid(bool internal, const CDate &stdate, const CTime &startime, const CTime &entime, int hours);
bool session_times_valid(TGSArray<TSessionTimes> *times, bool reportErr);
bool session_times_valid(TGSArray<TSessionTimes> *times, mstr &m);
void recalc_session_hours(TTimeFace *tStarTime, TTimeFace *tEnTime, TLongFace *hours);
