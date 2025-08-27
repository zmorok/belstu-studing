use Univer
--------------------------- B ---------------------------
begin tran
	insert into AUDITORIUM values ('888', 'ЛК', 100, '888n')
--------- t1 ---------
--------- t2 ---------
select * from AUDITORIUM
rollback tran
