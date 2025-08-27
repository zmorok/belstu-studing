use Univer
go

delete from TEACHER where TEACHER.TEACHER = '44'

begin try
    insert into TEACHER values (44, 'asdfsfsvsvsd',5, 'ИСиТ')
end try
begin catch
    print 'Ошибка ' + error_message()
end catch
go

select * from TR_AUDIT
go
