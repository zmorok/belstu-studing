use Univer;
go

-- task01
declare Disciplina cursor for 
	select S.SUBJECT from PULPIT P
	join SUBJECT S on P.PULPIT = S.PULPIT where P.PULPIT like 'ИСиТ'

declare @sp char(20), @s char(300) = ''
open Disciplina
	fetch Disciplina into @sp
	print 'Дисциплины:'
	while @@FETCH_STATUS = 0
	begin
		set @s = rtrim(@sp) + ', ' + @s
		fetch Disciplina into @sp
	end
	print @s
close Disciplina
deallocate Disciplina;

--task02
-----------------------local
declare local_curs cursor local for 
	select T.TEACHER_NAME from TEACHER T

declare @teacher char(30)

open local_curs
fetch local_curs into @teacher;
while @@FETCH_STATUS = 0
begin
	print @teacher;
	fetch local_curs into @teacher;
end
go
-----------------------
-----------------------global
declare global_curs cursor global for 
	select T.TEACHER_NAME from TEACHER T;

declare @teacher2 char(30);
open global_curs
	fetch global_curs into @teacher2;
	print 'Преподаватели:';

	declare @i int = 1;
	while @@fetch_status = 0
	begin
		print cast(@i as varchar) + ': ' + @teacher2
		fetch global_curs into @teacher2
		set @i += 1
	end
close global_curs
deallocate global_curs
-----------------------

--task03
-----------------------local static
declare static_curs cursor local static for
	select FACULTY, PROFESSION, PROFESSION_NAME, QUALIFICATION
	from PROFESSION where FACULTY = 'ИТ'

declare @static_fac varchar(5), @static_prof varchar(15), @static_profname varchar(40), @static_qual varchar(15)
open static_curs
	print 'Количество строк : ' + cast(@@cursor_rows as varchar(5))

	--insert PROFESSION (FACULTY, PROFESSION, PROFESSION_NAME, QUALIFICATION)
	--	values ('ИТ',  '1-55 13 13', 'INFO', '-системотехник');

	--delete from PROFESSION where PROFESSION_NAME = 'INFO'

	fetch static_curs into @static_fac, @static_prof, @static_profname, @static_qual;
	while @@fetch_status = 0
		begin
			print @static_fac + ' ' + @static_prof + ' ' + @static_profname + '		' + @static_qual;
			fetch static_curs into @static_fac, @static_prof, @static_profname, @static_qual;
		end;
-----------------------
-----------------------local dynamic
declare dynamic_curs cursor local dynamic for
	select FACULTY, PROFESSION, PROFESSION_NAME, QUALIFICATION
	from PROFESSION where FACULTY = 'ИТ'

declare @dynamic_fac varchar(5), @dynamic_prof varchar(15), @dynamic_profname varchar(40), @dynamic_qual varchar(15)
open dynamic_curs
	
	print 'Количество строк: ' + cast(@@cursor_rows as varchar(5))

	insert PROFESSION (FACULTY, PROFESSION, PROFESSION_NAME, QUALIFICATION)
		values ('ИТ',  '1-55 13 13', 'INFO', '-системотехник');

	--delete from PROFESSION where PROFESSION_NAME = 'INFO'

	fetch dynamic_curs into @dynamic_fac, @dynamic_prof, @dynamic_profname, @dynamic_qual;
	while @@fetch_status = 0
		begin
			print @dynamic_fac + ' ' + @dynamic_prof + ' ' + @dynamic_profname + '		' + @dynamic_qual;
			fetch dynamic_curs into @dynamic_fac, @dynamic_prof, @dynamic_profname, @dynamic_qual;
		end;
-----------------------

--task04
declare @cotnum int, @rownum int;
declare scroll_curs cursor local dynamic scroll for
	select row_number() over (order by IDSTUDENT) N, IDSTUDENT from STUDENT

open scroll_curs
	fetch scroll_curs into @cotnum, @rownum;                 
	print 'следующая строка	: ' + cast(@cotnum as varchar(3)) + '    '+  cast(@rownum as varchar(10));

	fetch last from scroll_curs into @cotnum, @rownum;
	print 'последняя строка	: ' + cast(@cotnum as varchar(3)) + '   '+  cast(@rownum as varchar(10));

--task05
declare @id int;

declare update_curs cursor local dynamic for 
	select * from STUDENT for update

open update_curs
	fetch update_curs into @id;

	delete STUDENT where current of update_curs;

	fetch update_curs into @id;

	update STUDENT set IDGROUP = IDGROUP + 1 where current of update_curs;

--task06
-----------------------delete notes under 4
declare @note char(10), @ids char(10);
declare del4_curs cursor local dynamic for
	select P.NOTE, S.IDSTUDENT
	from PROGRESS P
    join STUDENT S on P.IDSTUDENT = S.IDSTUDENT for update;

open del4_curs
	fetch del4_curs into @note, @ids
	while @@fetch_status = 0
	begin
		if @note <= 4
		begin
			print @note + '	' + @ids;
			delete from PROGRESS where IDSTUDENT = @ids and PROGRESS.NOTE = @note
			delete STUDENT where STUDENT.IDSTUDENT = @ids
		end
		fetch del4_curs into @note, @ids
	end

select * from PROGRESS
-----------------------
-----------------------update current student's mark to +1 
declare @mark int
declare inc_curs cursor local dynamic for
    select NOTE from PROGRESS where IDSTUDENT = '1005' for update;

open inc_curs
	fetch inc_curs into @mark
	while @@fetch_status = 0
	begin
		if @mark < 10
		begin
			update PROGRESS set NOTE = @mark + 1 where current of inc_curs;
		end
		fetch inc_curs into @mark
	end
-----------------------