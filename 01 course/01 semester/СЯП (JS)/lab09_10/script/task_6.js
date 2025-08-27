document.getElementById('nav').onmouseover = function (e) {
	//отслеживает нахождение мыши внутри блока
	let target = e.target
	//отслеживаем клик
	if (target.className === 'menu-item') {
		let s = target.getElementsByClassName('submenu')
		closeMenu()
		for (let i = 0; i < s.length; i++) s[i].style.display = 'block'
		//массив, содержащий ложное меню
	}
}

document.onmouseover = function (e) {
	let target = e.target
	if (target.className != 'menu-item' && target.className != 'submenu') {
		closeMenu()
	}
}
function closeMenu() {
	//получение всех элементов подменю в блоке nav, присваивание display: none
	let subm = document.getElementsByClassName('submenu')
	for (let i = 0; i < subm.length; i++) {
		subm[i].style.display = 'none'
	}
}

// $(document).ready(() => {
// 	$('#nav').on('mouseover', '.menu-item', function (e) {
// 		let target = e.target
// 		if ($(target).hasClass('menu-item')) {
// 			let s = $(target).find('.submenu')
// 			closeMenu()
// 			s.css('display', 'block')
// 		}
// 	})

// 	$(document).on('mouseover', function (e) {
// 		let target = e.target
// 		if (!$(target).hasClass('menu-item') && !$(target).hasClass('submenu')) {
// 			closeMenu()
// 		}
// 	})

// 	function closeMenu() {
// 		let subm = $('.submenu')
// 		subm.css('display', 'none')
// 	}
// })
