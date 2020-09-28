#include "cac.h"

constant
	TITLE_SECTION = "***_TITLE",
	WINDOWS_TITLE_SECTION = "***_WINDOWS_TITLE",
	HEADER_SECTION = "***_HEADER",
	FORMATTED_HEADER_SECTION = "***_FORMATTED_HEADER",
	HTML_FORMATTED_HEADER_SECTION = "***_HTML_FORMATTED_HEADER",
	FORMATTED_TEXT_SECTION = "***_FORMATTED_TEXT",
	MEDIUM_LINE_SECTION = "***_MEDIUM_LINE",
	ALT_PATCH_MEDIUM_LINE_SECTION = "***_ALT_PATCH_MEDIUM_LINE",
	COLUMNS_SECTION = "***_COLUMNS",
	WINDOWS_COLUMNS_SECTION = "***_WINDOWS_COLUMNS",
	ALT_PATCH_COLUMNS_SECTION = "***_ALT_PATCH_COLUMNS",
	PARAMETERS_SECTION = "***_PARAMETERS",
	WINDOWS_A3_PARAMETERS_SECTION = "***_WINDOWS_A3_PARAMETERS",
	WINDOWS_PARAMETERS_SECTION = "***_WINDOWS_PARAMETERS",
	TAIL_SECTION = "***_TAIL",
	SUBJECTS_SECTION = "***_SUBJECTS",
	SUBJECT_RANGES_SECTION = "***_SUBJECT_RANGES",
	WANT_KINDS_SECTION = "***_WANT_KINDS",
	MERGED_KINDS_SECTION = "***_MERGED_KINDS",
	WANT_COMPANY_TYPES_SECTION = "***_WANT_COMPANY_TYPES",
	FORMATTED_TAIL_SECTION = "***_FORMATTED_TAIL",
	TABLE_ROW_SECTION = "***_TABLE_ROW",
	COLUMN_WIDTH_SECTION = "***_COLUMN_WIDTH",
	COLUMN_ROWS_SECTION = "***_COLUMN_ROWS",
	END_OF_SECTION = "***_END";

static bool cannotFindSection(const char *fmtFName, const char *section)
{
	char mes[FMT_SECTION_NAME_SIZE + 128];

	ostrstream(mes, sizeof mes) << CANNOT_LOAD_THE_FORMAT_INFO << "\n" "- неоткрита секция ("
		<< (section ? section : "NULL pointer passed!") << ")\n" "във файл " << fmtFName << ends;

	mes[sizeof mes - 1] = '\0';
	return error("%s", mes);
}

bool findSection(ifstream & ifs, const char *fmtFName, const char *section, bool errMess)
{
	bool found = false;

	if (section && section [0])
	{
		char tmp[SIZE_OF_LINE + 50];
		long oldPos = ifs.tellg();

		if (ifs.good())
		{  // -- 1 --
			ifs.seekg(0);

			while (ifs.good () && !found)
			{
				ifs.getline(tmp, sizeof tmp);

				if (ifs.good())
				{
					strDelLastSpaces(tmp);
					found = !strcmp(tmp, section);
				}
			}

			if (!found)
			{

				if (!ifs.good())
				{
					ifs.clear();
					ifs.seekg(oldPos);
				}

				if (errMess)
					cannotFindSection(fmtFName, section);
			}
		}  // -- 1 --
		else
			cannotFindSection(fmtFName, section);
	}
	else
		error("%s\n%s", WRONG_ARGS_PASSED_TO, "findSection");

	return found;
}

bool endSection(const char *fileLine)
{
	if (fileLine == NULL)
	{
		error("%s\n%s", WRONG_ARGS_PASSED_TO, "endSection");
		fileLine = "";
	}

	size_t eSL = strlen(END_OF_SECTION);

	return !strncmp(fileLine, END_OF_SECTION, eSL) && strIsBlank(fileLine + eSL);
}

bool section_says_yes(ifstream & ifs, const char *section, bool errMess, const char *fmtFName)
{
	bool sayYes = false;

	if (section && section [0])
	{
		if (findSection(ifs, fmtFName, section, errMess))
		{  // -- 2 --
			char *line = new char[SIZE_OF_LINE];

			line[0] = '\0';

			while (ifs.good() && !endSection(line))
			{  // -- 3 --
				line[0] = '\0';
				ifs.getline(line, SIZE_OF_LINE);

				if (ifs.good() && !endSection(line))
				{  // -- 4 --
					{
						char *semicolonPtr = strchr(line, ';');

						if (semicolonPtr)
							*semicolonPtr = '\0';
					}

					strDelLastSpaces(line);

					if (!strIsBlank(line))
						sayYes = !strcmp(line, "YES");
				}  // -- 4 --
			}  // -- 3 --

			delete[] line;
		}  // -- 2 --
	}
	else
		error("%s\n%s", WRONG_ARGS_PASSED_TO, "section_says_yes");

	return sayYes;
}

