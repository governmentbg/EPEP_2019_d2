//{{TFloatArchiveDialog Implementation}}

TFloatArchiveDialog::TFloatArchiveDialog(TWindow* parent, TFloatArchiveGroup *group)
:
	TFloatSubjectDialog(parent, group, IDD_FLOAT_ARCHIVE)
{
	new TTypeListFace(this, IDC_FLOAT_ARCHIVE_TYPES, "F_TYPES", NULL, false);
	minNo = new TLongFace(this, IDC_FLOAT_ARCHIVE_MIN_NO, "F_MIN_NO", SIZE_OF_NO);
	maxNo = new TLongFace(this, IDC_FLOAT_ARCHIVE_MAX_NO, "F_MAX_NO", SIZE_OF_NO);
	minYear = new TYearFace(this, IDC_FLOAT_ARCHIVE_MIN_YEAR, "F_MIN_YEAR");
	maxYear = new TYearFace(this, IDC_FLOAT_ARCHIVE_MAX_YEAR, "F_MAX_YEAR");
	minLinkNo = new TLongFace(this, IDC_FLOAT_ARCHIVE_MIN_LINK_NO, "F_MIN_LINK_NO", SIZE_OF_NO);
	maxLinkNo = new TLongFace(this, IDC_FLOAT_ARCHIVE_MAX_LINK_NO, "F_MAX_LINK_NO", SIZE_OF_NO);
	minArchiveNo = new TLongFace(this, IDC_FLOAT_ARCHIVE_MIN_ARCHIVE_NO, "F_MIN_ARCHIVE_NO", SIZE_OF_NO);
	maxArchiveNo = new TLongFace(this, IDC_FLOAT_ARCHIVE_MAX_ARCHIVE_NO, "F_MAX_ARCHIVE_NO", SIZE_OF_NO);
	minArchiveYear = new TYearFace(this, IDC_FLOAT_ARCHIVE_MIN_ARCHIVE_YEAR, "F_MIN_ARCHIVE_YEAR");
	maxArchiveYear = new TYearFace(this, IDC_FLOAT_ARCHIVE_MAX_ARCHIVE_YEAR, "F_MAX_ARCHIVE_YEAR");
	minArchiveDate = new TDateFace(this, IDC_FLOAT_ARCHIVE_MIN_ARCHIVE_DATE, "F_MIN_ARCHIVE_DATE", false);
	maxArchiveDate = new TDateFace(this, IDC_FLOAT_ARCHIVE_MAX_ARCHIVE_DATE, "F_MAX_ARCHIVE_DATE", false);
	minKeep = new TLongFace(this, IDC_FLOAT_ARCHIVE_MIN_KEEP, "F_MIN_KEEP", SIZE_OF_NO);
	maxKeep = new TLongFace(this, IDC_FLOAT_ARCHIVE_MAX_KEEP, "F_MAX_KEEP", SIZE_OF_NO);
	new TCharListFace(this, IDC_FLOAT_ARCHIVE_GENERAL_KINDS, "F_GENERAL_KINDS", Kind, KIND_ALL_GENERALS, SIZE_OF_KINDS);
	indexes = new TArchiveIndexListBox(this, IDC_FLOAT_ARCHIVE_INDEXES, true);
	new TCheckFace(this, IDC_FLOAT_ARCHIVE_FLAGS_EC, "F_FLAGS_EC");
	excess = new TCheckFace(this, IDC_FLOAT_ARCHIVE_EXCESS, "F_EXCESS");
	new TRadioFace(this, IDC_FLOAT_ARCHIVE_BY_TYN, "F_BY_TYN");
	new TRadioFace(this, IDC_FLOAT_ARCHIVE_BY_ATYN, "F_BY_ATYN");
}

bool TFloatArchiveDialog::IsValid()
{
	bool isValid = TFloatSubjectDialog::IsValid();

	if (isValid)
	{
		if (invalid_range(minNo, maxNo))
			isValid = error("Началния номер дело е по-голям от крайния.");
		else if (invalid_range(minYear, maxYear))
			isValid = error("Началната година е по-голяма от крайната.");
		else if (invalid_range(minLinkNo, maxLinkNo))
			isValid = error("Началния номер на връзка е по-голям от крайния.");
		else if (invalid_range(minArchiveNo, maxArchiveNo))
			isValid = error("Началния архивен номер е по-голям от крайния.");
		else if (invalid_range(minArchiveYear, maxArchiveYear))
			isValid = error("Началната архивна година е по-голяма от крайната.");
		else if (invalid_range(minArchiveDate, maxArchiveDate))
			isValid = error("Началната дата на архивиране е по-голяма от крайната.");
		else if (invalid_range(minKeep, maxKeep))
			isValid = error("Минималния срок за съхранение е по-голям от максималния.");
	}

	return isValid;
}

void TFloatArchiveDialog::Action(bool execute)
{
	msql m;

	TFloatSubjectDialog::GetSubjects(FloatArchiveGroup()->newSubjects, FloatArchiveGroup()->oldSubjects);
	indexes->GetMark(&FloatArchiveGroup()->indexes);
	FloatArchiveGroup()->nullKeep = !maxKeep->L() && *maxKeep->S();
	FloatArchiveGroup()->FormCriteria(m);

	if (execute)
		query_archive(this, str(m));
	else
		Message(m);
}

void TFloatArchiveDialog::SetupWindow()
{
	TFloatSubjectDialog::SetupWindow();

	if (COURT_CODE == COURT_SOFGS)
	{
		excess->EnableWindow(true);
		excess->ShowWindow(SW_SHOW);
	}
}
