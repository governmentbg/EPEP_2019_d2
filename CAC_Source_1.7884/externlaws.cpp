#include "cac.h"

#if COURT_TYPE == COURT_AREAL

#define TEXPARRAY(T) TXArray<TExport##T>(#T)

// ----- TExportSide4A -------------------------------------------------------
class TExportSide4A : public TXMLGroup
{
public:
	TExportSide4A(const char *name);

	char id[SIZE_OF_EISPP_NMR];
	long involvement;

protected:
	void Load4A(const TSideWind &sideWind);
};

TExportSide4A::TExportSide4A(const char *name)
	: TXMLGroup(name)
{
	attribs.Add(
		new TXMLString("id", id, sizeof id),
		new TXMLInt("Involvement", &involvement),
		NULL
	);

	Apply("*", XML_INCLUDED | XML_REQUIRED);
}

void TExportSide4A::Load4A(const TSideWind &sideWind)
{
	involvement = sideWind.involvement;
	sprintf(id, "E%03d%10s", sideWind.ucnType, sideWind.ucn);
}

// ----- TExportPhysicalSide -------------------------------------------------
class TExportPhysicalSide : public TExportSide4A
{
public:
	TExportPhysicalSide();

	static TExportPhysicalSide *Load(const TSideWind &sideWind, mstr &m);

	char ucn[SIZE_OF_UCN];
	char name[SIZE_OF_CITIZEN_NAME];
	char family[SIZE_OF_CITIZEN_FAMILY];
	char reName[SIZE_OF_CITIZEN_RENAME];
	char reFamily[SIZE_OF_CITIZEN_REFAMILY];
	char country[SIZE_OF_COUNTRY];
	char reCountry[SIZE_OF_COUNTRY];
};

TExportPhysicalSide::TExportPhysicalSide()
	: TExportSide4A("PhysicalSide")
{
	attribs.Add(
		new TXMLString("EGN", ucn, sizeof ucn),
		new TXMLString("Name", name, sizeof name),
		new TXMLString("ReName", reName, sizeof reName),
		new TXMLString("Family", family, sizeof family),
		new TXMLString("ReFamily", reFamily, sizeof reFamily),
		new TXMLString("Country", country, sizeof country),
		new TXMLString("ReCountry", reCountry, sizeof reCountry),
		NULL
	);

	Apply("*", XML_INCLUDED);
	Apply("Name", XML_REQUIRED);
	Apply("Family", XML_REQUIRED);
	Apply("Country", XML_REQUIRED);
}

TExportPhysicalSide *TExportPhysicalSide::Load(const TSideWind &sideWind, mstr &m)
{
	TExportPhysicalSide *side = NULL;

	if (strchr(UCN_CITIZENS, sideWind.ucnType))
	{
		TCitizen citizen;

		citizen.ucnType = sideWind.ucnType;
		strcpy(citizen.ucn, sideWind.ucn);

		if (citizen.Get())
		{
			side = new TExportPhysicalSide;
			side->Load4A(sideWind);
			strcpy(side->ucn, sideWind.ucn);
			strcpy(side->name, citizen.name);
			strcpy(side->reName, citizen.reName);
			strcpy(side->family, citizen.family);
			strcpy(side->reFamily, citizen.reFamily);
			strcpy(side->country, citizen.country);
			strcpy(side->reCountry, citizen.reCountry);

			m.sep("\v");
			m.printf("%s %s %s %s %s %s %s %s", Involvement->Seek(sideWind.involvement), side->ucn, side->name,
				side->reName, side->family, side->reFamily, side->country, side->reCountry);
		}
	}

	return side;
}

// ----- TExportJudicialSide -------------------------------------------------
class TExportJudicialSide : public TExportSide4A
{
public:
	TExportJudicialSide();

	static TExportJudicialSide *Load(const TSideWind &sideWind, mstr &m);

	long involvement;
	char name[SIZE_OF_FIRM_NAME];
	char bulstat[SIZE_OF_BULSTAT];
	char country[SIZE_OF_COUNTRY];
};

