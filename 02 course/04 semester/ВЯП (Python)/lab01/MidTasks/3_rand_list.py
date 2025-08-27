import random as r
n = int(input('Количество случайных чисел: '))
a = [r.randint(1, 100) for _ in range(n)] 
b = [sum(a[i::-1]) for i in range(n)]

print(f'Список-источник: {a}')
print(f'Список сумм предыдущих элементов: {b}')