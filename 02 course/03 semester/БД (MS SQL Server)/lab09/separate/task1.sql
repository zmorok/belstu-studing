declare @char char(10) = 'какая-то строка фикс.',
        @varchar varchar(20) = 'какая-то строка динам.',
        @datetime datetime,
        @time time,
        @int int,
        @smallint smallint,
        @tinyint tinyint,
        @numeric numeric(12, 5);

set @datetime = '2024-10-18T14:30:00';
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

print 'Целое: ' + cast(@int as varchar(10));
print 'Малое целое: ' + cast(@smallint as varchar(10));
print 'Очень малое целое: ' + cast(@tinyint as varchar(10));
print 'Вещественное: ' + cast(@numeric as varchar(20));
