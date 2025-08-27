alert('Вас приветствует учебный центр')
function user_name() {
	let name = prompt('Введите имя:', 'Имя')
	alert('Добро пожаловать на наши курсы, ' + name)
}
function web_designer() {
	let choice1 = confirm('Хотите стать Web-дизайнером?')
	if (choice1 == true) {
		alert('Учите стили CSS и JavaScript!')
	} else {
		alert('Упускаете время!')
	}
}
function consol_1() {
	console.log(10 + 5)
	console.log('10' + '5')
	console.log(22 + '5')
	console.log('22' + 5)
	alert('Проверьте консоль через Ctrl+shift+C')
}
function formula() {
	let x = prompt('Введите x')
	let y = prompt('Введите y')
	let form_1 = (35 * y - 25 * x) / 5 + 232
	let form_2 = ((8 * y) / x + (5 * x) / y - 43) * 6
	alert('Первое выражение(35y-25x)/5+232)= ' + form_1)
	alert('Второе выражение(8*y/x+5*x/y - 43)*6)= ' + form_2)
	alert('Остаток от деления= ' + (form_1 % form_2))
}
function sravnenie_1() {
	let digit = prompt('Введите правильное число')
	if ((digit != 15 && digit < 20) || (digit > 40 && digit % 15 == 0)) {
		alert('Правильное значение')
	} else {
		alert('Неправильное значение')
	}
}
function sravnenie_2() {
	let number_1 = prompt('Введите десятичное число')
	if (number_1 <= 99 && number_1 >= 10) {
		alert('Вы ввели десятичное число')
	} else {
		alert('Число не десятичное')
	}
}
function sravnenie_3() {
	let num_1_3 = prompt('Введите первое число')
	let num_2_3 = prompt('Введите второе число')
	if (num_1_3 > num_2_3) {
		alert(
			'Первое: ' +
				num_1_3 +
				' больше Второго: ' +
				num_2_3 +
				' на ' +
				(num_1_3 - num_2_3)
		)
	} else if (num_1_3 == num_2_3) {
		alert('Оба числа равны: ' + num_1_3 + '==' + num_2_3)
	} else {
		alert(
			'Первое: ' +
				num_1_3 +
				' меньше Второго: ' +
				num_2_3 +
				' на ' +
				(num_2_3 - num_1_3)
		)
	}
}
function day_of_week() {
	let day = parseInt(prompt('Введите номер дня'))
	switch (day) {
		case 1:
			alert('Понедельник')
			break
		case 2:
			alert('Вторник')
			break
		case 3:
			alert('Среда')
			break
		case 4:
			alert('Четверг')
			break
		case 5:
			alert('Пятница')
			break
		case 6:
			alert('Суббота')
			break
		case 7:
			alert('Воскресенье')
			break
		default:
			alert('Введите номер дня от 1 до 7')
	}
}

function digitDif() {
	const numBin = 0b11
	const numHex = 0x3
	const numOct = 0o3
	console.log(numBin, numHex, numOct)
	alert('Числа выведены в консоль')
}

try {
	throw SyntaxError
} catch (e) {
	console.log(e)
}
