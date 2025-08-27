use tempdb;
go

-- task05
create table #t5 (
	ID int identity(1,1),
	num float
);

set nocount on;
declare @k int = 0
while @k < 20000
begin  
    insert #t5(num) values (1000 + rand() * 40000);
	set @k += 1;
end;

create nonclustered index #idx_num on #t5(num)

insert top(20000) #t5(num) select num from #t5;

select name [Индекс], avg_fragmentation_in_percent [Фрагментация (%)]
from sys.dm_db_index_physical_stats(DB_ID(N'TEMPDB'), 
OBJECT_ID(N'#t5'), NULL, NULL, NULL) ss JOIN sys.indexes ii on ss.object_id = ii.object_id and ss.index_id = ii.index_id  WHERE name is not null;

alter index #idx_num on #t5 reorganize;
alter index #idx_num on #t5 rebuild with (online = off);

drop index #idx_num on #t5
drop table #t5