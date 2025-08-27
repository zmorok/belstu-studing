use Univer
--------------------------- A ---------------------------
set transaction isolation level read uncommitted
begin tran
--------- t1 ---------
	select * from AUDITORIUM where AUDITORIUM = '888'
commit tran
--------- t2 ---------