const int
                                        // Книги
                                        // -----

  REGIME_INVENTORY_BOOK        =   100,  // Описна
  REGIME_ABC_BOOK              =   200,  // Азбучник
  REGIME_TIME_BOOK             =   300,  // Срочна
  REGIME_CLOSED_SESS_BOOK      =   400,  // Закрити заседания
  REGIME_INPUT_REGISTER_BOOK   =   500,  // Входящ дневник за преписки
  REGIME_APPEALED_IN_REG_BOOK  =   600,  // Входящ регистър
  REGIME_APPEALED_OUT_REG_BOOK =   700,  // Изходящ регистър
  REGIME_APPEALED_LAWS_BOOK    =   800,  // Обжалване на дела
  REGIME_EXECUTION_BOOK        =   900,  // Изпълнение на присъди
  REGIME_TRADE_634_BOOK        =  1000,  // Чл. 634В от ТЗ
  REGIME_SLOWDOWN_BOOK         =  1100,  // Жалби за бавност
  REGIME_PROVE_BOOK            =  1200,  // Веществени доказателства
  REGIME_PENALTY_BOOK          =  1300,  // Глоби и присъдени на държавата суми
  REGIME_ARCHIVE_BOOK          =  1400,  // Архивна
  REGIME_WHOLE_INPUT_FLOW_BOOK =  1500,  // Входящ дневник

  REGIME_BOOK_ZZDN             =  1600,  // Книга ЗЗДН          // 20051010
  REGIME_BOOK_HERITAGE         =  1700,  // Книга Приемане/Отказ от наследство // 20060201

                                         // Рекапитулации
                                         // -------------

  REGIME_INVENTORY_SUMMARY         = 10100,  // Описна - printDescBkSummary
  REGIME_INVENTORY_CODEVAL_SUMMARY = 10100,  // Описна - Код/стойност - print_code_value_desc_bk_summary
  REGIME_TIME_SUMMARY              = 10300,  // Срочна
  REGIME_TIME_JUDGES_SUMMARY       = 10320,  // Срочна - съдии
  REGIME_TIME_RESULTS_SUMMARY      = 10340,  // Срочна - резултати
  REGIME_CLOSED_SESS_SUMMARY       = 10400,  // Закрити заседания
  REGIME_CLOSED_SESS_RES_SUMMARY   = 10420,  // Закрити заседания - резултати
  REGIME_APPEALED_LAWS_SUMMARY     = 10800,  // Обжалване на дела

                                         // Справки
                                         // -------

  REGIME_JUDGE_CHECKUP             = 20100,  // По съдии
  REGIME_JUDGE_EXTENDED_CHECKUP    = 20200,  // По съдии - разширена
  REGIME_JUDGE_INLAWDISTR_CHECKUP  = 20400,  // По съдии - разпр. постъпили дела
  REGIME_JUDGE_FINLAWDISTR_CHECKUP = 20600,  // По съдии - разпр. свършили дела
  REGIME_JUDGE_SESS_DISTR_CHECKUP  = 20600,  // По съдии - заседания
  REGIME_WEB_CHECKUP               = 20700,  // За Web страница
  REGIME_WEB_BANKRUPTCY_CHECKUP    = 20800,  // За Web страница - несъстоятелност
  REGIME_WEB_POLICE_CHECKUP        = 20900,  // За Web страница - за РДВР
  REGIME_BANKRUPTCY_WIDE_CHECKUP   = 21000,  // За делата по несъстоятелност
  REGIME_APPEAL_JUDGE_CHECKUP      = 21100,  // По съдии - апелативен съд

                                         // Списъци
                                         // -------

  REGIME_SIDE_LIST                 = 30100,  // На страните
  REGIME_SUBPOENA_LIST             = 30200,  // На уведомяванията
  REGIME_SESSION_LIST              = 30300,  // На насрочените заседания
  REGIME_MAIL_LIST                 = 30400,  // На документите, изпратани по пощата
  REGIME_KNOWN_PERSON_LIST         = 30500,  // На лицата с юридическа правоспособност

                                         // Уведомяване
                                         // -----------

  REGIME_NOTIFYING_NOTIFYING       = 40100,  // Уведомяване
  REGIME_SUBPOENA_NOTIFYING        = 40200,  // Призовка
  REGIME_MDR_NOTIFYING             = 40300,  // СРГ
  REGIME_INREG_ANNOUNCE_NOTIFYING  = 40400,  // Разпореждане по входящ регистър
  REGIME_REQ_ANNOUNCE_NOTIFYING    = 40500,  // Резолюция по входящ документ
  REGIME_LETTER_NOTIFYING          = 40600,  // Писмо до
  REGIME_STATE_PAPER_NOTIFYING     = 40600,  // Обява в Държавен вестник

                                         // Съдебни актове
                                         // --------------

  REGIME_ACT_EXECUTION             = 50100,  // Съдебни актове

                                         // Отчети
                                         // ------

  REGIME_REPORT                    = 60100,  // Отчет I инстанция
  REGIME_REPORT_2                  = 60200,  // Отчет II инстанция

                                         // История
                                         // -------

  REGIME_DISTRIBUTION_HISTORY      = 70100,  // На разпределените преписки
  REGIME_FIRM_DISTRIBUTION_HISTORY = 70200,  // На разпределените фирмени дела и заявления за промяна

                                         // Фишове
                                         // -----
  //??? - Set proper values below...
  //+ 3 functions from _DesJudg.cpp
  REGIME_DECISION_OR_JUDGEM_PATCH  = 80100,  // Книга Срочна - printDecisionOrJudgement
  REGIME_UNDO_RESULT_PATCH         = 80200,  // Книга Срочна?
  REGIME_ENDDOCS_SURRMENTS_PATCH   = 80300,  // Книга Срочна?
  REGIME_PENALTY_BOOK_PATCH        = 80400,  // Книга Глоби и присъдени на държавата суми
  REGIME_APPEALED_LAWS_BOOK_PATCH  = 80500,  // Книга Обжалване на дела
  REGIME_APPL_OUT_REG_BOOK_PATCH   = 80600,  // Книга Изходящ регистър
  REGIME_EXECUTION_BOOK_PATCH      = 80700,  // Книга Изпълнение на присъди - print_one_judgement

                                         // Разпределение
                                         // -------------

  REGIME_PREPARE_DISTRIBUTION      = 90100,  // Подготовка на документи за разпределение
  REGIME_PREPARE_FIRM_DISTRIBUTION = 90200,  // Подготовка на фирмени документи за разпределение
  REGIME_PERFORM_DISTRIBUTION      = 90300,  // Разпределение на документи
  REGIME_PERFORM_FIRM_DISTRIBUTION = 90400,  // Разпределение на фирмени документи

                                         // Други
                                         // -----

  REGIME_OUTREG_ACCOMPANY_LETTER   = 100100,  // По изходящ регистър
  REGIME_OUTREG_RET_ACCOMP_LETTER  = 100200,  // По изходящ регистър - за върнати дела
  REGIME_EXPENSE_CASH_WARRANT      = 100300   // Разходен касов ордер
  
;

