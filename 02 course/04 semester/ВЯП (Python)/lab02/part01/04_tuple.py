tuple_1 = (11, 12, 13, [10, 20, 30])
q, w, e, r = tuple_1

dict = {}
for i, value in enumerate(r):
  dict[f'item_{i+1}'] = value


print(f'q: {q}')
print(f'w: {w}')
print(f'e: {e}')
print(f'dict: {dict}')