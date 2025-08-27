use Univer
--------------------------- A ---------------------------
set tran isolation level read committed
begin tran
--------- t1 ---------
--------- t2 ---------
	select * from AUDITORIUM where AUDITORIUM = '222'
commit tran

-- delete from AUDITORIUM where AUDITORIUM = '222'
-- select * from AUDITORIUM