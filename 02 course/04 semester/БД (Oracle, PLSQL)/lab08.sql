-- 1
select SID, SERIAL#, USERNAME, MACHINE from v$session
where USERNAME is not null
