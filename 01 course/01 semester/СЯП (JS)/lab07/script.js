'use strict'

const divInfo = document.createElement('div')
divInfo.className = 'output-info'

function print() {
	let faculty = document.querySelector('textarea').value
	let surnameStud = document.getElementById('surname').value

	//возвращает course, который является значением выбранного радио
	let courses = document.querySelectorAll('input[name="course"]')
	let course
	for (let i of courses) {
		if (i.checked) {
			course = i.value
		}
	}

	//возвращает specialitySelected, который является выбранным значением option
	let speciality = document.getElementById('speciality')
	let specialitySelected = speciality.value

	//возвращает disciplinesSelected, который является массивом выбранных checkbox
	//для вывода строки ---- disciplinesSelected.join(', ');
	let disciplines = document.querySelectorAll('input[type="checkbox"]')
	let disciplinesSelected = []
	for (let i of disciplines) {
		if (i.checked) {
			disciplinesSelected.push(i.value)
		}
	}

	let text =
		'<p>' +
		faculty +
		'<br>' +
		' Студент ' +
		'<span>' +
		surnameStud +
		'</span>' +
		' специальность ' +
		'<span>' +
		specialitySelected +
		'</span>' +
		' курс ' +
		'<span>' +
		course +
		'</span>' +
		' должен сдавать следующие предметы:<br>' +
		'</p>'

	text += '<ul>'

	for (let k = 0; k < disciplinesSelected.length; k++) {
		text += '<li>' + '<span>' + disciplinesSelected[k] + '</span>' + '</li>'
	}

	text += '</ul>'

	let divCont = document.querySelector('body')
	divInfo.classList.add('info')
	divInfo.innerHTML = text
	divCont.appendChild(divInfo)

	let second_form = document.forms['second-form'].elements['second-select']
	let text_second = '<option disabled selected>Список предметов</option>'
	for (let i = 0; i < disciplinesSelected.length; i++) {
		text_second += `<option>${disciplinesSelected[i]}</option>`
		second_form.innerHTML = text_second
	}
}
function printt() {
	divInfo.innerHTML = ''
	print()
}
