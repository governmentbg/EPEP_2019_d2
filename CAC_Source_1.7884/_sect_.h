// ----------------------- Sections ------------------------------
extern constant
	TITLE_SECTION,
	HTML_TITLE_SECTION,
	WINDOWS_TITLE_SECTION,
	HEADER_SECTION,
	HTML_HEADER_SECTION,
	FORMATTED_HEADER_SECTION,
	HTML_FORMATTED_HEADER_SECTION,
	FORMATTED_TEXT_SECTION,
	COLUMNS_SECTION,
	WINDOWS_COLUMNS_SECTION,
	ALT_PATCH_COLUMNS_SECTION,
	PARAMETERS_SECTION,
	WINDOWS_A3_PARAMETERS_SECTION,
	WINDOWS_PARAMETERS_SECTION,
	MEDIUM_LINE_SECTION,
	ALT_PATCH_MEDIUM_LINE_SECTION,
	TAIL_SECTION,
	SUBJECTS_SECTION,
	SUBJECT_RANGES_SECTION,
	WANT_KINDS_SECTION,
	MERGED_KINDS_SECTION,
	WANT_COMPANY_TYPES_SECTION,
	FORMATTED_TAIL_SECTION,
	TABLE_ROW_SECTION,
	COLUMN_WIDTH_SECTION,
	COLUMN_ROWS_SECTION,
	END_OF_SECTION;

bool findSection(ifstream &ifs, const char *fmtFName, const char *section, bool errMess = true);
bool endSection(const char *fileLine);
bool section_says_yes(ifstream &ifs, const char *section, bool errMess, const char *fmtFName);
bool section_says_yes(ifstream &ifs, const char *section, bool errMess = true);
bool section_says_yes(const char *fmtFName, const char *section, bool errMess = true);
int get_section_int(ifstream &ifs, const char *section, int defaultVal, bool errMess, const char *fmtFName);
int get_section_int(ifstream &ifs, const char *section, int defaultVal = 0, bool errMess = true);
int get_section_int(const char *fmtFName, const char *section, int defaultVal = 0, bool errMess = true);
void get_section_string(ifstream &ifs, const char *fmtFName, const char *section, char *dest, size_t destSz,
	const char *defaultStr, bool trimSideSpaces = true, bool errMess = true);
void get_section_string(ifstream &ifs, const char *fmtFName, const char *section, char *dest, size_t destSz);
void get_section_string(const char *fmtFName, const char *section, char *dest, size_t destSz, const char *defaultStr,
	bool trimSideSpaces = true, bool errMess = true);
void get_section_string(const char *fmtFName, const char *section, char *dest, size_t destSz);
