'use strict'

//задание 1
function Gruppa() {
	this.students = []
	this.newStudents = []
}
Gruppa.prototype.add_stud = function (student) {
	this.students.push(student)
	this.newStudents.push(student)
}
Gruppa.prototype.sub_stud = function (k) {
	this.newStudents.splice(this.students.length - k)
}

let gr1 = new Gruppa()
let gr2 = new Gruppa()
let gr3 = new Gruppa()

for (let counter_1 = 1; counter_1 <= 6; counter_1++) {
	gr1.add_stud(counter_1)
}
for (let counter_2 = 1; counter_2 <= 12; counter_2++) {
	gr2.add_stud(counter_2)
}
for (let counter_3 = 1; counter_3 <= 10; counter_3++) {
	gr3.add_stud(counter_3)
}

Gruppa.prototype.outputInfo = function () {
	const gruppaContainer = document.getElementById('container')
	const gruppaDiv = document.createElement('div')
	gruppaDiv.classList.add('gruppa-info')
	gruppaDiv.innerHTML = `
    <h1 align="center">Задание 1</h1>
    <p>gr1 до отчисления=  ${gr1.students}</p>
    <p>gr2 до отчисления=  ${gr2.students}</p>
    <p>gr3 до отчисления=  ${gr3.students}</p><br>
    <p>gr1 после отчисления=  ${gr1.newStudents}</p>
    <p>gr2 после отчисления=  ${gr2.newStudents}</p>
    <p>gr3 после отчисления=  ${gr3.newStudents}</p><hr>`
	gruppaContainer.appendChild(gruppaDiv)
}

function taskOne() {
	gr1.sub_stud(3)
	gr2.sub_stud(4)
	gr3.sub_stud(5)
	gr1.outputInfo()
}

//задание 2
function Car(brand, model, year, color, price) {
	this.brand = brand
	this.model = model
	this.year = year
	this.color = color
	this.price = price
}

Car.prototype.power = this.power // добавляем свойство "power" к конструктору Car

let car1 = new Car('Lamborghini', 'Centenario', 2016, 'Серый', '$ 1.900.000')
let car2 = new Car('BMW', 'BMW X6 M Competition', 2019, 'Белый', '$ 142.400')
let car3 = new Car('Alfa Romeo', 'Alfa Romeo 4C', 2018, 'Красный', '$ 80.000')
let carInfo = new Car()

//задаем значения свойству "power" для каждого экземпляра
car1.power = 770
car2.power = 525
car3.power = 240

Car.prototype.outputInfo = function () {
	const carInfoContainer = document.getElementById('container')
	const carInfoDiv = document.createElement('div')
	carInfoDiv.classList.add('car-info')
	carInfoDiv.innerHTML = `
    <h1 align="center">Задание 2</h1>
    <ol>
        <li>${car1.brand}
        <ul>
            <li>Бренд: ${car1.brand}</li>
            <li>Модель: ${car1.model}</li>
            <li>Год выпуска: ${car1.year}</li>
            <li>Цвет: ${car1.color}</li>
            <li>Цена: ${car1.price}</li>
            <li>Мощность: ${car1.power}</li>
        </ul></li>
        <li>${car2.brand}
        <ul>
            <li>Бренд: ${car2.brand}</li>
            <li>Модель: ${car2.model}</li>
            <li>Год выпуска: ${car2.year}</li>
            <li>Цвет: ${car2.color}</li>
            <li>Цена: ${car2.price}</li>
            <li>Мощность: ${car2.power}</li>
        </ul></li>
        <li>${car3.brand}
        <ul>
            <li>Бренд: ${car3.brand}</li>
            <li>Модель: ${car3.model}</li>
            <li>Год выпуска: ${car3.year}</li>
            <li>Цвет: ${car3.color}</li>
            <li>Цена: ${car3.price}</li>
            <li>Мощность: ${car3.power}</li>
        </ul></li>
    </ol><hr>
    `
	carInfoContainer.appendChild(carInfoDiv)
}

function taskTwo() {
	carInfo.outputInfo()
}

//задание 3
let arrayBefore = [1, 23, 'Pavel', 123.324]
let arrayAfter = arrayBefore.slice(0) // создаем копию массива arrayBefore через метод slice(0)
delete arrayAfter[2]

function returningFive() {
	return 5
}

function outputInfoOfTaskThree() {
	const taskThreeContainer = document.getElementById('container')
	const taskTheeDiv = document.createElement('div')
	taskTheeDiv.classList.add('taskThree-info')
	taskTheeDiv.innerHTML = `
    <h1 align="center">Задание 3</h1>
    <p>1. Начальный массив (arrayBefore)= ${arrayBefore}</p>
    <p>2. Конечный массив (arrayAfter)= ${arrayAfter}</p>
    <p>3. Есть ли второй элемент в массиве arrayBefore (2 in arrayBefore)= ${
			2 in arrayBefore
		}</p>
    <p>4. Есть ли второй элемент в массиве arrayAfter (2 in arrayAfter)= ${
			2 in arrayAfter
		}</p>
    <p>5. Проверяет наличие свойства mileage у конструктора Car ("mileage" in car1)= ${
			'mileage' in car1
		}</p>
    <p>6. Проверяет наличие свойства students у экземпляра gr1 ("students" in gr1)= ${
			'students' in gr1
		}</p>
    <p>7. Проверяет создан ли gr1 из Array (gr1 instanceof Array)= ${
			gr1 instanceof Array
		}</p>
    <p>8. Проверяет создан ли car1 из Object (car1 instanceof Object)= ${
			car1 instanceof Object
		}</p>
    <p>9. Проверяет создан ли car1 из Car (car1 instanceof Car)= ${
			car1 instanceof Car
		}</p>
    <caption>10. Проверка типов разных объктов:</caption>
    <ul>
        <li>returningFive=  ${typeof returningFive}</li>
        <li>gr1=  ${typeof gr1}</li>
        <li>Gruppa=  ${typeof Gruppa}</li>
        <li>arrayAfter=  ${typeof arrayAfter}</li>
        <li>car1=  ${typeof car1}</li>
    </ul><hr>
    `
	taskThreeContainer.appendChild(taskTheeDiv)
}
