bool bulstat_valid(const char *value);
bool connect_no_valid(char connectType, const char *value);
bool country_valid(const char *value);
bool date_valid(const char *value, sign mandatory);
bool eispp_nmr_valid(const char *value, char letter);
bool find_replace_valid(const char *find, const char *replace, bool complain);
bool firm_name_valid(const char *value);
bool fzl_name_valid(sign cyrillic, const char *value);
bool time_valid(const char *value, bool mandatory);
bool lnc_valid(const char *value);
bool long_set_valid(const char *value);
bool long_valid(const char *value);
bool index_valid(const char *value, const char *kinds);
bool money_valid(const char *value, int len);
bool trans_id_valid(const char *value, bool partial);
bool ucn_valid(char type, const char *value);
bool name_valid(const char *value);
bool year_valid(const char *value);
bool keywords_valid(const char *value);