TExportJudicialSide::TExportJudicialSide()
	: TExportSide4A("JudicialSide")
{
	attribs.Add(
		new TXMLString("Name", name, sizeof name),
		new TXMLString("EIK", bulstat, sizeof bulstat),
		new TXMLString("Country", country, sizeof country),
		NULL
	);

	Apply("*", XML_INCLUDED);
	Apply("Name", XML_REQUIRED);
	Apply("Country", XML_REQUIRED);
}

TExportJudicialSide *TExportJudicialSide::Load(const TSideWind &sideWind, mstr &m)
{
	TExportJudicialSide *side = NULL;

	if (strchr(UCN_FIRMS, sideWind.ucnType))
	{
		TFirm firm;

		firm.ucnType = sideWind.ucnType;
		strcpy(firm.ucn, sideWind.ucn);

		if (firm.Get())
		{
			side = new TExportJudicialSide;
			side->Load4A(sideWind);
			strcpy(side->name, firm.name);
			strcpy(side->country, firm.country);

			m.sep("\v");
			m.printf("%s %s %s %s", Involvement->Seek(sideWind.involvement), side->name, side->bulstat, side->country);
		}
	}

	return side;
}

// ----- TExportLawsuit4ArchBase ---------------------------------------------
class TExportLawsuit4ArchBase : public TXMLGroup
{
public:
	TExportLawsuit4ArchBase(const char *name);

	virtual void Write(TEisFile &x);

	struct TKey
	{
		long kind;
		long no;
		long year;
	};

	TKey key;
	char id[SIZE_OF_EISPP_NMR];
	long quick;

protected:
	void Load(const TExternContainer *lawsuit);
};

TExportLawsuit4ArchBase::TExportLawsuit4ArchBase(const char *name)
	: TXMLGroup(name)
{
	attribs.Add(
		new TXMLString("id", id, sizeof id),
		new TXMLInt("kind", &key.kind),
		new TXMLInt("no", &key.no),
		new TXMLInt("year", &key.year),
		NULL
	);
}

void TExportLawsuit4ArchBase::Load(const TExternContainer *lawsuit)
{
	key.kind = lawsuit->kind;
	key.no = lawsuit->key.no;
	key.year = lawsuit->key.year;
	quick = lawsuit->quick & QUICK_ALL;
}

void TExportLawsuit4ArchBase::Write(TEisFile &x)
{
	sprintf(id, "E%1ld%03ld%05ld%04ld", quick, key.kind, key.no, key.year);
	TXMLGroup::Write(x);
}

// ----- TExportLawsuit4A ----------------------------------------------------
class TExportLawsuit4A : public TExportLawsuit4ArchBase
{
public:
	TExportLawsuit4A();

	void Load(TLawsuit *lawsuit, msql &m);
	void Save(TExternLawsuit &lawsuit) const;

	long subject;
	CDate date;
	CDate forceDate;
	long sourceKind;

	TXArray<TExportPhysicalSide> *physicalSides;
	TXArray<TExportJudicialSide> *judicialSides;
};

TExportLawsuit4A::TExportLawsuit4A()
	: TExportLawsuit4ArchBase("Lawsuit4A"), physicalSides(new TEXPARRAY(PhysicalSide)),
		judicialSides(new TEXPARRAY(JudicialSide))
{
	attribs.Add(
		new TXMLInt("subject", &subject),
		new TXMLDate("forceDate", &forceDate),
		new TXMLDate("date", &date),
		new TXMLInt("quick", &quick),
		NULL
	);

	Apply("*", XML_REQUIRED);
	Apply("quick", XML_NULLABLE);
	attribs.Add(new TXMLInt("sourceKind", &sourceKind));	// INCLUDED only
	Apply("*", XML_INCLUDED);

	objects.Add(physicalSides, judicialSides, NULL);
	Apply("PhysicalSide", XML_INCLUDED);
	Apply("JudicialSide", XML_INCLUDED);
}

