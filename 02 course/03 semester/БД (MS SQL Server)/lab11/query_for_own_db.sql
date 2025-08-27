use lab11_db;
go

--1 товары для каждого заказа
declare cursor_orders cursor for 
	select Наименование_товара from ЗАКАЗЫ

declare @товар nvarchar(20), @товары_list nvarchar(300) = ''
open cursor_orders
print 'Список товаров в заказах:'
fetch cursor_orders into @товар

while @@FETCH_STATUS = 0
begin
	set @товары_list = rtrim(@товар) + ', ' + @товары_list
	fetch cursor_orders into @товар
end
print @товары_list
close cursor_orders
deallocate cursor_orders
go

--2 все заказчики
declare cursor_customers cursor local for 
	select Наименование_фирмы from ЗАКАЗЧИКИ

declare @customer nvarchar(20)

open cursor_customers
fetch cursor_customers into @customer

print 'Заказчики:'
while @@FETCH_STATUS = 0
begin
	print @customer
	fetch cursor_customers into @customer
end
go

--3
declare static_cursor cursor local static for
	select Наименование, Цена, Количество from ТОВАРЫ

declare @название nvarchar(20), @цена real, @количество int
open static_cursor
print 'Количество строк в курсоре товаров: ' + cast(@@cursor_rows as varchar)

fetch static_cursor into @название, @цена, @количество
while @@FETCH_STATUS = 0
begin
	print @название + ' - ' + cast(@цена as varchar) + ' руб., в наличии: ' + cast(@количество as varchar)
	fetch static_cursor into @название, @цена, @количество
end
go

--4 scroll
declare @order_num nvarchar(10), @rownum int
declare scroll_cursor cursor local scroll for 
	select row_number() over (order by Номер_заказа) as RowNum, Номер_заказа from ЗАКАЗЫ

open scroll_cursor
fetch next from scroll_cursor into @rownum, @order_num
print 'Первая строка: ' + cast(@rownum as varchar) + ' - ' + @order_num

fetch last from scroll_cursor into @rownum, @order_num
print 'Последняя строка: ' + cast(@rownum as varchar) + ' - ' + @order_num

go

--5 удаление заказов с количеством менее 3
declare delete_cursor cursor local dynamic for 
	select Номер_заказа, Количество from ЗАКАЗЫ for update

declare @order_id nvarchar(10), @order_quantity int
open delete_cursor
fetch delete_cursor into @order_id, @order_quantity

while @@FETCH_STATUS = 0
begin
	if @order_quantity < 3
	begin
		print 'Удаление заказа ' + @order_id + ' с количеством: ' + cast(@order_quantity as varchar)
		delete from ЗАКАЗЫ where current of delete_cursor
	end
	fetch delete_cursor into @order_id, @order_quantity
end
go

--6 цена товаров +10%
declare update_price_cursor cursor local dynamic for
	select Наименование, Цена from ТОВАРЫ for update

declare @product_name nvarchar(20), @current_price real
open update_price_cursor

fetch update_price_cursor into @product_name, @current_price
while @@FETCH_STATUS = 0
begin
	update ТОВАРЫ set Цена = @current_price * 1.1 where current of update_price_cursor
	print 'Цена товара ' + @product_name + ' увеличена до: ' + cast(@current_price * 1.1 as varchar)
	fetch update_price_cursor into @product_name, @current_price
end

go


