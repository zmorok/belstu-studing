use Univer
--------------------------- B ---------------------------
begin tran
--------- t1 ---------
	insert into AUDITORIUM values ('66', 'ЛБ-К', 40, 'task')
	commit tran
--------- t2 ---------

-- delete from AUDITORIUM where AUDITORIUM = '66'