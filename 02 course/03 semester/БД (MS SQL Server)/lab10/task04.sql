use tempdb;
go

-- task04
create table #t4 (
    tkey int,
    val varchar(20)
);

set nocount on;
declare @i int = 1;
while @i <= 20000
begin
    insert #t4 values (@i, 'val ' + cast(@i as varchar(10)));
    set @i += 1;
end;

set statistics profile on;
select tkey from #t4 where tkey between 5000 and 19999;
select tkey from #t4 where tkey > 15000 and TKEY < 20000;
select tkey from #t4 where tkey = 17000;
set statistics profile off;

create nonclustered index #T4_WHERE on #t4(tkey) 
where (tkey >= 15000 and tkey < 20000);

checkpoint
dbcc dropcleanbuffers

drop index #T4_WHERE on #t4;
drop table #t4;
