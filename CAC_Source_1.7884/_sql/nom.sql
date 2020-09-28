CREATE TABLE T_TYPE
(
 F_TYPE CHAR(1) NOT NULL,
 F_ORDER INTEGER,
 F_NAME CHAR(40) NOT NULL,
 F_ALIAS CHAR(15) NOT NULL,
 F_ECODE INTEGER,
 CONSTRAINT I_TYPE PRIMARY KEY (F_TYPE),
 CONSTRAINT C_TYPE_TYPE CHECK ((F_TYPE BETWEEN 'a' AND 'z') OR (F_TYPE BETWEEN 'A' AND 'Z')),
 CONSTRAINT C_TYPE_ORDER CHECK ((F_ORDER BETWEEN 1 AND 9999) OR (F_ORDER IS NULL))
);
CREATE TABLE T_KIND
(
 F_KIND CHAR(1) NOT NULL,
 F_ORDER INTEGER,
 F_NAME CHAR(40) NOT NULL,
 F_TYPE CHAR(1),
 F_LAW_KINDS CHAR(40),
 F_SUITORS CHAR(60),
 F_DEFENDANTS CHAR(60),
 F_OTHERS CHAR(60),
 F_ALIAS CHAR(15) NOT NULL,
 F_XFER INTEGER,
 F_EISPP INTEGER,
 F_WHAT INTEGER,
 CONSTRAINT I_KIND PRIMARY KEY (F_KIND),
 CONSTRAINT C_KIND_KIND CHECK (F_KIND BETWEEN '!' AND '�'),
 CONSTRAINT C_KIND_ORDER CHECK ((F_ORDER BETWEEN 1 AND 9999) OR (F_ORDER IS NULL)),
 CONSTRAINT C_KIND_TYPE CHECK ((F_TYPE BETWEEN 'a' AND 'z') OR (F_TYPE BETWEEN 'A' AND 'Z') OR (F_TYPE IS NULL)),
 CONSTRAINT C_KIND_XFER CHECK ((F_XFER BETWEEN 1 AND 99999) OR (F_XFER IS NULL)),
 CONSTRAINT C_KIND_EISPP CHECK ((F_EISPP >= 1) OR (F_EISPP IS NULL)),
 CONSTRAINT C_KIND_WHAT CHECK ((F_WHAT BETWEEN 1 AND 99999) OR (F_WHAT IS NULL))
);
CREATE TABLE T_XFER_KIND
(
 F_XFER_KIND INTEGER NOT NULL,
 F_TYPES INTEGER,
 F_NAME CHAR(40) NOT NULL,
 F_CONNECT_KIND CHAR(1),
 F_EISPP INTEGER,
 CONSTRAINT I_XFER_KIND PRIMARY KEY (F_XFER_KIND),
 CONSTRAINT C_XFER_KIND_XFER_KIND CHECK (F_XFER_KIND BETWEEN 1 AND 99999),
 CONSTRAINT C_XFER_KIND_CONNECT_KIND CHECK ((F_CONNECT_KIND BETWEEN '!' AND '�') OR (F_CONNECT_KIND IS NULL)),
 CONSTRAINT C_XFER_KIND_EISPP CHECK ((F_EISPP >= 1) OR (F_EISPP IS NULL))
);
CREATE TABLE T_SUBJECT
(
 F_SUBJECT INTEGER NOT NULL,
 F_TYPE CHAR(1) NOT NULL,
 F_NAME CHAR(500) NOT NULL,
 F_ALIAS CHAR(40),
 F_FLAGS_X INTEGER,
 F_KIND_I CHAR(1),
 F_KIND_II CHAR(1),
 F_FLAGS_I INTEGER,
 F_FLAGS_II INTEGER,
 F_ATTRIB_I CHAR(60),
 F_ATTRIB_II CHAR(60),
 F_RESULTS_I CHAR(60),
 F_RESULTS_II CHAR(60),
 F_CLOSEDS_I CHAR(60),
 F_CLOSEDS_II CHAR(60),
 F_PENALITIES_I CHAR(99),
 F_PENALITIES_II CHAR(99),
 F_REASONS_I CHAR(99),
 F_REASONS_II CHAR(99),
 F_JUDGE_REPORT_I INTEGER,
 F_JUDGE_REPORT_II INTEGER,
 F_QUICK_REPORT_I INTEGER,
 F_QUICK_REPORT_II INTEGER,
 F_VID_I INTEGER,
 F_VID_II INTEGER,
 F_SBES_I CHAR(255),
 F_SBES_II CHAR(255),
 CONSTRAINT I_SUBJECT PRIMARY KEY (F_SUBJECT, F_TYPE),
 CONSTRAINT C_SUBJECT_SUBJECT CHECK (F_SUBJECT BETWEEN 1 AND 9999999),
 CONSTRAINT C_SUBJECT_TYPE CHECK (F_TYPE IN ('c', 'm', 'p', 't', 'a', 'd')),
 CONSTRAINT C_SUBJECT_KIND_I CHECK ((F_KIND_I BETWEEN '!' AND '�') OR (F_KIND_I IS NULL)),
 CONSTRAINT C_SUBJECT_KIND_II CHECK ((F_KIND_II BETWEEN '!' AND '�') OR (F_KIND_II IS NULL)),
 CONSTRAINT C_SUBJECT_KIND CHECK (NOT (F_KIND_I IS NULL AND F_KIND_II IS NULL)),
 CONSTRAINT C_SUBJECT_JUDGE_REPORT_I CHECK ((F_JUDGE_REPORT_I BETWEEN 1 AND 20) OR (F_JUDGE_REPORT_I IS NULL)),
 CONSTRAINT C_SUBJECT_QUICK_REPORT_I CHECK ((F_QUICK_REPORT_I BETWEEN 1 AND 20) OR (F_QUICK_REPORT_I IS NULL)),
 CONSTRAINT C_SUBJECT_JUDGE_REPORT_II CHECK ((F_JUDGE_REPORT_II BETWEEN 1 AND 20) OR (F_JUDGE_REPORT_II IS NULL)),
 CONSTRAINT C_SUBJECT_QUICK_REPORT_II CHECK ((F_QUICK_REPORT_II BETWEEN 1 AND 20) OR (F_QUICK_REPORT_II IS NULL))
);
CREATE TABLE T_OLD_SUBJECT
(
 F_SUBJECT INTEGER NOT NULL,
 F_NAME CHAR(500) NOT NULL,
 F_TYPE CHAR(1) NOT NULL,
 F_ATTRIB CHAR(60),
 F_RESULTS CHAR(60) NOT NULL,
 F_KIND CHAR(1) NOT NULL,
 F_ALIAS CHAR(40),
 F_COLLEGE CHAR(1),
 F_POSTS CHAR(20),
 F_PENALITIES CHAR(99),
 F_FLAGS INTEGER,
 F_REASONS CHAR(99),
 F_JUDGE_REPORT INTEGER,
 F_QUICK_REPORT INTEGER,
 F_CLOSEDS CHAR(60) NOT NULL,
 F_VID INTEGER,
 F_SBES CHAR(255),
 CONSTRAINT I_OLD_SUBJECT PRIMARY KEY (F_SUBJECT, F_TYPE),
 CONSTRAINT C_OLD_SUBJECT_SUBJECT CHECK (F_SUBJECT BETWEEN 1 AND 9999999),
 CONSTRAINT C_OLD_SUBJECT_TYPE CHECK (F_TYPE IN ('c', 'm', 'p', 't', 'a', 'd')),
 CONSTRAINT C_OLD_SUBJECT_KIND CHECK (F_KIND BETWEEN '!' AND '�'),
 CONSTRAINT C_OLD_SUBJECT_COLLEGE CHECK ((F_COLLEGE IN ('c', 'm', 'p', 't', 'a', 'd')) OR (F_COLLEGE IS NULL)),
 CONSTRAINT C_OLD_SUBJECT_JUDGE_REPORT CHECK ((F_JUDGE_REPORT BETWEEN 1 AND 20) OR (F_JUDGE_REPORT IS NULL)),
 CONSTRAINT C_OLD_SUBJECT_QUICK_REPORT CHECK ((F_QUICK_REPORT BETWEEN 1 AND 20) OR (F_QUICK_REPORT IS NULL))
);
CREATE TABLE T_RESOLUTION
(
 F_RESOLUTION CHAR(1) NOT NULL,
 F_ORDER INTEGER,
 F_NAME CHAR(60) NOT NULL,
 CONSTRAINT I_RESOLUTION PRIMARY KEY (F_RESOLUTION),
 CONSTRAINT C_RESOLUTION_RESOLUTION CHECK (F_RESOLUTION BETWEEN '0' AND '�'),
 CONSTRAINT C_RESOLUTION_ORDER CHECK ((F_ORDER BETWEEN 1 AND 9999) OR (F_ORDER IS NULL))
);
CREATE TABLE T_RESULT
(
 F_RESULT CHAR(1) NOT NULL,
 F_ORDER INTEGER,
 F_NAME CHAR(60) NOT NULL,
 F_XFER INTEGER,
 CONSTRAINT I_RESULT PRIMARY KEY (F_RESULT),
 CONSTRAINT C_RESULT_RESULT CHECK (F_RESULT BETWEEN '0' AND '�'),
 CONSTRAINT C_RESULT_ORDER CHECK ((F_ORDER BETWEEN 1 AND 9999) OR (F_ORDER IS NULL))
);
CREATE TABLE T_UXN_TYPE
(
 F_UCN_TYPE CHAR(1) NOT NULL,
 F_ORDER INTEGER,
 F_NAME CHAR(15) NOT NULL,
 CONSTRAINT I_UXN_TYPE PRIMARY KEY (F_UCN_TYPE),
 CONSTRAINT C_UXN_TYPE_UXN_TYPE CHECK (F_UCN_TYPE BETWEEN 'a' AND 'z'),
 CONSTRAINT C_UXN_TYPE_ORDER CHECK ((F_ORDER BETWEEN 1 AND 9999) OR (F_ORDER IS NULL))
);
CREATE TABLE T_UCN_TYPE
(
 F_UCN_TYPE CHAR(1) NOT NULL,
 F_ORDER INTEGER,
 F_NAME CHAR(15) NOT NULL,
 F_UXN_TYPE CHAR(1) NOT NULL,
 CONSTRAINT I_UCN_TYPE PRIMARY KEY (F_UCN_TYPE),
 CONSTRAINT C_UCN_TYPE_UCN_TYPE CHECK ((F_UCN_TYPE BETWEEN 'a' AND 'z') OR (F_UCN_TYPE BETWEEN 'A' AND 'Z')),
 CONSTRAINT C_UCN_TYPE_ORDER CHECK ((F_ORDER BETWEEN 1 AND 9999) OR (F_ORDER IS NULL)),
 CONSTRAINT C_UCN_TYPE_UXN_TYPE CHECK (F_UXN_TYPE BETWEEN 'a' AND 'z')
);
CREATE TABLE T_INVOLVEMENT
(
 F_INVOLVEMENT CHAR(1) NOT NULL,
 F_ORDER INTEGER,
 F_NAME CHAR(40) NOT NULL,
 F_XFER INTEGER,
 CONSTRAINT I_INVOLVEMENT PRIMARY KEY (F_INVOLVEMENT),
 CONSTRAINT C_INVOLVEMENT_INVOLVEMENT CHECK (F_INVOLVEMENT BETWEEN '0' AND '�'),
 CONSTRAINT C_INVOLVEMENT_ORDER CHECK ((F_ORDER BETWEEN 1 AND 9999) OR (F_ORDER IS NULL)),
 CONSTRAINT C_INVOLVEMENT_XFER CHECK ((F_XFER BETWEEN 1 AND 99999) OR (F_XFER IS NULL))
);
CREATE TABLE T_POST
(
 F_POST CHAR(1) NOT NULL,
 F_ORDER INTEGER,
 F_NAME CHAR(40) NOT NULL,
 CONSTRAINT I_POST PRIMARY KEY (F_POST),
 CONSTRAINT C_POST_POST CHECK (F_POST BETWEEN '0' AND 'z'),
 CONSTRAINT C_POST_ORDER CHECK ((F_ORDER BETWEEN 1 AND 9999) OR (F_ORDER IS NULL))
);
CREATE TABLE T_COLLEGE
(
 F_COLLEGE CHAR(1) NOT NULL,
 F_ORDER INTEGER,
 F_NAME CHAR(40) NOT NULL,
 F_RIGHT_NAME CHAR(40) NOT NULL,
 CONSTRAINT I_COLLEGE PRIMARY KEY (F_COLLEGE),
 CONSTRAINT C_COLLEGE_COLLEGE CHECK (F_COLLEGE BETWEEN 'a' AND 'z'),
 CONSTRAINT C_COLLEGE_ORDER CHECK ((F_ORDER BETWEEN 1 AND 9999) OR (F_ORDER IS NULL))
);
CREATE TABLE T_USER
(
 F_UCN CHAR(10) NOT NULL,
 F_LOGIN CHAR(40) NOT NULL,
 F_MENUS CHAR(10),
 F_FUNCTIONS CHAR(200),
 F_COLLEGES CHAR(10),
 F_POST CHAR(1) NOT NULL,
 F_KIND CHAR(1) NOT NULL,
 F_FLAGS INTEGER,
 F_DATE INTEGER NOT NULL,
 F_LAW_KINDS CHAR(40),
 F_MD5SUM CHAR(32) NOT NULL,
 F_PERCENT INTEGER,
 F_PRELOAD INTEGER,
 F_COMPOSITIONS CHAR(250),
 F_FAILOGS INTEGER,
 F_COMPOS INTEGER,
 F_ACTYPE INTEGER,
 CONSTRAINT I_USER PRIMARY KEY (F_UCN),
 CONSTRAINT I_USER_BY_LOGIN UNIQUE (F_LOGIN),
 CONSTRAINT C_USER_POST CHECK (F_POST BETWEEN '0' AND 'z'),
 CONSTRAINT C_USER_KIND CHECK (F_KIND BETWEEN '!' AND '�'),
 CONSTRAINT C_USER_PERCENT CHECK (F_PERCENT BETWEEN 1 AND 100),
 CONSTRAINT C_USER_OPT_COMPOS CHECK ((F_COMPOS BETWEEN 1 AND 250) OR (F_COMPOS IS NULL))
);
CREATE TABLE T_COMPOSITION
(
 F_NAME CHAR(15) NOT NULL,
 F_JUDGE CHAR(10) NOT NULL,
 F_SECRETAR CHAR(10),
 F_JUDGE_X CHAR(10),
 F_REJUDGE_X CHAR(10),
 F_PRESIDENT CHAR(10),
 F_COLLEGE CHAR(1) NOT NULL,
 F_FLAGS INTEGER,
 F_COMPOSITION INTEGER NOT NULL,
 CONSTRAINT I_COMPOSITION PRIMARY KEY (F_COLLEGE, F_COMPOSITION),
 CONSTRAINT C_COMPOSITION_COLLEGE CHECK (F_COLLEGE IN ('c', 'm', 'p', 't', 'a', 'd')),
 CONSTRAINT C_COMPOSITION_COMPOSITION CHECK (F_COMPOSITION BETWEEN 1 AND 250)
);
CREATE TABLE T_RIGHT
(
 F_RIGHT CHAR(1) NOT NULL,
 F_NAME CHAR(60) NOT NULL,
 CONSTRAINT I_RIGHT PRIMARY KEY (F_RIGHT),
 CONSTRAINT C_RIGHT_RIGHT CHECK (F_RIGHT BETWEEN '0' AND '�')
);
CREATE TABLE T_AGE
(
 F_AGE CHAR(1) NOT NULL,
 F_ORDER INTEGER,
 F_NAME CHAR(15) NOT NULL,
 CONSTRAINT I_AGE PRIMARY KEY (F_AGE),
 CONSTRAINT C_AGE_AGE CHECK (F_AGE BETWEEN '0' AND '9'),
 CONSTRAINT C_AGE_ORDER CHECK ((F_ORDER BETWEEN 1 AND 9999) OR (F_ORDER IS NULL))
);
CREATE TABLE T_GRANT
(
 F_GRANT CHAR(1) NOT NULL,
 F_ORDER INTEGER,
 F_NAME CHAR(40) NOT NULL,
 CONSTRAINT I_GRANT PRIMARY KEY (F_GRANT),
 CONSTRAINT C_GRANT_GRANT CHECK (F_GRANT BETWEEN 'a' AND 'z'),
 CONSTRAINT C_GRANT_ORDER CHECK ((F_ORDER BETWEEN 1 AND 9999) OR (F_ORDER IS NULL))
);
CREATE TABLE T_JUDGED
(
 F_JUDGED CHAR(1) NOT NULL,
 F_ORDER INTEGER,
 F_NAME CHAR(40) NOT NULL,
 CONSTRAINT I_JUDGED PRIMARY KEY (F_JUDGED),
 CONSTRAINT C_JUDGED_JUDGED CHECK (F_JUDGED BETWEEN '0' AND 'z'),
 CONSTRAINT C_JUDGED_ORDER CHECK ((F_ORDER BETWEEN 1 AND 9999) OR (F_ORDER IS NULL))
);
CREATE TABLE T_PUNISHMENT
(
 F_PUNISHMENT CHAR(1) NOT NULL,
 F_ORDER INTEGER,
 F_NAME CHAR(40) NOT NULL,
 CONSTRAINT I_PUNISHMENT PRIMARY KEY (F_PUNISHMENT),
 CONSTRAINT C_PUNISHMENT_PUNISHMENT CHECK (F_PUNISHMENT BETWEEN '0' AND 'z'),
 CONSTRAINT C_PUNISHMENT_ORDER CHECK ((F_ORDER BETWEEN 1 AND 9999) OR (F_ORDER IS NULL))
);
CREATE TABLE T_RECIDIVE
(
 F_RECIDIVE CHAR(1) NOT NULL,
 F_ORDER INTEGER,
 F_NAME CHAR(40) NOT NULL,
 CONSTRAINT I_RECEDIVE PRIMARY KEY (F_RECIDIVE),
 CONSTRAINT C_RECEDIVE_RECIDIVE CHECK (F_RECIDIVE IN ('n', 'c', 's', 'd')),
 CONSTRAINT C_RECEDIVE_ORDER CHECK ((F_ORDER BETWEEN 1 AND 9999) OR (F_ORDER IS NULL))
);
CREATE TABLE T_COUNTRY
(
 F_COUNTRY CHAR(2) NOT NULL,
 F_NAME CHAR(25) NOT NULL,
 F_EISPP INTEGER,
 CONSTRAINT I_COUNTRY PRIMARY KEY (F_COUNTRY),
 CONSTRAINT C_COUNTRY_EISPP CHECK ((F_EISPP >= 1) OR (F_EISPP IS NULL))
);
CREATE TABLE T_AREA
(
 F_AREA INTEGER NOT NULL,
 F_NAME CHAR(25) NOT NULL,
 CONSTRAINT I_AREA PRIMARY KEY (F_AREA),
 CONSTRAINT C_AREA_AREA CHECK ((F_AREA BETWEEN -9 AND -1) OR (F_AREA BETWEEN 1 AND 99))
);
CREATE TABLE T_REGION
(
 F_AREA INTEGER NOT NULL,
 F_REGION INTEGER NOT NULL,
 F_NAME CHAR(25) NOT NULL,
 CONSTRAINT I_REGION PRIMARY KEY (F_AREA, F_REGION),
 CONSTRAINT C_REGION_AREA CHECK ((F_AREA BETWEEN -9 AND -1) OR (F_AREA BETWEEN 1 AND 99)),
 CONSTRAINT C_REGION_REGION CHECK ((F_REGION BETWEEN -9 AND -1) OR (F_REGION BETWEEN 1 AND 99))
);
CREATE TABLE T_UCLP
(
 F_UCLP INTEGER NOT NULL,
 F_AREA INTEGER NOT NULL,
 F_REGION INTEGER NOT NULL,
 F_NAME CHAR(25) NOT NULL,
 CONSTRAINT I_UCLP PRIMARY KEY (F_UCLP),
 CONSTRAINT C_UCLP_AREA CHECK ((F_AREA BETWEEN -9 AND -1) OR (F_AREA BETWEEN 1 AND 99)),
 CONSTRAINT C_UCLP_REGION CHECK ((F_REGION BETWEEN -9 AND -1) OR (F_REGION BETWEEN 1 AND 99)),
 CONSTRAINT C_UCLP_UCLP CHECK ((F_UCLP BETWEEN -9999 AND -1) OR (F_UCLP BETWEEN 1 AND 99999))
);
CREATE TABLE T_STREET
(
 F_UCLP INTEGER NOT NULL,
 F_STREET INTEGER NOT NULL,
 F_NAME CHAR(30) NOT NULL,
 CONSTRAINT I_STREET PRIMARY KEY (F_UCLP, F_STREET),
 CONSTRAINT C_STREET_UCLP CHECK ((F_UCLP BETWEEN -9999 AND -1) OR (F_UCLP BETWEEN 1 AND 99999)),
 CONSTRAINT C_STREET_STREET CHECK ((F_STREET BETWEEN -9999 AND -1) OR (F_STREET BETWEEN 1 AND 99999))
);
CREATE TABLE T_SUBJECT_RANGE
(
 F_SUBJECT INTEGER NOT NULL,
 F_TYPE CHAR(1) NOT NULL,
 F_LOW_SUBJECT_RANGE INTEGER NOT NULL,
 F_HIGH_SUBJECT_RANGE INTEGER NOT NULL,
 F_NAME CHAR(120) NOT NULL,
 F_ATTRIB CHAR(60),
 F_LETTERS CHAR(30),
 CONSTRAINT I_SUBJECT_RANGE PRIMARY KEY (F_SUBJECT, F_TYPE),
 CONSTRAINT C_SUBJECT_RANGE_SUBJECT CHECK (F_SUBJECT BETWEEN 1 AND 9999999),
 CONSTRAINT C_SUBJECT_RANGE_TYPE CHECK (F_TYPE IN ('c', 'm', 'p', 't', 'a', 'd')),
 CONSTRAINT C_SUBJECT_RANGE_LOW_RANGE CHECK (F_LOW_SUBJECT_RANGE BETWEEN 1 AND 9999999),
 CONSTRAINT C_SUBJECT_RANGE_HIGH_RANGE CHECK (F_HIGH_SUBJECT_RANGE BETWEEN 1 AND 9999999)
);
CREATE TABLE T_OLD_SUBJECT_RANGE
(
 F_SUBJECT INTEGER NOT NULL,
 F_TYPE CHAR(1) NOT NULL,
 F_LOW_SUBJECT_RANGE INTEGER NOT NULL,
 F_HIGH_SUBJECT_RANGE INTEGER NOT NULL,
 F_NAME CHAR(500) NOT NULL,
 F_ATTRIB CHAR(60),
 F_LETTERS CHAR(30),
 CONSTRAINT I_OLD_SUBJECT_RANGE PRIMARY KEY (F_SUBJECT, F_TYPE),
 CONSTRAINT C_OLD_SUBJECT_RANGE_SUBJECT CHECK (F_SUBJECT BETWEEN 1 AND 9999999),
 CONSTRAINT C_OLD_SUBJECT_RANGE_TYPE CHECK (F_TYPE IN ('c', 'm', 'p', 't', 'a', 'd')),
 CONSTRAINT C_OLD_SUBJECT_RANGE_LOW_RANGE CHECK (F_LOW_SUBJECT_RANGE BETWEEN 1 AND 9999999),
 CONSTRAINT C_OLD_SUBJECT_RANGE_HIGH_RANGE CHECK (F_HIGH_SUBJECT_RANGE BETWEEN 1 AND 9999999)
);
CREATE TABLE T_MONTH
(
 F_MONTH INTEGER NOT NULL,
 F_NAME CHAR(10) NOT NULL,
 F_ALIAS CHAR(3) NOT NULL,
 CONSTRAINT I_MONTH PRIMARY KEY (F_MONTH),
 CONSTRAINT C_MONTH_MONTH CHECK (F_MONTH BETWEEN 1 AND 12)
);
CREATE TABLE T_COMPANY_TYPE
(
 F_COMPANY_TYPE CHAR(1) NOT NULL,
 F_ORDER INTEGER,
 F_SUBJECT INTEGER NOT NULL,
 F_NAME CHAR(15) NOT NULL,
 F_REGISTER INTEGER,
 F_RENAME CHAR(40) NOT NULL,
 CONSTRAINT I_COMPANY_TYPE PRIMARY KEY (F_COMPANY_TYPE),
 CONSTRAINT C_COMPANY_TYPE_COMPANY_TYPE CHECK (F_COMPANY_TYPE BETWEEN '0' AND 'z'),
 CONSTRAINT C_COMPANY_TYPE_ORDER CHECK ((F_ORDER BETWEEN 1 AND 9999) OR (F_ORDER IS NULL)),
 CONSTRAINT C_COMPANY_TYPE_SUBJECT CHECK (F_SUBJECT BETWEEN 1 AND 9999999),
 CONSTRAINT C_COMPANY_TYPE_COMPANY_REGISTER CHECK ((F_REGISTER BETWEEN 1 AND 99) OR (F_REGISTER IS NULL))
);
CREATE TABLE T_HALL
(
 F_HALL INTEGER NOT NULL,
 F_NAME CHAR(40) NOT NULL,
 F_ALIAS CHAR(5) NOT NULL,
 F_ADDRESS CHAR(60),
 F_FLAGS INTEGER,
 CONSTRAINT I_HALL PRIMARY KEY (F_HALL),
 CONSTRAINT I_HALL_NAME UNIQUE (F_NAME),
 CONSTRAINT I_HALL_ALIAS UNIQUE (F_ALIAS),
 CONSTRAINT C_HALL_HALL CHECK (F_HALL BETWEEN 1 AND 99)
);
CREATE TABLE T_INTERVAL
(
 F_INTERVAL CHAR(1) NOT NULL,
 F_ORDER INTEGER,
 F_NAME CHAR(40) NOT NULL,
 CONSTRAINT I_INTERVAL PRIMARY KEY (F_INTERVAL),
 CONSTRAINT C_INTERVAL_INTERVAL CHECK (F_INTERVAL BETWEEN '0' AND '9'),
 CONSTRAINT C_INTERVAL_ORDER CHECK ((F_ORDER BETWEEN 1 AND 9999) OR (F_ORDER IS NULL))
);
CREATE TABLE T_APPEAL_RESOLUTION
(
 F_RESOLUTION CHAR(1) NOT NULL,
 F_ORDER INTEGER,
 F_RESOLUTIONS CHAR(40),
 F_NAME CHAR(40) NOT NULL,
 CONSTRAINT I_APPEAL_RESOLUTION PRIMARY KEY (F_RESOLUTION),
 CONSTRAINT C_APPEAL_RESOLUTION_RESOLUTION CHECK (F_RESOLUTION BETWEEN '0' AND '�'),
 CONSTRAINT C_APPEAL_RESOLUTION_ORDER CHECK ((F_ORDER BETWEEN 1 AND 9999) OR (F_ORDER IS NULL))
);
CREATE TABLE T_CRIME_FORM
(
 F_CRIME_FORM CHAR(1) NOT NULL,
 F_ORDER INTEGER,
 F_NAME CHAR(40) NOT NULL,
 CONSTRAINT I_CRIME_FORM PRIMARY KEY (F_CRIME_FORM),
 CONSTRAINT C_CRIME_FORM_CRIME_FORM CHECK (F_CRIME_FORM IN ('n', 'e', 'i', 's')),
 CONSTRAINT C_CRIME_FORM_ORDER CHECK ((F_ORDER BETWEEN 1 AND 9999) OR (F_ORDER IS NULL))
);
CREATE TABLE T_CRIME_STAGE
(
 F_CRIME_STAGE CHAR(1) NOT NULL,
 F_ORDER INTEGER,
 F_NAME CHAR(40) NOT NULL,
 CONSTRAINT I_CRIME_STAGE PRIMARY KEY (F_CRIME_STAGE),
 CONSTRAINT C_CRIME_STAGE_CRIME_STAGE CHECK (F_CRIME_STAGE IN ('n', 'c', 'a', 'p')),
 CONSTRAINT C_CRIME_STAGE_ORDER_ORDER CHECK ((F_ORDER BETWEEN 1 AND 9999) OR (F_ORDER IS NULL))
);
CREATE TABLE T_JAIL_MODE
(
 F_JAIL_MODE CHAR(1) NOT NULL,
 F_ORDER INTEGER,
 F_NAME CHAR(40) NOT NULL,
 CONSTRAINT I_JAIL_MODE PRIMARY KEY (F_JAIL_MODE),
 CONSTRAINT C_JAIL_MODE_JAIL_MODE CHECK (F_JAIL_MODE IN ('n', 'l', 'c', 'h', 's', 'H')),
 CONSTRAINT C_JAIL_MODE_ORDER CHECK ((F_ORDER BETWEEN 1 AND 9999) OR (F_ORDER IS NULL))
);
CREATE TABLE T_ACCOMPLY
(
 F_ACCOMPLY CHAR(1) NOT NULL,
 F_ORDER INTEGER,
 F_NAME CHAR(120) NOT NULL,
 F_XFER INTEGER,
 F_FLAGS INTEGER,
 F_DEBTOR_STATUS CHAR(1),
 CONSTRAINT I_ACCOMPLY PRIMARY KEY (F_ACCOMPLY),
 CONSTRAINT C_ACCOMPLY_ACCOMPLY CHECK (F_ACCOMPLY BETWEEN '0' AND '�'),
 CONSTRAINT C_ACCOMPLY_ORDER CHECK ((F_ORDER BETWEEN 1 AND 9999) OR (F_ORDER IS NULL)),
 CONSTRAINT C_ACCOMPLY_XFER CHECK ((F_XFER BETWEEN 1 AND 99999) OR (F_XFER IS NULL)),
 CONSTRAINT C_ACCOMPLY_DEBTOR_STATUS CHECK ((F_DEBTOR_STATUS BETWEEN '0' AND 'z') OR (F_DEBTOR_STATUS IS NULL))
);
CREATE TABLE T_CALENDAR
(
 F_YEAR INTEGER NOT NULL,
 F_JAN CHAR(31),
 F_FEB CHAR(29),
 F_MAR CHAR(31),
 F_APR CHAR(30),
 F_MAY CHAR(31),
 F_JUN CHAR(30),
 F_JUL CHAR(31),
 F_AUG CHAR(31),
 F_SEP CHAR(30),
 F_OCT CHAR(31),
 F_NOV CHAR(30),
 F_DEC CHAR(31),
 CONSTRAINT I_CALENDAR PRIMARY KEY (F_YEAR),
 CONSTRAINT C_CALENDAR_YEAR CHECK (F_YEAR BETWEEN 1970 AND 2037)
);
CREATE TABLE T_CAL_INTERVAL
(
 F_CAL_INTERVAL CHAR(1) NOT NULL,
 F_ORDER INTEGER,
 F_INTERVAL CHAR(1) NOT NULL,
 F_COUNT INTEGER,
 F_DELTA INTEGER,
 F_POST CHAR(1) NOT NULL,
 F_NAME CHAR(40) NOT NULL,
 CONSTRAINT I_CAL_INTERVAL PRIMARY KEY (F_CAL_INTERVAL),
 CONSTRAINT C_CAL_INTERVAL_CAL_INTERVAL CHECK (F_CAL_INTERVAL BETWEEN '0' AND '�'),
 CONSTRAINT C_CAL_INTERVAL_ORDER CHECK ((F_ORDER BETWEEN 1 AND 9999) OR (F_ORDER IS NULL)),
 CONSTRAINT C_CAL_INTERVAL_INTERVAL CHECK (F_INTERVAL BETWEEN '0' AND '9'),
 CONSTRAINT C_CAL_INTERVAL_COUNT CHECK ((F_COUNT BETWEEN 1 AND 90) OR (F_COUNT IS NULL)),
 CONSTRAINT C_CAL_INTERVAL_DELTA CHECK ((F_DELTA BETWEEN -12 AND -11) OR (F_DELTA IS NULL)),
 CONSTRAINT C_CAL_INTERVAL_POST CHECK (F_POST BETWEEN '0' AND 'z')
);
CREATE TABLE T_SUM_TYPE
(
 F_SUM_TYPE CHAR(1) NOT NULL,
 F_ORDER INTEGER,
 F_NAME CHAR(40) NOT NULL,
 F_ALIAS CHAR(15) NOT NULL,
 F_ACCOUNT_TYPE CHAR(1),
 F_EXEC_LIST_TEMPLATE CHAR(250),
 F_FLAGS INTEGER,
 CONSTRAINT I_SUM_TYPE PRIMARY KEY (F_SUM_TYPE),
 CONSTRAINT C_SUM_TYPE_SUM_TYPE CHECK ((F_SUM_TYPE BETWEEN 'a' AND 'z') OR (F_SUM_TYPE BETWEEN 'A' AND 'Z')),
 CONSTRAINT C_SUM_TYPE_ORDER CHECK ((F_ORDER BETWEEN 1 AND 9999) OR (F_ORDER IS NULL)),
 CONSTRAINT C_SUM_TYPE_ACCOUNT_TYPE CHECK ((F_ACCOUNT_TYPE BETWEEN 'a' AND 'z') OR (F_ACCOUNT_TYPE IS NULL))
);
CREATE TABLE T_SUM_STATE
(
 F_SUM_STATE CHAR(1) NOT NULL,
 F_ORDER INTEGER,
 F_NAME CHAR(15) NOT NULL,
 CONSTRAINT I_SUM_STATE PRIMARY KEY (F_SUM_STATE),
 CONSTRAINT C_SUM_STATE_SUM_STATE CHECK (F_SUM_STATE BETWEEN 'a' AND 'z'),
 CONSTRAINT C_SUM_STATE_ORDER CHECK ((F_ORDER BETWEEN 1 AND 9999) OR (F_ORDER IS NULL))
);
CREATE TABLE T_ACCOUNT_TYPE
(
 F_ACCOUNT_TYPE CHAR(1) NOT NULL,
 F_ORDER INTEGER,
 F_NAME CHAR(15) NOT NULL,
 CONSTRAINT I_ACCOUNT_TYPE PRIMARY KEY (F_ACCOUNT_TYPE),
 CONSTRAINT C_ACCOUNT_TYPE_ACCOUNT_TYPE CHECK (F_ACCOUNT_TYPE BETWEEN 'a' AND 'z'),
 CONSTRAINT C_ACCOUNT_TYPE_ORDER CHECK ((F_ORDER BETWEEN 1 AND 9999) OR (F_ORDER IS NULL))
);
CREATE TABLE T_PENALITY_TYPE
(
 F_PENALITY_TYPE INTEGER NOT NULL,
 F_NAME CHAR(20) NOT NULL,
 F_FLAGS INTEGER,
 CONSTRAINT I_PENALITY_TYPE PRIMARY KEY (F_PENALITY_TYPE),
 CONSTRAINT C_PENALITY_TYPE_PENALITY_TYPE CHECK (F_PENALITY_TYPE BETWEEN 1 AND 99)
);
CREATE TABLE T_MAIL_TYPE
(
 F_MAIL_TYPE CHAR(1) NOT NULL,
 F_ORDER INTEGER,
 F_NAME CHAR(40) NOT NULL,
 F_ALIAS CHAR(15) NOT NULL,
 CONSTRAINT I_MAIL_TYPE PRIMARY KEY (F_MAIL_TYPE),
 CONSTRAINT C_MAIL_TYPE_MAIL_TYPE CHECK ((F_MAIL_TYPE BETWEEN 'a' AND 'z') OR (F_MAIL_TYPE BETWEEN 'A' AND 'Z')),
 CONSTRAINT C_MAIL_TYPE_ORDER CHECK ((F_ORDER BETWEEN 1 AND 9999) OR (F_ORDER IS NULL))
);
CREATE TABLE T_CONFIG_NAME
(
 F_CONFIG CHAR(15) NOT NULL,
 F_NO INTEGER,
 F_DATE INTEGER,
 F_VERSION CHAR(10),
 CONSTRAINT I_CONFIG_NAME PRIMARY KEY (F_CONFIG)
);
CREATE TABLE T_CONFIG_DATA
(
 F_CONFIG CHAR(15) NOT NULL,
 F_SECTION CHAR(15) NOT NULL,
 F_NAME CHAR(40) NOT NULL,
 F_VALUE CHAR(255),
 CONSTRAINT I_CONFIG_DATA PRIMARY KEY (F_CONFIG, F_SECTION, F_NAME)
);
CREATE TABLE T_STREET_MAP
(
 F_UCN CHAR(10) NOT NULL,
 F_UCLP INTEGER NOT NULL,
 F_STREET INTEGER NOT NULL,
 F_MAP VARCHAR(999),
 CONSTRAINT I_STREET_MAP PRIMARY KEY (F_UCN, F_UCLP, F_STREET),
 CONSTRAINT C_STREET_MAP_STREET_MAP_UCLP CHECK ((F_UCLP BETWEEN -9999 AND -1) OR (F_UCLP BETWEEN 1 AND 99999)),
 CONSTRAINT C_STREET_MAP_STREET_MAP_STREET CHECK ((F_STREET BETWEEN -9999 AND -1) OR (F_STREET BETWEEN 1 AND 99999))
);
CREATE TABLE T_RECEIVED_STATUS
(
 F_RECEIVED_STATUS CHAR(1) NOT NULL,
 F_NAME CHAR(40) NOT NULL,
 F_ALIAS CHAR(15) NOT NULL,
 F_CLERK CHAR(1),
 F_DELIVERER CHAR(1),
 F_ALLOW_DATE_RECEIVED CHAR(1),
 F_ALLOW_DATE_RETURNED CHAR(1),
 F_ALLOW_SCHEDULE CHAR(1),
 F_SHOW_SUBPOENA_DATE CHAR(1),
 F_ORDER INTEGER,
 CONSTRAINT I_RECEIVED_STATUS PRIMARY KEY (F_RECEIVED_STATUS),
 CONSTRAINT C_RECEIVED_STATUS CHECK (F_RECEIVED_STATUS BETWEEN 'a' AND 'z')
);
CREATE TABLE T_SUBPOENA_KIND
(
 F_SUBPOENA_KIND CHAR(1) NOT NULL,
 F_NAME CHAR(50) NOT NULL,
 F_ALIAS CHAR(15) NOT NULL,
 F_KINDS CHAR(64),
 F_STATUSES CHAR(64),
 F_INVOLVEMENTS CHAR(64),
 F_LAWSUIT_KINDS CHAR(40),
 F_ATTRIBUTES CHAR(50),
 F_PROC CHAR(1),
 F_ORDER INTEGER,
 F_TEMPLATE CHAR(14),
 F_GENERIC_KIND CHAR(1),
 F_INFO CHAR(100),
 F_ADDITIONAL_TEXT VARCHAR(1200),
 F_SURROUND_KINDS CHAR(40),
 CONSTRAINT I_SUBPOENA_KIND PRIMARY KEY (F_SUBPOENA_KIND),
 CONSTRAINT C_SUBPOENA_KIND_SUBPOENA_KIND CHECK (F_SUBPOENA_KIND BETWEEN '0' AND '�')
);
CREATE TABLE T_KNOWN_PERSON
(
 F_UCN CHAR(10) NOT NULL,
 F_UCN_TYPE CHAR(1) NOT NULL,
 F_INVOLVEMENT CHAR(1) NOT NULL,
 F_FLAGS INTEGER,
 F_CSJID INTEGER,
 F_EXPERT_ID INTEGER,
 F_PERSONAL_NO CHAR(10),
 F_BIRTH_DATE TIMESTAMP,
 CONSTRAINT I_KNOWN_PERSON PRIMARY KEY (F_UCN, F_UCN_TYPE, F_INVOLVEMENT),
 CONSTRAINT C_KNOWN_PERSON_UCN_TYPE CHECK ((F_UCN_TYPE BETWEEN 'a' AND 'z') OR (F_UCN_TYPE BETWEEN 'A' AND 'Z')),
 CONSTRAINT C_KNOWN_PERSON_INVOLVEMENT CHECK (F_INVOLVEMENT BETWEEN '0' AND '�'),
 CONSTRAINT C_KNOWN_PERSON_CSJID CHECK ((F_CSJID BETWEEN 1 AND 9999) OR (F_CSJID IS NULL))
);
CREATE TABLE T_COURT
(
 F_COURT INTEGER NOT NULL,
 F_COURT_TYPE CHAR(1) NOT NULL,
 F_UCLP INTEGER NOT NULL,
 F_COURT_AREA INTEGER NOT NULL,
 F_NAME CHAR(60) NOT NULL,
 F_EISPP INTEGER,
 F_FLAGS INTEGER,
 CONSTRAINT I_COURT PRIMARY KEY (F_COURT),
 CONSTRAINT C_COURT_COURT CHECK (F_COURT BETWEEN 1 AND 999),
 CONSTRAINT C_COURT_TYPE CHECK (F_COURT_TYPE BETWEEN '1' AND '9'),
 CONSTRAINT C_COURT_UCLP CHECK (F_UCLP BETWEEN 1 AND 99999),
 CONSTRAINT C_COURT_COURT_AREA CHECK (F_COURT_AREA BETWEEN 1 AND 999),
 CONSTRAINT C_COURT_EISPP CHECK ((F_EISPP >= 1) OR (F_EISPP IS NULL))
);
CREATE TABLE T_WEEKDAY
(
 F_WEEKDAY CHAR(1) NOT NULL,
 F_ORDER INTEGER,
 F_NAME CHAR(10) NOT NULL,
 F_ALIAS CHAR(3) NOT NULL,
 CONSTRAINT I_WEEKDAY PRIMARY KEY (F_WEEKDAY),
 CONSTRAINT C_WEEKDAY_WEEKDAY CHECK (F_WEEKDAY BETWEEN '1' AND '7'),
 CONSTRAINT C_WEEKDAY_ORDER CHECK ((F_ORDER BETWEEN 1 AND 9999) OR (F_ORDER IS NULL))
);
CREATE TABLE T_LOAD_WEEK
(
 F_WEEKDAY CHAR(1) NOT NULL,
 F_HALL INTEGER NOT NULL,
 F_BEF_COLLEGE CHAR(1),
 F_BEF_COMPOSITION INTEGER,
 F_BEF_FLAGS INTEGER,
 F_BEF_TEXT CHAR(40),
 F_AFT_COLLEGE CHAR(1),
 F_AFT_COMPOSITION INTEGER,
 F_AFT_FLAGS INTEGER,
 F_AFT_TEXT CHAR(40),
 CONSTRAINT I_LOAD_WEEK PRIMARY KEY (F_WEEKDAY, F_HALL),
 CONSTRAINT C_LOAD_WEEK_WEEKDAY CHECK (F_WEEKDAY BETWEEN '1' AND '7'),
 CONSTRAINT C_LOAD_WEEK_HALL CHECK (F_HALL BETWEEN 1 AND 99),
 CONSTRAINT C_LOAD_WEEK_BEF_COLLEGE CHECK ((F_BEF_COLLEGE IN ('c', 'm', 'p', 't', 'a', 'd')) OR (F_BEF_COLLEGE IS NULL)),
 CONSTRAINT C_LOAD_WEEK_BEF_COMPOSITION CHECK ((F_BEF_COMPOSITION BETWEEN 1 AND 250) OR (F_BEF_COMPOSITION IS NULL)),
 CONSTRAINT C_LOAD_WEEK_AFT_COLLEGE CHECK ((F_AFT_COLLEGE IN ('c', 'm', 'p', 't', 'a', 'd')) OR (F_AFT_COLLEGE IS NULL)),
 CONSTRAINT C_LOAD_WEEK_AFT_COMPOSITION CHECK ((F_AFT_COMPOSITION BETWEEN 1 AND 250) OR (F_AFT_COMPOSITION IS NULL))
);
CREATE TABLE T_LOAD_DATE
(
 F_DATE INTEGER NOT NULL,
 F_HALL INTEGER NOT NULL,
 F_BEF_COLLEGE CHAR(1),
 F_BEF_COMPOSITION INTEGER,
 F_BEF_FLAGS INTEGER,
 F_BEF_TEXT CHAR(40),
 F_AFT_COLLEGE CHAR(1),
 F_AFT_COMPOSITION INTEGER,
 F_AFT_FLAGS INTEGER,
 F_AFT_TEXT CHAR(40),
 CONSTRAINT I_LOAD_DATE PRIMARY KEY (F_DATE, F_HALL),
 CONSTRAINT C_LOAD_DATE_HALL CHECK (F_HALL BETWEEN 1 AND 99),
 CONSTRAINT C_LOAD_DATE_BEF_COLLEGE CHECK ((F_BEF_COLLEGE IN ('c', 'm', 'p', 't', 'a', 'd')) OR (F_BEF_COLLEGE IS NULL)),
 CONSTRAINT C_LOAD_DATE_BEF_COMPOSITION CHECK ((F_BEF_COMPOSITION BETWEEN 1 AND 250) OR (F_BEF_COMPOSITION IS NULL)),
 CONSTRAINT C_LOAD_DATE_AFT_COLLEGE CHECK ((F_AFT_COLLEGE IN ('c', 'm', 'p', 't', 'a', 'd')) OR (F_AFT_COLLEGE IS NULL)),
 CONSTRAINT C_LOAD_DATE_AFT_COMPOSITION CHECK ((F_AFT_COMPOSITION BETWEEN 1 AND 250) OR (F_AFT_COMPOSITION IS NULL))
);
CREATE TABLE T_REASON_TYPE
(
 F_REASON_TYPE INTEGER NOT NULL,
 F_NAME CHAR(20) NOT NULL,
 F_FLAGS INTEGER,
 CONSTRAINT I_REASON_TYPE PRIMARY KEY (F_REASON_TYPE),
 CONSTRAINT C_REASON_TYPE_REASON_TYPE CHECK (F_REASON_TYPE BETWEEN 1 AND 99)
);
CREATE TABLE T_SECTION
(
 F_SECTION CHAR(1) NOT NULL,
 F_NAME CHAR(15) NOT NULL,
 CONSTRAINT I_SECTION PRIMARY KEY (F_SECTION),
 CONSTRAINT C_SECTION_SECTION CHECK (F_SECTION BETWEEN 'a' AND 'z')
);
CREATE TABLE T_CONNECT_TYPE
(
 F_CONNECT_TYPE CHAR(1) NOT NULL,
 F_ORDER INTEGER,
 F_NAME CHAR(40) NOT NULL,
 F_ALIAS CHAR(15) NOT NULL,
 F_CONNECT_KINDS CHAR(20),
 F_SENDER_TYPE CHAR(1) NOT NULL,
 F_XFER INTEGER,
 F_EISPP INTEGER,
 CONSTRAINT I_CONNECT_TYPE PRIMARY KEY (F_CONNECT_TYPE),
 CONSTRAINT C_CONNECT_TYPE_CONNECT_TYPE CHECK ((F_CONNECT_TYPE BETWEEN 'a' AND 'z') OR (F_CONNECT_TYPE BETWEEN 'A' AND 'Z')),
 CONSTRAINT C_CONNECT_TYPE_ORDER CHECK ((F_ORDER BETWEEN 1 AND 9999) OR (F_ORDER IS NULL)),
 CONSTRAINT C_CONNECT_TYPE_SENDER_TYPE CHECK ((F_SENDER_TYPE BETWEEN 'a' AND 'z') OR (F_SENDER_TYPE BETWEEN 'A' AND 'Z')),
 CONSTRAINT C_CONNECT_TYPE_XFER CHECK ((F_XFER BETWEEN 1 AND 99999) OR (F_XFER IS NULL)),
 CONSTRAINT C_CONNECT_TYPE_EISPP CHECK ((F_EISPP >= 1) OR (F_EISPP IS NULL))
);
CREATE TABLE T_ARTICLE
(
 F_ARTICLE INTEGER NOT NULL,
 F_NAME CHAR(40) NOT NULL,
 F_FLAGS INTEGER,
 CONSTRAINT I_ARTICLE PRIMARY KEY (F_ARTICLE),
 CONSTRAINT C_ARTICLE_ARTICLE CHECK (F_ARTICLE BETWEEN 1 AND 999)
);
CREATE TABLE T_ARTICLE_SUBJECT
(
 F_ARTICLE INTEGER NOT NULL,
 F_SUBJECT INTEGER NOT NULL,
 CONSTRAINT I_ARTICLE_SUBJECT PRIMARY KEY (F_ARTICLE, F_SUBJECT),
 CONSTRAINT C_ARTICLE_SUBJECT_ARTICLE CHECK (F_ARTICLE BETWEEN 1 AND 999),
 CONSTRAINT C_ARTICLE_SUBJECT_SUBJECT CHECK (F_SUBJECT BETWEEN 1 AND 9999999)
);
CREATE TABLE T_OLD_ARTICLE_SUBJECT
(
 F_ARTICLE INTEGER NOT NULL,
 F_SUBJECT INTEGER NOT NULL,
 CONSTRAINT I_OLD_ARTICLE_SUBJECT PRIMARY KEY (F_ARTICLE, F_SUBJECT),
 CONSTRAINT C_OLD_ARTICLE_SUBJECT_ARTICLE CHECK (F_ARTICLE BETWEEN 1 AND 999),
 CONSTRAINT C_OLD_ARTICLE_SUBJECT_SUBJECT CHECK (F_SUBJECT BETWEEN 1 AND 9999999)
);
CREATE TABLE T_DANGER
(
 F_DANGER INTEGER NOT NULL,
 F_NAME CHAR(150) NOT NULL,
 F_FLAGS INTEGER,
 CONSTRAINT I_DANGER PRIMARY KEY (F_DANGER),
 CONSTRAINT C_DANGER_DANGER CHECK (F_DANGER BETWEEN 1 AND 999999999)
);
CREATE TABLE T_DANGER_SUBJECT
(
 F_DANGER INTEGER NOT NULL,
 F_SUBJECT INTEGER NOT NULL,
 CONSTRAINT I_DANGER_SUBJECT PRIMARY KEY (F_DANGER, F_SUBJECT),
 CONSTRAINT C_DANGER_SUBJECT_DANGER CHECK (F_DANGER BETWEEN 1 AND 999999999),
 CONSTRAINT C_DANGER_SUBJECT_SUBJECT CHECK (F_SUBJECT BETWEEN 1 AND 9999999)
);
CREATE TABLE T_OLD_DANGER_SUBJECT
(
 F_DANGER INTEGER NOT NULL,
 F_SUBJECT INTEGER NOT NULL,
 CONSTRAINT I_OLD_DANGER_SUBJECT PRIMARY KEY (F_DANGER, F_SUBJECT),
 CONSTRAINT C_OLD_DANGER_SUBJECT_DANGER CHECK (F_DANGER BETWEEN 1 AND 999999999),
 CONSTRAINT C_OLD_DANGER_SUBJECT_SUBJECT CHECK (F_SUBJECT BETWEEN 1 AND 9999999)
);
CREATE TABLE T_RANK
(
 F_RANK INTEGER NOT NULL,
 F_NAME CHAR(40) NOT NULL,
 F_ALIAS CHAR(15) NOT NULL,
 CONSTRAINT I_RANK PRIMARY KEY (F_RANK),
 CONSTRAINT C_RANK_RANK CHECK (F_RANK BETWEEN 1 AND 999)
);
CREATE TABLE T_GOP
(
 F_GOP CHAR(1) NOT NULL,
 F_ORDER INTEGER,
 F_NAME CHAR(15) NOT NULL,
 CONSTRAINT I_GOP PRIMARY KEY (F_GOP),
 CONSTRAINT C_GOP_GOP CHECK (F_GOP BETWEEN '0' AND 'z'),
 CONSTRAINT C_GOP_ORDER CHECK ((F_ORDER BETWEEN 1 AND 9999) OR (F_ORDER IS NULL))
);
CREATE TABLE T_CONNECT_KIND
(
 F_CONNECT_KIND CHAR(1) NOT NULL,
 F_ORDER INTEGER,
 F_NAME CHAR(40) NOT NULL,
 F_XFER INTEGER,
 F_ALIAS CHAR(15) NOT NULL,
 CONSTRAINT I_CONNECT_KIND PRIMARY KEY (F_CONNECT_KIND),
 CONSTRAINT C_CONNECT_KIND_CONNECT_KIND CHECK (F_CONNECT_KIND BETWEEN '!' AND '�'),
 CONSTRAINT C_CONNECT_KIND_ORDER CHECK ((F_ORDER BETWEEN 1 AND 9999) OR (F_ORDER IS NULL)),
 CONSTRAINT C_CONNECT_KIND_XFER CHECK ((F_XFER BETWEEN 1 AND 99999) OR (F_XFER IS NULL))
);
CREATE TABLE T_DEBTOR_STATUS
(
 F_DEBTOR_STATUS CHAR(1) NOT NULL,
 F_ORDER INTEGER,
 F_NAME CHAR(40) NOT NULL,
 F_XFER INTEGER,
 CONSTRAINT I_DEBTOR_STATUS PRIMARY KEY (F_DEBTOR_STATUS),
 CONSTRAINT C_DEBTOR_STATUS_DEBTOR_STATUS CHECK (F_DEBTOR_STATUS BETWEEN '0' AND 'z'),
 CONSTRAINT C_DEBTOR_STATUS_ORDER CHECK ((F_ORDER BETWEEN 1 AND 9999) OR (F_ORDER IS NULL))
);
CREATE TABLE T_BANCONSIST
(
 F_BANCONSIST INTEGER NOT NULL,
 F_TEXT VARCHAR(1200) NOT NULL,
 F_FLAGS INTEGER,
 CONSTRAINT I_BANCONSIST PRIMARY KEY(F_BANCONSIST),
 CONSTRAINT C_BANCONSIST_BANCONSIST CHECK (F_BANCONSIST BETWEEN 1 AND 9999)
);
CREATE TABLE T_EISPP_VAL
(
 F_NOM INTEGER NOT NULL,
 F_CODE INTEGER NOT NULL,
 F_NAME CHAR(255) NOT NULL,
 F_FLAGS INTEGER,
 F_ALIAS CHAR(40) NOT NULL,
 CONSTRAINT I_EISPP_VAL PRIMARY KEY(F_NOM, F_CODE),
 CONSTRAINT C_EISPP_VAL_NOM CHECK (F_NOM >= 1),
 CONSTRAINT C_EISPP_VAL_CODE CHECK (F_CODE >= 1)
);
CREATE TABLE T_SBE_DESC
(
 F_VID INTEGER NOT NULL,
 F_NAME CHAR(80) NOT NULL,
 F_FLAGS INTEGER,
 F_VALUE CHAR(255),
 CONSTRAINT I_SBE_DESC PRIMARY KEY(F_VID, F_NAME),
 CONSTRAINT C_SBE_DESC_VID CHECK ((F_VID BETWEEN -9 AND -1) OR (F_VID BETWEEN 1 AND 99999))
);
CREATE TABLE T_SESSION_PRICE
(
 F_DATE INTEGER NOT NULL,
 F_PRICE DOUBLE PRECISION NOT NULL,
 CONSTRAINT I_SESSION_PRICE PRIMARY KEY(F_DATE)
);
CREATE TABLE T_EISPP_CRIME
(
 F_CODE INTEGER NOT NULL,
 F_NAME CHAR(255) NOT NULL,
 F_FLAGS INTEGER,
 F_ALIAS CHAR(40) NOT NULL,
 CONSTRAINT I_EISPP_CRIME PRIMARY KEY(F_CODE),
 CONSTRAINT C_EISPP_CRIME_CODE CHECK (F_CODE >= 1)
);
CREATE TABLE T_EISPP_DEPART
(
 F_NOM INTEGER NOT NULL,
 F_CODE INTEGER NOT NULL,
 F_NAME CHAR(255) NOT NULL,
 F_TLACR CHAR(15),
 F_FLAGS INTEGER,
 F_ALIAS CHAR(40) NOT NULL,
 CONSTRAINT I_EISPP_DEPART PRIMARY KEY(F_CODE),
 CONSTRAINT C_EISPP_DEPART_NOM CHECK (F_NOM >= 1)
);
CREATE TABLE T_EISPP_TRANSL
(
 F_NAME CHAR(15) NOT NULL,
 F_TEXT CHAR(40) NOT NULL,
 CONSTRAINT I_EISPP_TRANSL PRIMARY KEY(F_NAME)
);
CREATE TABLE T_EISPP_PLACE
(
 F_NOM INTEGER NOT NULL,
 F_CODE INTEGER NOT NULL,
 F_NAME CHAR(255) NOT NULL,
 F_UCLP INTEGER NOT NULL,
 F_FLAGS INTEGER,
 F_ALIAS CHAR(40) NOT NULL,
 CONSTRAINT I_EISPP_PLACE PRIMARY KEY(F_CODE),
 CONSTRAINT C_EISPP_PLACE_NOM CHECK (F_NOM >= 1),
 CONSTRAINT C_EISPP_PLACE_CODE CHECK (F_CODE >= 1)
);
CREATE TABLE T_ACCOUNT
(
 F_UCN CHAR(10) NOT NULL,
 F_UCN_TYPE CHAR(1) NOT NULL,
 F_ACCOUNT_TYPE CHAR(1) NOT NULL,
 F_BANK_ACCOUNT CHAR(34) NOT NULL,
 F_BIC CHAR(11) NOT NULL,
 F_BANK_NAME CHAR(100) NOT NULL,
 F_NAME CHAR(40) NOT NULL,
 F_FLAGS INTEGER,
 F_ORDER INTEGER,
 CONSTRAINT I_ACCOUNT PRIMARY KEY (F_ACCOUNT_TYPE, F_UCN, F_UCN_TYPE),
 CONSTRAINT C_ACCOUNT_ACCOUNT_TYPE CHECK (F_ACCOUNT_TYPE BETWEEN 'a' AND 'z'),
 CONSTRAINT C_ACCOUNT_UCN_TYPE CHECK ((F_UCN_TYPE BETWEEN 'a' AND 'z') OR (F_UCN_TYPE BETWEEN 'A' AND 'Z'))
);
CREATE TABLE T_LEGAL_AID_TYPE
(
 F_LEGAL_AID_TYPE INTEGER NOT NULL,
 F_NAME CHAR(40),
 CONSTRAINT I_LEGAL_AID_TYPE PRIMARY KEY (F_LEGAL_AID_TYPE)
);
CREATE TABLE T_ARCHIVE_INDEX
(
 F_ARCHIVE_INDEX CHAR(15) NOT NULL,
 F_NAME CHAR(240) NOT NULL,
 F_KEEP INTEGER NOT NULL,
 F_FLAGS INTEGER,
 CONSTRAINT I_ARCHIVE_INDEX PRIMARY KEY (F_ARCHIVE_INDEX),
 CONSTRAINT C_ARCHIVE_INDEX_KEEP CHECK ((F_KEEP BETWEEN 1 AND 200) OR (F_KEEP IS NULL))
);
CREATE TABLE T_ARCHIVE_SUBJECT
(
 F_ARCHIVE_INDEX CHAR(15) NOT NULL,
 F_SUBJECT INTEGER NOT NULL,
 F_KIND CHAR(1) NOT NULL,
 CONSTRAINT I_ARCHIVE_SUBJECT PRIMARY KEY (F_ARCHIVE_INDEX, F_SUBJECT, F_KIND),
 CONSTRAINT C_ARCHIVE_SUBJECT_SUBJECT CHECK (F_SUBJECT BETWEEN 1 AND 9999999),
 CONSTRAINT C_ARCHIVE_SUBJECT_KIND CHECK (F_KIND BETWEEN '!' AND '�')
);
CREATE TABLE T_OLD_ARCHIVE_SUBJECT
(
 F_ARCHIVE_INDEX CHAR(15) NOT NULL,
 F_SUBJECT INTEGER NOT NULL,
 F_TYPE CHAR(1) NOT NULL,
 CONSTRAINT I_OLD_ARCHIVE_SUBJECT PRIMARY KEY (F_ARCHIVE_INDEX, F_SUBJECT, F_TYPE),
 CONSTRAINT C_OLD_ARCHIVE_SUBJECT_SUBJECT CHECK (F_SUBJECT BETWEEN 1 AND 9999999),
 CONSTRAINT C_OLD_ARCHIVE_SUBJECT_TYPE CHECK ((F_TYPE BETWEEN 'a' AND 'z') OR (F_TYPE BETWEEN 'A' AND 'Z'))
);
CREATE TABLE T_CURRENCY
(
 F_CURRENCY CHAR(3) NOT NULL,
 F_ORDER INTEGER,
 F_NAME CHAR(40) NOT NULL,
 F_NUMERIC CHAR(25) NOT NULL,
 F_INTEGER_2 CHAR(25) NOT NULL,
 F_INTEGER_1 CHAR(25),
 F_INTEGER_0 CHAR(25),
 F_INTEGER_MFO INTEGER,
 F_FRACTION_2 CHAR(25) NOT NULL,
 F_FRACTION_1 CHAR(25),
 F_FRACTION_0 CHAR(25),
 F_FRACTION_MFO INTEGER,
 CONSTRAINT I_CURRENCY PRIMARY KEY (F_CURRENCY),
 CONSTRAINT C_CURRENCY_ORDER CHECK ((F_ORDER BETWEEN 1 AND 9999) OR (F_ORDER IS NULL))
);
CREATE TABLE T_SLC_TYPE
(
 F_SLC_TYPE CHAR(1) NOT NULL,
 F_NAME CHAR(40),
 F_ORDER INTEGER,
 CONSTRAINT I_SLC_TYPE PRIMARY KEY (F_SLC_TYPE)
);
CREATE TABLE T_SLC_REASON
(
 F_SLC_REASON INTEGER NOT NULL,
 F_SLC_TYPE CHAR(1) NOT NULL,
 F_NAME CHAR(150) NOT NULL,
 F_LAW_KINDS CHAR(40) NOT NULL,
 F_FLAGS INTEGER,
 F_ORDER INTEGER,
 CONSTRAINT I_SLC_REASON PRIMARY KEY (F_SLC_REASON),
 CONSTRAINT C_SLC_REASON_ORDER CHECK ((F_ORDER BETWEEN 1 AND 9999) OR (F_ORDER IS NULL))
);
CREATE TABLE T_VSS_INDEXES
(
 F_COURT_TYPE CHAR(1) NOT NULL,
 F_ORDER INTEGER NOT NULL,
 F_START INTEGER NOT NULL,
 F_FINAL INTEGER NOT NULL,
 F_TYPES CHAR(10) NOT NULL,
 F_KINDS CHAR(40),
 F_INDEXES CHAR(250) NOT NULL,
 F_ACCEPTS CHAR(50),
 F_REJECTS CHAR(50),
 CONSTRAINT I_VSS_INDEXES PRIMARY KEY (F_COURT_TYPE, F_ORDER, F_START)
);
CREATE TABLE T_REPORT_RANGE
(
 F_TYPES CHAR(10) NOT NULL,
 F_INSTANCES INTEGER NOT NULL,
 F_RANGE_ID INTEGER NOT NULL,
 F_SUBJECTS CHAR(255) NOT NULL,
 F_ATTRIB CHAR(20),
 F_NAME CHAR(500) NOT NULL,
 F_ALIAS CHAR(40),
 CONSTRAINT I_REPORT_RANGE PRIMARY KEY (F_TYPES, F_INSTANCES, F_RANGE_ID),
 CONSTRAINT C_REPORT_RANGE_INSTANCE CHECK (F_INSTANCES BETWEEN 1 AND 3),
 CONSTRAINT C_REPORT_RANGE_RANGE_ID CHECK (F_RANGE_ID BETWEEN 1 AND 9999999)
);
CREATE TABLE T_WHAT
(
 F_WHAT INTEGER NOT NULL,
 F_NAME CHAR(40) NOT NULL,
 CONSTRAINT I_WHAT PRIMARY KEY (F_WHAT),
 CONSTRAINT C_WHAT_WHAT CHECK ((F_WHAT BETWEEN 1 AND 99999) OR (F_WHAT IS NULL))
);