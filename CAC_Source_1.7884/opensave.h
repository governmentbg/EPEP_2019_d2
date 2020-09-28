bool GetOpenFileName(TWindow *parent, char *name, int type, bool primary, size_t size, const char *defname = NULL,
	bool limitdir = true);
bool GetOpenFileName(TWindow *parent, char *name, const char *whats, char *initdir, const char *ext, size_t size,
	const char *defname = NULL, bool limitdir = true);
bool GetSaveFileName(TWindow *parent, char *name, const char *whats, char *initdir, const char *ext, size_t size,
	const char *defname = NULL, bool limitdir = true);
