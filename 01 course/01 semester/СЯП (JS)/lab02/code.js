const multiplicationTable = () => {
	let a = prompt('Введите кол-во строк таблицы умножения')
	let b = prompt('Введите кол-во столбцов таблицы умножения')
	document.write(
		'<table class="table" border="1" bgcolor="#008080" style="text-align: center; color: yellow;">'
	)
	for (let i = 1; i <= a; i++) {
		document.write('<tr>')
		document.write('<td>' + i + '</td>')
		for (let k = 2; k <= b; k++) {
			document.write('<td>' + i * k + '</td>')
		}
		document.write('</tr>')
	}
	document.write('</table')
}
function areaLength() {
	let rad_1 = parseInt(prompt('Введите начальный радиус'))
	let rad_2 = parseInt(prompt('Введите конечный радиус'))
	document.write(
		'<table border="1" bgcolor="#008080" style="text-align: center; color: yellow;">'
	)
	document.write('<tr>')
	document.write('<th>' + 'Радиус' + '</th>')
	document.write('<th>' + 'Площадь круга' + '</th>')
	document.write('<th>' + 'Длина окружности' + '</th>')
	document.write('</tr>')
	do {
		document.write('<tr>')
		document.write('<td>' + rad_1.toFixed(1) + '</td>')
		document.write('<td>' + (Math.PI * rad_1 * rad_1).toFixed(3) + '</td>')
		document.write('<td>' + (Math.PI * 2 * rad_1).toFixed(3) + '</td>')
		document.write('</tr>')
		rad_1 += 0.3
	} while (rad_1 <= rad_2)
	document.write('</table>')
}
function twoNumbers() {
	let num_1 = parseFloat(prompt('Введите дробное число'))
	let num_2 = parseInt(prompt('Введите целое число'))
	document.write(
		'<table border="1" bgcolor="#008080" style="text-align: center; color: yellow;">'
	)
	document.write('<tr>')
	document.write('<th>' + 'Число' + '</th>')
	document.write('<th>' + 'Метод' + '</th>')
	document.write('<th>' + 'Результат' + '</th>')
	document.write('<th>' + 'Описание метода' + '</th>')
	document.write('</tr>')
	document.write('<tr>')
	document.write('<td>' + num_1 + '</td>')
	document.write('<td>' + 'toExponential(4)' + '</td>')
	document.write('<td>' + num_1.toExponential(4) + '</td>')
	document.write(
		'<td>' +
			'Экспоненциальная форма числа с 4-мя цифрами после запятой' +
			'</td>'
	)
	document.write('</tr>')

	document.write('<tr>')
	document.write('<td>' + num_1 + '</td>')
	document.write('<td>' + 'toFixed(1)' + '</td>')
	document.write('<td>' + num_1.toFixed(1) + '</td>')
	document.write(
		'<td>' + 'Число с фиксированным кол-вом цифр (1) после запятой' + '</td>'
	)
	document.write('</tr>')

	document.write('<tr>')
	document.write('<td>' + num_1 + '</td>')
	document.write('<td>' + 'toPrecision(2)' + '</td>')
	document.write('<td>' + num_1.toPrecision(2) + '</td>')
	document.write(
		'<td>' + 'Число с кол-вом цифр (2) до и после запятой' + '</td>'
	)
	document.write('</tr>')

	document.write('<tr>')
	document.write('<td>' + num_1 + '</td>')
	document.write('<td>' + 'toString(16)' + '</td>')
	document.write('<td>' + num_1.toString(16) + '</td>')
	document.write(
		'<td>' +
			'Строковое представление числа в 16-ричной системе счисления' +
			'</td>'
	)
	document.write('</tr>')
	// ---------------------------------------------------------------------------------------------------------------------------------------------
	document.write('<tr>')
	document.write('<td>' + num_2 + '</td>')
	document.write('<td>' + 'toExponential(4)' + '</td>')
	document.write('<td>' + num_2.toExponential(4) + '</td>')
	document.write(
		'<td>' +
			'Экспоненциальная форма числа с 4-мя цифрами после запятой' +
			'</td>'
	)
	document.write('</tr>')

	document.write('<tr>')
	document.write('<td>' + num_2 + '</td>')
	document.write('<td>' + 'toFixed(1)' + '</td>')
	document.write('<td>' + num_2.toFixed(1) + '</td>')
	document.write(
		'<td>' + 'Число с фиксированным кол-вом цифр (1) после запятой' + '</td>'
	)
	document.write('</tr>')

	document.write('<tr>')
	document.write('<td>' + num_2 + '</td>')
	document.write('<td>' + 'toPrecision(2)' + '</td>')
	document.write('<td>' + num_2.toPrecision(2) + '</td>')
	document.write(
		'<td>' + 'Число с кол-вом цифр (2) до и после запятой' + '</td>'
	)
	document.write('</tr>')

	document.write('<tr>')
	document.write('<td>' + num_2 + '</td>')
	document.write('<td>' + 'toString(16)' + '</td>')
	document.write('<td>' + num_2.toString(16) + '</td>')
	document.write(
		'<td>' +
			'Строковое представление числа в 16-ричной системе счисления' +
			'</td>'
	)
	document.write('</tr>')
	document.write('</table>')
}
