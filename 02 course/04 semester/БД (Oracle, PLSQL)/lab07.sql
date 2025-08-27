-- 1
SELECT * FROM v$sga;
SELECT sum(value) FROM v$sga;

-- 2 & 3
select
    component,
    current_size,
    max_size,
    granule_size,
    current_size/granule_size as Ration
from gv$sga_dynamic_components
where current_size > 0

-- 4
SELECT sum(bytes) FROM v$sgastat WHERE name = 'free memory';

-- 5
SHOW PARAMETER sga_max_size;
SHOW PARAMETER sga_target;

-- 6
SELECT component, min_size, current_size
FROM gv$sga_dynamic_components
WHERE component LIKE 'DEFAULT b%' 
    OR component LIKE 'KEEP b%' 
    OR component LIKE 'RECYCLE b%';

-- 7 & 8
create table tkeep (id number) storage(buffer_pool keep);
create table tcache (id number) cache;

SELECT 
    segment_name, segment_type, tablespace_name, buffer_pool
FROM user_segments
WHERE segment_name = 'TKEEP' 
    or segment_name = 'TCACHE';

DROP TABLE tkeep PURGE;
DROP TABLE tcache PURGE;

-- 9
show parameter log_buffer;

-- 10
select
    pool, name, bytes
from v$sgastat
where pool = 'large pool' 
    and name = 'free memory';
    
-- 11
select distinct username, service_name, server from v$session;

-- 12
SELECT * FROM v$bgprocess WHERE paddr != '00';

-- 13
select * from v$process;

-- 14
SELECT * FROM V$BGPROCESS WHERE NAME LIKE 'DBWn%';

-- 15
select * from v$services;

-- 16
select * from v$dispatcher;

-- 17 ps -ef | grep lsnrctl tnslsnr 

-- 18 /opt/oracle/product/23ai/dbhomeFree/network/admin

-- 19
/*
lsnrctl start
lsnrctl stop
lsnrctl status
lsnrctl services
lsnrctl reload
*/
-- 20
-- lsnrctl services
