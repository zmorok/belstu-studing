order_1 = {'apple', 'orange', 'banana'}
order_2 = {'apple', 'pear', 'orange'}

# common_items = order_1 & order_2
common_items = order_1.intersection(order_2)
print(f'Повторяющиеся позиции: {common_items}')

# unique_items = order_1 ^ order_2
unique_items = order_1.symmetric_difference(order_2)
print(f'Уникальные позиции: {unique_items}')

# merged_order = order_1 | order_2 
merged_order = order_1.union(order_2)
print(f'Объединенный заказ: {merged_order}')