logs = [
  ('192.168.1.1', '200 OK', 1543),
  ('192.168.1.2', '404 Not Found', 234),
  ('192.168.1.1', '500 Internal Server Error', 542),
  ('192.168.1.3', '200 OK', 876),
  ('192.168.1.2', '200 OK', 1324),
]

ip_count = {}
for ip, _, _ in logs:
  if ip in ip_count: ip_count[ip] += 1
  else: ip_count[ip] = 1

status_count = {}
for _, status, _ in logs:
  if status in status_count: status_count[status] += 1
  else: status_count[status] = 1

most_common_status = max(status_count, key=status_count.get)

total_data = sum(size for _, _, size in logs)

print('Количество запросов от каждого IP:', ip_count)
print('Самый частый HTTP-статус:', most_common_status,'\b,', status_count[most_common_status])
print('Общий объем переданных данных:', total_data)
