'use strict'

function openwindow1() {
	openwindow1 = window.open('', '', 'width=400,height=300')
}
function openwindow2() {
	openwindow2 = window.open('', '', 'width=800,height=400')
}

function closewindow1() {
	openwindow1.close()
}
function closewindow2() {
	openwindow2.close()
}
let writeInWindow1 = () => {
	openwindow1.document.write(
		'<h1>' +
			'Информация о браузере: ' +
			openwindow1.navigator.userAgent +
			'</h1>' +
			'<br>'
	)
	openwindow2.document.write(
		'<h1>' +
			'Информация о браузере: ' +
			openwindow2.navigator.userAgent +
			'</h1>' +
			'<br>'
	)
}
