#ifdef __GNUC__
bool message(unsigned flags, const char *format, ...) __attribute__ ((format(printf, 2, 3)));
bool message(const char *format, ...) __attribute__ ((format(printf, 1, 2)));

bool ask(const char *format, ...) __attribute__ ((format(printf, 1, 2)));
bool error(const char *format, ...) __attribute__ ((format(printf, 1, 2)));
void info(const char *format, ...) __attribute__ ((format(printf, 1, 2)));
bool warn(const char *format, ...) __attribute__ ((format(printf, 1, 2)));
void fatal(const char *format, ...) __attribute__ ((format(printf, 1, 2)));
#endif  // __GNUC__

bool message(unsigned flags, const char *format, ...);
bool message(const char *format, ...);

bool ask(const char *format, ...);
bool error(const char *format, ...);
void info(const char *format, ...);
bool warn(const char *format, ...);
void fatal(const char *format, ...);
