str = input('Введите строку: ')
print(f'Длина вашей строки: {len(str)}')
print(f'Строка в верхнем регистре: {str.upper()}')

print(f'Есть ли в строке цифры? {'Да' if any(c.isdigit() for c in str) else 'Нет' }')

print(f'Вторая половина строки: {str[len(str)//2:]}')