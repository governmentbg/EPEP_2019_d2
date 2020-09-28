#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define EISTR 0x100

typedef struct
{
	int osidicsid;
	int osidicvsq;
	char osidicvat[EISTR];
	char osidickrt[EISTR];
	char osidicopi[EISTR];
	int osidictip;
	int osidicsas;
	int osidicniv;
	char osidickls[EISTR];
	int osidictbl;
	int osidicact;
	char osidicotd[EISTR];
	char osidicdod[EISTR];
	int dokadmsidvkl;
	int dokadmsidizk;
	char wrevav[EISTR];
} RECH;

#define RECHC 20000
static RECH rechs[RECHC];
static unsigned rechc = 0;

static int rech_cmp(const void *rech1, const void *rech2)
{
	return ((RECH *) rech1)->osidicsid - ((RECH *) rech2)->osidicsid;
}

static RECH *find_rech(RECH *rech)
{
	return (RECH *) bsearch(rech, rechs, rechc, sizeof(RECH), rech_cmp);
}

static RECH *find_rech_sid(int osidicsid)
{
	RECH rech;
	rech.osidicsid = osidicsid;
	return find_rech(&rech);
}

static RECH *find_rech_vat(const char *osidicvat, int osidictip)
{
	RECH *rech;
	int i;

	for (i = 0, rech = rechs; i < rechc; i++, rech++)
		if (!strcmp(rech->osidicvat, osidicvat) && rech->osidictip == osidictip)
			return rech;

	return NULL;
}

typedef struct
{
	int osinomsid;
	int osinomvsq;
	int osinomvod;
	int osinompod;
	int osinomnmr;
	int osinomact;
	char osinomotd[EISTR];
	char osinomdod[EISTR];
	int dokadmsidvkl;
	int dokadmsidizk;
	char wrevav[EISTR];
} NOME;

#define NOMEC 40000
static NOME nomes[NOMEC];
static unsigned nomec = 0;

static char *fetch_str(char *s, char *dst)
{
	char *tab;
	int quote = 0;

	assert(s);
	tab = strchr(s, '\t');

	if (tab)
		*tab++ = '\0';

	if (*s == '"')
	{
		s++;
		quote = 1;
	}

	while (*s <= 127 ? isspace(*s) : *s == 160)
		assert(*++s);

	assert(strlen(s) < EISTR);
	strcpy(dst, s);

	for (s = dst; (s = strchr(s, '\'')) != NULL; s++)
		*s = '"';

	if (quote)
	{
		for (s = dst; (s = strstr(s, "\"\"")) != NULL; s++)
			memmove(s, s + 1, strlen(s));

		if (*dst && *(s = dst + strlen(dst) - 1) == '"')
			*s = '\0';
	}

	return tab;
}

static char *fetch_int(char *s, int *dst)
{
	char buffer[EISTR];
	char *tab = fetch_str(s, buffer);

	// 2014:016 LPR: bool support
	if (!strcmp(buffer, "TRUE"))
		*dst = 1;
	else if (!strcmp(buffer, "FALSE"))
		*dst = 0;
	// else if (!strcmp(buffer, "ENOENT"))
	//	*dst = ENOENT;
	else
	{
		assert(isdigit(*buffer));
		*dst = atoi(buffer);
	}

	return tab;
}

#define prosta 2
#define nomenklatura 3
#define vsistemna 6
#define vedomstvena 7
#define grj 8
#define mpppne 23
#define str_ars	1097
#define str_sls	1104
#define str_pbc	13626
// 2014:017
#define tip_nflgrp 16
#define tip_nflvid 17
#define tip_mo_zvn 18

#define date_value(year, month, day) ((year) * 10000 + (month) * 100 + (day))
#define act_text(act) ((act) ? "1" : "NULL")

// 2013:003
#define obstina 1191
#define oblast 1192
#define nas_miasto 1190
#define raion 1193

// 2014:016
#define str_vaa 2386
#define str_vmp_bez_to 12661

// 2014:178 FIX: added noms used only via sbe_desc; 2015:056 +229
// 2016:111 +213
static int izp_noms[] = { 190, 191, 192, 193, 203, 204, 205, 206, 207, 208, 209, 210, 211, 214, 215, 216, 217, 218, 219, 220,
	221, 223, 224, 225, 228, 229, 230, 231, 234, 238, 240, 241, 242, 244, 246, 253, 255, 256, 272, 308, 309, 310, 311, 312,
	313, 314, 1167, 1665, 1667, 1632, 1724, 1865, 8616, 8617, 8618, 8619, 8624, 12478, 14113, 14125, 213, 0 };

