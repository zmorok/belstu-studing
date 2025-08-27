// 1
show con_name;
select * from dba_pdbs;
select * from dba_tablespaces;
select * from dba_profiles where profile = 'PF_DSZ_PDB';
select * from dba_roles where role = 'RL_DSZ_PDB';
select * from dba_users;
select * from dba_sys_privs;

alter session set container = DSZ_PDB;

// 2
create pluggable database DSZ_PDB
admin user DSZ_PDB_CORE identified by 125634
FILE_NAME_CONVERT = ('/opt/oracle/oradata/FREE/pdbseed', '/opt/oracle/oradata/FREE/DSZ_PDB');
// '/opt/oracle/oradata/FREE/pdbseed'   - шаблон создания pdb
// '/opt/oracle/oradata/FREE/my_pdb'    - где будет создана pbd

alter pluggable database DSZ_PDB open;       -- открыть pdb
alter pluggable database DSZ_PDB save state; -- авто-открытие pdb

alter session set container = DSZ_PDB;
alter session set container = CDB$ROOT;

// 3
select * from dba_pdbs;

// 4
create TABLESPACE TS_DSZ_PDB datafile 'TS_DSZ_PDB.dbf' size 7M autoextend on next 5m maxsize 30m;
create temporary TABLESPACE TS_DSZ_PDB_TEMP tempfile 'TS_DSZ_PDB_TEMP.dbf' size 5M autoextend on next 3m maxsize 20m;

create role RL_DSZ_PDB;
grant create session, create table, create view, create procedure to RL_DSZ_PDB; 

CREATE PROFILE PF_DSZ_PDB LIMIT
    PASSWORD_LIFE_TIME 180
    SESSIONS_PER_USER 3 
    FAILED_LOGIN_ATTEMPTS 7
    PASSWORD_LOCK_TIME 1
    PASSWORD_REUSE_TIME 10
    PASSWORD_GRACE_TIME DEFAULT
    CONNECT_TIME 180 
    IDLE_TIME 30;



create user U1_DSZ_PDB IDENTIFIED BY 125634
    default tablespace TS_DSZ_PDB
    temporary TABLESPACE TS_DSZ_PDB_TEMP
    profile PF_DSZ_PDB
    account unlock;
    
grant RL_DSZ_PDB to U1_DSZ_PDB;
alter user U1_DSZ_PDB quota unlimited on TS_DSZ_PDB;

drop user U1_DSZ_PDB;
drop profile PF_DSZ_PDB;
drop role RL_DSZ_PDB;

// 5 (подключение к юзеру)
create table tablee(id int not null);
insert into tablee(id) values (1);
insert into tablee(id) values (2);
insert into tablee(id) values (1534);
commit;

select * from tablee;

// 6
alter session set container = dsz_pdb;
select * from dba_tablespaces;

select FILE_NAME, TABLESPACE_NAME from dba_data_files
union all
select FILE_NAME, TABLESPACE_NAME from dba_temp_files;

select g.GRANTEE, r.ROLE from dba_role_privs g inner join dba_roles r on g.GRANTED_ROLE = r.ROLE order by g.GRANTEE;

select * from dba_profiles;
select * from dba_users;

// 7
-- На уровне CDB:
CREATE USER C##DSZ IDENTIFIED BY 125634;
GRANT CREATE SESSION TO C##DSZ;
GRANT SET CONTAINER TO C##DSZ;

ALTER SESSION SET CONTAINER = DSZ_PDB;
GRANT CREATE SESSION TO C##DSZ;

alter user c##dsz default tablespace TS_DSZ_PDB;
alter user c##dsz quota unlimited on TS_DSZ_PDB;

// 8
GRANT CREATE TABLE TO C##DSZ;

// 9
/*
    два подключения:
    C__DSZ_CDB: C##DSZ к контейнеру
    C__DSZ_PDB: C##DSZ к PDB
    пароли 125634
*/



select * from dba_pdbs

// 10
show con_name;
create table dsz_tab (syn varchar(50));
insert into dsz_tab (syn) values ('adda');
commit;
select * from dsz_tab;

alter session set CONTAINER = DSZ_PDB;

drop table dsz_tab;

SELECT TABLE_NAME, TABLESPACE_NAME FROM USER_TABLES WHERE TABLE_NAME = 'DSZ_TAB';

// 11
// для подключений C##DSZ и U1_DSZ_PDB
SELECT OBJECT_NAME, OBJECT_TYPE FROM USER_OBJECTS;

// 13
SELECT * FROM V$SESSION;

// 14
SELECT NAME FROM V$DATAFILE WHERE CON_ID = (SELECT CON_ID FROM V$PDBS WHERE NAME = 'DSZ_PDB');

// 15

ALTER PLUGGABLE DATABASE DSZ_PDB CLOSE IMMEDIATE;
DROP PLUGGABLE DATABASE DSZ_PDB INCLUDING DATAFILES;

drop tablespace TS_DSZ_PDB including CONTENTS and DATAFILES;
drop tablespace TS_DSZ_PDB_TEMP including CONTENTS and DATAFILES;

select PDB_ID, PDB_NAME, STATUS from dba_pdbs;
alter session set container = CDB$ROOT;
DROP USER C##DSZ CASCADE;

/*
/opt/oracle/oradata/FREE/DSZ_PDB/system01.dbf
/opt/oracle/oradata/FREE/DSZ_PDB/sysaux01.dbf
/opt/oracle/oradata/FREE/DSZ_PDB/undotbs01.dbf
/opt/oracle/product/23ai/dbhomeFree/dbs/TS_DSZ_PDB.dbf
*/