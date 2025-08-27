import random as r

tuple = tuple(r.randint(1, 100) for _ in range(20))

print(f'Кортеж: {tuple}\n')
print(f'Минимальное значение в кортеже: {min(tuple)}')
print(f'Максимально значение в кортеже: {max(tuple)}')
print(f'Сумма значений в кортеже: {sum(tuple)}')
print(f'Среднее значений в кортеже: {sum(tuple)/len(tuple)}')