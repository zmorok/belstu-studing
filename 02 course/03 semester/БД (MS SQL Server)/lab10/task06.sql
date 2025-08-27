use tempdb;
go

-- task06
create table #t6 (
	ID int identity(1,1),
	num float
);

set nocount on;
declare @k int = 0
while @k < 20000
begin  
    insert #t6(num) values (1000 + rand() * 40000);
	set @k += 1;
end;

set statistics profile on;
select * from #t6
set statistics profile off;

create nonclustered index #idx_num on #t6(num) with (fillfactor = 65)

insert top(20000) #t6(num) select num from #t6;

select name [Индекс], avg_fragmentation_in_percent [Фрагментация (%)]
from sys.dm_db_index_physical_stats(DB_ID(N'TEMPDB'), 
OBJECT_ID(N'#t6'), NULL, NULL, NULL) ss  JOIN sys.indexes ii on ss.object_id = ii.object_id and ss.index_id = ii.index_id  WHERE name is not null;

alter index #idx_num on #t6 reorganize;
alter index #idx_num on #t6 rebuild with (online = off);

drop index #idx_num on #t6
drop table #t6