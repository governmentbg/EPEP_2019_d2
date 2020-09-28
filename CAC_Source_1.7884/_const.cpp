#include "cac.h"

// ----- KIND_ ---------------------------------------------------------------
// ----- RESULT_ -------------------------------------------------------------

constant EMPTY_STRING = "" ;
constant STAR_STRING = "*" ;
constant Q3_STRING = "???" ;
constant DASH_STRING = "-" ;

constant JUDGE_STRING = "докл" ;
constant SECRETAR_STRING = "ск" ;
constant PROSECUTOR_STRING = "п" ;
constant JUROR_STRING = "з" ;
constant REJUROR_STRING = "з" ;
constant JUDGEX_STRING = "с" ;
constant REJUDGEX_STRING = "с" ;
constant JURORX_STRING = "з" ;

// 2015:120 +D where h

# if INSTANCE
#   if APPEAL  // 25.jul.2000
constant RESULT_UNMOVEABLES =
  "hD"
   ;  //
constant RESULT_STOPPEDS =
  "S"
   ;  // Do not forget RESULT_LEFTS_OR_STOPPEDS !!!
constant RESULT_CANCELEDS =
  "O"
  "P"
  "c"
  "Б"  // 14.jun.2000  Със споразумение
   ;  // Do not forget RESULT_CANCELEDS_OR_RETURNEDS
	// and RESULT_WITH_DECS_OR_CANCELEDS !!!
constant RESULT_WITH_DECISIONS =
  "W"
  "Z"
  "d"
  "j"
  "p" // обявено за решаване
  "y"
  "T"
  "Г"
  "Д"
  "И"
  "Й"
  "Л"
  "П"
  "У"
  "Ф"
  "Ц"
  "Ч"
  "z"		// 2011:173
  //"Щ"	// 2011:173 duplicate, comout
  "v"		// 2011:173
  //"Ъ"	// 2011:173 duplicate, comout
  "C"
  "E"
  "G"
  "H"
	"0"
	"п"  // С определение по чл.37 ГПК
  "с"  // 2015:080
  "т"  // 2015:080
   ; // Do not forget RESULT_WITH_DECS_OR_CANCELEDS !!!
constant RESULT_RETURNEDS =
//  "r"  // 09.jan.2001
  "B"
   ;  // Do not forget RESULT_CANCELEDS_OR_RETURNEDS !!!
constant RESULT_CANCELEDS_OR_RETURNEDS =
  "O"
  "P"
  "c"
  "B"
  "Б"  // 14.jun.2000  Със споразумение
   ;
constant RESULT_M_COL_FINISHEDS =
  "A"
  "Z"
  "d"  // С определение, приключващо делото  // 01.sep.2000
  "P"
  "a"
  "c"
  "p"
  "y"
   ;
constant RESULT_M_COL_NOT_FINISHEDS =
  "1"
  "2"
  "3"
  "4"
  "5"
  "6"
  "I"
  "M"
  "N"
  "m"
  "l"
  "o"
  "L"
  "S"
  "hD"
  "s"
  "7"
  "8"
  "9"
  "б"
  "В"  // 09.feb.2000
  "б"  // 25.jul.2000
  "K"  // Отложено поради нередовно призоваване  // 25.jul.2000
  "Q"  // Отложено на осн. чл.107, ал.2 ГПК  // 25.jul.2000
  "V"  // Отложено с прил. чл.65 ГПК  // 25.jul.2000
  "F"  // Отложено за нови доказателства  // 27.jul.2000
  "Ў"  // Отложено с налагане на глоба  // 25.may.2004
  "Ѕ"  // С определение за отвод
  "І"  // С разпореждане за отвод
   ;   // *  RESULT_LEFTS + RESULT_STOPPEDS + "hs"
constant RESULT_LEFT_RECONCILES =
  "M"
  "N"
  "m" ;
constant RESULT_WITH_DECS_OR_CANCELEDS =
  "W"
  "Z"
  "d"
  "j"
  "p"
  "y"
  "T"
  "Г"
  "Д"
  "И"
  "Й"
  "Л"
  "П"
  "У"
  "Ф"
  "Ц"
  "Ч"
  "z"		// 2011:173
  //"Щ"	// 2011:173 duplicate, comout
  "v"		// 2011:173
  //"Ъ"	// 2011:173 duplicate, comout
  "C"
  "E"
  "G"
  "H"
  "O"
  "P"
  "c"
  "0"
  "Б"
  "с"  // 2015:080
  "т"  // 2015:080
   ;
constant RESULT_DECISIONS_CLSESBK =
  "Z"
   ;
constant RESULT_DEFINITIONS_CLSESBK =
  "J"
  "S"
   ;
constant RESULT_DEFIN_2ND_TERMIN_CLSESBK =
  "Г"
  "Д"
  "И"
  "Й"
  "Л"
  "П"
  "У"
  "Ф"
  "Ц"
  "Ч"
  "C"
  "E"
  "G"
  "H"
  "z"		// 2011:173
  //"Щ"	// 2011:173 duplicate, comout
  "v"		// 2011:173
  //"Ъ"	// 2011:173 duplicate, comout
   ;
constant RESULT_TO_UP_LOOK_INTO_RETURNEDS =  // 16.jun.2000
  "B"  // Прекратено - за доразследване
   ;
constant RESULT_CONFIRMATIONS =
  //"Ъ"  // Потвърждава          // RESULT_CONFIRMATION; 2011:173 duplicate, comout
  "v"  // Потвърждава          // RESULT_CONFIRMATION_1
  "С"  // Уважава              // RESULT_GRANT
  "q"  // Уважава              // RESULT_GRANT_1
   ;
constant RESULT_CHANGINGS =
  "Ь"  // Изменя               // RESULT_CHANGE
  "w"  // Изменя               // RESULT_CHANGE_1
   ;
constant RESULT_REJECTS =
  //"Щ"  // Отменя               // RESULT_ABOLISH; 2011:173 duplicate, comout
  "z"  // Отменя               // RESULT_ABOLISH_1
  "Т"  // Без уважение         // RESULT_NOT_GRANT
  "u"  // Оставя без уважение  // RESULT_NOT_GRANT_1
   ;
constant RESULT_SLOWDOWN_CONFIRMATIONS =
  "1"  //  Указва
  "2"  //  Задължава
   ;
constant RESULT_SLOWDOWN_REJECTS =
  "u"  // Оставя без уважение
  "Ш"  // Прекратява
  "К"  // Оставя без разглеждане	// 2016:025
   ;
constant RESULT_SLOWDOWN_REJECT_NOT_GR =   // 20060417
  "u"  // Оставя без уважение
  ;
constant RESULT_SLOWDOWN_REJECT_CEASED =   // 20060417
  "Ш"  // Прекратява
  "К"  // Оставя без разглеждане	// 2016:025
   ;
constant RESULT_IRE_UNMOVEABLES =  // 27.apr.2001
  "а"  //  В срок - без движение
   ;
constant RESULT_IRE_SENDS =  // 03.may.2001
  "Ж"  // Изпратен
  "д"  // В срок - изпратени преписи
   ;
constant RESULT_IRE_RETURNEDS =  // 03.may.2001
  "Н"  // Върнат
  "Х"  // Върнат като просрочен
  "Ь"  // Върнат като нередовен
  "Ю"  // Върнат като недопустим
  "в"  // Върнат - оттеглен
  "з"  // Върнат
   ;

#   else  // of APPEAL  // 25.jul.2000
constant RESULT_UNMOVEABLES =
  "hD"
   ;
constant RESULT_STOPPEDS =
  "S"
   ;  // *  Do not forget RESULT_LEFTS_OR_STOPPEDS !!!
constant RESULT_CANCELEDS =
  "O"
  "P"
  "c"
  "Б"  // 14.jun.2000  Със споразумение
	"A"  // Прекратено производство по регистрация  // 22.jan.2001
	"†"  // Присъединено към друго дело 2015:104
	 ;   // *  Do not forget RESULT_CANCELEDS_OR_RETURNEDS and
			 // RESULT_WITH_DECS_OR_CANCELEDS !!!
constant RESULT_LEFTS_1ST_SESS =  // 24.sep.2001  // AREAL COURT
	"L"  // Отложено в I-во заседание
	 ;
constant RESULT_WITH_DECISIONS =
// =========== DO NOT INCLUDE '9' IN THIS SET !!! ====================
  "W"
  "Z"
  "d"
  "j"
  "p"
  "y"
  "T"
  "Г"
  "Д"
  "И"
  "Й"
  "Л"
  "П"
  "У"
  "Ф"
  "Ц"
  "Ч"
  "z"		// 2011:173
  //"Щ"	// 2011:173 duplicate, comout
  "v"		// 2011:173
  //"Ъ"	// 2011:173 duplicate, comout
  "C"
  "E"
  "F"
  "G"
  "H"
  "K"
  "0"  // С протокол
  "4"  // С разпореждане, приключващо делото  // 24.oct.2000
  "5"  // С готово разпореждане  // 24.oct.2000
  "6"  // С готов протокол  // 24.oct.2000
  "Љ"  // С присъда и споразумение  // 25.jul.2001
  "ґ"  // С определение по кумулация  // 08.nov.2004
//"ј"  // 2010:350 LRQ; 2013:072 TRQ/LRQ: comout
  "9"  // 2011:020 IRQ
  "Q"  // 2011:020 IRQ
  "ј"  // 2013:336 IRQ
  "с"  // 2015:080
  "т"  // 2015:080
   ;
  // Do not forget RESULT_WITH_DECS_OR_CANCELEDS !!!
