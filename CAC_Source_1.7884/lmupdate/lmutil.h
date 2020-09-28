const char *basename(const char *name);
bool exists(const char *name, bool file = true);
const char *invalid_port(const char *port);
extern constant ibase_sql_dirs[], pgres_sql_dirs[];
void shorten_and_check(const char *fullname, char *shortname);
bool find_sql_executable(const char *basename, constant *search_dirs, char *fullname);
void lm_combine_scripts(const char *output_name, constant *script_names, const char *interbase_db);
