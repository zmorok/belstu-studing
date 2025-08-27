/*
	create table #t1 (
		column1 varchar(10),
		column2 int
		)
*/
--drop table #t1

set implicit_transactions on
declare @counter int, @flag char = 'c'

insert into #t1 
	values ('строка1', 1),
			('строка4', 2),
			('н строка', 3)

select * from #t1
set @counter = (select count(*) from #t1)

print 'кол-во строк в таблице t1: ' + cast(@counter as varchar)
if @flag = 'c' commit
else rollback

set implicit_transactions off
