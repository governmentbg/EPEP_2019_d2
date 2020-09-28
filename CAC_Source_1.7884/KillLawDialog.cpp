DEFINE_RESPONSE_TABLE1(TKillLawDialog, TGroupDialog)
//{{TKillLawDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TKillLawDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TKillLawDialog Implementation}}

TKillLawDialog::TKillLawDialog(TWindow* parent, TArchive *archive, TLawsuit *tLawsuit, bool tEdit)
:
	TGroupDialog(parent, archive, IDD_KILL_LAW), lawsuit(tLawsuit), edit(tEdit), hasKeep(archive->keep)
{
	new TCharAliasFace(this, IDC_KILL_LAW_TYPE, "F_TYPE", Type, NULL);
	new TLongFace(this, IDC_KILL_LAW_NO, "F_NO", SIZE_OF_NO);
	new TYearFace(this, IDC_KILL_LAW_YEAR, "F_YEAR");
	new TLongFace(this, IDC_KILL_LAW_LINK_NO, "F_LINK_NO", SIZE_OF_NO);
	new TLongFace(this, IDC_KILL_LAW_ARCHIVE_NO, "F_ARCHIVE_NO", SIZE_OF_NO);
	archiveDate = new TDateFace(this, IDC_KILL_LAW_ARCHIVE_DATE, "F_ARCHIVE_DATE");
	killDate = new TDateFace(this, IDC_KILL_LAW_KILL_DATE, "F_KILL_DATE");
	new TStringFace(this, IDC_KILL_LAW_KILL_REMARK, "F_KILL_REMARK", SIZE_OF_ARCHIVE_TEXT);
	killerNo = new TLongFace(this, IDC_KILL_LAW_KILLER_NO, "F_KILLER_NO", SIZE_OF_NO);
	killerDate = new TDateFace(this, IDC_KILL_LAW_KILLER_DATE, "F_KILLER_DATE", false);
	new TStringFace(this, IDC_KILL_LAW_REMAINDER, "F_REMAINDER", SIZE_OF_REMAINDER_TEXT);
	remainderNo = new TLongFace(this, IDC_KILL_LAW_REMAINDER_NO, "F_REMAINDER_NO", SIZE_OF_NO);
	remainderYear = new TYearFace(this, IDC_KILL_LAW_REMAINDER_YEAR, "F_REMAINDER_YEAR");
	keep = new TLongFace(this, IDC_KILL_LAW_KEEP, "F_KEEP", SIZE_OF_ARCHIVE_KEEP);
	archiveIndex = new TStringFace(this, IDC_KILL_LAW_ARCHIVE_INDEX, "F_ARCHIVE_INDEX_STRING", SIZE_OF_ALIAS);
}

void TKillLawDialog::SetupWindow()
{
	TGroupDialog::SetupWindow();

	if (*archiveIndex->S() && !hasKeep)
	{
		ArchiveIndex->Seek(archiveIndex->S());
		keep->SetL(ArchiveIndex->keep);
	}

	if (!edit)
		killDate->SetDate(Today);
}

void TKillLawDialog::OKBNClicked()
{
	if (IsValid())
	{
		if (edit || warn("Внимание! След унищожаване делото няма да може да се възстанови. %s", Q_CONTINUE))
		{
			*Group() << this;

			if (!hasKeep)
				Archive()->keep = 0;

			if (Group()->Update() && (edit || destroy_lawsuit(lawsuit)))
				CmOk();
		}
	}
}

bool TKillLawDialog::IsValid()
{
	bool isValid = TGroupDialog::IsValid();

	if (isValid)
	{
		long keep = Archive()->keep;

		if (!keep && Archive()->archiveIndex)
		{
			ArchiveIndex->Seek(Archive()->archiveIndex);
			keep = ArchiveIndex->keep;
		}

		if (isValid)
		{
			if (!keep)
				isValid = error("Срокът за съхранение на делото е неизвестен.");
			else if (killDate->Date() - archiveDate->Date() < keep * 12)
				isValid = error("Датата на унищожаване трябва да е поне %ld години след датата на архивиране.", keep);
			else if (killDate->Date() > Today)
				isValid = error("Датата на унищожаване е по-голяма от текущата дата.");
			else if (killerNo->L() == 0 || killerDate->Date().Empty())
				isValid = error("Полетата номер и дата на протокола за унищожаване са задължителни.");
			else if ((remainderNo->L() == 0) != (remainderYear->L() == 0))
			{
				isValid = error("Полетата номер и година на тома в който са съхранени запазенито документи "
					"не си съответстват.");
			}
		}
	}

	return isValid;
}
