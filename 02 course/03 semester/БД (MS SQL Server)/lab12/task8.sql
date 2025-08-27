use tempdb
/*
select * into #t8 from Univer.dbo.AUDITORIUM
select * from #t8
-- drop table #t8
*/

begin tran

begin tran 
    insert into #t8 values ('500-12', 'ЛБ-К', 40, '500-12');
    select * from #t8 where AUDITORIUM = '500-12'
	print('Вложенность транзакции1 = ' + cast(@@trancount as varchar))
commit tran

begin tran
    update #t8 set AUDITORIUM_CAPACITY = 200 where AUDITORIUM = '500-12';
    select * from #t8 where AUDITORIUM = '500-12'
	print('Вложенность транзакции1 = ' + cast(@@trancount as varchar))
commit tran

print('Вложенность транзакции1 = ' + cast(@@trancount as varchar))
commit tran

-- delete from #t8 where AUDITORIUM = '500-12'