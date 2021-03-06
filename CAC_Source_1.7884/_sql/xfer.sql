CREATE TABLE T_XFER_DOCUMENT
(
 F_XFER_KIND INTEGER NOT NULL,
 F_NO INTEGER NOT NULL,
 F_YEAR INTEGER NOT NULL,
 F_COURT_SENDER INTEGER NOT NULL,
 F_INDOC_KIND INTEGER,
 F_INDOC_NO INTEGER,
 F_INDOC_YEAR INTEGER,
 F_INDOC_FILE CHAR(255),
 F_OUTREG_KIND INTEGER NOT NULL,
 F_OUTREG_NO INTEGER NOT NULL,
 F_OUTREG_YEAR INTEGER NOT NULL,
 F_RETURN_RESULT CHAR(10),
 F_COURT_RECEIVER INTEGER NOT NULL,
 F_XFER_TYPE CHAR(1) NOT NULL,
 F_IO INTEGER,
 F_EISPP_NO CHAR(14),
 CONSTRAINT I_XFER_DOCUMENT PRIMARY KEY (F_XFER_KIND, F_NO, F_YEAR, F_COURT_SENDER),
 CONSTRAINT C_XFER_DOCUMENT_XFER_KIND CHECK (F_XFER_KIND BETWEEN 1 AND 99999),
 CONSTRAINT C_XFER_DOCUMENT_NO CHECK (F_NO BETWEEN 1 AND 999999),
 CONSTRAINT C_XFER_DOCUMENT_YEAR CHECK (F_YEAR BETWEEN 1970 AND 2037),
 CONSTRAINT C_XFER_DOCUMENT_COURT_SENDER CHECK (F_COURT_SENDER BETWEEN 1 AND 999),
 CONSTRAINT C_XFER_DOCUMENT_INDOC_KIND CHECK ((F_INDOC_KIND BETWEEN 0 AND 99999) OR (F_INDOC_KIND IS NULL)),
 CONSTRAINT C_XFER_DOCUMENT_INDOC_NO CHECK ((F_INDOC_NO BETWEEN 0 AND 999999) OR (F_INDOC_NO IS NULL)),
 CONSTRAINT C_XFER_DOCUMENT_INDOC_YEAR CHECK ((F_INDOC_YEAR = 0) OR (F_INDOC_YEAR BETWEEN 1970 AND 2037) OR (F_INDOC_YEAR IS NULL)),
 CONSTRAINT C_XFER_DOCUMENT_OUTREG_KIND CHECK (F_OUTREG_KIND BETWEEN 1 AND 99999),
 CONSTRAINT C_XFER_DOCUMENT_OUTREG_NO CHECK (F_OUTREG_NO BETWEEN 1 AND 9999999),
 CONSTRAINT C_XFER_DOCUMENT_OUTREG_YEAR CHECK (F_OUTREG_YEAR BETWEEN 1970 AND 2037),
 CONSTRAINT C_XFER_DOCUMENT_COURT_RECEIVER CHECK (F_COURT_RECEIVER BETWEEN 1 AND 999),
 CONSTRAINT C_XFER_DOCUMENT_XFER_TYPE CHECK ((F_XFER_TYPE BETWEEN 'a' AND 'z') OR (F_XFER_TYPE BETWEEN 'A' AND 'Z')),
 CONSTRAINT C_XFER_DOCUMENT_IO CHECK ((F_IO = 1) OR (F_IO IS NULL))
);
CREATE TABLE T_XFER_CONNECT
(
 F_XFER_KIND INTEGER NOT NULL,
 F_NO INTEGER NOT NULL,
 F_YEAR INTEGER NOT NULL,
 F_COURT_SENDER INTEGER NOT NULL,
 F_CONNECT_TYPE INTEGER NOT NULL,
 F_CONNECT_COURT INTEGER NOT NULL,
 F_CONNECT_KIND INTEGER NOT NULL,
 F_CONNECT_NO INTEGER NOT NULL,
 F_CONNECT_YEAR INTEGER NOT NULL,
 CONSTRAINT I_XFER_CONNECT PRIMARY KEY (F_XFER_KIND, F_NO, F_YEAR, F_COURT_SENDER, F_CONNECT_TYPE),
 CONSTRAINT C_XFER_CONNECT_XFER_KIND CHECK (F_XFER_KIND BETWEEN 1 AND 99999),
 CONSTRAINT C_XFER_CONNECT_NO CHECK (F_NO BETWEEN 1 AND 999999),
 CONSTRAINT C_XFER_CONNECT_YEAR CHECK (F_YEAR BETWEEN 1970 AND 2037),
 CONSTRAINT C_XFER_CONNECT_COURT_SENDER CHECK (F_COURT_SENDER BETWEEN 1 AND 999),
 CONSTRAINT C_XFER_CONNECT_CONNECT_TYPE CHECK (F_CONNECT_TYPE BETWEEN 1 AND 99999),
 CONSTRAINT C_XFER_CONNECT_CONNECT_COURT CHECK (F_CONNECT_COURT BETWEEN 1 AND 999),
 CONSTRAINT C_XFER_CONNECT_CONNECT_KIND CHECK (F_CONNECT_KIND BETWEEN 1 AND 99999),
 CONSTRAINT C_XFER_CONNECT_CONECT_NO CHECK (F_CONNECT_NO BETWEEN 1 AND 999999),
 CONSTRAINT C_XFER_CONNECT_CONNECT_YEAR CHECK (F_CONNECT_YEAR BETWEEN 1970 AND 2037)
);
CREATE TABLE T_XFER_DECISION
(
 F_XFER_KIND INTEGER NOT NULL,
 F_NO INTEGER NOT NULL,
 F_YEAR INTEGER NOT NULL,
 F_COURT_SENDER INTEGER NOT NULL,
 F_DECISION_KIND INTEGER NOT NULL,
 F_DECISION_NO INTEGER,
 F_DECISION_DATE TIMESTAMP NOT NULL,
 F_DECISION_FILE CHAR(255),
 F_JUDGE1_NAME CHAR(14),
 F_JUDGE1_RENAME CHAR(15),
 F_JUDGE1_FAMILY_1 CHAR(22),
 F_JUDGE1_FAMILY_2 CHAR(22),
 F_JUDGE1_EGN CHAR(10),
 F_JUDGE2_NAME CHAR(14),
 F_JUDGE2_RENAME CHAR(15),
 F_JUDGE2_FAMILY_1 CHAR(22),
 F_JUDGE2_FAMILY_2 CHAR(22),
 F_JUDGE2_EGN CHAR(10),
 F_JUDGE3_NAME CHAR(14),
 F_JUDGE3_RENAME CHAR(15),
 F_JUDGE3_FAMILY_1 CHAR(22),
 F_JUDGE3_FAMILY_2 CHAR(22),
 F_JUDGE3_EGN CHAR(10),
 F_MOTIVES_FILE CHAR(255),
 CONSTRAINT I_XFER_DECISION PRIMARY KEY (F_XFER_KIND, F_NO, F_YEAR, F_COURT_SENDER),
 CONSTRAINT C_XFER_DECISION_XFER_KIND CHECK (F_XFER_KIND BETWEEN 1 AND 99999),
 CONSTRAINT C_XFER_DECISION_NO CHECK (F_NO BETWEEN 1 AND 999999),
 CONSTRAINT C_XFER_DECISION_YEAR CHECK (F_YEAR BETWEEN 1970 AND 2037),
 CONSTRAINT C_XFER_DECISION_COURT_SENDER CHECK (F_COURT_SENDER BETWEEN 1 AND 999),
 CONSTRAINT C_XFER_DECISION_DECISION_KIND CHECK (F_DECISION_KIND BETWEEN 1 AND 99999),
 CONSTRAINT C_XFER_DECISION_DECISION_NO CHECK ((F_DECISION_NO BETWEEN 1 AND 999999) OR (F_DECISION_NO IS NULL)),
 CONSTRAINT C_XFER_DECISION_DECISION_DATE CHECK (F_DECISION_DATE BETWEEN '01.01.1970 00:00:00' AND '31.12.2037 23:59:59')
);
CREATE TABLE T_XFER_APPEAL
(
 F_XFER_KIND INTEGER NOT NULL,
 F_NO INTEGER NOT NULL,
 F_YEAR INTEGER NOT NULL,
 F_COURT_SENDER INTEGER NOT NULL,
 F_APPEAL_KIND INTEGER NOT NULL,
 F_APPEAL_NO INTEGER NOT NULL,
 F_APPEAL_YEAR INTEGER NOT NULL,
 F_APPEAL_FILE CHAR(255),
 CONSTRAINT I_XFER_APPEAL PRIMARY KEY (F_XFER_KIND, F_NO, F_YEAR, F_COURT_SENDER, F_APPEAL_NO, F_APPEAL_YEAR),
 CONSTRAINT C_XFER_APPEAL_XFER_KIND CHECK (F_XFER_KIND BETWEEN 1 AND 99999),
 CONSTRAINT C_XFER_APPEAL_NO CHECK (F_NO BETWEEN 1 AND 999999),
 CONSTRAINT C_XFER_APPEAL_YEAR CHECK (F_YEAR BETWEEN 1970 AND 2037),
 CONSTRAINT C_XFER_APPEAL_COURT_SENDER CHECK (F_COURT_SENDER BETWEEN 1 AND 999),
 CONSTRAINT C_XFER_APPEAL_APPEAL_KIND CHECK (F_APPEAL_KIND BETWEEN 1 AND 99999),
 CONSTRAINT C_XFER_APPEAL_APPEAL_NO CHECK (F_APPEAL_NO BETWEEN 1 AND 999999),
 CONSTRAINT C_XFER_APPEAL_APPEAL_YEAR CHECK (F_APPEAL_YEAR BETWEEN 1970 AND 2037)
);
CREATE TABLE T_XFER_SIDE_CITIZEN
(
 F_XFER_KIND INTEGER NOT NULL,
 F_NO INTEGER NOT NULL,
 F_YEAR INTEGER NOT NULL,
 F_COURT_SENDER INTEGER NOT NULL,
 F_APPEAL_KIND INTEGER,
 F_APPEAL_NO INTEGER,
 F_APPEAL_YEAR INTEGER,
 F_SIDE_INVOLVEMENT INTEGER NOT NULL,
 F_SIDE_NAME CHAR(14) NOT NULL,
 F_SIDE_RENAME CHAR(15),
 F_SIDE_FAMILY_1 CHAR(22) NOT NULL,
 F_SIDE_FAMILY_2 CHAR(22),
 F_SIDE_EGN CHAR(10),
 F_SIDE_ID INTEGER NOT NULL,
 F_UCN CHAR(10),
 F_UCN_TYPE CHAR(1),
 CONSTRAINT I_XFER_SIDE_CITIZEN PRIMARY KEY (F_XFER_KIND, F_NO, F_YEAR, F_COURT_SENDER, F_SIDE_ID),
 CONSTRAINT C_XFER_CITIZEN_XFER_KIND CHECK (F_XFER_KIND BETWEEN 1 AND 99999),
 CONSTRAINT C_XFER_CITIZEN_NO CHECK (F_NO BETWEEN 1 AND 999999),
 CONSTRAINT C_XFER_CITIZEN_YEAR CHECK (F_YEAR BETWEEN 1970 AND 2037),
 CONSTRAINT C_XFER_CITIZEN_COURT_SENDER CHECK (F_COURT_SENDER BETWEEN 1 AND 999),
 CONSTRAINT C_XFER_CITIZEN_APPEAL_KIND CHECK ((F_APPEAL_KIND BETWEEN 1 AND 99999) OR (F_APPEAL_KIND IS NULL)),
 CONSTRAINT C_XFER_CITIZEN_APPEAL_NO CHECK ((F_APPEAL_NO BETWEEN 1 AND 999999) OR (F_APPEAL_NO IS NULL)),
 CONSTRAINT C_XFER_CITIZEN_APPEAL_YEAR CHECK ((F_YEAR BETWEEN 1970 AND 2037) OR (F_YEAR IS NULL)),
 CONSTRAINT C_XFER_CITIZEN_SIDE_INVOLVEMENT CHECK (F_SIDE_INVOLVEMENT BETWEEN 1 AND 99999),
 CONSTRAINT C_XFER_CITIZEN_UCN_TYPE CHECK ((F_UCN_TYPE BETWEEN 'a' AND 'z') OR (F_UCN_TYPE BETWEEN 'A' AND 'Z') OR (F_UCN_TYPE IS NULL))
);
CREATE TABLE T_XFER_SIDE_FIRM
(
 F_XFER_KIND INTEGER NOT NULL,
 F_NO INTEGER NOT NULL,
 F_YEAR INTEGER NOT NULL,
 F_COURT_SENDER INTEGER NOT NULL,
 F_APPEAL_KIND INTEGER,
 F_APPEAL_NO INTEGER,
 F_APPEAL_YEAR INTEGER,
 F_SIDE_INVOLVEMENT INTEGER NOT NULL,
 F_SIDE_NAME CHAR(100) NOT NULL,
 F_SIDE_BULSTAT CHAR(15),
 F_SIDE_ID INTEGER NOT NULL,
 F_UCN CHAR(10),
 F_UCN_TYPE CHAR(1),
 CONSTRAINT I_XFER_SIDE_FIRM PRIMARY KEY (F_XFER_KIND, F_NO, F_YEAR, F_COURT_SENDER, F_SIDE_ID),
 CONSTRAINT C_XFER_SIDE_FIRM_XFER_KIND CHECK (F_XFER_KIND BETWEEN 1 AND 99999),
 CONSTRAINT C_XFER_SIDE_FIRM_NO CHECK (F_NO BETWEEN 1 AND 999999),
 CONSTRAINT C_XFER_SIDE_FIRM_YEAR CHECK (F_YEAR BETWEEN 1970 AND 2037),
 CONSTRAINT C_XFER_SIDE_FIRM_COURT_SENDER CHECK (F_COURT_SENDER BETWEEN 1 AND 999),
 CONSTRAINT C_XFER_SIDE_FIRM_APPEAL_KIND CHECK ((F_APPEAL_KIND BETWEEN 1 AND 99999) OR (F_APPEAL_KIND IS NULL)),
 CONSTRAINT C_XFER_SIDE_FIRM_APPEAL_NO CHECK ((F_APPEAL_NO BETWEEN 1 AND 999999) OR (F_APPEAL_NO IS NULL)),
 CONSTRAINT C_XFER_SIDE_FIRM_APPEAL_YEAR CHECK ((F_YEAR BETWEEN 1970 AND 2037) OR (F_YEAR IS NULL)),
 CONSTRAINT C_XFER_SIDE_FIRM_INVOLVEMENT CHECK (F_SIDE_INVOLVEMENT BETWEEN 1 AND 99999),
 CONSTRAINT C_XFER_SIDE_FIRM_UCN_TYPE CHECK ((F_UCN_TYPE BETWEEN 'a' AND 'z') OR (F_UCN_TYPE BETWEEN 'A' AND 'Z') OR (F_UCN_TYPE IS NULL))
);
CREATE TABLE T_XFER_WEB
(
 F_XFER_KIND INTEGER NOT NULL,
 F_NO INTEGER NOT NULL,
 F_YEAR INTEGER NOT NULL,
 F_COURT_SENDER INTEGER NOT NULL,
 F_JUDGE_TEXT CHAR(250),
 F_LOWER_COURT INTEGER,
 F_LOWER_TYPE INTEGER,
 F_LOWER_KIND INTEGER,
 F_LOWER_NO INTEGER,
 F_LOWER_YEAR INTEGER,
 F_APPEALED_KIND INTEGER,
 F_APPEALED_NO INTEGER,
 F_APPEALED_DATE TIMESTAMP,
 F_JURISD_COURT INTEGER,
 F_JURISD_TYPE INTEGER,
 F_JURISD_KIND INTEGER,
 F_JURISD_NO INTEGER,
 F_JURISD_YEAR INTEGER,
 F_REBORN_COURT INTEGER,
 F_REBORN_TYPE INTEGER,
 F_REBORN_KIND INTEGER,
 F_REBORN_NO INTEGER,
 F_REBORN_YEAR INTEGER,
 F_DECISION_KIND INTEGER NOT NULL,
 F_DECISION_NO INTEGER,
 F_DECISION_DATE TIMESTAMP NOT NULL,
 F_DECISION_FILE CHAR(255),
 F_DECISION_ID CHAR(36) NOT NULL,
 F_MOTIVES_DATE TIMESTAMP,
 F_MOTIVES_FILE CHAR(255),
 F_FORCE_DATE TIMESTAMP,
 F_RETURN_RESULT CHAR(10),
 F_OUTREG_KIND INTEGER,
 F_OUTREG_NO INTEGER,
 F_OUTREG_YEAR INTEGER,
 F_OUTREG_COURT INTEGER,
 F_OUTREG_DATE TIMESTAMP,
 F_SESSION_DATE TIMESTAMP NOT NULL,
 F_DECISION_CHAR CHAR(1) NOT NULL,
 F_IO INTEGER,
 F_XFER_DATE TIMESTAMP,
 F_TYPE CHAR(1),
 CONSTRAINT I_XFER_WEB PRIMARY KEY (F_XFER_KIND, F_NO, F_YEAR, F_COURT_SENDER, F_SESSION_DATE, F_DECISION_CHAR),
 CONSTRAINT C_XFER_WEB_XFER_KIND CHECK (F_XFER_KIND BETWEEN 1 AND 99999),
 CONSTRAINT C_XFER_WEB_NO CHECK (F_NO BETWEEN 1 AND 999999),
 CONSTRAINT C_XFER_WEB_YEAR CHECK (F_YEAR BETWEEN 1970 AND 2037),
 CONSTRAINT C_XFER_WEB_COURT_SENDER CHECK (F_COURT_SENDER BETWEEN 1 AND 999),
 CONSTRAINT C_XFER_WEB_LOWER_COURT CHECK ((F_LOWER_COURT BETWEEN 1 AND 999) OR (F_LOWER_COURT IS NULL)),
 CONSTRAINT C_XFER_WEB_LOWER_TYPE CHECK ((F_LOWER_TYPE BETWEEN 1 AND 99999) OR (F_LOWER_TYPE IS NULL)),
 CONSTRAINT C_XFER_WEB_LOWER_KIND CHECK ((F_LOWER_KIND BETWEEN 1 AND 99999) OR (F_LOWER_KIND IS NULL)),
 CONSTRAINT C_XFER_WEB_LOWER_NO CHECK ((F_LOWER_NO BETWEEN 1 AND 999999) OR (F_LOWER_NO IS NULL)),
 CONSTRAINT C_XFER_WEB_LOWER_YEAR CHECK ((F_LOWER_YEAR BETWEEN 1970 AND 2037) OR (F_LOWER_YEAR IS NULL)),
 CONSTRAINT C_XFER_WEB_APPEALED_KIND CHECK ((F_APPEALED_KIND BETWEEN 1 AND 99999) OR (F_APPEALED_KIND IS NULL)),
 CONSTRAINT C_XFER_WEB_APPEALED_NO CHECK ((F_APPEALED_NO BETWEEN 1 AND 999999) OR (F_APPEALED_NO IS NULL)),
 CONSTRAINT C_XFER_WEB_JURISD_COURT CHECK ((F_JURISD_COURT BETWEEN 1 AND 999) OR (F_JURISD_COURT IS NULL)),
 CONSTRAINT C_XFER_WEB_JURISD_TYPE CHECK ((F_JURISD_TYPE BETWEEN 1 AND 99999) OR (F_JURISD_TYPE IS NULL)),
 CONSTRAINT C_XFER_WEB_JURISD_KIND CHECK ((F_JURISD_KIND BETWEEN 1 AND 99999) OR (F_JURISD_KIND IS NULL)),
 CONSTRAINT C_XFER_WEB_JURISD_NO CHECK ((F_JURISD_NO BETWEEN 1 AND 999999) OR (F_JURISD_NO IS NULL)),
 CONSTRAINT C_XFER_WEB_JURISD_YEAR CHECK ((F_JURISD_YEAR BETWEEN 1970 AND 2037) OR (F_JURISD_YEAR IS NULL)),
 CONSTRAINT C_XFER_WEB_REBORN_COURT CHECK ((F_REBORN_COURT BETWEEN 1 AND 999) OR (F_REBORN_COURT IS NULL)),
 CONSTRAINT C_XFER_WEB_REBORN_TYPE CHECK ((F_REBORN_TYPE BETWEEN 1 AND 99999) OR (F_REBORN_TYPE IS NULL)),
 CONSTRAINT C_XFER_WEB_REBORN_KIND CHECK ((F_REBORN_KIND BETWEEN 1 AND 99999) OR (F_REBORN_KIND IS NULL)),
 CONSTRAINT C_XFER_WEB_REBORN_NO CHECK ((F_REBORN_NO BETWEEN 1 AND 999999) OR (F_REBORN_NO IS NULL)),
 CONSTRAINT C_XFER_WEB_REBORN_YEAR CHECK ((F_REBORN_YEAR BETWEEN 1970 AND 2037) OR (F_REBORN_YEAR IS NULL)),
 CONSTRAINT C_XFER_WEB_DECISION_KIND CHECK (F_DECISION_KIND BETWEEN 1 AND 99999),
 CONSTRAINT C_XFER_WEB_DECISION_NO CHECK ((F_DECISION_NO BETWEEN 1 AND 999999) OR (F_DECISION_NO IS NULL)),
 CONSTRAINT C_XFER_WEB_OUTREG_KIND CHECK ((F_OUTREG_KIND BETWEEN 1 AND 99999) OR (F_OUTREG_KIND IS NULL)),
 CONSTRAINT C_XFER_WEB_OUTREG_NO CHECK ((F_OUTREG_NO BETWEEN 1 AND 99999) OR (F_OUTREG_NO IS NULL)),
 CONSTRAINT C_XFER_WEB_OUTREG_YEAR CHECK ((F_OUTREG_YEAR BETWEEN 1970 AND 2037) OR (F_OUTREG_YEAR IS NULL)),
 CONSTRAINT C_XFER_WEB_OUTREG_COURT CHECK ((F_OUTREG_COURT BETWEEN 1 AND 999) OR (F_OUTREG_COURT IS NULL)),
 CONSTRAINT C_XFER_WEB_IO CHECK ((F_IO = 1) OR (F_IO IS NULL)),
 CONSTRAINT C_XFER_WEB_TYPE CHECK ((F_TYPE IN ('c', 'm', 'p', 't', 'a', 'd')) OR (F_TYPE IS NULL))
);
CREATE TABLE T_XFER_INVEST
(
 F_CASE_NUMBER CHAR(21) NOT NULL,
 F_INVEST_ID INTEGER NOT NULL,
 F_RULING_ID CHAR(9),
 F_RULING_DATE TIMESTAMP,
 F_RULING_TEXT VARCHAR(7800),
 F_XFER_TYPE CHAR(1) NOT NULL,
 F_IO INTEGER,
 CONSTRAINT I_XFER_INVEST PRIMARY KEY (F_CASE_NUMBER, F_INVEST_ID, F_XFER_TYPE),
 CONSTRAINT C_XFER_INVEST_INVEST_ID CHECK (NOT F_INVEST_ID = 0),
 CONSTRAINT C_XFER_INVEST_XFER_TYPE CHECK (F_XFER_TYPE BETWEEN 'a' AND 'z'),
 CONSTRAINT C_XFER_INVEST_IO CHECK ((F_IO = 1) OR (F_IO IS NULL))
);
CREATE TABLE T_XFER_EXPERT
(
 F_CASE_NUMBER CHAR(21) NOT NULL,
 F_INVEST_ID INTEGER NOT NULL,
 F_INVEXP_ID INTEGER NOT NULL,
 F_EXPERT_ID INTEGER,
 F_EXPERT_EGN CHAR(10),
 F_EXPERT_NAME CHAR(14),
 F_EXPERT_RENAME CHAR(15),
 F_EXPERT_FAMILY CHAR(22),
 F_EXPERT_UCLP INTEGER,
 F_NOTIFY_DATE TIMESTAMP,
 F_CHALLENGE_DATE TIMESTAMP,
 F_XFER_TYPE CHAR(1) NOT NULL,
 F_IO INTEGER,
 CONSTRAINT I_XFER_EXPERT PRIMARY KEY (F_CASE_NUMBER, F_INVEST_ID, F_INVEXP_ID, F_XFER_TYPE),
 CONSTRAINT C_XFER_EXPERT_INVEST_ID CHECK (NOT F_INVEST_ID = 0),
 CONSTRAINT C_XFER_EXPERT_INVEXP_ID CHECK (NOT F_INVEXP_ID = 0),
 CONSTRAINT C_XFER_EXPERT_EXPERT_ID CHECK ((NOT F_EXPERT_ID = 0) OR (F_EXPERT_ID IS NULL)),
 CONSTRAINT C_XFER_EXPERT_EXPERT_UCLP CHECK ((F_EXPERT_UCLP BETWEEN -9999 AND -1) OR (F_EXPERT_UCLP BETWEEN 1 AND 99999) OR (F_EXPERT_UCLP IS NULL)),
 CONSTRAINT C_XFER_EXPERT_XFER_TYPE CHECK (F_XFER_TYPE BETWEEN 'a' AND 'z'),
 CONSTRAINT C_XFER_EXPERT_IO CHECK ((F_IO = 1) OR (F_IO IS NULL))
);
CREATE TABLE T_XFER_EMAIL
(
 F_XFER_DATE TIMESTAMP NOT NULL,
 F_NO INTEGER NOT NULL,
 F_YEAR INTEGER NOT NULL,
 F_TYPE CHAR(1) NOT NULL,
 F_UCN CHAR(10) NOT NULL,
 F_EMAIL CHAR(100) NOT NULL,
 F_TEXT VARCHAR(7800) NOT NULL,
 F_FILENAME CHAR(255),
 CONSTRAINT I_XFER_EMAIL PRIMARY KEY (F_XFER_DATE, F_NO, F_YEAR, F_TYPE, F_UCN, F_EMAIL)
);
CREATE TABLE T_EPORT_SEND
(
 F_NO INTEGER NOT NULL,
 F_YEAR INTEGER NOT NULL,
 F_TYPE CHAR(1) NOT NULL,
 F_RECORD_NO INTEGER NOT NULL,
 F_WHAT INTEGER NOT NULL,
 F_KIND CHAR(1),
 F_DATE INTEGER,
 F_UCN CHAR(10),
 F_UCN_TYPE CHAR(1),
 F_INVOLVEMENT CHAR(1),
 F_EVENT_STATUS CHAR(1),
 F_SUBPOENA_NO INTEGER,
 F_ELECTRIC CHAR(1),
 F_OPER CHAR(1) NOT NULL,
 F_STATUS CHAR(1) NOT NULL,
 F_TEXT VARCHAR(1024),
 CONSTRAINT I_EPORT_SEND PRIMARY KEY (F_NO, F_YEAR, F_TYPE, F_RECORD_NO),
 CONSTRAINT C_EPORT_SEND_NO CHECK (F_NO BETWEEN 1 AND 999999),
 CONSTRAINT C_EPORT_SEND_YEAR CHECK (F_YEAR BETWEEN 1970 AND 2037),
 CONSTRAINT C_EPORT_SEND_TYPE CHECK ((F_TYPE BETWEEN 'a' AND 'z') OR (F_TYPE BETWEEN 'A' AND 'Z')),
 CONSTRAINT C_EPORT_SEND_KIND CHECK ((F_KIND BETWEEN '!' AND '�') OR (F_KIND IS NULL)),
 CONSTRAINT C_EPORT_SEND_UCN_TYPE CHECK ((F_UCN_TYPE BETWEEN 'a' AND 'z') OR (F_UCN_TYPE BETWEEN 'A' AND 'Z') OR (F_UCN_TYPE IS NULL)),
 CONSTRAINT C_EPORT_SEND_INVOLVEMENT CHECK ((F_INVOLVEMENT BETWEEN '0' AND '�') OR (F_INVOLVEMENT IS NULL)),
 CONSTRAINT C_EPORT_SEND_EVENT_STATUS CHECK ((F_EVENT_STATUS BETWEEN '0' AND '�') OR (F_EVENT_STATUS IS NULL)),
 CONSTRAINT C_EPORT_SEND_SUBPOENA_NO CHECK ((F_SUBPOENA_NO BETWEEN 1 AND 64000) OR (F_SUBPOENA_NO IS NULL)),
 CONSTRAINT C_EPORT_SEND_ELECTRIC CHECK ((F_ELECTRIC BETWEEN '0' AND '9') OR (F_ELECTRIC IS NULL))
);
CREATE TABLE T_EPORT_GUID
(
 F_GUID CHAR(36) NOT NULL,
 F_WHAT INTEGER NOT NULL,
 F_NO INTEGER NOT NULL,
 F_YEAR INTEGER NOT NULL,
 F_TYPE CHAR(1) NOT NULL,
 F_KIND CHAR(1),
 F_DATE INTEGER,
 F_UCN CHAR(10),
 F_UCN_TYPE CHAR(1),
 F_INVOLVEMENT CHAR(1),
 F_EVENT_STATUS CHAR(1),
 F_SUBPOENA_NO INTEGER,
 F_ELECTRIC CHAR(1),
 F_STAMP TIMESTAMP,
 CONSTRAINT I_EPORT_GUID PRIMARY KEY (F_GUID),
 CONSTRAINT C_EPORT_GUID_NO CHECK (F_NO BETWEEN 1 AND 999999),
 CONSTRAINT C_EPORT_GUID_YEAR CHECK (F_YEAR BETWEEN 1970 AND 2037),
 CONSTRAINT C_EPORT_GUID_TYPE CHECK ((F_TYPE BETWEEN 'a' AND 'z') OR (F_TYPE BETWEEN 'A' AND 'Z')),
 CONSTRAINT C_EPORT_GUID_KIND CHECK ((F_KIND BETWEEN '!' AND '�') OR (F_KIND IS NULL)),
 CONSTRAINT C_EPORT_GUID_UCN_TYPE CHECK ((F_UCN_TYPE BETWEEN 'a' AND 'z') OR (F_UCN_TYPE BETWEEN 'A' AND 'Z') OR (F_UCN_TYPE IS NULL)),
 CONSTRAINT C_EPORT_GUID_INVOLVEMENT CHECK ((F_INVOLVEMENT BETWEEN '0' AND '�') OR (F_INVOLVEMENT IS NULL)),
 CONSTRAINT C_EPORT_GUID_EVENT_STATUS CHECK ((F_EVENT_STATUS BETWEEN '0' AND '�') OR (F_EVENT_STATUS IS NULL)),
 CONSTRAINT C_EPORT_GUID_SUBPOENA_NO CHECK ((F_SUBPOENA_NO BETWEEN 1 AND 64000) OR (F_SUBPOENA_NO IS NULL)),
 CONSTRAINT C_EPORT_GUID_ELECTRIC CHECK ((F_ELECTRIC BETWEEN '0' AND '9') OR (F_ELECTRIC IS NULL))
);
CREATE TABLE T_EPORT_ASSIGN
(
 F_NO INTEGER NOT NULL,
 F_YEAR INTEGER NOT NULL,
 F_TYPE CHAR(1) NOT NULL,
 F_LAWSUIT_NO INTEGER NOT NULL,
 F_LAWSUIT_YEAR INTEGER NOT NULL,
 F_LAWSUIT_TYPE CHAR(1) NOT NULL,
 F_INVOLVEMENT CHAR(1) NOT NULL,
 F_FLAGS INTEGER,
 CONSTRAINT I_EPORT_ASSIGN PRIMARY KEY (F_NO, F_YEAR, F_TYPE, F_LAWSUIT_NO, F_LAWSUIT_YEAR, F_LAWSUIT_TYPE,
  F_INVOLVEMENT),
 CONSTRAINT C_EPORT_ASSIGN_NO CHECK (F_NO BETWEEN 1 AND 99999),
 CONSTRAINT C_EPORT_ASSIGN_YEAR CHECK (F_YEAR BETWEEN 1970 AND 2037),
 CONSTRAINT C_EPORT_ASSIGN_TYPE CHECK (F_TYPE = 'i'),
 CONSTRAINT C_EPORT_ASSIGN_LAWSUIT_NO CHECK (F_LAWSUIT_NO BETWEEN 1 AND 99999),
 CONSTRAINT C_EPORT_ASSIGN_LAWSUIT_YEAR CHECK (F_LAWSUIT_YEAR BETWEEN 1970 AND 2037),
 CONSTRAINT C_EPORT_ASSIGN_LAWSUIT_TYPE CHECK (F_LAWSUIT_TYPE IN ('c', 'm', 'p', 't', 'a', 'd')),
 CONSTRAINT C_EPORT_ASSIGN_INVOLVEMENT CHECK (F_INVOLVEMENT BETWEEN '0' AND '�')
);
