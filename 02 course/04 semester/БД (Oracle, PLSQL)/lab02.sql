/*
show con_name;
alter session set container = my_pdb;
alter session set container = cdb$root;

select * from DBA_USERS;
*/

-- 1
create TABLESPACE TS_DSZ datafile 'TS_DSZ.dbf'
    size 7M
    autoextend on
    next 5m
    maxsize 30m;

-- 2    
create temporary TABLESPACE TS_DSZ_TEMP tempfile 'TS_DSZ_TEMP.dbf'
    size 5M
    autoextend on
    next 3m
    maxsize 20m;

-- 3
SELECT * FROM DBA_TABLESPACES;

-- 4
SELECT FILE_NAME, TABLESPACE_NAME FROM DBA_DATA_FILES
UNION ALL
SELECT FILE_NAME, TABLESPACE_NAME FROM DBA_TEMP_FILES;

drop tablespace TS_DSZ including CONTENTS and DATAFILES;
drop tablespace TS_DSZ_TEMP including CONTENTS and DATAFILES;
    
-- 5 
create role RL_DSZCORE;
grant create session to RL_DSZCORE; 
grant create table to RL_DSZCORE; 
grant create view to RL_DSZCORE; 
grant create procedure to RL_DSZCORE;

drop role RL_DSZCORE;

-- 6
select * from DBA_ROLES where role like 'RL%';

-- 7
select * from dba_sys_privs where grantee like 'RL%';

-- 8
CREATE PROFILE PF_DSZCORE LIMIT
    PASSWORD_LIFE_TIME 180
    SESSIONS_PER_USER 3 
    FAILED_LOGIN_ATTEMPTS 7
    PASSWORD_LOCK_TIME 1
    PASSWORD_REUSE_TIME 10
    PASSWORD_GRACE_TIME DEFAULT
    CONNECT_TIME 180 
    IDLE_TIME 30;

drop profile PF_DSZCORE;

-- 9
select * from dba_profiles;

-- 10
select * from dba_profiles where profile = 'PF_DSZCORE';

-- 11
select * from dba_profiles where profile = 'DEFAULT';

-- 12
create user DSZCORE IDENTIFIED BY 1
    default tablespace TS_DSZ
    temporary TABLESPACE TS_DSZ_TEMP
    profile PF_DSZCORE
    account unlock
    password expire;
drop user DSZCORE;
grant RL_DSZCORE to DSZCORE;

-- 15
create table DSZ_t 
(
    digits number(3) primary key,
    charrs varchar2(50)
)

insert into DSZ_t (digits, charrs) values (1, 'sdsd');
insert into DSZ_t (digits, charrs) values (2, 'sdwqeqd');
insert into DSZ_t (digits, charrs) values (3, 'e12eqws');
insert into DSZ_t (digits, charrs) values (4, 'pfgbcm 4');
insert into DSZ_t (digits, charrs) values (5, 'pfgbcm 5');
commit

-- 16
ALTER USER DSZCORE QUOTA 2M ON TS_DSZ;

-- 18
ALTER TABLESPACE TS_DSZ OFFLINE;

select * from dsz_t

-- 20
ALTER TABLESPACE TS_DSZ ONLINE;

drop table dsz_t;

/*
create user DBAUSER_MY identified by 1234;
drop user DBAUSER_MY;

grant sysdba to DBAUSER_MY;
grant connect to DBAUSER_MY;

alter session set container = my_pdb; 

select * from dba_tab_privs where grantee = 'DBAUSER_MY';

SELECT * FROM V$PWFILE_USERS WHERE USERNAME = 'DBAUSER_MY';
*/
