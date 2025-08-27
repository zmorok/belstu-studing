use Univer
--------------------------- B ---------------------------
set tran isolation level read committed
begin tran
--------- t1 ---------
insert into AUDITORIUM values ('222', 'ЛК-К', 130, '888nв')
--------- t2 ---------
commit tran
select * from AUDITORIUM

-- delete from AUDITORIUM where AUDITORIUM = '222'
-- select * from AUDITORIUM
