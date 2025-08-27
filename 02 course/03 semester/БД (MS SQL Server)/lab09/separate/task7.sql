use tempdb;
go

create table #local_tb (
	firstC varchar(10) not null,
	secondC int not null,
	thirdC int not null
)

declare @i int = 0
while @i < 10
begin
	insert #local_tb(firstC, secondC, thirdC)
	values('строка: ', @i + 1, rand()*3234)
	set @i += 1
end

select * from #local_tb

drop table #local_tb