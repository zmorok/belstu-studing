use Univer
--------------------------- B ---------------------------
begin tran
--------- t1 ---------
	-- insert into AUDITORIUM values ('66', 'ЛБ-К', 40, 'task')
	update AUDITORIUM set AUDITORIUM_CAPACITY = 40 where AUDITORIUM = '206-1'
	commit tran
--------- t2 ---------

-- delete from AUDITORIUM where AUDITORIUM = '66'
-- update AUDITORIUM set AUDITORIUM_CAPACITY = 15 where AUDITORIUM = '206-1'
-- select * from AUDITORIUM