bool section_says_yes(ifstream &ifs, const char *section, bool errMess)
{
	return section_says_yes(ifs, section, errMess, " ");
}

bool section_says_yes(const char *fmtFName, const char *section, bool errMess)
{
	if (!(fmtFName && section))
		return false;

	int sZ = strlen(fmtFName) + SIZE_OF_LINE;
	char *hName = new char[sZ];

	ostrstream(hName, sZ) << fmtFName << ends;

	if (access(hName, 0))
	{
		if (!referenceToFmtFile(hName, sZ))
		{
			delete[] hName;
			return false;
		}
	}

	bool res = false;
	ifstream ifs(hName);

	if (ifs)
		res = section_says_yes(ifs, section, errMess, fmtFName);
	else
		cannotOpenFmtFile(hName);

	delete[] hName;
	return res;
}

int get_section_int(ifstream &ifs, const char *section, int defaultVal, bool errMess, const char *fmtFName)
{
	int sectionInt = defaultVal;

	if (section && section [0])
	{
		if (findSection(ifs, fmtFName, section, errMess))
		{  // -- 2 --
			CharPtr line(SIZE_OF_LINE);

			line[0] = '\0';

			while (ifs.good () && !endSection(line))
			{  // -- 3 --
				line[0] = '\0';
				ifs.getline(line, line.Size());

				if (ifs.good() && !endSection(line))
				{  // -- 4 --
					{
						char *semicolonPtr = strchr(line, ';');

						if (semicolonPtr)
							*semicolonPtr = '\0';
					}

					if (strIsBlank(line) == false)
						istrstream(line, strlen(line)) >> sectionInt;
				}  // -- 4 --
			}  // -- 3 --
		}  // -- 2 --
	}
	else
		error("%s\n%s", WRONG_ARGS_PASSED_TO, "get_section_int");

	return sectionInt;
}

int get_section_int(ifstream &ifs, const char *section, int defaultVal, bool errMess)
{
	return get_section_int(ifs, section, defaultVal, errMess, " ");
}

int get_section_int(const char *fmtFName, const char *section, int defaultVal, bool errMess)
{
	int sectionInt = defaultVal;

	if (fmtFName && section)
	{
		CharPtr hName(strlen(fmtFName) + SIZE_OF_LINE);

		ostrstream(hName, hName.Size()) << fmtFName << ends;

		if (access(hName, 0) == 0 || referenceToFmtFile(hName, hName.Size()))
		{
			ifstream ifs(hName);

			if (ifs)
				sectionInt = get_section_int(ifs, section, defaultVal, errMess, fmtFName);
			else
				cannotOpenFmtFile(hName);
		}
	}

	return sectionInt;
}

void get_section_string(ifstream &ifs, const char *fmtFName, const char *section, char *dest, size_t destSz,
	const char *defaultStr, bool trimSideSpaces, bool errMess)
{
	if (defaultStr == NULL)
		defaultStr = "";

	if (dest && destSz && section && section[0])
	{
		ostrstream(dest, destSz) << defaultStr << ends;

		dest [destSz - 1] = '\0';

		if (findSection(ifs, fmtFName, section, errMess))
		{  // -- 2 --
			CharPtr line(SIZE_OF_LINE);

			line [0] = '\0';

			while (ifs.good () && !endSection(line))
			{  // -- 3 --
				line [0] = '\0';
				ifs.getline(line, line.Size());

				if (ifs.good() && !endSection(line))
				{  // -- 4 --
					if (trimSideSpaces)
					{
						strDelLastSpaces(line);
						strDelLeadingSpaces(line);
					}

					ostrstream(dest, destSz) << line << ends;
					dest[destSz - 1] = '\0';
				}  // -- 4 --
			}  // -- 3 --
		}  // -- 2 --
	}
	else
		error("%s\n%s", WRONG_ARGS_PASSED_TO, "get_section_string");
}

void get_section_string(ifstream & ifs, const char *fmtFName, const char *section, char *dest, size_t destSz)
{
	get_section_string(ifs, fmtFName, section, dest, destSz, "");
}

void get_section_string(const char *fmtFName, const char *section, char *dest, size_t destSz, const char *defaultStr,
	bool trimSideSpaces, bool errMess)
{
	if (fmtFName && section)
	{
		CharPtr hName(strlen(fmtFName) + SIZE_OF_LINE);

		ostrstream(hName, hName.Size()) << fmtFName << ends;

		if (access(hName, 0) == 0 || referenceToFmtFile(hName, hName.Size()))
		{
			ifstream ifs(hName);

			if (ifs)
				get_section_string(ifs, fmtFName, section, dest, destSz, defaultStr, trimSideSpaces, errMess);
			else
				cannotOpenFmtFile(hName);
		}
	}
}

void get_section_string(const char *fmtFName, const char *section, char *dest, size_t destSz)
{
	get_section_string(fmtFName, section, dest, destSz, "");
}
