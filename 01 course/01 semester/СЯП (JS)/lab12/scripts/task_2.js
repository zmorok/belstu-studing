let obj = new Set()

obj.add(1).add(3).add(5).add(1)

console.group('Задание 2 (коллекция Set)')
console.log(obj)
console.log('obj.size= ' + obj.size)
console.log('obj.has(1)= ' + obj.has(1))
console.log('obj.has(3)= ' + obj.has(3))
console.log('obj.has(5)= ' + obj.has(5))
console.groupEnd()
