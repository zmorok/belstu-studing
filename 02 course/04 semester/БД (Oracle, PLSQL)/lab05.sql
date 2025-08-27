alter session set container = dsz_pdb;
show con_name;
-- 1
SELECT TABLESPACE_NAME FROM dba_tablespaces;

-- 2
CREATE TABLESPACE DSZ_QDATA
DATAFILE 'DSZ_QDATA.dbf' SIZE 10M
OFFLINE;

ALTER TABLESPACE DSZ_QDATA ONLINE;

ALTER USER DSZ QUOTA 2M ON DSZ_QDATA;

CREATE TABLE DSZ_T1 (
    id NUMBER PRIMARY KEY,
    data VARCHAR2(100)
) TABLESPACE DSZ_QDATA;

// ALTER USER DSZ DEFAULT TABLESPACE DSZ_QDATA;

INSERT INTO DSZ_T1 VALUES (1, 'дата 1');
INSERT INTO DSZ_T1 VALUES (2, 'дата 2');
INSERT INTO DSZ_T1 VALUES (3, 'дата 3');
COMMIT;

-- 3
SELECT segment_name, segment_type 
FROM dba_segments 
WHERE tablespace_name = 'DSZ_QDATA';

-- 4
SELECT segment_name, segment_type 
FROM dba_segments 
WHERE segment_name = 'DSZ_T1';

-- 5
SELECT segment_name, segment_type 
FROM dba_segments 
WHERE tablespace_name = 'DSZ_QDATA' AND segment_name <> 'DSZ_T1';

-- 6
DROP TABLE DSZ.DSZ_T1;

-- 7
SELECT segment_name, segment_type 
FROM dba_segments 
WHERE tablespace_name = 'DSZ_QDATA';

SELECT segment_name 
FROM dba_segments 
WHERE segment_name = 'DSZ_T1';

SELECT object_name, original_name, operation, droptime 
FROM user_recyclebin;

-- 8
FLASHBACK TABLE DSZ.DSZ_T1 TO BEFORE DROP;

-- 9
BEGIN
    FOR i IN 1..10000 LOOP
        INSERT INTO DSZ.DSZ_T1 VALUES (i, 'дата= ' || i);
    END LOOP;
    COMMIT;
END;

select * from DSZ_T1;

delete from DSZ.DSZ_T1 where id < 5;

-- 10
SELECT segment_name, COUNT(*) AS extents, 
       sum(blocks) AS total_blocks,
       sum(bytes) AS total_bytes 
FROM dba_extents 
WHERE segment_name = 'DSZ_T1'
GROUP BY segment_name, blocks, bytes;

-- 11
SELECT tablespace_name, segment_name, extent_id, blocks, bytes 
FROM dba_extents 
ORDER BY tablespace_name, segment_name, extent_id;

-- 12
SELECT rowid, id, data 
FROM DSZ.DSZ_T1 
WHERE ROWNUM <= 15;

-- 13
SELECT id, ORA_ROWSCN, data 
FROM DSZ.DSZ_T1;

-- 14
ALTER TABLE DSZ.DSZ_T1 ENABLE ROW MOVEMENT;

UPDATE DSZ.DSZ_T1 SET data = data || ' изменено' WHERE id <= 10;
COMMIT;

SELECT id, ORA_ROWSCN, data 
FROM DSZ.DSZ_T1 WHERE ROWNUM <= 15;

DROP TABLESPACE DSZ_QDATA INCLUDING CONTENTS AND DATAFILES;

select * from v$diag_info;
select * from v$instance;