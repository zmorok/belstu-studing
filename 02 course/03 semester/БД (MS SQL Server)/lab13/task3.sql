use Univer
go

/*
create table #SUBJECT(
	код char(10),
	[имя предмета] varchar(100),
	кафедра char(20)
)

drop table #SUBJECT
*/

alter procedure PSUBJECT @p varchar(20)
as begin
	select * from SUBJECT where PULPIT = @p
end

insert into #SUBJECT exec psubject 'ИСиТ'
insert into #SUBJECT exec psubject 'ОХ'
insert into #SUBJECT exec psubject 'ТЛ'

select * from #SUBJECT