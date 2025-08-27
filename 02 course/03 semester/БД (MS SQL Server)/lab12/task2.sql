use Univer
begin try
	begin tran
		update TEACHER set PULPIT = 1234 where TEACHER = 'БЗБРДВ'
	commit tran
end try
begin catch
	if @@trancount > 0 rollback tran

	select 
		error_number() [Номер ошибки],
		error_severity() [Серьёзность ошибки],
		error_state() [Состояние ошибки],
		error_procedure() [Место появления ошибки],
		error_line() [Строка с ошибкой],
		error_message() [Содержимое ошибки]
end catch

select * from TEACHER where TEACHER = 'БЗБРДВ'
