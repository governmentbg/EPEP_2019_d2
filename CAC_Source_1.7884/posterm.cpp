#include "cac.h"

// ----- TPosText ------------------------------------------------------------
enum
{
	STX = '\02',
	ETX = '\03',
	ACK = '\06',
	NAK = '\025'
};

class TPosText : public mstr
{
public:
	TPosText() { }
	TPosText(char message);

	void cach(char c);
	void Complete();
};

TPosText::TPosText(char message)
{
	cach(STX);
	printf("%c%02ld", message, POSTERM_ECR);
}

void TPosText::cach(char c)
{
	if (c)
		mstr::cach(c);
	else
	{
		mstr::cach(' ');
		data[len() - 1] = '\0';
	}
}

void TPosText::Complete()
{
	cach(ETX);

	unsigned char lrc = '\0';

	for (size_t n = 1; n < len(); n++)
		lrc ^= data[n];

	cach(lrc);
}

// ----- TPosTerm ------------------------------------------------------------
enum
{
	APPROVAL = '1',
	ABORTED,
	ASK_AMMOUNT,
	SEND_AMMOUNT,
	AUTHORIZED
};

enum
{
	PURCHASE = '1',
	MAXREPLY = 256,
	ACK_TIMEOUT = 660,
	PAY_TIMEOUT = 300000,	// 5 minutes
	RETRIES = 3
};

long TPosTerm::initicks = 0;
HANDLE TPosTerm::hcomm[POSTERM_COUNT] = { INVALID_HANDLE_VALUE, INVALID_HANDLE_VALUE };

void TPosTerm::Initialize()
{
	initicks = GetTickCount();

	for (int port = 0; port < POSTERM_COUNT; port++)
		if (POSTERM_ACTIVE[port] && !InitPort(port))
			ClosePort(port);
}

void TPosTerm::Shutdown()
{
	for (int port = 0; port < POSTERM_COUNT; port++)
		ClosePort(port);
}

#if TESTVER
bool TPosTerm::Approval(TWindow *, int)
{
	return true;
}
#else  // TESTVER
bool TPosTerm::Approval(TWindow *parent, int port)
{
	TPosText send(APPROVAL);
	TPosText recv;

	return Send(parent, port, send, recv, ASK_AMMOUNT, NULL);
}
#endif  // TESTVER

bool TPosTerm::Error(int port, const char *text, const char *format, ...)
{
	mstr m;
	va_list ap;

	m.printf("%s: %s", POSTERM_PORT_NAME[port], text);
	log(LOG_POSTERM, 0, str(m));

	m.clear();
	va_start(ap, format);
	m.vprintf(format, ap);
	va_end(ap);
	return error("%s: %s", POSTERM_PORT_NAME[port], str(m));
}

#if TESTVER
long TPosTerm::Payment(TWindow *parent, int, double, char *transId, char *authId, char *cardId)
{
	TWaitWindow wait(parent, "Комуникация с ПОС терминала...");
	unsigned long ticks = (unsigned long) GetTickCount();

	Sleep(2000);
	sprintf(transId, "%015lu", ticks % 2 ? ticks : 0);
	sprintf(authId, "%06lu", ticks % 1000000);
	sprintf(cardId, "%04lu", ticks % 10000);
	return 100000 + POSTERM_ECR;
}
#else  // TESTVER
long TPosTerm::Payment(TWindow *parent, int port, double value, char *transId, char *authId, char *cardId)
{
	TPosText send(SEND_AMMOUNT);
	TPosText recv;
	sign result;

	send.cach(PURCHASE);
	send.printf("%012.0lf", value * 100.0);
	send.printf("%03lu", POSTERM_CURRENCY);
	send.printf("%04lu", POSTERM_OPERATOR);

	if ((result = Send(parent, port, send, recv, AUTHORIZED, "Въвеждане на карта и PIN код...")) < 0)
		return result;

	if (result)
	{
		long termId;

		if (result < 0)
			return result;

		*cardId = '\0';
		// note: +2 chars len for etx, lrc
		if (recv.len() < 54)
		{
			Error(port, "липсва сума", "в отговора на ПОС терминала липсва точната сума!");

			if (recv.len() < 42)
			{
				return Error(port, "липсва транзакция",
					"в отговора на ПОС терминала липсва идентификатор на транзакцията!");
			}
		}
		else
		{
			if (memcmp(str(recv) + 40, str(send) + 5, 12))
				return Error(port, "грешна сума", "сумата в отговора на ПОС терминала не съвпада с изпратената!");

			if (recv.len() >= 54 + LENGTH_OF_PAYMENT_CARD_ID && recv[52] == '0' &&
				recv[53] == '0' + LENGTH_OF_PAYMENT_CARD_ID)
			{
				memcpy(cardId, str(recv) + 54, LENGTH_OF_PAYMENT_CARD_ID);
				cardId[LENGTH_OF_PAYMENT_CARD_ID] = '\0';
			}
		}

		char tid[9];
		char *end;

		memcpy(tid, str(recv) + 5, 8);
		tid[8] = '\0';
		termId = strtol(tid, &end, 10);

		if (!termId || end != tid + 8)
			return Error(port, "грешен # терминал", "номера на ПОС терминала липсва или е невалиден!");

		// 2013:296 authId
		memcpy(authId, str(recv) + 19, LENGTH_OF_PAYMENT_AUTH_ID);
		authId[LENGTH_OF_PAYMENT_AUTH_ID] = '\0';
		memcpy(transId, str(recv) + 25, LENGTH_OF_BORIKA_TRANS_ID);
		transId[LENGTH_OF_BORIKA_TRANS_ID] = '\0';
		return termId;
	}

	return 0;
}
#endif  // TESTVER

