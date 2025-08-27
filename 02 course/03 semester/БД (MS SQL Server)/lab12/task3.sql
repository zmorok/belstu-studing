use Univer

begin try
    begin tran

    insert into AUDITORIUM values ('111-23', 'ЛК', 100, 'tp6o23')
    save tran insertPoint

    update AUDITORIUM set AUDITORIUM_CAPACITY = 2 where AUDITORIUM = '111-23'
    save tran updatePoint1

    update AUDITORIUM set AUDITORIUM_CAPACITY = -100 where AUDITORIUM = '111-23'
    save tran updatePoint2

    commit tran
end try
begin catch
    if @@TRANCOUNT > 0 
    begin
        rollback tran updatePoint1
		commit tran
    end

    select 
        error_number() as [Номер ошибки],
        error_severity() as [Серьёзность ошибки],
        error_state() as [Состояние ошибки],
        error_procedure() as [Место появления ошибки],
        error_line() as [Строка с ошибкой],
        error_message() as [Содержимое ошибки]
end catch

select * from AUDITORIUM
delete from AUDITORIUM where AUDITORIUM = '111-23'
