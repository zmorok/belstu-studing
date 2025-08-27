use lab08_db;
go

select * from СТУДЕНТЫ

declare @id int
set @id = (select top 1 ID_студента from СТУДЕНТЫ)
if cast(@id as int) = 2
begin
	print 'найден студент с айди 1'
end
else
begin
	print 'у найденного студента айди НЕ 2'
end

/*
declare @id int

declare student_cursor cursor for
select ID_студента from СТУДЕНТЫ

open student_cursor
fetch next from student_cursor into @id

while @@FETCH_STATUS = 0
begin
    if cast(@id as int) = 1
    begin
        print 'найден студент с айди 1'
    end
    else
    begin
        print 'у найденного студента айди НЕ 1'
    end
    
    fetch next from student_cursor into @id
end

close student_cursor
deallocate student_cursor
*/