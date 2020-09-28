const int
                                        // �����
                                        // -----

  REGIME_INVENTORY_BOOK        =   100,  // ������
  REGIME_ABC_BOOK              =   200,  // ��������
  REGIME_TIME_BOOK             =   300,  // ������
  REGIME_CLOSED_SESS_BOOK      =   400,  // ������� ���������
  REGIME_INPUT_REGISTER_BOOK   =   500,  // ������ ������� �� ��������
  REGIME_APPEALED_IN_REG_BOOK  =   600,  // ������ ��������
  REGIME_APPEALED_OUT_REG_BOOK =   700,  // ������� ��������
  REGIME_APPEALED_LAWS_BOOK    =   800,  // ��������� �� ����
  REGIME_EXECUTION_BOOK        =   900,  // ���������� �� �������
  REGIME_TRADE_634_BOOK        =  1000,  // ��. 634� �� ��
  REGIME_SLOWDOWN_BOOK         =  1100,  // ����� �� �������
  REGIME_PROVE_BOOK            =  1200,  // ���������� �������������
  REGIME_PENALTY_BOOK          =  1300,  // ����� � ��������� �� ��������� ����
  REGIME_ARCHIVE_BOOK          =  1400,  // �������
  REGIME_WHOLE_INPUT_FLOW_BOOK =  1500,  // ������ �������

  REGIME_BOOK_ZZDN             =  1600,  // ����� ����          // 20051010
  REGIME_BOOK_HERITAGE         =  1700,  // ����� ��������/����� �� ���������� // 20060201

                                         // �������������
                                         // -------------

  REGIME_INVENTORY_SUMMARY         = 10100,  // ������ - printDescBkSummary
  REGIME_INVENTORY_CODEVAL_SUMMARY = 10100,  // ������ - ���/�������� - print_code_value_desc_bk_summary
  REGIME_TIME_SUMMARY              = 10300,  // ������
  REGIME_TIME_JUDGES_SUMMARY       = 10320,  // ������ - �����
  REGIME_TIME_RESULTS_SUMMARY      = 10340,  // ������ - ���������
  REGIME_CLOSED_SESS_SUMMARY       = 10400,  // ������� ���������
  REGIME_CLOSED_SESS_RES_SUMMARY   = 10420,  // ������� ��������� - ���������
  REGIME_APPEALED_LAWS_SUMMARY     = 10800,  // ��������� �� ����

                                         // �������
                                         // -------

  REGIME_JUDGE_CHECKUP             = 20100,  // �� �����
  REGIME_JUDGE_EXTENDED_CHECKUP    = 20200,  // �� ����� - ���������
  REGIME_JUDGE_INLAWDISTR_CHECKUP  = 20400,  // �� ����� - �����. ��������� ����
  REGIME_JUDGE_FINLAWDISTR_CHECKUP = 20600,  // �� ����� - �����. �������� ����
  REGIME_JUDGE_SESS_DISTR_CHECKUP  = 20600,  // �� ����� - ���������
  REGIME_WEB_CHECKUP               = 20700,  // �� Web ��������
  REGIME_WEB_BANKRUPTCY_CHECKUP    = 20800,  // �� Web �������� - ���������������
  REGIME_WEB_POLICE_CHECKUP        = 20900,  // �� Web �������� - �� ����
  REGIME_BANKRUPTCY_WIDE_CHECKUP   = 21000,  // �� ������ �� ���������������
  REGIME_APPEAL_JUDGE_CHECKUP      = 21100,  // �� ����� - ���������� ���

                                         // �������
                                         // -------

  REGIME_SIDE_LIST                 = 30100,  // �� ��������
  REGIME_SUBPOENA_LIST             = 30200,  // �� ��������������
  REGIME_SESSION_LIST              = 30300,  // �� ����������� ���������
  REGIME_MAIL_LIST                 = 30400,  // �� �����������, ��������� �� ������
  REGIME_KNOWN_PERSON_LIST         = 30500,  // �� ������ � ���������� ���������������

                                         // �����������
                                         // -----------

  REGIME_NOTIFYING_NOTIFYING       = 40100,  // �����������
  REGIME_SUBPOENA_NOTIFYING        = 40200,  // ��������
  REGIME_MDR_NOTIFYING             = 40300,  // ���
  REGIME_INREG_ANNOUNCE_NOTIFYING  = 40400,  // ������������ �� ������ ��������
  REGIME_REQ_ANNOUNCE_NOTIFYING    = 40500,  // ��������� �� ������ ��������
  REGIME_LETTER_NOTIFYING          = 40600,  // ����� ��
  REGIME_STATE_PAPER_NOTIFYING     = 40600,  // ����� � �������� �������

                                         // ������� ������
                                         // --------------

  REGIME_ACT_EXECUTION             = 50100,  // ������� ������

                                         // ������
                                         // ------

  REGIME_REPORT                    = 60100,  // ����� I ���������
  REGIME_REPORT_2                  = 60200,  // ����� II ���������

                                         // �������
                                         // -------

  REGIME_DISTRIBUTION_HISTORY      = 70100,  // �� �������������� ��������
  REGIME_FIRM_DISTRIBUTION_HISTORY = 70200,  // �� �������������� ������� ���� � ��������� �� �������

                                         // ������
                                         // -----
  //??? - Set proper values below...
  //+ 3 functions from _DesJudg.cpp
  REGIME_DECISION_OR_JUDGEM_PATCH  = 80100,  // ����� ������ - printDecisionOrJudgement
  REGIME_UNDO_RESULT_PATCH         = 80200,  // ����� ������?
  REGIME_ENDDOCS_SURRMENTS_PATCH   = 80300,  // ����� ������?
  REGIME_PENALTY_BOOK_PATCH        = 80400,  // ����� ����� � ��������� �� ��������� ����
  REGIME_APPEALED_LAWS_BOOK_PATCH  = 80500,  // ����� ��������� �� ����
  REGIME_APPL_OUT_REG_BOOK_PATCH   = 80600,  // ����� ������� ��������
  REGIME_EXECUTION_BOOK_PATCH      = 80700,  // ����� ���������� �� ������� - print_one_judgement

                                         // �������������
                                         // -------------

  REGIME_PREPARE_DISTRIBUTION      = 90100,  // ���������� �� ��������� �� �������������
  REGIME_PREPARE_FIRM_DISTRIBUTION = 90200,  // ���������� �� ������� ��������� �� �������������
  REGIME_PERFORM_DISTRIBUTION      = 90300,  // ������������� �� ���������
  REGIME_PERFORM_FIRM_DISTRIBUTION = 90400,  // ������������� �� ������� ���������

                                         // �����
                                         // -----

  REGIME_OUTREG_ACCOMPANY_LETTER   = 100100,  // �� ������� ��������
  REGIME_OUTREG_RET_ACCOMP_LETTER  = 100200,  // �� ������� �������� - �� ������� ����
  REGIME_EXPENSE_CASH_WARRANT      = 100300   // �������� ����� �����
  
;