static int tip_depart(const RECH *rech)
{
	return (rech->osidictip >= str_ars && rech->osidictip <= str_sls) || rech->osidictip == str_pbc;
}

static int tip_salone(const RECH *rech)
{
	return ((rech->osidictip == mpppne || tip_depart(rech)) && rech->osidicsas == prosta) ||
		// allow nomenklatura, LAMEISPP
		((rech->osidictip == nas_miasto || rech->osidicsid == str_vaa || rech->osidicsid == str_vmp_bez_to) &&
			(rech->osidicsas == prosta || rech->osidicsas == nomenklatura));
}

int main(void)
{
	// init
	static char buffer[0x1000];
	time_t timer = time(NULL);
	struct tm *tblock = localtime(&timer);
	int today = date_value(tblock->tm_year + 1900, tblock->tm_mon + 1, tblock->tm_mday);
	FILE *rechnik = fopen("rechnik.txt", "r");
	FILE *osinom = fopen("osinom.txt", "r");
	FILE *eispp = fopen("eispp.sql", "w");
	FILE *edict = fopen("edict.sql", "w");
	FILE *crime = fopen("crime.sql", "w");
	FILE *depart = fopen("depart.sql", "w");
	FILE *place = fopen("place.sql", "w");
	FILE *ignore = fopen("ignore.txt", "w");
	int i;
	RECH *rech;
	NOME *nome;

	printf("%d\n", today);

	assert(rechnik);
	assert(osinom);
	assert(eispp);
	assert(edict);
	assert(crime);
	assert(depart);
	assert(place);
	assert(ignore);

	// read
	while (fgets(buffer, sizeof buffer, rechnik) != NULL)
	{
		char *s = buffer;

		assert(rechc < RECHC);
		rech = rechs + rechc;

		s = fetch_int(s, &rech->osidicsid);
		s = fetch_int(s, &rech->osidicvsq);
		s = fetch_str(s, rech->osidicvat);
		s = fetch_str(s, rech->osidickrt);
		s = fetch_str(s, rech->osidicopi);
		s = fetch_int(s, &rech->osidictip);
		s = fetch_int(s, &rech->osidicsas);
		s = fetch_int(s, &rech->osidicniv);
		s = fetch_str(s, rech->osidickls);
		s = fetch_int(s, &rech->osidictbl);
		s = fetch_int(s, &rech->osidicact);
		s = fetch_str(s, rech->osidicotd);
		s = fetch_str(s, rech->osidicdod);
		s = fetch_int(s, &rech->dokadmsidvkl);
		s = fetch_int(s, &rech->dokadmsidizk);
		s = fetch_str(s, rech->wrevav);

		assert(s == NULL);

	#if 20140121
		if (rech->osidictip == vsistemna && rech->osidicsas == nomenklatura)
		{
			int *i;

			for (i = izp_noms; *i; i++)
				if (*i == rech->osidicsid)
					break;

			if (!*i)
			{
				fprintf(ignore, "nom %d: vsistemna !izp\n", rech->osidicsid);
				continue;
			}
		}
	#endif  // 20140121

		if (((rech->osidictip == vsistemna || rech->osidictip == grj || rech->osidictip == vedomstvena ||
			rech->osidictip == obstina || rech->osidictip == oblast || rech->osidictip == raion ||
			rech->osidictip == tip_nflgrp || rech->osidictip == tip_nflvid /*|| rech->osidictip == tip_mo_zvn*/) &&
			(rech->osidicsas == prosta || rech->osidicsas == nomenklatura)) || tip_salone(rech))
		{
			RECH *prev = find_rech(rech);

			if (prev)
			{
				if (prev->osidicvsq < rech->osidicvsq)
					memcpy(prev, rech, sizeof(RECH));
			}
			else
				rechc++;
		}
	}

	while (fgets(buffer, sizeof buffer, osinom))
	{
		char *s = buffer;

		assert(nomec < NOMEC);
		nome = nomes + nomec;

		s = fetch_int(s, &nome->osinomsid);
		s = fetch_int(s, &nome->osinomvsq);
		s = fetch_int(s, &nome->osinomvod);
		s = fetch_int(s, &nome->osinompod);
		s = fetch_int(s, &nome->osinomnmr);
		s = fetch_int(s, &nome->osinomact);
		s = fetch_str(s, nome->osinomotd);
		s = fetch_str(s, nome->osinomdod);
		s = fetch_int(s, &nome->dokadmsidvkl);
		s = fetch_int(s, &nome->dokadmsidizk);
		s = fetch_str(s, nome->wrevav);

		assert(s == NULL);

		rech = find_rech_sid(nome->osinomvod);

		if (!rech)
			fprintf(ignore, "nom %d: rech !vod %d\n", nome->osinomsid, nome->osinomvod);
		else
			nomec++;
	}

	assert(fclose(rechnik) == 0);
	assert(fclose(osinom) == 0);

	// write
	for (i = 0, nome = nomes; i < nomec; i++, nome++)
	{
		rech = find_rech_sid(nome->osinompod);

		if (!rech)
			fprintf(ignore, "nom %d: rech !pod %d\n", nome->osinomsid, nome->osinompod);
		else
		{
			if (!tip_salone(rech) || rech->osidictip == tip_nflgrp || rech->osidictip == tip_nflvid ||
				rech->osidictip == tip_mo_zvn)
			{
				if (nome->osinomact != rech->osidicact)
					fprintf(ignore, "nom %d: nom act = %d, rech act = %d%d\n", nome->osinomact != rech->osidicact);

				fprintf(eispp, "INSERT INTO T_EISPP_VAL VALUES (%d, %d, '%s', %s, '%s');\n", nome->osinomvod,
					nome->osinompod, rech->osidickrt, act_text(nome->osinomact), rech->osidicvat);
				//fprintf(eispp, "%-7d%-7d%-255s%d%-40s\n", nome->osinomvod, nome->osinompod, rech->osidickrt,
				//	nome->osinomact, rech->osidicvat);
			}
		}
	}

	for (i = 0, rech = rechs; i < rechc; i++, rech++)
	{
		int year, month, day;
		int active;

		assert(sscanf(rech->osidicdod, "%d.%d.%d", &day, &month, &year) == 3);
		active = date_value(year, month, day) >= today;

		if (active != rech->osidicact)
			fprintf(ignore, "rech %d: osidicact = %d, compdtact=%d\n", rech->osidicsid, rech->osidicact, active);

		if (rech->osidictip == mpppne)
		{
			fprintf(crime, "INSERT INTO T_EISPP_CRIME VALUES (%d, '%s', %s, '%s');\n", rech->osidicsid,
				rech->osidickrt, act_text(rech->osidicact), rech->osidicvat);
			//fprintf(crime, "%-7d%-255s%-40s%05d\n", rech->osidicsid, rech->osidickrt, rech->osidicvat);
		}
		else if (tip_depart(rech))
		{
			fprintf(depart, "INSERT INTO T_EISPP_DEPART VALUES (%d, %d, '%s', '%s', %s, '%s');\n", rech->osidictip,
				rech->osidicsid, rech->osidicopi, rech->osidickls, act_text(rech->osidicact), rech->osidicvat);
			//fprintf(depart, "%-7d%-7d%-255s%-40s\n", rech->osidictip, rech->osidicsid, rech->osidicopi,
			//	rech->osidicvat);
		}
		else if (rech->osidictip == nas_miasto)
		{
			RECH *obst;
			//RECH *obla;
			char osidicvat[EISTR];
			char *s;

			strcpy(osidicvat, rech->osidicvat);
			assert((s = strchr(osidicvat, '_')) != NULL);
			*s = '\0';
			obst = find_rech_vat(osidicvat, obstina);

			//for (s = osidicvat; isalpha(*s); s++);
			//assert(isdigit(*s));
			//*s = '\0';
			//obla = find_rech_vat(osidicvat, oblast);

			if (!obst)
				fprintf(ignore, "rech %d: !obshtina %s\n", rech->osidicsid, osidicvat);
			//else if (!obla)
			//	fprintf(ignore, "rech %d: !oblast %s\n", rech->osidicsid, osidicvat);
			else
			{
				fprintf(place, "INSERT INTO T_EISPP_PLACE VALUES (%d, %d, '%s', %s, %s, '%s');\n", obst->osidicsid,
					rech->osidicsid, rech->osidickrt, rech->osidickls, act_text(rech->osidicact), rech->osidicvat);
			}
		}
		else
		{
			fprintf(edict, "INSERT INTO T_EISPP_DICT VALUES (%d, %d, '%s', %s, '%s');\n", rech->osidicsas,
				rech->osidicsid, rech->osidickrt, act_text(rech->osidicact), rech->osidicvat);
			//fprintf(edict, "%-7d%-7d%-255s%d%-40s\n", rech->osidicsas, rech->osidicsid, rech->osidickrt,
			//	rech->osidicact, rech->osidicvat);
		}
	}

	// down
	assert(fclose(place) == 0);
	assert(fclose(depart) == 0);
	assert(fclose(crime) == 0);
	assert(fclose(ignore) == 0);
	assert(fclose(edict) == 0);
	assert(fclose(eispp) == 0);

	printf("rechc = %u, nomec = %d\n", rechc, nomec);
	return 0;
}
