use Univer;
go

declare @total int
select @total = sum(AUDITORIUM_CAPACITY) from AUDITORIUM

if @total > 200
begin
	declare @audCount int
	select @audCount = count(*) from AUDITORIUM

	declare @audAvgCap float
	select @audAvgCap = avg(AUDITORIUM_CAPACITY) from AUDITORIUM

	declare @belowAvgCount int
	select @belowAvgCount = count(*) from AUDITORIUM where AUDITORIUM_CAPACITY < @audAvgCap

	declare @belowAvgPercentage float
	select @belowAvgPercentage = (@belowAvgCount * 100.0) / @audCount

	print 'кол-во аудиторий: ' + cast(@audCount as varchar(10))
	print 'средняя вместительность: ' + cast(@audAvgCap as varchar(10))
	print 'кол-во аудиторий с вместительностью < avg: ' + cast(@belowAvgCount as varchar(10))
	print 'процент аудиторий с вместительностью < avg: ' + cast(@belowAvgPercentage as varchar(10)) + '%'
end
else
begin
	print 'общая вместительность аудиторий: ' + cast(@total as varchar(10))
end