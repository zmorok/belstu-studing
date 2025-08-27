use Univer
--------------------------- A ---------------------------
set tran isolation level repeatable read
begin tran
	select * from AUDITORIUM where AUDITORIUM_CAPACITY < 50
--------- t1 --------- 
--------- t2 ---------
commit tran
	select * from AUDITORIUM where AUDITORIUM_CAPACITY < 50

-- update AUDITORIUM set AUDITORIUM_CAPACITY = 15 where AUDITORIUM = '206-1'
-- select * from AUDITORIUM