void TExportLawsuit4A::Load(TLawsuit *lawsuit, msql &m)
{
	TExportLawsuit4ArchBase::Load(lawsuit);
	subject = lawsuit->subject;
	forceDate = lawsuit->forceDate;
	date = lawsuit->date;
	sourceKind = lawsuit->sourceKind;

	TSideWind sideWind;
	TCitizen citizen;
	TFirm firm;

	sideWind.key = lawsuit->key;
	m.Begin(sideWind.Name, "DISTINCT F_UCN, F_UCN_TYPE, F_INVOLVEMENT");
	m.Add(&sideWind, "F_NO, F_YEAR, F_TYPE");
	TQuery q(str(m));
	m.clear();

	while (q.Read())
	{
		sideWind << q;

		if (lawsuit->Other(sideWind.involvement))
			continue;

		if (lawsuit->key.type == TYPE_PUNISHMENT_LAW && lawsuit->Suitor(sideWind.involvement))
			continue;

		TExportPhysicalSide *physicalSide = TExportPhysicalSide::Load(sideWind, m);

		if (physicalSide)
			physicalSides->Add(physicalSide);
		else
		{
			TExportJudicialSide *judicialSide = TExportJudicialSide::Load(sideWind, m);

			if (judicialSide)
				judicialSides->Add(judicialSide);
		}
	}
}

void TExportLawsuit4A::Save(TExternLawsuit &lawsuit) const
{
	lawsuit.key.no = key.no;
	lawsuit.key.year = key.year;
	lawsuit.key.type = kind2type((char) key.kind);
	lawsuit.kind = (char) key.kind;
	lawsuit.subject = subject;
	lawsuit.forceDate = forceDate;
	lawsuit.date = date;
	lawsuit.quick = quick;
	lawsuit.sourceKind = (char) sourceKind;
}

// ----- TExportForArchive ---------------------------------------------------
class TExportForArchive : public TXMLGroup
{
public:
	TExportForArchive();

	TXArray<TExportLawsuit4A> *lawsuit4As;
};

TExportForArchive::TExportForArchive()
	: TXMLGroup("ExportForArchive"), lawsuit4As(new TEXPARRAY(Lawsuit4A))
{
	objects.Add(lawsuit4As);
	Apply("Lawsuit4A", XML_INCLUDED | XML_REQUIRED);
}

#define lawsuit OFFICE_GROUP_CAST(TLawsuit)

static void lawsuit_subject(mstr &m, TGroup *group)
{
	m.printf("%s %s", lawsuit->Code(), Subject->Seek(lawsuit->key.type, lawsuit->subject));
}

#undef lawsuit

static offield lawsuit_fields[] =
{
	{ "F_KIND",       container_kind },
	{ "F_NO_YEAR",    keygroup_key_no_year },
	{ "F_SUBJECT",    lawsuit_subject },
	{ "F_SIDES",      NULL },
	{ "F_FORCE_DATE", NULL },
	{ NULL, NULL }
};

class TExportSidesString : public TDummyGroup
{
public:
	TExportSidesString();

	msql m;
};

TExportSidesString::TExportSidesString()
{
	Add(new TText("F_SIDES", &m, INT_MAX));
}

// ----- export_archives -----------------------------------------------------
static void export_archives(TWindow *parent, const TGroupArray *array)
{
	TOffice office;
	TDummyGroup stub;
	char name[SIZE_OF_PATH];

	if (office.Open("ExportArch.htm") &&
		GetSaveFileName(parent, name, "XML файлове", NULL, "XML", sizeof name, NULL, false))
	{
		TWaitWindow wait(parent, WAIT);
		constant tr1start = office.Find("tr1 start");
		constant tr1sides = office.Find("tr1 sides");
		constant tr1force = office.Find("tr1 force");
		constant tr1final = office.Find("tr1 final");
		TExportForArchive x4a;
		TEisFile x(0);

		log(LOG_EXPORT, CM_CLERK_EXPORT_ARCHIVES);
		office.Emit(NULL, tr1start, null_fields, NULL);

		for (int i = 0; i < array->Count(); i++)
		{
			TLawsuit *lawsuit = (TLawsuit *) (*array)[i];
			TExportLawsuit4A *l4a = new TExportLawsuit4A;
			TExportSidesString sidestr;

			office.Emit(tr1start, tr1sides, lawsuit_fields, lawsuit);
			l4a->Load(lawsuit, sidestr.m);
			office.Emit(tr1sides, tr1force, lawsuit_fields, &sidestr);
			office.Emit(tr1force, tr1final, lawsuit_fields, lawsuit);
			x4a.lawsuit4As->Add(l4a);
		}

		x.OpenWrite(name);
		try
		{
			x4a.Write(x);
			office.Emit(tr1final, NULL, null_fields, NULL);
			office.Show(&wait);
		}
		catch (TXMLError *x)
		{
			x->Handle();
			delete x;
			office.Close();
		}
		x.Close();
	}
}

