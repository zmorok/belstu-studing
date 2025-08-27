'use strict'

// Задание 1
function expressions() {
	const form1 = 2 * Math.tan(5) + 6 - Math.PI
	const form2 = 2 * Math.cos(4) + Math.cos(12) + 8 - Math.exp(3)
	const form3 = 3 * Math.sin(9) + Math.log(5)
	const form4 = 6 - Math.PI ** 2 + 3 * Math.exp(8)

	let arrayOne = [form1, form2, form3, form4]
	document.write('Массив значений формул: ', arrayOne.join(' ; '), '<br>')
	document.write(
		'Макс. элемент= ' + Math.max(...arrayOne),
		'  Номер= ',
		arrayOne.indexOf(Math.max(...arrayOne)) + 1,
		'<br>'
	)
	document.write(
		'Мин. элемент= ' + Math.min.apply(null, arrayOne),
		'  Номер= ',
		arrayOne.indexOf(Math.min(...arrayOne)) + 1
	)
}

// Задание 2
function twoArrays() {
	let arrayOne = Array.from('1234567')
	let arrayExample = [123, 'Pavel', true, 123.45]
	let arrayTwo = Array.of(arrayOne, arrayExample)
	document.write("Массив из '1234567': ", arrayOne, '<br>')
	document.write('Массив из первого массива и массива значений: ', arrayTwo)
}

// Задание 3
function sortingArrays() {
	let arrArr_Math = [
		'pow',
		'pop',
		'push',
		'shift',
		'round',
		'floor',
		'slice',
		'sort',
	]
	let arrArr = []
	let arrMath = []
	for (let i = 0; i < arrArr_Math.length; i++) {
		if (Math.hasOwnProperty(arrArr_Math[i])) {
			arrMath.push(arrArr_Math[i])
		} else if (Array.prototype.hasOwnProperty(arrArr_Math[i])) {
			arrArr.push(arrArr_Math[i])
		} else {
			console.log('Неверный метод')
		}
	}
	arrArr.push('unshift') // метод unshift добавляется в конец arrArr
	arrMath.unshift('cos') // метод cos добавляется в начало arrMath
	document.write(
		'Исходный массив: ',
		arrArr_Math,
		'<br>',
		'Длинна исходного массива= ',
		arrArr_Math.length,
		'<br>'
	)
	document.write(
		'Массив методов Array: ',
		arrArr,
		' Длина= ',
		arrArr.length,
		'<br>'
	)
	document.write(
		'Массив методов Math: ',
		arrMath,
		' Длина= ',
		arrMath.length,
		'<br>'
	)
}

// Задание 4
function myName() {
	let myFullName = 'Иванов Иван Иванович'
	let lengthOfFullName = myFullName.length
	let toUpperFullName = myFullName.toUpperCase()
	let toLowerFullName = myFullName.toLowerCase()
	let concatCaseName = toUpperFullName.concat(toLowerFullName)
	let replacedFullName = myFullName.replace(myFullName, 'И.И.И.')
	document.write('Исходная строка: ', myFullName, '<br>')
	document.write('Длина исходной строки= ', lengthOfFullName, '<br>')
	document.write('Верхний регистр: ', toUpperFullName, '<br>')
	document.write('Нижний регистр: ', toLowerFullName, '<br>')
	document.write('Объединённые строки: ', concatCaseName, '<br>')
	document.write('Заменённая исходная строка: ', replacedFullName, '<br>')
}

// Задание 5
function tableTime() {
	let nowDate = new Date()
	document.write(
		'<table border="1" style="text-align: center; background: rgb(255,0,0);">'
	)
	document.write('<tr>')
	document.write(
		'<td style="font-size: 40px;">',
		'Год',
		'</td>',
		'<td style="font-weight: 900; background-color: aqua;">',
		nowDate.getFullYear(),
		'</td>'
	)
	document.write('</tr>')
	document.write('<tr>')
	document.write(
		'<td style="font-size: 40px;">',
		'Месяц',
		'</td>',
		'<td style="font-weight: 900; background-color: aqua;">',
		nowDate.getMonth(),
		'</td>'
	)
	document.write('</tr>')
	document.write('<tr>')
	document.write(
		'<td style="font-size: 40px;">',
		'День',
		'</td>',
		'<td style="font-weight: 900; background-color: aqua;">',
		nowDate.getDay(),
		'</td>'
	)
	document.write('</tr>')
	document.write('<tr>')
	document.write(
		'<td style="font-size: 40px;">',
		'Час',
		'</td>',
		'<td style="font-weight: 900; background-color: aqua;">',
		nowDate.getHours(),
		'</td>'
	)
	document.write('</tr>')
	document.write('<tr>')
	document.write(
		'<td style="font-size: 40px;">',
		'Минуты',
		'</td>',
		'<td style="font-weight: 900; background-color: aqua;">',
		nowDate.getMinutes(),
		'</td>'
	)
	document.write('</tr>')
	document.write('<tr>')
	document.write(
		'<td style="font-size: 40px;">',
		'Секунды',
		'</td>',
		'<td style="font-weight: 900; background-color: aqua;">',
		nowDate.getSeconds(),
		'</td>'
	)
	document.write('</tr>')
	document.write('</table>')
}