constant RESULT_RETURNEDS =
  "r"  // 09.jan.2001 commented, 28.feb.2001 back again
  "B"
   ;  // Do not forget RESULT_CANCELEDS_OR_RETURNEDS !!!
constant RESULT_CANCELEDS_OR_RETURNEDS =
  "O"
  "P"
  "c"
  "r"  // 09.jan.2001 commented, 28.feb.2001 back again
  "B"
  "Б"  // 14.jun.2000  Със споразумение
  "A"  // Прекратено производство по регистрация  // 22.jan.2001
  "†"  // Присъединено към друго дело 2015:104
   ;
constant RESULT_M_COL_FINISHEDS =
  "A"
  "Z"
  "d"  // С определение, приключващо делото  // 01.sep.2000
  "P"
  "a"
  "c"
  "p"
  "y"
  "†"  // Присъединено към друго дело 2015:104
   ;
constant RESULT_M_COL_NOT_FINISHEDS =
  "1"
  "2"
  "I"
  "M"
	"N"
  "m"
  "l"
  "o"
  "L"
  "S"
  "hD"
  "s"
//                     "78"  // 26.jul.2001
  "б"
	"В"
  "Ы"  // Отложено в прил?чл.65 ГПК  // 20.apr.2001 (23.may.2001)
  "Ў"  // Отложено с налагане на глоба  // 09.jun.2003
  "Ѕ"  // С определение за отвод
  "І"  // С разпореждане за отвод
#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL
  //"J"  // 2010:089 LRQ; 2010:089 SRQ: +AREAL; 2011:332 LRQ: comout
#endif  // ADMIN
   ;  // *  RESULT_LEFTS + RESULT_STOPPEDS + "hs"

constant RESULT_LEFT_RECONCILES =
  "M"
  "N"
	"m"
	 ;  // *
constant RESULT_WITH_DECS_OR_CANCELEDS =
  "W"
  "Z"
  "d"
  "j"
  "p"
  "y"
  "T"
  "Г"
  "Д"
  "И"
  "Й"
  "Л"
  "П"
  "У"
  "Ф"
  "Ц"
  "Ч"
  "z"		// 2011:173
  //"Щ"	// 2011:173 duplicate, comout
  "v"		// 2011:173
  //"Ъ"	// 2011:173 duplicate, comout
  "C"
  "E"
  "F"
  "G"
  "H"
  "K"
  "O"
  "P"
  "c"
  "0"
  "Б"  // 04.feb.2000
//  "3"  // Прекратено за доразследване по чл.241 // 20.oct.2000,
		  // 28.feb.2001 commented (joined with 'r')
  "4"  // С разпореждане, приключващо делото  // 24.oct.2000
  "5"  // С готово разпореждане  // 24.oct.2000
  "6"  // С готов протокол  // 24.oct.2000
  "A"  // Прекратено производство по регистрация  // 22.jan.2001
  "Љ"  // С присъда и споразумение  // 25.jul.2001
  "с"  // 2015:080
  "т"  // 2015:080
  "†"  // Присъединено към друго дело 2015:104
   ;
constant RESULT_DECISIONS_CLSESBK =
  "Z"
   ;
constant RESULT_DEFINITIONS_CLSESBK =
  "J"
  "S"
   ;
constant RESULT_DEFIN_2ND_TERMIN_CLSESBK =
  "Г"
  "Д"
  "И"
  "Й"
  "Л"
  "П"
  "У"
  "Ф"
  "Ц"
  "Ч"
  "C"
  "E"
  "F"
  "G"
  "H"
  "K"
  "z"		// 2011:173
  //"Щ"	// 2011:173 duplicate, comout
  "v"		// 2011:173
  //"Ъ"	// 2011:173 duplicate, comout
   ;
constant RESULT_FIRM_CANCELEDS =
  "й"  // Прекратяване по решение на съда
  "л"  // Прекратяване поради смърт на собственик
  "п"  // Прекратяване поради преобразуване в друг вид
  "ф"  // Прекратяване поради разделяне на дружества
  "ц"  // Прекратяване поради вливане в друго дружество
  "ч"  // Прекратяване поради сливане с друго дружество
  "1"  // Прекратяване поради прехвърляне на предприятие
//  "2"  // Прекратяване по решение на общото събрание  // 20.sep.2001
//  "ш"  // Прехвърляне на предприятие  // 03.aug.2000
  "3"  // Поемане по наследство  //20.apr.2001
   ;

constant RESULT_FIRM_DECISION_CHANGES =  // 03.aug.2000
  "ж"  // Промяна в обстоятелствата
  "ш"  // Прехвърляне на предприятие
  "ю"  // Отказ за промяна в обстоятелствата
  "щ"  // Ликвидация
  "м"  // Прекратяване произв.по ликвидация и възобнов.на дейността
  "о"  // Прекратяване произв.по ликвидация поради откр.произв.несъст.
  "2"  // Регистрация след отказ  // 17.sep.2001
  "Ё"  // RESULT_FIRM_DEFINITION_FOR_TR // С определение, подлежащо
						    // на вписване в ТР  21.feb.2005
  "ё"  // RESULT_FIRM_DECREE_FOR_TR  // С разпореждане, подлежащо
						    // на вписване в ТР  21.feb.2005

  //it's not a result of decision "a"  // Изпратено по подсъдност след регистрация  // 20060321
  "Ј"  // Изпратено по подсъдност след регистрация  // 20060414  - резултат от краен документ
   ;
constant RESULT_FIRM_REFUSED_CHANGES = // 03.apr.2001
  "ю"  // Отказ за промяна в обстоятелствата
   ;
constant RESULT_FIRM_REGISTRATIONS =  // 12.oct.2000
  "я"  // RESULT_FIRM_REGISTRATION
  "г"  // RESULT_BRANCH_REGISTRATION
  "ь"  // RESULT_REGISTRATION_DENIAL
   ;
constant RESULT_FIRM_LEGAL_FINALIZEDS =  // 04.jan.2001
//  "c"  // RESULT_CANCELED  // 26.jan.2001 removed
  "s"  // RESULT_SEND
  "A"  // Прекратено производство по регистрация  // 22.jan.2001
   ;
constant RESULT_FIRM_LEGAL_CANCELEDS =  // 24.jan.2001
//  "c"  // RESULT_CANCELED  // 26.jan.2001 removed
  "s"  // RESULT_SEND
  "A"  // Прекратено производство по регистрация
   ;
constant RESULT_FIRM_CL_S_BK_EXCLUDEDS =  // 22.jan.2001
  "V"  // Прекратено производство по промяна
  "a"  // Изпратено по подсъдност след регистрация  // 2006XXXX
   ;
constant RESULT_FIRM_CLSBK_MUST_INCLUDE =  // 22.jan.2001
  "c"  // RESULT_CANCELED                            // don't change
  "s"  // RESULT_SEND
  "A"  // Прекратено производство по регистрация
  "†"  // Присъединено към друго дело 2015:104
//20050322  - in comment ->
//  "Ё"  // RESULT_FIRM_DEFINITION_FOR_TR // С определение, подлежащо
//                                        // на вписване в ТР  21.feb.2005
//  "ё"  // RESULT_FIRM_DECREE_FOR_TR  // С разпореждане, подлежащо
//                                        // на вписване в ТР  21.feb.2005
   ;
constant RESULT_FIRM_FINISHEDS = "яг2" "йлпфцч1щъ" "ь" "Ј"
   ;
constant RESULT_TO_UP_LOOK_INTO_RETURNEDS =  // 16.jun.2000
  "r"  // Върнато - за доразследване  // 09.jan.2001 commented,
						  // 28.feb.2001 back again
  "B"  // Прекратено - за доразследване
//  "3"  // Прекратено за доразследване по чл.241 // 20.oct.2000,
		  // 28.feb.2001 commented (joined with 'r')
   ;
constant RESULT_CONFIRMATIONS =
  //"Ъ"  // Потвърждава          // RESULT_CONFIRMATION; 2011:173 duplicate, comout
  "v"  // Потвърждава          // RESULT_CONFIRMATION_1
  "С"  // Уважава              // RESULT_GRANT
  "q"  // Уважава              // RESULT_GRANT_1
   ;
constant RESULT_CHANGINGS =
#   if PUNISHM_2ND_REPORT_12_SEP_2001
#   else  // of PUNISHM_2ND_REPORT_12_SEP_2001
  "Ь"  // Изменя               // RESULT_CHANGE
#   endif  // of PUNISHM_2ND_REPORT_12_SEP_2001
  "w"  // Изменя               // RESULT_CHANGE_1
   ;
constant RESULT_REJECTS =
  //"Щ"  // Отменя               // RESULT_ABOLISH; 2011:173 duplicate, comout
  "z"  // Отменя               // RESULT_ABOLISH_1
  "Т"  // Без уважение         // RESULT_NOT_GRANT
  "u"  // Оставя без уважение  // RESULT_NOT_GRANT_1
   ;
constant RESULT_SLOWDOWN_CONFIRMATIONS =
  "7"  // Указва
  "8"  // Задължава
   ;
constant RESULT_SLOWDOWN_REJECTS =
  "u"  // Оставя без уважение
   ;
constant RESULT_IRE_UNMOVEABLES =  // 27.apr.2001
  "а"  //  В срок - без движение
   ;
constant RESULT_IRE_SENDS =  // 03.may.2001
  "Ж"  // Изпратен
  "д"  // В срок - изпратени преписи
   ;
