print('Введите два числа')
x = input('Первое число: ')
y = input('Второе число: ')

try:
  float(x)
  float(y)
except ValueError:
  print('Ошибка, тут не число')
  exit()

x, y = float(x), float(y)
print(f'{'*'*20}\nОперации над ними:')
print(f'Сумма = {x + y}')
print(f'Произведение = {x * y}')
print(f'Разность = {x - y}')
if y != 0:
  print(f'Деление = {x / y }')
  print(f'Округление до 2х знаков после запятой {round(x/y,2)}')
else:
  print('Второе число ноль, деление невозможно')
print(f'Первое число {int(x)} чётное? - {'Да' if x % 2 == 0 else 'Нет'}\n{'*'*20}') if x.is_integer() else print(f'{x} - не целое число')