bool TPosTerm::InitPort(int port)
{
	// 2016:075 // FILE_FLAG_NO_BUFFERING
	hcomm[port] = CreateFile(POSTERM_PORT_NAME[port], GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

	if (hcomm[port] == INVALID_HANDLE_VALUE)
		return Error(port, "грешка при Open", "грешка %lu при отваряне.", GETLASTERROR);

	DCB dcb;

	dcb.DCBlength = sizeof(DCB);

	if (!GetCommState(hcomm[port], &dcb))
		return Error(port, "грешка при Stat", "грешка %lu при четене на състоянието.", GETLASTERROR);

	dcb.fOutX = 0;
	dcb.fInX = 0;
	dcb.fNull = 0;
	dcb.BaudRate = CBR_1200;
	dcb.ByteSize = 7;
	dcb.StopBits = TWOSTOPBITS;
	dcb.Parity = EVENPARITY;

	if (!SetCommState(hcomm[port], &dcb))
		return Error(port, "грешка при SetStat", "грешка %lu при промяна на състоянието.", GETLASTERROR);

	return true;
}

void TPosTerm::ClosePort(int port)
{
	if (hcomm[port] != INVALID_HANDLE_VALUE)
	{
		if (!CloseHandle(hcomm[port]))
			Error(port, "грешка при Close", "грешка %lu при затваряне.", GETLASTERROR);

		hcomm[port] = INVALID_HANDLE_VALUE;
	}
}

sign TPosTerm::Send(TWindow *parent, int port, TPosText &send, TPosText &recv, char message, const char *prompt)
{
	mstr m;
	TWaitWindow wait(parent, "Комуникация с ПОС терминала...");
	int timeout = ACK_TIMEOUT;
	const char *text;

	send.Complete();

	for (int retry = 0; retry < RETRIES; retry++)
	{
		DWORD written;

		if (retry)
		{
			for (int sleep = 0; sleep < 19; sleep++)
				Sleep(30);

			m.clear();
		}

		if (!PurgeComm(hcomm[port], PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR))
		{
			m.printf("грешка %lu при изчистване", GETLASTERROR);
			text = "грешка при Clear";
			break;
		}

		if (!WriteFile(hcomm[port], str(send), send.len(), &written, NULL))
		{
			m.printf("грешка %lu при запис", GETLASTERROR);
			text = "грешка при Write";
		}
		else if (written < send.len())
		{
			m.printf("%lu записани байта вместо %u", written, send.len());
			text = "грешен брой written";
		}
		else
		{
			recv.clear();
			wait.SetText("Изчакване на отговор...");

			for (int sleep = 0; sleep < timeout; sleep += 30)
			{
				DWORD errors;
				COMSTAT comstat;

				Sleep(30);
				m.clear();

				if (retry == RETRIES && wait.Canceled())
					return -1;

				if (!ClearCommError(hcomm[port], &errors, &comstat))
				{
					m.printf("грешка %lu при вземане на статуса", GETLASTERROR);
					text = "грешка при Stat";
				}
				else if (recv.len() + comstat.cbInQue >= MAXREPLY)
				{
					m.printf("прекалено дълъг отговор, %lu байта", recv.len() + comstat.cbInQue);
					text = "дълъг отговор";
				}
				else
				{
					bool result;

					do
					{
						DWORD read;
						char buffer[MAXREPLY];

						// 2016:075 LPR: don't read 0 bytes, just in case
						if (comstat.cbInQue)
						{
							mstr w;

							w.printf("Прочитане на %lu байта...", comstat.cbInQue);
							wait.SetText(str(w));
							result = ReadFile(hcomm[port], buffer, comstat.cbInQue, &read, NULL);
						}
						else
						{
							result = TRUE;
							read = 0;
						}

						if (!result)
						{
							m.printf("грешка %lu при четене", GETLASTERROR);
							text = "грешка при Read";
						}

						if (read)
						{
							const char *s = buffer;

							for (int r = 0; r < read; s++, r++)
							{
								switch (*s)
								{
									case ACK :
									{
										if (prompt && retry < RETRIES)
										{
											sleep = 0;
											retry = RETRIES;
											timeout = PAY_TIMEOUT;
											wait.SetText(prompt);
											wait.AllowCancel(false);
										}
										break;
									}
									case NAK :
									{
										m.cat("отрицателно потвърждение");
										text = "получен NAK";
										goto resend;
									}
									case STX :
									{
										if (!TransmitCommChar(hcomm[port], ACK))
										{
											m.printf("грешка %lu при потвърждение", GETLASTERROR);
											text = "грешка при Xmit";
										}
										break;
									}
								}

								recv.cach(*s ? *s : ' ');
							}

							for (s = str(recv); (s = strchr(s, STX)) != NULL; s++)
							{
								if (s[1] == ABORTED)
								{
									recv.delead(s);

									// 2017:321 pre-try to complete abortion
									if (recv.len() < 7)
									{
										Sleep(100);  // 15 more bytes

										if (ClearCommError(hcomm[port], &errors, &comstat) &&
											comstat.cbInQue &&
											ReadFile(hcomm[port], buffer, comstat.cbInQue, &read, NULL))
										{
											recv.catn(buffer, read);
										}
									}

									if (recv.len() < 7)
									{
										m.cat("прекъсване, липсва основание");
										text = str(m);
									}
									else
									{
										const char *message;
										char *end;

										switch (strtol(str(recv) + 4, &end, 16))
										{
											case 0x17 : message = "потребителско прекъсване"; break;
											case 0x18 : message = "времето свърши"; break;
											case 0x08 : message = "отхвърляне от БОРИКА"; break;
											case 0x0D : message = "различна валута"; break;
											case 0x30 : message = "не е кредитна карта"; break;
											default : message = NULL;
										}

										if (message && end == str(recv) + 6)
										{
											m.cat(message);
											text = message;
										}
										else
										{
											if (isxdigit(recv[4]) && isxdigit(recv[5]))
											{
												m.printf("прекъсване, основание %c%c", recv[4],
													recv[5]);
											}
											else
											{
												m.printf("прекъсване, основание 0x%02X 0x%02X",
													recv[4], recv[5]);
											}

											text = str(m);
										}
									}

									goto failed;
								}

								const char *etx;

								if (s[1] == message && s[2] == send[2] && s[3] == send[3] &&
									(etx = strchr(s + 4, ETX)) != NULL && etx[1])
								{
									unsigned char lrc = '\0';

									for (const char *data = s + 1; data <= etx; data++)
										lrc ^= *data;

									if (etx[1] != lrc)
									{
										m.printf("невалидна контролна сума: 0x%2X вместо 0x%02X",
											etx[1], lrc);
										text = "грешна контролна сума";
										goto failed;
									}

									recv.delead(s);
									return true;
								}
							}

							sleep -= 30;
							comstat.cbInQue -= read;
						}
					} while (result && comstat.cbInQue);
				}
			}

			resend:
		}
	}

	if (!any(m))
	{
		m.printf("няма валиден отговор от терминала, %lu получени байта", recv.len());
		text = "няма отговор";

		if (recv.len())
		{
			for (int i = 0; i < 10 && i < recv.len(); i++)
				m.printf("%c 0x%02X", i ? ',' : ':', recv[i]);

			if (recv.len() > 10)
				m.cat("..");
		}
	}

failed:
	wait.ShowWindow(SW_HIDE);
	return Error(port, text, "%s.", str(m));
}