#include "ExportArchivesListWindow.h"
#include "ExportArchivesListWindow.cpp"

#include "ExportArchivesListDialog.h"
#include "ExportArchivesListDialog.cpp"

void export_archives(TWindow *parent)
{
	TExportArchivesListDialog(parent).Execute();
}

// ----- TExternSideWind4A ---------------------------------------------------
class TExternSideWind4A : public TExternSideWind
{
public:
	static TGroup *Clone();

	bool LoadPhysical(const TExportPhysicalSide *side4A);
	bool LoadJudicial(const TExportJudicialSide *side4A);
	void Write(const TExternLawsuit &lawsuit);

protected:
	bool Load(const TExportSide4A *side);
};

TGroup *TExternSideWind4A::Clone()
{
	return new TExternSideWind4A;
}

bool TExternSideWind4A::LoadPhysical(const TExportPhysicalSide *side)
{
	if (Load(side))
		return true;

	// TODO: TExportPhysicalSide::Load()
	TCitizen citizen;
	bool found;

	if (any(side->ucn))
	{
		strcpy(citizen.ucn, side->ucn);
		citizen.ucnType = UCN_CITIZEN_UCN;
		found = citizen.Try();
	}
	else
	{
		citizen.ucnType = UCN_CITIZEN_CODE;
		found = false;
	}

	if (!found)
	{
		strcpy(citizen.name, side->name);
		strcpy(citizen.reName, side->reName);
		strcpy(citizen.family, side->family);
		strcpy(citizen.reFamily, side->reFamily);
		strcpy(citizen.country, side->country);
		strcpy(citizen.reCountry, side->reCountry);

		if (!citizen.Insert())
			return false;
	}

	strcpy(ucn, citizen.ucn);
	ucnType = citizen.ucnType;
	return true;
}

bool TExternSideWind4A::LoadJudicial(const TExportJudicialSide *side)
{
	if (Load(side))
		return true;

	// TODO: TExportJudicialSide::Load()
	TFirm firm;

	firm.ucnType = UCN_FIRM_CODE;
	strcpy(firm.name, side->name);
	strcpy(firm.bulstat, side->bulstat);
	strcpy(firm.country, side->country);

	if (!firm.Insert())
		return false;

	strcpy(ucn, firm.ucn);
	ucnType = firm.ucnType;
	return true;
}

void TExternSideWind4A::Write(const TExternLawsuit &lawsuit)
{
	*this << lawsuit;
	Insert();
}

bool TExternSideWind4A::Load(const TExportSide4A *side)
{
	involvement = (char) side->involvement;
	strcpy(externId, side->id);

	msql m(Name, "F_NO");
	m.Add(this, "F_EXTERN_ID");
	TQuery q(str(m));
	return q.Read();
}