constant RESULT_IRE_RETURNEDS =  // 03.may.2001
  "Н"  // Върнат
  "Х"  // Върнат като просрочен
  "Ь"  // Върнат като нередовен
  "Ю"  // Върнат като недопустим
  "в"  // Върнат - оттеглен
  "з"  // Върнат
   ;
#   endif  // of APPEAL  // 25.jul.2000
# else  // of INSTANCE

					 // REGIONAL court only ===================
constant RESULT_UNMOVEABLES =
  "hD"
   ;  // Do not forget RESULT_LEFT_REPORT_DESIGNATION !!!
constant RESULT_STOPPEDS =
  "S"
   ;  //  Do not forget RESULT_LEFTS_OR_STOPPEDS !!!
constant RESULT_CANCELEDS =
  "O"
  "P"
  "c"
  "Б"  // 14.jun.2000  Със споразумение
   ;  // Do not forget RESULT_CANCELEDS_OR_RETURNEDS
	// and RESULT_WITH_DECS_OR_CANCELEDS !!!
constant RESULT_WITH_DECISIONS =
  "W"
  "Z"
  "d"
  "j"
  "p"
  "y"
  "T"
  "A"
  "0"
  "Љ"  // С присъда и споразумение  // 14.sep.2001
  "Ґ"  // С разпореждане, приключващо делото    //20080314
  "F"  // 2010:064 LRQ: FIX: was missing
  "7"  // 2015:048 TRQ: FIX: was missing
  "с"  // 2015:080
  "т"  // 2015:080
   ;   // Do not forget RESULT_WITH_DECS_OR_CANCELEDS !!!
constant RESULT_RETURNEDS =
//  "r"  // 09.jan.2001
  "B"
   ;  // Do not forget RESULT_CANCELEDS_OR_RETURNEDS !!!
constant RESULT_CANCELEDS_OR_RETURNEDS =
  "O"
  "P"
  "c"
  "B"
  "Б"  // 14.jun.2000  Със споразумение
   ;
constant RESULT_M_COL_FINISHEDS =
  "A"
  "Z"
  "d"  // С определение, приключващо делото  // 01.sep.2000
  "P"
  "a"
  "c"
  "p"
  "y"
   ;
constant RESULT_M_COL_NOT_FINISHEDS =
  "1"
  "2"
  "3"
  "4"
  "5"
  "6"
  "I"
  "M"
  "N"
  "m"
  "l"
  "o"
  "L"
  "S"
  "hD"
  "s"
  "7"
  "8"
  "9"
  "e"
  "k"
  "б"
  "Ў"  // Отложено с налагане на глоба  // 25.may.2004
  "Ѕ"  // С определение за отвод
  "І"  // С разпореждане за отвод

  "E"  // Отложено с привременни мерки  // 20060310

   ;  // RESULT_LEFTS + RESULT_STOPPEDS + "hs"
constant RESULT_LEFT_RECONCILES =
  "M"
  "N"
  "m"
   ;
constant RESULT_WITH_DECS_OR_CANCELEDS =
  "W"
  "Z"
  "d"
  "j"
  "p"
  "y"
  "T"
  "A"
  "O"
  "P"
  "c"
  "0"
  "Б"
  "Љ"  // С присъда и споразумение  // 14.sep.2001
  "F"		// 2010:064 LRQ: FIX: was missing
  "с"  // 2015:080
  "т"  // 2015:080
   ;
constant RESULT_DECISIONS_CLSESBK =
  "Z"
   ;
constant RESULT_DEFINITIONS_CLSESBK =
  "hD" // Без движение
  "S"  // Спряно
  "g"  // С отменен ход по същество
  "J"  // С определение
   ;
constant RESULT_DEFIN_TERMIN_CLSESBK =
  "d"
   ;
constant RESULT_KEEPING_METHOD_CONFIRMEDS =
  "v"  // Потвърждава
  "u"  // Оставя без уважение
   ;
constant RESULT_KEEPING_METHOD_CHANGEDS =
  "w"  // Изменя
  "q"  // Уважава
   ;
constant RESULT_TO_UP_LOOK_INTO_RETURNEDS =  // 16.jun.2000
  "B"  // Прекратено - за доразследване
   ;
constant RESULT_IRE_UNMOVEABLES =  // 27.apr.2001
  "а"  //  В срок - без движение
   ;
constant RESULT_IRE_SENDS =  // 03.may.2001
  "Ж"  // Изпратен
  "д"  // В срок - изпратени преписи
   ;
constant RESULT_IRE_RETURNEDS =  // 03.may.2001
  "Н"  // Върнат
  "Х"  // Върнат като просрочен
  "Ь"  // Върнат като нередовен
  "Ю"  // Върнат като недопустим
  "в"  // Върнат - оттеглен
  "з"  // Върнат
   ;
constant RESULT_PENALTIES =  // 25.may.2001
  "e"  // Отложено - неизготв. заключение на в.л. - чл.71
  "k"  // Отложено - несвоевр. поискани доп. док-ва - чл.65
   ;
constant RESULT_SPLITS =  // 25.may.2001
  "a"  // С решение по допускане на делба
  "A"  // С решение за отхвърляне на иска за делба
   ;
                               // REGIONAL court only

constant RESULT_LEFT_REPORT_DESIGNATION =  // 20060627  // Отчет РС, 16087, Отложени дела, Преди първо засед.
  "1"    		         // Отложено I-ва фаза
  "2"          		   // Отложено II-ра фаза
  "3"       		     // Отложено I-во заседание I-ва фаза
  "4"                // Отложено I-во заседание II-ра фаза
  "5"		             // Отложено II-ро заседание I-ва фаза
  "6"          		   // Отложено  II-ро заседание II-ра фаза
  "8"          		   // Отложено III-та фаза
  "9"          		   // Отложено II-ро заседание III-та фаза
  "I"    		         // Отложено във II-ро заседание
  "M"         	     //	Отложено в помир. зас. без месечен срок
  "N"          	     //	Отложено за ново помирително заседание
//   L                 	Отложено в I-во заседание -
  "m"          	     //	Отложено в помир. зас. месечен срок
  "l"         	     //	Отложено
  "e"          	     //	Отложено - неизготв. заключение на в.л. - чл.71
  "k"           		 // Отложено - несвоевр. поискани доп. док-ва - чл.65
  "б"    		         // Отложено с вписване на споразумение
  "Ў"          	     //	Отложено с налагане на глоба
  "E"          		   // Отложено с привременни мерки

  "А"         	     //	Отложено без ново насрочване
  "hD"                // RESULT_UNMOVEABLES
  "Ѕ"  // С определение за отвод
  "І"  // С разпореждане за отвод
   ;
constant RESULT_LEFT_1_REPORT_DESIGNATION =  // 20060627  // Отчет РС, 16094, Отложени дела, В първо засед.
  "L"                //	Отложено в I-во заседание
   ;

constant RESULT_DECIDED_REPORT_DESIGNATION =  // 20060627 // Отчет РС, кол 15, Свършени дела/С решение
  "p"          		   // Обявено за решаване
  "y"          		   // С готово решение
  //"D"          		   // С готово решение; 2011:173 duplicate, comout
  "A"          		   // С решение за отхвърляне на иска за делба
   ;

constant RESULT_ESS_CS_REPORT_DESIGNATION =    // 20060627 // Отчет РС, кол 19, 16090, Свършени дела/ С акт по същество в З.З.
  "y"          		   // С готово решение
  //"D"          		   // С готово решение; 2011:173 duplicate, comout
  "A"          		   // С решение за отхвърляне на иска за делба
  "d"                // С определение, приключващо делото
   ;

constant KIND_PRIVATE_COMPL_REPORT_DESIGNATION =    // 20060627 // Отчет РС, кол 32, 16091,  Обжалвани дела
  "Х"                // Въззивна жалба		      i
  "Ь"                // Въззивна частна жалба	  i
  "д"                // Касационна жалба		    i
  "н"                // Касационна частна жалба	i
  "р"                // Жалба по чл.126з ГПК		i
  "а"			// 2011:074 LRQ: Протест
  "в"			// 2011:074 LRQ: Частен протест
  ;

# endif  // of INSTANCE                 // REGIONAL COURT only <--

const char NON_SESSION_DATED_CONT_RESULT = RESULT_FAKE ;  // 25.mar.2002

constant SUMMONS_STANDARD_RESULTS =  // 18.may.2001
  "f"  // SUMMONS_FIXED
  "u"  // SUMMONS_UNMOVEABLE
  "s"  // SUMMONS_STOPPED
  "c"  // SUMMONS_CANCELED
  "o"  // SUMMONS_ANY_OTHER_RESULT  // Why is it here (08.jun.2004)?
   ;

		    // За тези участия се разрешава да са представляващи - T_LINK_WIND, F_PROXY_*
constant SUMMONS_FOR_SIDE_INDIRECT_INVS =
	"B"  // Законен представител
	"D"  // Особен представител
	"A"  // Служ.защитник
	"L"  // Адвокат
	"H"  // Пълномощник
	"J"  // Представляващ
	"K"  // Лице за съдебен адрес
	"P"  // Процесуален представител
	"U"  // Настойник  // 21.jul.2000
	"Y"  // Попечител  // 21.jul.2000
	"S"  // Синдик  // 21.jul.2000
	"Г"  // Ликвидатор  // 31.jan.2001
	"А"  // Представител // 20050602
	"R"  // Родител   // 20051103
	"И"  // Защитник  // 20070130
	"и"  // Резервен защитник  // 20110018
	"В"  // Повереник // 20070130
	"К"  // Временен синдик // 20120405
	"т"  // Упълномощен защитник // 20130259
	"р"  // Приемен родител // 20140143
	"‡"  // Юрисконсулт // 20160927	
