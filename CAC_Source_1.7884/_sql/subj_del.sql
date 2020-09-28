drop index i_request_subject;
alter table t_request drop constraint c_request_subject;
alter table t_request drop f_subject;
alter table t_request add f_subject integer;
alter table t_request add constraint c_request_subject check ((f_subject between 1 and 99999) or (f_subject is null));
create index i_request_subject on t_request (f_subject);

drop index i_lawsuit_subject;
alter table t_lawsuit drop constraint c_lawsuit_subject;
alter table t_lawsuit drop f_subject;
alter table t_lawsuit add f_subject integer not null;
alter table t_lawsuit add constraint c_lawsuit_subject check (f_subject between 1 and 99999);
create index i_lawsuit_subject on t_lawsuit (f_subject);

delete from t_not_finished_beg;
delete from t_article_subject;
delete from t_danger_subject;
delete from t_archive_subject;
