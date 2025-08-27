//task1
const pattern = /first.+second/s.test('first1234Dsecond')

console.group('Задание 1')
console.log(pattern)
console.groupEnd()

//task 2
const text = '[u] Some text [/u], [u] Another text [/u]'
const replacedText = text.replace(/\[\u\](.*?)\[\/\u\]/g, '<u>$1</u>')
console.group('Задание 2')
console.log(text)
console.log(replacedText)
console.groupEnd()
// флаг g (global) используется для замены всех вхождений, без него изменится только первое

//task 3
const regexReplaceATags = /<a/g
const originalStr = '<a href="#">Ссылка 1</a> и еще <a href="#">Ссылка 2</a>'
const replacedStr = originalStr.replace(regexReplaceATags, '<not-a')

console.group('Задание 3')
console.log(originalStr)
console.log(replacedStr)
console.groupEnd()

//task 4
const originalString = '1Первая строка, 2Вторая строка'
const rearrangedString = originalString.replace(/(.+), (.+)/, '$2, $1')

console.group('Задание 4')
console.log(originalString)
console.log(rearrangedString)
console.groupEnd()

//task 5
const originalObject = { prop1: 'value1', prop2: 'value2', prop3: 'value3' }
const { prop2, ...rest } = originalObject

console.group('Задание 5')
console.log(originalObject)
console.log(rest)
console.groupEnd()

//task 6
const originalObject2 = { key1: 'value1', key2: 'value2', key3: 'value3' }
const { ...rest2 } = originalObject2
const updatedObject = {
	firstName: 'Иванов',
	lastName: 'Иванович',
	...rest2,
}
console.group('Задание 6')
console.log(originalObject2)
console.log(updatedObject)
console.groupEnd()
