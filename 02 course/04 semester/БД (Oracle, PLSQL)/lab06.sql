-- 1
show parameter spfile;
-- /opt/oracle/product/23ai/dbhomeFree/dbs/spfileFREE.ora

-- 2
-- cd /opt/oracle/product/23ai/dbhomeFree/dbs/

-- 3
create pfile = 'DSZ_PFILE.ora' from spfile;

-- 4 
-- чота меняем в dsz_psile.ora (open_cursors)
show parameter open_cursors; // 300

-- 5
create spfile = 'spfileFREE_new.ora' from pfile='DSZ_PFILE.ora';
show parameter open_cursors; // 300

-- 6
-- меняем ориг spfileFREE.ora на spfileFREE_old.ora
-- а spfileFREE_new.ora на spfileFREE.ora
-- запускаем инстанс

-- 7
alter system set open_cursors=300 scope=spfile;

-- 8
show parameter control_files;
-- control_files string /opt/oracle/oradata/FREE/control01.ctl, /opt/oracle/oradata/FREE/control02.ctl 

show parameter pwfile; 

-- 9 ????????????????????????
alter system set control_files='/opt/oracle/oradata/FREE/test/control01.ctl','/opt/oracle/oradata/FREE/test/control02.ctl' scope=spfile;
alter system set control_files='/opt/oracle/oradata/FREE/control01.ctl','/opt/oracle/oradata/FREE/control02.ctl' scope=spfile;
-- 10
select * from v$pwfile_users;
-- ls $ORACLE_HOME/dbs/orapw$ORACLE_SID -- по умолчанию
-- ORACLE_HOME = /opt/oraclшe/product/23ai/dbhomeFree
-- orapwFREE - файл паролей

-- 11 cd /opt/oracle/product/23ai/dbhomeFree/dbs     orapwFREE

-- 12
show parameter diagnostic_dest;
select * from v$diag_info;

-- 13
-- /opt/oracle/diag/rdbms/free/FREE/alert
-- log.xml

-- 14
-- /opt/oracle/diag/rdbms/free/FREE/trace
-- grep -i "control file" *.trc