;

constant SUMMONS_FOR_SIDE_LEGAL_SPOK_INVS =
	"B"  // Законен представител
	"U"  // Настойник  // 21.jul.2000
	"Y"  // Попечител  // 21.jul.2000
	"R"  // Родител
	"р"  // Приемен родител // 20140143
;

constant SUMMONS_FOR_SIDE_WITH_INVS =
	"L"  // Адвокат
	"A"  // Служебен защитник
	"и"  // Резервен защитник
	"И"  // Защитник
	"H"  // Пълномощник	// 2014:302
	"В"  // Повереник		// 2015:075
	"‡"  // Юрисконсулт	// 2016:271
;

constant PERSLST_BONEVA_0_INVS =  // 22.jan.2001
	"L"  // Адвокат
	"H"  // Пълномощник
	"K"  // Лице за съдебен адрес
	"P"  // Процесуален представител
	"A"  // Служебен защитник
	"т"  // Упълномощен защитник
	"‡"  // Юрисконсулт
;

constant INVOLV_TIME_BK_SUIT_TO_DEFENDS = "ra" ;

# if INSTANCE
#   if APPEAL
constant INVOLV_2ND_T_BK_SUIT_TO_DEFENDS = "ra""m""u"
  "i"  // Касатор
  "л"  // Жалбоподател-подсъдим  // 22.nov.2000
  "п"  // Жалбоподател-гр.ищец  // 22.nov.2000
  "ф"  // Жалбоподател-ч.обвинител  // 22.nov.2000
  "ц"  // Жалбоподател-ответник  // 22.nov.2000
   ;
#   else  // APPEAL
constant INVOLV_2ND_T_BK_SUIT_TO_DEFENDS = "ra""m""u"
  "i"  // Касатор
   ;
#   endif  // APPEAL
# else
constant INVOLV_DEFENDS_TO_EXCLUDE_P_ADM = "p" ;
# endif

//# if INSTANCE
//#   if APPEAL  // 04.may.2000                  // COURT_APPEAL  -->

#if COURT_TYPE == COURT_APPEAL
// constant KIND_COMPLAINTS  // 24.oct.2000 removed

constant KIND_EXACT_COMPLAINTS =       // Описна книга - НД;
								 // Книга ЗЗ, Рекапитулация -код 10452 - НД
								 // ... НД
    "1"  // Жалба (НОХД)
    "3"  // Жалба (ЧНД)
    "2"  // Жалба (НАХД/АНД)  // 1.3240  20070416
//  "Q"  // Жалба и протест  // "и протест" - let it be  // 10.oct.2000
    "7"  // Жалба-гражд. дело
//  "Й"  // Жалба-гражд. дело-БП  // 18.dec.2003 died out
//  "j"  // Жалба за бавност-чл.217  // 24.oct.2000
    "l"  // Жалба
    "Ж"  // Жалба-търг. дело  // 28.jun.2004
    "ж"  // Жалба-търг. дело (несъстоятелност)  // 20110902
    "P"  // Касационна жалба (ГрД)  // 20080315
    "А"  // Касационна жалба (ТД)   // 20080315
    "g"  // Жалба-гражд. дело (несъстоятелност)  // 20120120
  ;
constant KIND_PRIVATE_COMPLAINTS =
  "4"  // Частна жалба (ЧНД)
  "9"  // Частна жалба (НАХД)  // 1.3240  20070416
  "ц"  // Частна жалба (НОХД)
  "v"  // Частна жалба

  "0"  // Ч. жалба-гражд.дело
  "h"  //Ч. жалба-гражд.дело (несъстоятелност)  // 20120120
  "M"  // Молба за обезпечение  // KIND_ENSURE_REQUEST_R  // 21.nov.2000
  "m"  // Молба за обезпечение  // KIND_ENSURE_REQUEST
  "Г"  // Дело по отстраняване на съдии-чл.14 ал.2  // KIND_JUDGE_ELIMINATION
  "f"  // Препирня за подсъдност  // KIND_JURISD_QUARREL  // 21.nov.2000
	"8"  // Молба за възст.на срок за обж.-чл.37  // 19.jun.2002
  "Ё"  // 2011:231 TRQ/LRP: Касационна частна жалба (ГрД)
  "o"  // Ч. жалба-ГД - чл.274, ал.2 ГПК

  // Trade - begin  // 28.jun.2004
  "Ч"  // Ч. жалба-търг.дело      // KIND_PRV_COMPL_TRA
  "ч"  // Ч. жалба-търг.дело (несъстоятелност)  // 20110902
  "Ф"  // Молба за обезпечение    // KIND_TRADE_ENSURE_REQUEST_R
  "Ґ"  // Молба за обезпечение    // KIND_TRADE_ENSURE_REQUEST
  "y"  // Дело по отстраняване на съдии-чл.14 ал.2//KIND_TRADE_JUDGE_ELIMINATION
  "ґ"  // Препирня за подсъдност  // KIND_TRADE_JURISD_QUARREL
  "w"  // Молба за възст.на срок за обж.-чл.37 ГПК
  "ю"  // 2011:231 TRQ/LRP: Касационна частна жалба (ТД)
  "p"  // Ч. жалба-ТД - чл.274, ал.2 ГПК
  // Trade - end
	;
constant KIND_SLOWDOWN_COMPLAINTS =  // 24.oct.2000
  "j"  // Жалба за бавност-чл.217
  "Э"  // Жалба за бавност-чл.217  // Trade  // 28.jun.2004
	 ;
constant KIND_PETITIONS = // 11.jun.2002
	"8"  // Молба за възст.на срок за обж.-чл.37
	"m"  // Молба за обезпечение
	"M"  // Молба за обезпечение

  // Trade - begin  // 28.jun.2004
  "w"  // Молба за възст.на срок за обж.-чл.37 ГПК
  "Ф"  // Молба за обезпечение    // KIND_TRADE_ENSURE_REQUEST_R
  "Ґ"  // Молба за обезпечение    // KIND_TRADE_ENSURE_REQUEST
  // Trade - end
	 ;

constant KIND_EXACT_OBJECTIONS =
	"5"  // Протест (НОХД)
  "В"  // Протест (ЧНД)
  "L"  // Протест
  "Q"  // Протест и жалба  // 10.oct.2000
  "q"  // Протест и жалба  // 12.sep.2002
  "X"  // Протест (НАХД)   // 20070926
  ;
constant KIND_PRIVATE_OBJECTIONS =
  "6"  // Частен протест (ЧНД)
  "V"  // Частен протест
  "F"  // Частен протест и частна жалба
  "Y"  // Частен протест (НАХД)  // 20070926
  ;
constant KIND_PRIVATE_PUN_C_O_INDOCS =
  "3"  // Жалба (ЧНД)
  "В"  // Протест (ЧНД)
  ;
constant KIND_INREG_EXACT_COMPLAINTS =
  "ь"  // Касационна жалба
   ;
constant KIND_INREG_PRIVATE_COMPLAINTS =
  "н"  // Касационна частна жалба
   ;
constant KIND_INREG_EXACT_OBJECTIONS =
  "о"  // Касационен протест
   ;
constant KIND_INREG_PRIVATE_OBJECTIONS =
  "р"  // Касационен частен протест
   ;
constant KIND_INREG_REJ_RETR_COMPLAINTS =
  "е"  // Молба за отмяна
   ;

constant KIND_DEMAND_INDOCS = "" ;

#endif  // APPEAL <--

#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY

#if COURT_TYPE == COURT_AREAL
	constant KIND_EXACT_COMPLAINTS =
	"l"  // Жалба
	"1"  // Жалба (НОХД)
	"ц"  // Касационна жалба (НАХД)
	"Ж"  // Жалба-търг. дело
//      "9"  // Жалба-админ. дело
//      "P"  // Жалба-админ. дело  // 20.sep.1999 - begin
//  "й"  // Частна жалба-админ. дело-БП  // 07.jun.2001  Why here?
//  "ф"  // Частна жалба-админ. дело-БП  // 07.jun.2001  Why here?
	"б"  // Касационна жалба
	"Ц"  // Касационна жалба  // 20.sep.1999 - end
//  "с"  // Жалба-гражд. дело-БП  // 18.jul.2000  // 18.dec.2003 died out
//  "у"  // Жалба-търг. дело-БП  // 18.jul.2000  // 18.dec.2003 died out
	"І"  // Жалба с-у действия на съдия-изпълнител  // 08.jan.2004
	"–"  // Протест и жалба  // 2010:025
	"&"  // Касационен протест (НАХД)  // 2011:315
	 ;

constant KIND_EXACT_OBJECTIONS =
  "5"  // Протест (НОХД)
  "L"  // Протест
  ;
#endif  // AREAL

#if COURT_TYPE == COURT_MILITARY
	constant KIND_EXACT_COMPLAINTS =
	"l"  // Жалба
	"1"  // Жалба (НОХД)
	"2"  // Жалба (НЧХД)
	"3"  // Жалба (ЧНД)
	"ц"  // Касационна жалба (АНД)
	"7"  // Жалба-гражд. дело
	"Ф"  // Жалба (АНД)  // 20060718
	 ;

constant KIND_EXACT_OBJECTIONS =
  "5L""В"
  "т"  // Протест (АНД)              // 20060718
  ;
