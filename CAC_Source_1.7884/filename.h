char *build_fn(const char *dir, const char *name, char *path);  // dir may be == path
const char *basename(const char *name);
inline char *basename(char *name) { return (char *) basename((const char *) name); }
// ext (without dot) or end of string if none or starts with dot
const char *file_ext(const char *name);
inline char *file_ext(char *name) { return (char *) file_ext((const char *) name); }
// whether name matches ext; an ext-less name does not match empty ext
bool matches_exts(const char *name, const char *ext, ...);
