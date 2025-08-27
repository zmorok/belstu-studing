import random as r

tuple = tuple(r.randint(1, 100) for _ in range(20))
list = list(tuple)

print(f'Размер в байтах кортежа: {tuple.__sizeof__()}')
print(f'Размер в байтах списка: {list.__sizeof__()}')