#endif  // MILITARY

#if COURT_TYPE == COURT_ADMIN
	constant KIND_EXACT_COMPLAINTS =
	// 2008:308 LRQ: -9PЁe
	//"9"  //              200    Жалба - оспорване на админ.акт
	//"P"  //              200    Жалба - оспорване на админ.акт
	//"Ё"  //              202    Жалба с-у действие на органа по изпълн.
	//"e"  //              202    Жалба с-у действие на органа по изпълн.
	"б"  //              222    Касационна жалба
      "Є"  //              224    Касационна жалба (АНД)-админ.дело
      "Ц"  //              222    Касационна жалба
	"ц"  //              224    Касационна жалба (АНД)-адм.дело
      "g"  //              300    Касационна жалба (АНД)-наказ.дело
      "й"  //              300    Касационна жалба (АНД)-наказ.дело
      "д"  //                5    Касационна жалба
       ;

constant KIND_EXACT_OBJECTIONS =
      "о"  //                7    Касационен протест
  ;

#endif  // ADMIN


constant KIND_EXACT_COMPLAINTS_CONCERN =  // 16.jan.2001
  "ц"  // Касационна жалба (НАХД)
  "б"  // Касационна жалба
  "Ц"  // Касационна жалба
  "&"  // Касационен протест (НАХД)  // 2011:315
   ;

#if COURT_TYPE == COURT_ADMIN
	constant KIND_PRIVATE_COMPLAINTS =
	"b"  //              214    Частна жалба
	"К"  //              214    Частна жалба
	"э"  //              226    Частна касационна жалба
	"ы"  //              226    Частна касационна жалба
	"н"  //                6    Касационна частна жалба
	// 2008:308 LRQ: +єЏ
	"є"  // Частна касационна жалба (АНД)-админ.дело
	"Џ"  // Частна касационна жалба (АНД)-наказ.дело
	// 2012:079 LRQ: +ҐґXџ
	"X"  // Частна касационна жалба (АНД)-админ.дело
	"џ"  // Частна касационна жалба (АНД)-наказ.дело
	"Ґ"  // Препирня за подсъдност
	"ґ"  // Препирня за подсъдност
	 ;

	constant KIND_PRIVATE_OBJECTIONS =
	"р"  //                8    Касационен частен протест
	 ;
#endif  // ADMIN

#if COURT_TYPE == COURT_AREAL
	constant KIND_PRIVATE_COMPLAINTS =
	  "4"  // Частна жалба (ЧНД)
	  "v"  // Частна жалба
	  "Ч"  // Ч. жалба-търг. дело
	  "ш"  // Частна жалба - админ. дело // 10.oct.2000
	  "э"  // Частна жалба - админ. дело // 10.oct.2000
	  "ю"  // Частна касационна жалба    // 10.oct.2000
	  "ы"  // Частна касационна жалба        // 10.oct.2000
	  "g"  // Частна касационна жалба (НАХД) // 25.jan.2001
	  "ў"  // Частна жалба (НОХД)            // 04.may.2001
	  "ґ"  // Препирня за подсъдност     // 17.sep.2001
	  "й"  // Частна жалба (НАХД)        // 20060718 added
	  // 2009:182 LRQ: moved here from EXACT
	  "‡"  // Жалба с-у действия на съдия по вписван.  // 2008:255
	  "Ў"  // Частна жалба (НЧХД)	// 2009:182
	  "—"  // Частен протест и частна жалба  // 2010:025
	  "‚"  // Възражение по чл.423 ГПК // 2010:025
	  "™"  // Дело по отстран.на съдии чл.23, ал.3 ГПК // 2015:091
	 ;

	constant KIND_PRIVATE_OBJECTIONS =
	  "6"  // Частен протест (ЧНД)
	  "V"  // Частен протест
	  "Є"  // Частен протест (НОХД)       // 04.may.2001
	  "“"  // Частен протест (НАХД)  // 2011:320 FIX
	 ;
#endif  // AREAL

#if COURT_TYPE == COURT_MILITARY
	constant KIND_PRIVATE_COMPLAINTS =
	  "4"  // Частна жалба (ЧНД)
	  "v"  // Частна жалба
	  "0"  // Ч. жалба-гражд.дело
	  "g"  // Частна касационна жалба (АНД) // 25.jan.2001
	  "ў"  // Частна жалба (НОХД)            // 04.may.2001
	  "Ґ"  // Препирня за подсъдност     // 17.sep.2001
	  "й"  // Частна жалба (АНД)        // 20060718 added
	 ;

	constant KIND_PRIVATE_OBJECTIONS =
	  "6"  // Частен протест (ЧНД)
	  "V"  // Частен протест
	  "Є"  // Частен протест (НОХД)       // 04.may.2001
	 ;
#endif  // MILITARY

constant KIND_PRIVATE_COMPLAINTS_CONCERN =  // 16.jan.2001
  "ю"  // Частна касационна жалба
  "ы"  // Частна касационна жалба
  "g"  // Частна касационна жалба (НАХД)  // 25.jan.2001
//  "й"  // Частна касационна жалба-БП  // 07.jun.2001  // 18.dec.2003 died out
//  "ф"  // Частна касационна жалба-БП  // 07.jun.2001  // 18.dec.2003 died out
   ;
constant KIND_SLOWDOWN_COMPLAINTS =  // 24.oct.2000
  "Ћ"  // Жалба за бавност // 20060208    'Ћ', 0407, 'Жалба за бавност'
   ;
//constant KIND_OBJECTIONS  // 24.oct.2000 removed

constant KIND_INREG_EXACT_COMPLAINTS =
  "Х"  // Въззивна жалба
  "д"  // Касационна жалба
   ;
constant KIND_INREG_PRIVATE_COMPLAINTS =
  "Ь"  // Въззивна частна жалба
  "н"  // Касационна частна жалба
   ;
constant KIND_INREG_EXACT_OBJECTIONS =
  "а"  // Протест
  "о"  // Касационен протест
   ;
constant KIND_INREG_PRIVATE_OBJECTIONS =
  "в"  // Частен протест
  "р"  // Касационен частен протест
   ;
constant KIND_INREG_REJ_RETR_COMPLAINTS =
  "е"  // Молба за отмяна
  "з"  // Молба за преглед
   ;

constant KIND_TRADE_BOZA_INDOCS =  // 08.may.2002
  "Ђ"  // Възражение
  "ђ"  // Частна жалба среду действие на синдик
   ;

constant KIND_DEMAND_INDOCS = "" ;

#endif  // AREAL || ADMIN || MILITARY

#if COURT_TYPE == COURT_APPEAL
constant KIND_PSEUDO_SESSIONS =
  "У"  // Друго заседание
   ;
constant KIND_PSEUDO_OR_CLOSED_SESSIONS =
  "У"  // Друго заседание
  "И"
  "I"  // 12.jan.2001
  "J"  // 12.jan.2001
   ;
constant KIND_JUDGEMENTS_BY_NAME =
  "ъ"  // Присъда
   ;
constant KIND_DECISIONS_BY_NAME =
  "Ш"  // Решение
   ;
constant KIND_DEFINITIONS_BY_NAME =
  "л"  // Определение
   ;
constant KIND_DECREES_BY_NAME =
  "м"  // Разпореждане
   ;
constant KIND_PUN_GENERAL_LAWS = // 08.apr.2002
  "W"  // ВНОХД
	 ;
constant KIND_PUN_PRIVATE_LAWS = // 08.apr.2002
	"H"  // ВЧНД
	 ;
constant KIND_KEEP_METHOD_CHG_COMPLAINTS = "x" ;
constant KIND_ACCUSE_ACTS = "Au" ;

#endif  // APPEAL

#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
constant KIND_PSEUDO_SESSIONS =
  "У"  // Друго заседание
#if COURT_TYPE != COURT_MILITARY
  "Л"  // Събрание на кредиторите
  "Ю"  // Разпореждане извън заседание
#endif  // !MILITARY
   ;
constant KIND_PSEUDO_OR_CLOSED_SESSIONS =
  "У"  // Друго заседание
#if COURT_TYPE != COURT_MILITARY
  "Л"  // Събрание на кредиторите
  "Ю"  // Разпореждане извън заседание
#endif  // !MILITARY
  "И"  // Закрито/разпоредително заседание
  "F"  // Закрито/разпоредително заседание 2  // 12.jan.2001
  "G"  // Закрито/разпоредително заседание 3  // 12.jan.2001
  "Ѓ"  // Закрито/разпоредително заседание 4  // 21.sep.2001
  "ѓ"  // Закрито/разпоредително заседание 5
  "Љ"  // Закрито/разпоредително заседание 6
  "Њ"  // Закрито/разпоредително заседание 7
  "Ќ"  // Закрито/разпоредително заседание 8
  "љ"  // Закрито/разпоредително заседание 9
	"њ"  // Закрито/разпоредително заседание 10
#if COURT_TYPE == COURT_ADMIN
  "‚"  // Закрито/разпоредително заседание 11
  "„"  // Закрито/разпоредително заседание 12
  "…"  // Закрито/разпоредително заседание 13
  "†"  // Закрито/разпоредително заседание 14
  "‡"  // Закрито/разпоредително заседание 15
  "‹"  // Закрито/разпоредително заседание 16
  "‘"  // Закрито/разпоредително заседание 17
  "’"  // Закрито/разпоредително заседание 18
  "“"  // Закрито/разпоредително заседание 19
  "”"  // Закрито/разпоредително заседание 20
  "•"  // Закрито/разпоредително заседание 21
  "–"  // Закрито/разпоредително заседание 22
  "—"  // Закрито/разпоредително заседание 23
  "›"  // Закрито/разпоредително заседание 24
  "¦"  // Закрито/разпоредително заседание 25
