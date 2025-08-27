--1.	Выбрать все заказы, выполненные после определенной даты.
select * from orders where order_date > '2008-01-01';

--2.	Выбрать все офисы из определенного региона и управляемые определенным сотрудником.
select * from offices where region = 'Eastern' and mgr = 106;

--3.	Выбрать заказы, сделанные в определенный период.
select * from orders where order_date between '2007-06-01' and '2007-12-01';

--4.	Выбрать офисы из 12, 13 и 21 региона.
select * from offices where office in (12, 13, 21);

--5.	Выбрать сотрудника, у которого нет менеджера (самого главного).
select * from salesreps where manager is null;

--6.	Выбрать офисы из региона, который начинается на East.
select * from offices where region like 'East%';

--7.	Выбрать фамилии и даты найма всех сотрудников и отсортировать по возрасту.
select name, hire_date from salesreps order by age;

--8.	Выбрать все заказы и отсортировать вначале по стоимости по убыванию, а затем по количеству заказанного по возрастанию.
select * from orders order by amount desc, qty;

--9.	Выбрать 5 самых дорогих товаров.
select top 5 * from products order by price desc;

--10.	Выбрать 30% самых дорогих заказов.
select top 30 percent * from orders order by amount desc;

--11.	Выбрать сотрудников с 4 по 7, отсортированных по дате найма.
select * from salesreps where empl_num between 104 and 107 order by hire_date;

--12.	Выбрать уникальные товары в заказах.
select distinct mfr, product from orders;

--13.	Подсчитать количество заказов для каждого покупателя.
select cust, count(*) [Количество заказов] from orders group by cust;

--14.	Подсчитать итоговую сумму заказа для каждого покупателя.
select cust, sum(amount) [Итоговая сумма заказа] from orders group by cust;

--15.	Подсчитать среднюю цену заказа для каждого сотрудника.
select rep, avg(amount) [Средняя цена заказа] from orders group by rep;

--16.	Найти самый дорогой товар каждого производителя.
select mfr_id, max(price) [Самый дорогой товар] from products group by mfr_id;

--17.	Найти покупателей и их заказы (в результирующем наборе должны быть: наименование покупателя, наименование товара, производитель, количество и итоговая сумма).
select c.company [Наименование покупателя], p.description [Наименование товара], p.mfr_id [Производитель], o.qty [Количество], o.amount [Итоговая сумма] 
from orders o
join customers c on o.cust = c.cust_num 
join products p on o.mfr = p.mfr_id and o.product = p.product_id;

--18.	Найти всех покупателей, которые делали больше 3 заказов.
select cust, count(*) [Количество заказов] from orders 
group by cust having count(*) > 3;

--19.	Найти покупателей, у которых нет заказов.
select * from customers 
where cust_num not in (select cust from orders);

--20.	Найти покупателей, у которых нет заказов в определенный период.
select * from customers 
where cust_num not in (
    select cust from orders 
    where order_date between '2007-10-01' and '2008-01-01'
);

--21.	Найти товары, которые купили покупатели с кредитным лимитом больше 40000.
select distinct p.description from products p
join orders o on p.PRODUCT_ID = o.PRODUCT
join customers c on o.cust = c.cust_num where c.credit_limit > 40000;

--22.	Найти сотрудников одного возраста.
select age, count(*) from salesreps 
group by age having count(*) > 1;

--23.	Выбрать всех покупателей в порядке уменьшения обшей стоимости заказов.
select cust, sum(amount) [Общая стоимость] from orders 
group by cust order by [Общая стоимость] desc;

--24.	Выбрать заказы, сумма которых больше среднего значения.
select * from orders
where amount > (select avg(amount) from orders)

--25.	Подсчитать, на какую сумму каждый офис выполнил заказы, и отсортировать их в порядке убывания.
select rep_office, sum(amount) [Общие продажи] from orders o
join salesreps s on o.rep = s.empl_num 
group by rep_office order by [Общие продажи] desc;

--26.	Выбрать сотрудников, которые являются начальниками (у которых есть подчиненные).
select distinct s1.empl_num from salesreps s1 
join salesreps s2 on s1.empl_num = s2.manager

select distinct manager from salesreps where manager is not null

--27.	Выбрать сотрудников, которые не являются начальниками (у которых нет подчиненных).
select * from salesreps 
where empl_num not in (select distinct manager from salesreps where manager is not null);

select * from salesreps 
where empl_num not in (select distinct s1.empl_num from salesreps s1 
join salesreps s2 on s1.empl_num = s2.manager)

--28.	Выбрать всех продукты, продаваемые менеджерами из западного региона.
select distinct products.description 
from orders 
join salesreps on orders.rep = salesreps.empl_num 
join offices on salesreps.rep_office = offices.office 
join products on orders.mfr = products.mfr_id and orders.product = products.product_id 
where offices.region = 'Western';

--29.	Выбрать товары, которые по стоимости ниже среднего значения стоимости заказа по покупателю.
select p.description from products p 
join orders o on p.mfr_id = o.mfr and p.product_id = o.product 
group by p.description 
having avg(o.amount) < (select avg(amount) from orders);

--30.	Найти организации, которые не делали заказы в 2008, но делали в 2007 (как минимум 2-мя разными способами).
-- Первый способ
select company 
from customers 
where cust_num in (
    select cust from orders where year(order_date) = 2007
) 
and cust_num not in (
    select cust from orders where year(order_date) = 2008
);

-- Второй способ
select distinct c.company 
from customers c 
left join orders o1 on c.cust_num = o1.cust and year(o1.order_date) = 2007 
left join orders o2 on c.cust_num = o2.cust and year(o2.order_date) = 2008 
where o1.order_num is not null and o2.order_num is null;

------------------------------------------------------------------------
-- Найти 3 товара, которых заказывали меньше всего
select p.DESCRIPTION, sum(o.QTY) from PRODUCTS p
join orders o on o.PRODUCT = p.PRODUCT_ID
group by p.DESCRIPTION
order by sum(o.QTY)