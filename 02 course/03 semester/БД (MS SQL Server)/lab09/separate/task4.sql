use Univer;
go

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

---------------------------

declare @name char, @surname varchar(10), @lastname char
set @name = 'Татьяна'
set @surname = 'Макейчик'
set @lastname = 'Леонидовна'
print 'ФИО: ' + @surname + ' ' + @name + '. ' + @lastname + '.'

---------------------------

select NAME [ФИО], BDAY [ДР], IDSTUDENT [ИД], year(getdate()) - year(BDAY) [Возраст] from STUDENT
where month(BDAY) = month(getdate()) + 1

---------------------------

select S.IDSTUDENT, G.IDGROUP, P.NOTE, P.PDATE, P.SUBJECT, datename(weekday, BDAY) [День] 
from PROGRESS P
	join STUDENT S on P.IDSTUDENT = S.IDSTUDENT
	join GROUPS G on S.IDGROUP = G.IDGROUP
where P.SUBJECT = 'СУБД'