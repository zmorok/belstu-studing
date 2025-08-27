use Univer
--------------------------- A ---------------------------
set tran isolation level serializable
begin tran
	select * from AUDITORIUM where AUDITORIUM_CAPACITY < 50
--------- t1 --------- 
--------- t2 ---------
commit tran
	select * from AUDITORIUM where AUDITORIUM_CAPACITY < 50

-- delete from AUDITORIUM where AUDITORIUM = '66'