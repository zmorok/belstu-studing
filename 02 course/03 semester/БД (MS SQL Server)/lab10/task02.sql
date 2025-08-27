use tempdb;
go

-- task02
create table #t2 (
	id int,
	num int identity(1,1),
	val varchar(20)
)

set nocount on;
declare @i int = 1
while @i <= 20000
begin
	insert #t2 values (@i * rand(), @i * rand() / 100)
	set @i += 1
end

set statistics profile on;
select * from #t2
set statistics profile off;

create nonclustered index #idx_id_num on #t2(id,num)

set statistics profile on;
select * from #t2 where id = 10 and num > 20
set statistics profile off;

checkpoint
dbcc dropcleanbuffers

drop index #idx_id_num on #t2
drop table #t2