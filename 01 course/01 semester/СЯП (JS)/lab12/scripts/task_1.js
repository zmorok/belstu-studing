const nums = new Map([
	['1', '+375291111111'],
	['2', '+375292222222'],
	['3', '+375293333333'],
	['4', '+375294444444'],
	['5', '+375295555555'],
])

console.group('Задание 1 (коллекция Map)')
console.log(nums)
console.groupEnd()

let phoneNumber = function (key) {
	if (nums.has(key)) {
		return nums.get(key)
	} else {
		return 'По этому ключу номера нет'
	}
}

$(document).ready(() => {
	$('#task_1').on('click', () => {
		alert('Сейчас вам надо будет ввести ключ для получения номера')
		let key = prompt('Введите ключ (1-5)')
		if (key !== null && key !== '') {
			alert(`Номер телефона по ключу ${key}: ` + phoneNumber(key))
		} else {
			alert('Ошибка')
		}
	})
})
