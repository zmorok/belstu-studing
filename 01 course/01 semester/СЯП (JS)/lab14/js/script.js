//задание 1
let obj1 = {}

obj1.name = 'Ivan'
obj1.surname = 'Ivanov'
obj1.lastname = 'Ivanovich'

let valuesKeys = Object.keys(obj1)
	.map(key => obj1[key])
	.join(' ')

let valuesValues = Object.values(obj1).join(' ')

let task1 = document.getElementById('task1')
task1.onclick = function () {
	document.getElementById('p1').innerHTML = `
    Через Object.keys() + .map : ${valuesKeys}
    <hr>
    Через перечисление свойств : 
    ${obj1.name} 
    ${obj1.surname}
    ${obj1.lastname}
    <hr>
    Через Object.values() : 
    ${valuesValues}
  `
}

//задание 2

let task2 = document.getElementById('task2')
task2.onclick = function () {
	let listOfNums = [1, 23, 345, 6789, 1023, 413, 453456, 2342, 34, 1442]
	document.getElementById('p2').style.textAlign = 'center'
	document.getElementById('p2').innerHTML = ''
	let text2 = ''

	listOfNums.forEach(num => (text2 += num + ' '))
	text2 += '<br>' + '<hr>'

	document.getElementById('p2').innerHTML = text2

	listOfNums.sort((a, b) => a - b)

	listOfNums.forEach(num => (text2 += num + ' '))
	text2 += '<br>' + '<hr>'

	document.getElementById('p2').innerHTML = text2

	listOfNums.map(num => {
		text2 += num.toString().padStart(10, '0').padEnd(15, '-') + '<br>'
	})

	document.getElementById('p2').innerHTML = text2 + '<hr>'
}

//задание 3
let myName = 'Иван'
myName = myName.padStart(myName.length + 3, '#')
let mySurname = 'Иванов'
let myAge = '18'
mySurname = mySurname.concat(myAge)

let stringFinal = myName.concat(' ' + mySurname)

let task3 = document.getElementById('task3')
task3.onclick = function () {
	document.getElementById('p3').style.textAlign = 'center'
	document.getElementById('p3').innerHTML = ''
	document.getElementById('p3').innerHTML += `
    <hr> ${stringFinal} <br> <hr>
  `
}

//задание 4
let task4 = document.getElementById('task4')
task4.onclick = function () {
	document.getElementById('p4').style.textAlign = 'center'
	document.getElementById('p4').innerHTML = ''

	let charArray = []
	for (let i = 0; i < Math.random() * 100; i++) {
		charArray.push(Math.random() < 0.5 ? '1' : '0')
	}

	charArray.sort((a, b) => b - a)

	charArray.push('*', ' ', ' ', ' ', ' ')
	document.getElementById('p4').innerHTML += charArray
}

//задание 6
let myObject = {}

Object.defineProperty(myObject, 'myProperty', {
	enumerable: true,
	configurable: true,
	get: function () {
		return this.value
	},
	set: function (newValue) {
		this.value = newValue + '*'
	},
})

myObject.myProperty = 'fer'

let description = Object.getOwnPropertyDescriptor(myObject, 'myProperty')

document.getElementById('task5').onclick = function () {
	console.log('Описание свойства myProperty: ', description)
	alert('Вывод в консоль.')
}
