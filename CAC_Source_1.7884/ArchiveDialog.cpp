DEFINE_RESPONSE_TABLE1(TArchiveDialog, TGroupDialog)
//{{TArchiveDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_CBN_SELCHANGE(IDC_ARCHIVE_ARCHIVE_INDEX, ArchiveIndexCBNSelChange),
//{{TArchiveDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TArchiveDialog Implementation}}

TArchiveDialog::TArchiveDialog(TWindow* parent, TArchive *archive, TExternContainer *tLawsuit, bool tEdit, int resId)
:
	TGroupDialog(parent, archive, resId), edit(tEdit), lawsuit(tLawsuit), hasKeep(archive->keep)
{
	new TCharAliasFace(this, IDC_ARCHIVE_TYPE, "F_TYPE", Type, NULL);
	new TLongFace(this, IDC_ARCHIVE_NO, "F_NO", SIZE_OF_NO);
	new TYearFace(this, IDC_ARCHIVE_YEAR, "F_YEAR");
	new TLongFace(this, IDC_ARCHIVE_LINK_NO, "F_LINK_NO", SIZE_OF_NO);
	archiveNo = new TLongFace(this, IDC_ARCHIVE_ARCHIVE_NO, "F_ARCHIVE_NO", SIZE_OF_NO);
	archiveDate = new TDateFace(this, IDC_ARCHIVE_ARCHIVE_DATE, "F_ARCHIVE_DATE");
	new TStringFace(this, IDC_ARCHIVE_ARCHIVE_REMARK, "F_ARCHIVE_REMARK", SIZE_OF_ARCHIVE_TEXT);
	keep = new TLongFace(this, IDC_ARCHIVE_KEEP, "F_KEEP", SIZE_OF_ARCHIVE_KEEP);
	archiveIndex = new TArchiveIndexAliasFace(this, IDC_ARCHIVE_ARCHIVE_INDEX, "F_ARCHIVE_INDEX", lawsuit,
		edit ? any(archive->archiveIndex) ? FLAG_NULL : ALIAS_EMPTY : FLAG_ACTIVE);
}

bool TArchiveDialog::IsValid()
{
	bool isValid = TGroupDialog::IsValid();

	if (isValid)
	{
		if (any(archiveIndex->S()) && !keep->L())
			isValid = error("Срокът за съхранение е задължителен.");
	}

	return isValid;
}

void TArchiveDialog::OKBNClicked()
{
	if (IsValid())
	{
		*Group() << this;

		// 2005:154 LRQ: allow archiving if requested
		if (lawsuit->key.type == TYPE_FIRM_LAW || archiveDate->Date() - lawsuit->ForceDate() >= ARCHIVE_MONTHS ||
			ask("Датата на архивиране трябва да е поне %d месеца след датата на влизане в законна сила. %s",
				ARCHIVE_MONTHS, Q_CONTINUE))
		{
			// 2014:059 LPR: don't save keep == index
			if (any(archiveIndex->S()) && !hasKeep)
			{
				ArchiveIndex->Seek(archiveIndex->S());

				if (keep->L() == ArchiveIndex->keep)
					Archive()->keep = 0;
			}

			if (Group()->Save(edit))
				CmOk();
		}
	}
}

void TArchiveDialog::SetupWindow()
{
	TGroupDialog::SetupWindow();

	if (!edit)
	{
		archiveDate->EnableWindow(true);
		archiveNo->EnableWindow(true);
		archiveDate->SetDate(Today);	// 2006:019
	}

	ArchiveIndexCBNSelChange();
}

void TArchiveDialog::ArchiveIndexCBNSelChange()
{
	if (*archiveIndex->S() && !hasKeep)
	{
		ArchiveIndex->Seek(archiveIndex->S());
		keep->SetL(ArchiveIndex->keep);
		keep->EnableWindow(ArchiveIndex->flags & ARCHIVE_INDEX_FLOAT);
	}
	else
		keep->EnableWindow(true);
}