// ----- import_archives -----------------------------------------------------
void import_archives(TWindow *parent)
{
	char name[SIZE_OF_PATH];

	if (GetOpenFileName(parent, name, "XML файлове", NULL, "XML", sizeof name, NULL, false))
	{
		TExportForArchive x4a;
		TEisFile x(0);
		char *const text = x.ReadAll(name);
		TCharPtr CP(text);
		TExternLawsuit lawsuit;
		TExternSideWind4A sideWind;

		log(LOG_IMPORT, CM_ARCH_IMPORT_ARCHIVES);

		try
		{
			int count = 0, wrong = 0;

			x4a.ReadStruct(text);

			for (int i = 0; i < x4a.lawsuit4As->Count(); i++)
			{
				const TExportLawsuit4A *lawsuit4A = (*x4a.lawsuit4As)[i];
				lawsuit4A->Save(lawsuit);

				if (lawsuit.Try("F_NO"))
				{
					if (lawsuit.Update("F_FORCE_DATE, F_SOURCE_KIND"))
						count++;
					else
						wrong++;
				}
				else if (lawsuit.Insert())
				{
					count++;

					for (int n = 0; n < lawsuit4A->physicalSides->Count(); n++)
						if (sideWind.LoadPhysical((*lawsuit4A->physicalSides)[n]))
							sideWind.Write(lawsuit);

					for (int n = 0; n < lawsuit4A->judicialSides->Count(); n++)
						if (sideWind.LoadJudicial((*lawsuit4A->judicialSides)[n]))
							sideWind.Write(lawsuit);
				}
				else
					wrong++;
			}

			message(wrong ? MB_ERROR : MB_INFO, "Успешно заредени дела: %d\nГрешки при запис: %d", count, wrong);
		}
		catch (TXMLError *x)
		{
			x->Handle();
			delete x;
		}
	}
}

// ----- TExportLawsuit4B ----------------------------------------------------
class TExportLawsuit4B : public TExportLawsuit4ArchBase
{
public:
	TExportLawsuit4B();

	void Load(const TExternLawsuit &lawsuit, const TArchive &archive);
	void Save(TArchive &archive) const;

	long linkNo;
	long archiveNo;
	long archiveYear;
	CDate archiveDate;
	char archiveRemark[SIZE_OF_ARCHIVE_TEXT];
	long keep;
	char archiveIndex[SIZE_OF_ALIAS];
};

TExportLawsuit4B::TExportLawsuit4B()
	: TExportLawsuit4ArchBase("Lawsuit4B")
{
	attribs.Add(
		new TXMLInt("F_ARCHIVE_NO", &archiveNo),
		new TXMLInt("F_LINK_NO", &linkNo),
		new TXMLDate("F_ARCHIVE_DATE", &archiveDate),
		new TXMLInt("F_ARCHIVE_YEAR", &archiveYear),
		NULL
	);

	Apply("*", XML_REQUIRED);

	attribs.Add(
		new TXMLString("F_ARCHIVE_INDEX", archiveIndex, sizeof archiveIndex),
		new TXMLInt("F_KEEP", &keep),
		new TXMLString("F_ARCHIVE_REMARK", archiveRemark, SIZE_OF_ARCHIVE_TEXT),
		NULL
	);

	Apply("*", XML_INCLUDED);
}

void TExportLawsuit4B::Load(const TExternLawsuit &lawsuit, const TArchive &archive)
{
	TExportLawsuit4ArchBase::Load(&lawsuit);

	archiveNo = archive.archiveNo;
	linkNo = archive.linkNo;
	archiveDate = archive.archiveDate;
	archiveYear = archive.archiveYear;
	strcpy(archiveIndex, archive.archiveIndex);
	keep = archive.keep;
	strcpy(archiveRemark, str(archive.archiveRemark));
}

void TExportLawsuit4B::Save(TArchive &archive) const
{
	archive.key.no = key.no;
	archive.key.year = key.year;
	archive.key.type = kind2type((char) key.kind);
	archive.linkNo = linkNo;
	archive.archiveNo = archiveNo;
	archive.archiveYear = archiveYear;
	archive.archiveDate = archiveDate;
	archive.archiveRemark.cpy(archiveRemark);
	archive.keep = keep;
	strcpy(archive.archiveIndex, archiveIndex);
}

// ----- TExportForBacking ---------------------------------------------------
class TExportForBacking : public TXMLGroup
{
public:
	TExportForBacking();

	TXArray<TExportLawsuit4B> *lawsuit4Bs;
};

TExportForBacking::TExportForBacking()
	: TXMLGroup("ExportForBacking"), lawsuit4Bs(new TEXPARRAY(Lawsuit4B))
{
	objects.Add(lawsuit4Bs);
	Apply("Lawsuit4B", XML_INCLUDED | XML_REQUIRED);
}

// ----- export_archived -----------------------------------------------------
#define archive OFFICE_GROUP_CAST(TArchive)

static void archive_no_year(mstr &m, TGroup *group) { m.printf("%ld/%ld", archive->archiveNo, archive->archiveYear); }