#endif  // COURT_ADMIN
   ;
constant KIND_JUDGEMENTS_BY_NAME =
  "ъ"  // Присъда
   ;
constant KIND_DECISIONS_BY_NAME =
  "Ш"  // Решение
  "R"  // Решение 2
   ;
constant KIND_DEFINITIONS_BY_NAME =
  "л"  // Определение
   ;
constant KIND_DECREES_BY_NAME =
  "м"  // Разпореждане
   ;
constant KIND_PUN_GENERAL_LAWS = // 08.apr.2002
  "W"  // ВНОХД
  "O"  // НОХД
   ;
constant KIND_PUN_PRIVATE_LAWS = // 08.apr.2002
	"C"  // ВНЧХД
	"H"  // ВЧНД
	"I"  // ЧНД
	 ;

constant KIND_KEEP_METHOD_CHG_COMPLAINTS = "x" ;
constant KIND_ACCUSE_ACTS = "Au" ;

#endif  // ADMIN || AREAL || MILITARY


#if COURT_TYPE == COURT_REGIONAL

constant KIND_TEMP_MEASURES_SESSIONS =    // 20060310
  "Ј"   // Заседание по привременни мерки
   ;

constant KIND_SESS_COMPLE_REPORT_DESIGNATION =    // 20060627 // RC, Report, 16089, Свършени дела/С решение
  "Ј"   // Заседание по привременни мерки
  "4"   // Помирително заседание
  "5"   // Първо заседание
  "6"   // Второ заседание
   ;

constant KIND_PSEUDO_SESSIONS =
	"9"   // Друго заседание
	"d"   // Резолюция извън заседание
	"L"   // Изслушване по чл. 126 "Ж" ГПК
   ;
constant KIND_PSEUDO_OR_CLOSED_SESSIONS =
  "9"
  "d"
  "L"  // 04.may.2000
  "l"
  "V"  // 12.jan.2001
  "W"  // 12.jan.2001
   ;

constant KIND_JUDGEMENTS_BY_NAME =
  "G"
//  "М"  // 01.aug.2000
    ;
constant KIND_DEFINITIONS_BY_NAME =
							    "S" ;
constant KIND_DECISIONS_BY_NAME =
                                            "E" ;
constant KIND_KEEP_METHOD_CHG_COMPLAINTS = "x" ;
constant KIND_ACCUSE_ACTS = "AQ" ;

constant KIND_INREG_EXACT_COMPLAINTS =
  "Х"  // Въззивна жалба
  "д"  // Касационна жалба
   ;
constant KIND_INREG_PRIVATE_COMPLAINTS =
  "Ь"  // Въззивна частна жалба
  "н"  // Касационна частна жалба
   ;
constant KIND_INREG_EXACT_OBJECTIONS =
  "а"  // Протест
   ;
constant KIND_INREG_PRIVATE_OBJECTIONS =
  "в"  // Частен протест
   ;
constant KIND_INREG_REJ_RETR_COMPLAINTS =
  "е"  // Молба за отмяна
  "з"  // Молба за преглед
   ;
constant KIND_PUN_GENERAL_LAWS = // 08.apr.2002
  "O"  // НОХД
   ;
constant KIND_PUN_PRIVATE_LAWS = // 08.apr.2002
  "I"  // ЧНД
   ;
#endif  // REGIONAL

#if INSTANCE
constant JUDGED_JUDGEDS = "ij"
						"p"  // 13.jun.2000
  "o"  // Отпадане на наказуемостта др. основания  // 25.jul.2001
  "r"  // Частично оправдателна присъда  // 25.jul.2001
  "7"  // Освобожд.от наказ.отгов.-чл.78а НК  // 25.jul.2001
  "6"  // Освобожд.от наказ.отгов.-чл.61 или 64 НК  // 25.jul.2001
						   ;
constant JUDGED_CONVICTEDS =
  "j"
  "p"
  "r"  // Частично оправдателна присъда  // 25.jul.2001
  "7"  // Освобожд.от наказ.отгов.-чл.78а НК  // 25.jul.2001
  "6"  // Освобожд.от наказ.отгов.-чл.61 или 64 НК  // 25.jul.2001
   ;
constant JUDGED_CONVICTED_BULWARKS = "p" ;  // 13.jun.2000
constant JUDGED_ACQUITS =
  "i"
  "o"  // Отпадане на наказуемостта др. основания  // 25.jul.2001
   ;
constant JUDGED_CANCELEDS =
//  "c"  // 10.sep.2001
  "s"  // Прекратяване по чл.21 ал.1 т.1,2,3,4,5,6  // 10.sep.2001
   ;
constant JUDGED_FREEDS =
  "7"  // Освобожд.от наказ.отгов.-чл.78а НК  // 25.jul.2001
   ;
#else  // of INSTANCE
constant JUDGED_JUDGEDS = "ij"
						"p"  // 13.jun.2000
  "o"  // Отпадане на наказуемостта др. основания  // 25.jul.2001
  "r"  // Частично оправдателна присъда  // 25.jul.2001
  "7"  // Освобожд.от наказ.отгов.-чл.78а НК  // 25.jul.2001
  "6"  // Освобожд.от наказ.отгов.-чл.61 или 64 НК  // 25.jul.2001
						   ;
constant JUDGED_CONVICTEDS =
  "j"
  "p"
  "r"  // Частично оправдателна присъда  // 25.jul.2001
  "7"  // Освобожд.от наказ.отгов.-чл.78а НК  // 25.jul.2001
  "6"  // Освобожд.от наказ.отгов.-чл.61 или 64 НК  // 25.jul.2001
   ;
constant JUDGED_CONVICTED_BULWARKS = "p" ;  // 13.jun.2000
constant JUDGED_ACQUITS =
  "i"
  "o"  // Отпадане на наказуемостта др. основания  // 25.jul.2001
   ;
constant JUDGED_CANCELEDS =
//  "c"
  "s"  // Прекратяване по чл.21 ал.1 т.1,2,3,4,5,6  // 10.sep.2001
   ;
constant JUDGED_FREEDS =
  "7"  // Освобожд.от наказ.отгов.-чл.78а НК  // 25.jul.2001
   ;
#endif  // of INSTANCE

#if INSTANCE
#else
constant PS2_KIND_INVESTIGATIONS = "G";// "p" ;
#endif  // 11.jan.2000 - end


const char

# if INSTANCE
#   if APPEAL
  * const PUNISHMENT_UP_TO_3_YEARS_S = "NC",
  * const PUNISHMENT_UP_TO_3_YEARS_CONDS = "C",
  * const PUNISHMENT_UP_TO_3_YEARS_EFFS = "N",
  * const PUNISHMENT_OTHERS_S = "o",
//  * const PUNISHMENT_COMPLEMENTS_S = "x",  16.may.2000

  * const PUNISHMENT_3_TO_10_YEARS_S = "3",
  * const PUNISHMENT_10_TO_30_YEARS_S = "t",
  * const PUNISHMENT_LIFE_IMPRISONMENTS = "w",
  * const PUNISHMENT_LIFE_IMPRISONMENT_NCS = "s"
#   else  // of APPEAL
	    // COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_ADMIN
  * const PUNISHMENT_FINE_ARL = "M",   // Глоба     // 20070611 - за "корупционен" отчет

  * const PUNISHMENT_UP_TO_3_YEARS_S =
    "N"  // ЛС до 3 г.-ефективно
    "C"  // ЛС до 3 г.-условно
    ,
  * const PUNISHMENT_UP_TO_3_YEARS_CONDS =
    "C"
    ,
  * const PUNISHMENT_UP_TO_3_YEARS_EFFS =
    "N"
    ,
  * const PUNISHMENT_OTHERS_S =
    "o"  // Други наказания
    "W"  // Поправителен труд       // 26.jul.2001
    "m"  // Задължително заселване  // 26.jul.2001
    "M"  // Глоба                   // 26.jul.2001
    "P"  // Обществено порицание    // 26.jul.2001
    "d"  // Лишаване от права       // 26.jul.2001
    "p"  // Пробация                // 02.jun.2004
    ,

  * const PUNISHMENT_OTHERS_S_CORR =
    "o"  // Други наказания
    "W"  // Поправителен труд       // 26.jul.2001
    "m"  // Задължително заселване  // 26.jul.2001
    // "M"  // Глоба                   // 26.jul.2001
    "P"  // Обществено порицание    // 26.jul.2001
    "d"  // Лишаване от права       // 26.jul.2001
    //"p"  // Пробация                // 02.jun.2004
    ,

  * const PUNISHMENT_3_TO_10_YEARS_S =
    "3"  // ЛС 3-5г.
    "5"  // ЛС 5-10г.  // 26.jul.2001
    ,
  * const PUNISHMENT_10_TO_30_YEARS_S =
    "t"  // ЛС 10-15г.
    "f"  // ЛС 15-20г.  // 26.jul.2001
    "e"  // ЛС 20-30г.  // 26.jul.2001
    "T"  // ЛС над 30г.  // 26.jul.2001
    ,
  * const PUNISHMENT_LIFE_IMPRISONMENTS =
    "w"  // Доживотен затвор
    ,
  * const PUNISHMENT_LIFE_IMPRISONMENT_NCS =
    "s"  // Доживотен затвор без замяна
