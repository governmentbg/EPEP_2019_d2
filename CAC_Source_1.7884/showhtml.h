// 2016:004 LPR: TYPE_HTML now includes "html"
//inline bool html_name(const char *name) { return matches_exts(name, "htm", "html", NULL); }
#define html_name(name) electric_matches((name), TYPE_HTML, true, true)
// parent != NULL -> wait
bool show_html(TWindow *parent, const char *name, bool remove);
bool show_html(TWindow *parent, char *text);
