def is_year_leap(year):
  return (year % 4 == 0 and year % 100 != 0) or (year % 400 == 0)

years = [1700, 1800, 1900, 2000, 2100]
for year in years:
  print(f'Год {year} високосный? {'Да' if is_year_leap(year) else 'Нет'}')
