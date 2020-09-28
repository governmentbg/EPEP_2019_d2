// initialize tmp before using tempdir
extern constant TEMPDIR;
extern const char *tempdir;
void initialize_tmp();
// 2015:287 mostly from office -> generic
void add_unlink_file(const char *base, bool wordFile, bool unreadOnly);
void shutdown_tmp();