#   endif  // of APPEAL
# else  // of INSTANCE
  * const PUNISHMENT_UP_TO_3_YEARS_S =
	"N"  // ЛС до 3 г-ефективно
	"C"  // ЛС до 3 г-условно
	"k"  // ЛС до 1 г-ефективно  // 10.jan.2001
	"u"  // ЛС до 1 г-условно  // 10.jan.2001
	,
  * const PUNISHMENT_UP_TO_3_YEARS_CONDS =
	"C"  // ЛС до 3 г-условно
	"u"  // ЛС до 1 г-условно  // 10.jan.2001
	,
  * const PUNISHMENT_UP_TO_3_YEARS_EFFS =
	"N"  // ЛС до 3 г-ефективно
	"k"  // ЛС до 1 г-ефективно  // 10.jan.2001
	,
  * const PUNISHMENT_3_YEARS_EFFS =  // 01.mar.2001
	"N"  // ЛС до 3 г-ефективно
	,
  * const PUNISHMENT_UP_TO_1_YEAR_S =  // 10.jan.2001
	"k"  // ЛС до 1 г-ефективно
	"u"  // ЛС до 1 г-условно
	,
  * const PUNISHMENT_UP_TO_1_YEAR_EFFS =  // 01.mar.2001
	"k"  // ЛС до 1 г-ефективно
	,
  * const PUNISHMENT_OTHERS_S =
	"o"
	"s"  // 28.jul.1999
	"O"  // Други наказания - условно  // 20.apr.2001
	"m"  // Задължително заселване  // 26.jul.2001
	"P"  // Обществено порицание    // 26.jul.2001
	"d"  // Лишаване от права       // 26.jul.2001
    //"p"  // Пробация                // 20050512  // 20050601 in comment
	"w"  // 20050601 added  поправителен труд
	"a"  // Задържане в поделение на МВР // 20050512
	"l"  // Не се наказва // 2014:006
	,
//  * const PUNISHMENT_COMPLEMENTS_S = "x",  // 16.may.2000

  * const PUNISHMENT_OTHER_CONDIT_S =  // 20.apr.2001
	"O"  // Други наказания - условно
	,
  * const PUNISHMENT_3_TO_15_YEARS_S =
	"3"  // ЛС 3-5г.
	"5"  // ЛС 5-10г.  // 26.jul.2001
	"T"  // ЛС 10-15г.  // 26.jul.2001
	,
  * const PUNISHMENT_PENALTIES = "t",     //
  * const PUNISHMENT_WORK_EDUCATE_SCHOOL_S = "s"
# endif  // of INSTANCE
 ;

constant INVOLVEMENT_COMPLAINTLODGERS =  // 22.jan.2001
  "a"  // Жалбоподател
  "л"  // Жалбоподател- подсъдим
  "п"  // Жалбоподател- гр.ищец
  "ф"  // Жалбоподател- ч.обвинител
  "ц"  // Жалбоподател- ответник
   ;

# if INSTANCE
#   if APPEAL
constant TYPE_TRADE_OR_CITIZEN_LAWS = "tc" ;
#   else
constant TYPE_TRADE_OR_CITIZEN_LAWS = "tc" ;
#   endif
# else
constant TYPE_TRADE_OR_CITIZEN_LAWS = "c" ;
# endif

#if COURT_TYPE == COURT_AREAL
constant
	ACCOMPLY_TL_630S = "0EF",	// former '0'
	ACCOMPLY_TL_632S = "2H";	// former '2'
#endif  // AREAL

const char
  * const CANNOT_OPEN_THE_TEMPORARY_FILE =
    "Не мога да отворя временния файл",
  * const UNEXPECTED_EVENT =
    "Неочаквани (за програмата) обстоятелства",
  * const CANNOT_LOAD_ANY_COLLEGES =
    "Неуспешно зареждане на колегии" ,
  * const CANNOT_LOAD_ANY_COMPOSITIONS =
    "Неуспешно зареждане на състави",
//  * const BUFF_TOO_SMALL = "Твърде малък буфер"
  * const OMISSIONS_WILL_BE_OCCURED =
    "Ще имате пропуски в генерирания документ",
  * const CANNOT_LOAD_ANY_LAW_TYPES =
    "Неуспешно зареждане на типове дела",
  * const MORE_THAN_ONE_L_JUDGEMENT =
    "Повече от една частна присъда/решение/определение"
//    " за лице по заседание",
    " за лице по дело",
  * const MORE_THAN_ONE_DES_G_JUDG =
    "Повече от една присъда/решение/определение по заседание",
  * const CANNOT_LOAD_THE_FORMAT_INFO =
    "Не мога да заредя информацията за формата на документа",
  * const BAD_FORMAT_INFO =
    "Липсваща или некоректна информация за формата на документа",
  * const CANNOT_READ_FROM_TMP_FILE =  // 22.jun.2001
    "Грешка при четене от временния файл",
  * const CANNOT_RW_TEMP_FILE =
    "Грешка при обработката (четене/запис) на временния файл",
  * const CANNOT_WRITE_TO_TEMP_FILE =
    "Грешка при запис във временния файл",
  * const TYPES_BUFF_TOO_SHORT =
    "Твърде къс буфер за колегии/видове дела",
  * const UNABLE_TO_DEL_INS_STR =
    "Unable to delete/insert string to list",
  * const NO_TYPE_CHOOSEN =
    "Няма избран вид дело",
  * const CANNOT_UNIFY_PUNISHMENT =
    "Наказателните дела не могат да се комбинират с другите видове.",
  * const FLYING_INREG_DOCS_STRING =
    "Несвързани с дела документи",
  * const WRONG_COMPOSITION_INDEXES = "Wrong composition indexes",
  * const WRONG_HALL_INDEXES = "Wrong hall indexes",
  * const WRONG_ARGS_PASSED_TO = "Incorrect argument(s) passed to",
  * const IMPROPER_ABSTRACT_METHOD_USE =
	  "Improper use of abstract method",
  * const IMPROPER_USE_OF = "Improper use of",
  * const IS_RETURNING_NULL = "is returning NULL",
  * const UNDAJUSTED_ARRAYS_PASSED_TO = "Unadjusted arrays passed to",
  * const NO_OUTPUT_CHOSEN = "Не е избран изход",
  * const UNSUCCESSFUL_DOC_TEXT_GENERATING =
    "Неуспешно генериране на текста на документа",
  * const WRONG_ARR_DIMENSION = "wrong array dimension",
  * const UNSUCCESSFUL_OPERATION = "unsuccessful operation",
  * const UNSUCCESSFUL_ADD_TO_ARRAY = "unsuccessful add to array", //03.nov.2003
  * const UNSUCCESSFUL_DETECTING_OF = "Неуспешно определяне на", // 24.aug.2004
  * const CANNOT_PERFORM_OPERATION = "cannot perform the operation",
  * const IS_NOT_IMPLEMENTED = "is not implemented",
  * const NULL_POINTER_IN_ARRAY = "NULL pointer in array",
  * const DISTRIB_HISTORY_CHECKUP = "Справка за извършеното разпределение",
  * const NO_CHOICES =
    "Няма възможности за избор за това поле от този документ",
  * const NO_DATA_STR = "Липсват данни",
  * const WANT_HISTORY_CHECKUP_QUESTION =
//    "Желаете ли справка за извършеното разпределение?",
    "Желаете ли справка за последното извършено разпределение?",
  * const RETURNING_EMPTY_OBJ_INSTEAD =
    "Returning empty object instead.",
  * const INCORRECT_PERIOD_GIVEN = "Неправилно зададен период",
  * const UNSPECIFIED_EXCEPTION_CAUGHT = "Прихванато е неописано изключение",
  * const BADLY_FILLED_OBJECT = "the object is not filled correctly",
  * const WANT_CONTINUE_QUESTION = "Продължавате ли?",
  * const ALL_COMPOSITIONS = "Всички",
  * const RESULT_WILL_BE_DELAYED =
    "Това би довело до забавяне на резултата",
  * const LINK_COULD_NOT_BE_ADDED =
    "Неуспешно добавяне на новосъздадена връзка към масив",
  * const NEWLY_ADDED_LINK_NOT_FOUND =
    "Неоткрита новодобавена към масив връзка",
  * const ABOLISHED_MOVE_STR =  // 29.may.2001
    "Отмяна на хода по същество - ",
  * const ARG_IS_NULL_OR_EMPTY =  // 17.oct.2001
    " argument is NULL or empty",
  * const EMPTY_FMT = "text\\empty.fmt",  // 17.oct.2001
  * const NO_LAW_TYPES_TO_START_REGIME =
    "Няма видове дела, за които да бъде стартиран режимът.", // 02.nov.2001
  * const INSUFFICIENT_RIGHTS_TO_START =
    "Вие нямате достатъчно права, за да стартирате режима.", // 02.nov.2001
  * const SQL_SELECT = "SELECT ",  // 19.mar.2002
  * const SQL_WHERE = " WHERE ",  // 19.mar.2002
  * const SQL_ORDER_BY = " ORDER BY ",  // 19.mar.2002
  * const PLEASE_CHECK_ANY_INST =
# if INSTANCE
#   if APPEAL
    " "
#   else  // of APPEAL
    "Моля, изберете някакъв вид дела по инстанции"
#   endif  // of APPEAL
# else  // of INSTANCE
    " "
# endif  // of INSTANCE
  ,
  * const FIRM_LAWS_ARE_1ST_INSTANCE =
# if INSTANCE
#   if APPEAL
    " "
