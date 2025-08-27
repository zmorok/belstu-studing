use tempdb;
go

-- task03
create table #t3 (
    id int,
    num int identity(1,1),
    val varchar(20)
);

set nocount on;
declare @i int = 1;
while @i <= 20000
begin
    insert #t3 values (@i * rand(), @i * rand() / 100);
    set @i += 1;
end;

create nonclustered index #idx_id_num_val on #t3(id, num) include (val);

set statistics profile on;
select * from #t3 where id = 10 and num > 500 order by val;
set statistics profile off;

checkpoint
dbcc dropcleanbuffers

drop index #idx_id_num_val on #t3;
drop table #t3;
