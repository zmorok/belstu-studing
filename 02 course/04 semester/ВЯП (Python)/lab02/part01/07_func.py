import math as m

def square(side):
  perimeter = side * 4
  area = side ** 2
  diagonal = round(side * m.sqrt(2), 2)
  return perimeter, area, diagonal

side_length = 5
result = square(side_length)
print(f'Для квадрата со стороной {side_length}:')
print(f'Периметр: {result[0]}, Площадь: {result[1]}, Диагональ: {result[2]}')
