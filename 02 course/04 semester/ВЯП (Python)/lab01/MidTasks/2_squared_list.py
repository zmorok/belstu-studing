squares = []
for i in range(10,19): squares.append(i ** 2)
print(f'Список квадратов: {squares}')
print(f'Сумма всех элементов: {sum(squares)}')
odd_squares = [i for i in squares if i % 2 != 0]
print(f'Список после удаления чётных элементов: {odd_squares}')
print(f'Количество элементов после удаления: {len(odd_squares)}')