use Univer;
go

-- task01
declare @char char(10) = 'какая-то строка фикс.',
        @varchar varchar(20) = 'какая-то строка динам.',
        @datetime datetime,
        @time time,
        @int int,
        @smallint smallint,
        @tinyint tinyint,
        @numeric numeric(12, 5);

set @datetime = '20241018 14:30:00';
set @time = '15:45:00';
set @int = 123456;
set @smallint = 32000;
set @tinyint = 255;
set @numeric = 123456.78912;

select @datetime = '2024-10-18T16:00:00',
       @time = '16:30:00',
       @int = 654321,
       @smallint = 31000,
       @tinyint = 200,
       @numeric = 987654.12345;

select @char [Строка charов],
       @varchar [Строка varcharов],
       @datetime [Время datetime],
       @time [Время time];

print 'ЗАДАНИЕ 1'
print 'Целое: ' + cast(@int as varchar(10));
print 'Малое целое: ' + cast(@smallint as varchar(10));
print 'Очень малое целое: ' + cast(@tinyint as varchar(10));
print 'Вещественное: ' + cast(@numeric as varchar(20));
print 'Время из переменной: ' + convert(varchar(30), @datetime, 104);

-- task02
print '' print 'ЗАДАНИЕ 2'
declare @total int
select @total = sum(AUDITORIUM_CAPACITY) from AUDITORIUM

if @total > 200
begin
	declare @audCount int= (select count(*) from AUDITORIUM)
	 
	declare @audAvgCap float = (select avg(AUDITORIUM_CAPACITY) from AUDITORIUM)

	declare @belowAvgCount int = (select count(*) from AUDITORIUM where AUDITORIUM_CAPACITY < @audAvgCap)

	declare @belowAvgPercentage float = (@belowAvgCount * 100.0) / @audCount

	print 'кол-во аудиторий: ' + cast(@audCount as varchar(10))
	print 'средняя вместительность: ' + cast(@audAvgCap as varchar(10))
	print 'кол-во аудиторий с вместительностью < avg: ' + cast(@belowAvgCount as varchar(10))
	print 'процент аудиторий с вместительностью < avg: ' + cast(@belowAvgPercentage as varchar(10)) + '%'
end
else
begin
	print 'общая вместительность аудиторий: ' + cast(@total as varchar(10))
end

-- task03
print '' print 'ЗАДАНИЕ 3'
print 'ROWCOUNT: ' + convert(varchar, @@rowcount)
print 'VERSION: ' + @@version
print 'SPID: ' + convert(varchar, @@spid)
print 'ERROR: ' + convert(varchar, @@error)
print 'SERVERNAME: ' + @@servername
print 'TRANCOUNT: ' + convert(varchar, @@trancount)
print 'FETCH_STATUS: ' + convert(varchar, @@fetch_status)
print 'NESTLEVEL: ' + convert(varchar, @@nestlevel)

-- task04
	-- 04.01
	print '' print 'ЗАДАНИЕ 4.1'
declare @x int, @t int, @z float;
set @t = 5
set @x = 5
if (@t > @x) 
	set @z = power(sin(@t), 2)
else if (@t < @x) 
	set @z =4*(@t+@x)
else
	set @z=1- exp(@x-2)

print 'z=' +  cast(@z as varchar)

	-- 04.02
	print '' print 'ЗАДАНИЕ 4.2'
declare @name char, @surname varchar(10), @lastname char
set @name = 'Татьяна'
set @surname = 'Макейчик'
set @lastname = 'Леонидовна'
print 'ФИО: ' + @surname + ' ' + @name + '. ' + @lastname + '.'

	-- 04.03
	print '' print 'ЗАДАНИЕ 4.3 (select)'
select NAME [ФИО], BDAY [ДР], IDSTUDENT [ИД], year(getdate()) - year(BDAY) [Возраст] from STUDENT
where month(BDAY) = month(getdate()) + 1

	-- 04.04
	print '' print 'ЗАДАНИЕ 4.4 (select)'
select S.IDSTUDENT, G.IDGROUP, P.NOTE, P.PDATE, P.SUBJECT, datename(weekday, BDAY) [День] 
from PROGRESS P
	join STUDENT S on P.IDSTUDENT = S.IDSTUDENT
	join GROUPS G on S.IDGROUP = G.IDGROUP
where P.SUBJECT = 'СУБД'

-- task05
use lab08_db;
go

print '' print 'ЗАДАНИЕ 5'
declare @id int
set @id = (select top 1 ID_студента from СТУДЕНТЫ)
if @id  = 2
begin
	print 'найден студент с айди 1'
end
else
begin
	print 'у найденного студента айди НЕ 2'
end

-- task06
use Univer;
go

print '' print 'ЗАДАНИЕ 6 (select)'
select S.NAME [ФИО], G.PROFESSION [Специальность], P.SUBJECT [Дисциплина], P.NOTE [Оценка],
	case
		when P.NOTE >= 9 then 'Отлично'
		when P.NOTE >= 7 then 'Хорошо'
		when P.NOTE >= 4 then 'Удовлетворительно'
		else 'Неудовлетворительно'
	end [Категория оценки]
from STUDENT S
	join GROUPS G on S.IDGROUP = G.IDGROUP
	join PROGRESS P on S.IDSTUDENT = P.IDSTUDENT
	join FACULTY F on G.FACULTY = F.FACULTY
where F.FACULTY = 'ИТ'

-- task07
use tempdb;
go

print '' print 'ЗАДАНИЕ 7 (local bd, select)'
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

-- task08
print '' print 'ЗАДАНИЕ 8 return'
declare @var int = 2
print @var + 1
print @var + 2
return
print @var + 3

-- task09
print '' print 'ЗАДАНИЕ 9 try catch'
begin try
    create table #TT (ID int not null);
    insert into #TT (ID) values (null);
end try
begin catch
    select 
        ERROR_NUMBER() [Номер],
        ERROR_MESSAGE() [Сообщение],
        ERROR_LINE() [Номер строки],
        ERROR_PROCEDURE() [Имя процедуру или null],
        ERROR_SEVERITY() [Уровень серьёзности],
        ERROR_STATE() [Метка ошибки];
end catch

drop table #TT