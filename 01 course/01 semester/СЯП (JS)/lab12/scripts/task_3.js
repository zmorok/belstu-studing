let proto = {
	name: 'Иван',
	surname: 'Иванов',
	lastname: 'Иванович',
	printInfo: function () {
		alert(
			`Здравствуйте! Вас зовут ${this.surname} ${this.name} ${this.lastname}.`
		)
	},
}

let proto2 = Object.create(proto)

proto2.name = 'Алексей'
proto2.printInfo = function () {
	alert(1)
}

$(document).ready(() => {
	$('#task_3').on('click', () => {
		proto.printInfo()

		proto2.printInfo()
	})
})

console.group('Задание 3 (proto, proto2)')
console.group('Свойства и методы proto: ')
for (let prop in proto) {
	console.log(`${prop}: ${proto[prop]}`)
}
console.groupEnd()
console.group('Свойства и методы proto2: ')
for (let prop in proto2) {
	console.log(`${prop}: ${proto2[prop]}`)
}
console.groupEnd()
console.groupEnd()