#   else  // of APPEAL
    "Фирмените дела са само първоинстанционни"
#   endif  // of APPEAL
# else  // of INSTANCE
    " "
# endif  // of INSTANCE
  ,
  * const BUFFER_TOO_SHORT = "Buffer is too short." ;
   ;

constant PLEASE_WAIT = "Моля, изчакайте ..." ;
constant PLEASE_WAIT_EXTRACTING =
  "Моля изчакайте, извличам данни ..." ;
constant PLEASE_WAIT_GENERATING =
	"Моля изчакайте, генерирам изходния документ ..." ;
constant SAVING = "Запис..." ;  // 28.feb.2003
constant LOADING = "Зареждане...";  // 28.feb.2003
constant EXTRACTING_DATA = "Извличане на данни" ;
constant MAKING_GENERAL_ROW = "Подготовка на сумарен ред" ;
constant CREATING_OUT_DOC = "Генерирне на изходния документ" ;
constant STEP = "Стъпка" ;  // 11.feb.2003
constant PASSEDS = "Обработени" ;  // 11.feb.2003
constant PLEASE_WAIT_CHECKING_FINISHEDS =
	"Бегла проверка на свършилите дела. Моля, изчакайте ..." ;
constant PLEASE_WAIT_CHECKING_SESSIONS =
	"Бегла проверка на заседанията. Моля, изчакайте ..." ;
constant PLEASE_WAIT_PROCESS_FINISHEDS =
	"Моля изчакайте, обработвам свършените дела ..." ;
constant PLEASE_WAIT_PROCESS_NON_FINISHEDS =
	"Моля изчакайте, броя останалите несвършени дела ..." ;
constant PLEASE_WAIT_PROCESS_SESSIONS =
	"Моля изчакайте, обработвам заседанията ..." ;
constant PLEASE_WAIT_LOOKING_UNWRITTENS =
  "Моля изчакайте, оглеждам за ненаписани актове ..." ;
constant PLEASE_WAIT_PROCESS_OTHERS =
  "Моля изчакайте, обработвам другите съдебни актове ..." ;
constant PLEASE_WAIT_PROCESS_FOR_EXAM =
  "Моля изчакайте, обработвам делата за разглеждане ..." ;
constant PLEASE_WAIT_PROCESS_REQUESTS =
  "Моля изчакайте, обработвам преписките ..." ;
constant PLEASE_WAIT_PROCESS_NEW_LAWS =
  "Моля изчакайте, обработвам новопостъпилите дела ..." ;
constant PLEASE_WAIT_PROCESS_DECISIONS =
  "Моля изчакайте, обработвам решенията ..." ;
constant ALMOST_DONE = "Почти готово ..." ;  // 06.feb.2002
constant CHECKUPS = "Справки" ;  // 14.feb.2002
constant SUMMARY = "Рекапитулация" ;  // 14.feb.2002
constant SUMMARY_DATA = "Данни за рекапитулация" ; // 21.jun.2002
constant FINISHED_LAWS = "Свършили дела" ;  // 21.jun.2002
constant SURROUND_KIND_OMITTED =
	"The current surround kind is not fully supported yet." ;
constant DEFINITION_OR_DECREE =
	"Определение/Разпореждане" ;

constant CONDENSED_PRINT_S = "\xf" ;
const char* const NORMAL_PRINT_S     = "\x12" ;

constant CONTINUE_QUEST = "Продължавате ли?" ;
constant WRONG_DATED_SIDE_TYPE =
  "Unexpected kind of the dated side" ;
constant WRONG_DATED_SIDE_STATE =
  "Unexpected state of the dated side" ;
constant WRONG_DATED_CONTAINER_TYPE =
  "Unexpected kind of the dated container" ;

# if INSTANCE
#   if APPEAL
constant SUPREME_CONCERN_COURT = "ВКС" ;
#   else  // of APPEAL
constant SUPREME_CONCERN_COURT = "ВКС/ВАдС" ;
#   endif  // of APPEAL
# else  // of INSTANCE
constant SUPREME_CONCERN_COURT = "ВКС/ВАдС" ;
constant PUNISHM_ADMIN_REPORT_FILE_NAME = "text\\reportpa.fmt" ;
# endif  // of INSTANCE

const char* const HTML_CODE_ON = "(_!H!T!M!L!_!C!O!D!E!_!O!N!_)";
const char* const HTML_CODE_OFF = "(_!H!T!M!L!_!C!O!D!E!_!O!F!F!_)";

constant BLOCK_NO = "Бл.N " ;

constant AppNameMinistry = "МИНИСТЕРСТВО НА ПРАВОСЪДИЕТО" ;
constant AppNameMinistryShort = "МП" ;  // МП->МППЕ->МП-> ...
constant AppNameTheMinistry = "Министерството на правосъдието" ;

constant KEEPING_METHOD_PRT_STRING = "Мерки за неотклонение" ;

#if COURT_TYPE == COURT_APPEAL
   constant DEFAULT_FORMAT_FILE_EXTENSION = ".fmp" ;
#endif  // APPEAL
#if COURT_TYPE == COURT_ADMIN
   constant DEFAULT_FORMAT_FILE_EXTENSION = ".fmd" ;
#endif  // ADMIN
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
   constant DEFAULT_FORMAT_FILE_EXTENSION = ".fma" ;
#endif  // AREAL
#if COURT_TYPE == COURT_REGIONAL
   constant DEFAULT_FORMAT_FILE_EXTENSION = ".fmt" ;
#endif  // REGIONAL

constant UNKNOWN_CRIMINAL = "Неизвестен извършител" ;

#if COURT_TYPE == COURT_APPEAL
constant HIGHER_INST_1 = "По-висока инстанция" ;
constant HIGHER_INST_2 = "По-висока инстанция II" ;
#endif  // APPEAL

#if COURT_TYPE == COURT_ADMIN
constant HIGHER_INST_1 = "По-висока инстанция" ;
constant HIGHER_INST_2 = "По-висока инстанция II" ;
#endif  // ADMIN

#if COURT_TYPE == COURT_AREAL
constant HIGHER_INST_1 = "Апелативен съд" ;
constant HIGHER_INST_2 = "Апелативен съд II" ;
#endif  // AREAL

#if COURT_TYPE == COURT_MILITARY
constant HIGHER_INST_1 = "По-висока инстанция" ;
constant HIGHER_INST_2 = "По-висока инстанция II" ;
#endif  // MILITARY

#if COURT_TYPE == COURT_REGIONAL
constant HIGHER_INST_1 = "Окръжен съд" ;
constant HIGHER_INST_2 = "Окръжен съд II" ;
#endif  // REGIONAL

constant AGE_NONES =
  "0"
  ;
constant AGE_UP_TO_ONES =  // 10.feb.2005
  "1"  // До 1 месец
  ;
constant AGE_UP_TO_THREES =
  "1"  // До 1 месец
  "2"  // От 1 до 3 месеца
  "3"  // До 3 месеца
  ;

constant  AGE_MORE_THAN_THREES_ALL =  //20050318
  "6"  // От 3 до 6 месеца
  "7"  // От 6 до 12 месеца
  "9"  // Над 3 месеца
  "8"  // Над 12 месеца
  ;

constant AGE_UP_TO_SIXES =  // 10.feb.2005
  "1"  // До 1 месец
  "2"  // От 1 до 3 месеца
  "3"  // До 3 месеца
  "6"  // От 3 до 6 месеца
  ;
constant AGE_UP_TO_TWELVES =  // 10.feb.2005
  "1"  // До 1 месец
  "2"  // От 1 до 3 месеца
  "3"  // До 3 месеца
  "6"  // От 3 до 6 месеца
  "7"  // От 6 до 12 месеца
  ;
constant AGE_MORE_THAN_THREES =
  "9"  // Над 3 месеца
  ;
constant AGE_MORE_THAN_TWELVES =  // 10.feb.2005
  "8"  // Над 12 месеца
  ;

#     if PUNISHMENT_04_AUG_2000_PATCH  // 30.oct.2000
const CDate PATCH_04_AUG_2000_RDT_DATE (1, 1, 2001) ;  // 28.sep.2000
#     endif  // PUNISHMENT_04_AUG_2000_PATCH  // 30.oct.2000
const CDate VARNA_FIRM_LAWS_NOT_FIN_DATE (1, 1, 1998) ;  // 05.oct.2000

constant GRANT_REJECTEDS =
  "r"  // Отхвърлен
  "z"  // Обявява за нищожно
  ;  // 20.apr.2001

constant DEFAULT_BRIEF_SESSION_FIELDS =
  "F_TYPE, F_YEAR, F_NO, F_DATE, F_KIND, F_RESULT, F_RETURNED, "
  "F_JUDGE, F_JUDGE_X, F_REJUDGE_X, F_PRESIDENT, F_TIME" ;  // 05.jun.2001


# if INSTANCE    // 20060331
#    if APPEAL   // APPEAL
  constant TB_SUPPRESS_SESS_RESULTS = " ";
#    else        // AREAL
  constant TB_SUPPRESS_SESS_RESULTS =
    "B" // Прекратено за доразследване по чл.287
    "P"	// Прекратено по спогодба
    "s" // Изпратено по подсъдност
    "c" // Прекратено
     ;
#    endif
# else           // REGIONAL
  constant TB_SUPPRESS_SESS_RESULTS =
    "B" // Върнато за доразследване
    "P"	// Прекратено по спогодба
    "s" // Изпратено по подсъдност
    "c" // Прекратено
     ;
# endif

//
