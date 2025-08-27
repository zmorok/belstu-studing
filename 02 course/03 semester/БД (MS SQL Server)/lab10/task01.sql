use Univer;
go

-- task01
exec sp_helpindex 'AUDITORIUM'
exec sp_helpindex 'AUDITORIUM_TYPE'
exec sp_helpindex 'FACULTY'
exec sp_helpindex 'PULPIT'
exec sp_helpindex 'SUBJECT'
exec sp_helpindex 'GROUPS'
exec sp_helpindex 'STUDENT'
exec sp_helpindex 'PROGRESS'
exec sp_helpindex 'PROFESSION'
exec sp_helpindex 'TEACHER'

use tempdb;
go

create table #t1
(
    ID int,
    символы varchar(50),
    числа int 
)

set nocount on;
declare @i int = 1
while @i <= 120000
begin
    insert into #t1 values (@i, 'строка: ', @i * rand());
    set @i += 1;
end

set statistics time on;
set statistics profile on;
select * from #t1 where id between 1000 and 9000 order by id;
set statistics time off;
set statistics profile off;

checkpoint;
dbcc DROPCLEANBUFFERS;

create clustered index #t1_ID on #t1(ID asc)
drop index #t1_ID on #t1

drop table #t1