#undef archive

static offield archive_fields[] =
{
	{ "F_KIND",          keygroup_key_type },
	{ "F_NO_YEAR",       keygroup_key_no_year },
	{ "F_ARCHIVE",       archive_no_year },
	{ "F_LINK_NO",       NULL },
	{ "F_ARCHIVE_INDEX", NULL },
	{ "F_KEEP",          NULL },
	{ NULL, NULL }
};

void export_archived(TWindow *parent)
{
	TPeriodGroup period("T_PERIOD");

	period.minDate = WeekAgo;

	while (get_period(parent, &period))
	{
		msql m("T_ARCHIVE A JOIN T_EXTERN_LAWSUIT L ON L.F_NO = A.F_NO AND L.F_YEAR = A.F_YEAR AND L.F_TYPE = A.F_TYPE",
			"A.*, L.F_KIND, L.F_QUICK");
		m.AddRange("F_ARCHIVE_DATE", period.minDate, period.maxDate);
		m.AddZLong("F_EXCESS", 1);

		TQuery q(str(m));

		if (q.Read())
		{
			char name[SIZE_OF_PATH];
			TOffice office;

			if (office.Open("ExportBack.htm") &&
				GetSaveFileName(parent, name, "XML файлове", NULL, "XML", sizeof name, NULL, false))
			{
				TArchive archive;
				TExternLawsuit lawsuit;
				constant tr1start = office.Find("tr1 start");
				constant tr1archive = office.Find("tr1 archive");
				constant tr1final = office.Find("tr1 final");

				log(LOG_EXPORT, CM_ARCH_EXPORT_ARCHIVED);
				office.Emit(NULL, tr1start, null_fields, NULL);

				TWaitWindow wait(parent, WAIT);
				TExportForBacking x4b;
				TEisFile x(0);

				do
				{
					TExportLawsuit4B *l4b = new TExportLawsuit4B;

					lawsuit << q;
					office.Emit(tr1start, tr1archive, archive_fields, &lawsuit);
					archive << q;
					office.Emit(tr1archive, tr1final, archive_fields, &archive);
					l4b->Load(lawsuit, archive);
					x4b.lawsuit4Bs->Add(l4b);
					//
				} while (q.Read());

				x.OpenWrite(name);
				try
				{
					x4b.Write(x);
					office.Emit(tr1final, NULL, null_fields, NULL);
					office.Show(&wait);
				}
				catch (TXMLError *x)
				{
					x->Handle();
					delete x;
					office.Close();
				}
				x.Close();
			}

			break;  // get_period()
		}
		else
			error("Няма секретни дела, архивирани в посочения период.");
	}
}

// ---------------------------------------------------------------------------
void import_archived(TWindow *parent)
{
	char name[SIZE_OF_PATH];

	if (GetOpenFileName(parent, name, "XML файлове", NULL, "XML", sizeof name, NULL, false))
	{
		TExportForBacking x4b;
		TEisFile x(0);
		char *const text = x.ReadAll(name);
		TCharPtr CP(text);
		TArchive archive;

		log(LOG_IMPORT, CM_CLERK_IMPORT_ARCHIVED);

		try
		{
			int count = 0, wrong = 0;

			x4b.ReadStruct(text);

			for (int i = 0; i < x4b.lawsuit4Bs->Count(); i++)
			{
				const TExportLawsuit4B *lawsuit4B = (*x4b.lawsuit4Bs)[i];
				lawsuit4B->Save(archive);

				if (archive.Try("F_NO") ? archive.Update() : archive.InsertImported())
					count++;
				else
					wrong++;
			}

			message(wrong ? MB_ERROR : MB_INFO, "Успешно заредени дела: %d\nГрешки при запис: %d", count, wrong);
		}
		catch (TXMLError *x)
		{
			x->Handle();
			delete x;
		}
	}
}
#endif  // AREAL

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
void export_archives(TWindow *) { }
void import_archives(TWindow *) { }
void export_archived(TWindow *) { }
void import_archived(TWindow *) { }
#endif  // APPEAL || AREAL || MILITARY || REGIONAL
