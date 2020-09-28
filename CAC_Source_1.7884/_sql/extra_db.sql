CREATE DATABASE "CAC_EXTCOURT"
WITH OWNER "CAC_REGADMIN"
TEMPLATE "template0"
ENCODING 'WIN1251'
TABLESPACE = pg_default
LC_COLLATE = 'C'
LC_CTYPE = 'Bulgarian_Bulgaria.1251'
CONNECTION LIMIT = -1;

\connect "CAC_EXTCOURT" "CAC_REGADMIN"

CREATE TABLE T_REGISTER_ACT
(
 F_NO INTEGER NOT NULL,
 F_YEAR INTEGER NOT NULL,
 F_TYPE CHAR(1) NOT NULL,
 F_KIND CHAR(1) NOT NULL,
 F_DATE INTEGER NOT NULL,
 F_JUDGE CHAR(10) NOT NULL,
 F_ACT_NO INTEGER NOT NULL,
 F_TEXT_MTIME INTEGER,
 F_TEXT_DATA BYTEA,
 F_IMAGE_MTIME INTEGER,
 F_IMAGE_DATA BYTEA,
 F_TARGET_KIND CHAR(1),
 F_TARGET_NO INTEGER,
 F_TARGET_YEAR INTEGER,
 F_TARGET_TYPE CHAR(1),
 F_TARGET_DATE INTEGER,
 F_TARGET_STAMP TIMESTAMP,
 F_TEXT CHAR(100),
 CONSTRAINT I_REGISTER_ACT PRIMARY KEY(F_NO, F_YEAR, F_TYPE, F_KIND, F_DATE, F_JUDGE, F_ACT_NO),
 CONSTRAINT C_REGISTER_ACT_NO CHECK (F_NO BETWEEN 1 AND 99999),
 CONSTRAINT C_REGISTER_ACT_YEAR CHECK (F_YEAR BETWEEN 1970 AND 2037),
 CONSTRAINT C_REGISTER_ACT_TYPE CHECK (F_TYPE IN ('c', 'm', 'p', 't', 'a', 'd', 'r', 'i', 's')),
 CONSTRAINT C_REGISTER_ACT_KIND CHECK (F_KIND BETWEEN '!' AND '�'),
 CONSTRAINT C_REGISTER_ACT_ACT_NO CHECK (F_ACT_NO BETWEEN 1 AND 99),
 CONSTRAINT C_REGISTER_ACT_TARGET_KIND CHECK ((F_TARGET_KIND BETWEEN '!' AND '�') OR (F_TARGET_KIND IS NULL)),
 CONSTRAINT C_REGISTER_ACT_TARGET_NO CHECK ((F_TARGET_NO BETWEEN 1 AND 999999) OR (F_TARGET_NO IS NULL)),
 CONSTRAINT C_REGISTER_ACT_TARGET_YEAR CHECK ((F_TARGET_YEAR BETWEEN 1970 AND 2037) OR (F_TARGET_YEAR IS NULL)),
 CONSTRAINT C_REGISTER_ACT_TARGET_TYPE CHECK ((F_TARGET_TYPE BETWEEN 'a' AND 'z') OR (F_TARGET_TYPE BETWEEN 'A' AND 'Z') OR (F_TARGET_TYPE IS NULL))
);
CREATE INDEX I_REGISTER_ACT_DATE ON T_REGISTER_ACT
(
 F_DATE,
 F_JUDGE,
 F_KIND
);
CREATE ROLE "????????" LOGIN ENCRYPTED PASSWORD '????????' VALID UNTIL 'infinity';
GRANT SELECT, INSERT, UPDATE, DELETE ON T_REGISTER_ACT TO "????????";