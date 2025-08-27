function getRandomColor() {
	let letters = '0123456789ABCDEF'
	let color = '#'
	for (let i = 0; i < 6; i++) {
		color += letters[Math.floor(Math.random() * 16)]
	}
	return color
}

////task 1
$(document).ready(function () {
	$('#text').mouseover(function () {
		$(this).css('color', 'red')
	})

	$('#text').mouseout(function () {
		$(this).css('color', 'black')
	})
})

////task 2
$(document).ready(function () {
	$('#text').click(function () {
		if ($(this).css('font-size') === '30px') {
			$(this).css('font-size', '20px')
		} else {
			$(this).css('font-size', '30px')
		}
	})
})

////task 3
$(document).ready(function () {
	$('#currentImage').click(function () {
		if ($(this).attr('src').includes('images/3.jpg')) {
			$(this).attr('src', 'images/6.jpg')
		} else {
			$(this).attr('src', 'images/3.jpg')
		}
	})
})

////task 4
$(document).ready(function () {
	$('#photo-text').click(function () {
		if ($('#myText').css('display') === 'block') {
			$('#myText').css('display', 'none')
			$('#myImg').css('display', 'block')
		} else {
			$('#myText').css('display', 'block')
			$('#myImg').css('display', 'none')
		}
	})
})

////task 5
$(document).ready(function () {
	$('#imgFrom5').mouseover(function () {
		$(this).css('width', '450px')
		$(this).css('height', '450px')
	})
	$('#imgFrom5').mouseout(function () {
		$(this).css('width', '250px')
		$(this).css('height', '250px')
	})
})

////task 6
$(document).ready(function () {
	$('#coloredBorder').dblclick(function () {
		let color = getRandomColor()
		if ($(this).css('border-style') === 'none') {
			$(this).css('border', '5px dotted ' + color)
		} else {
			$(this).css('border', 'none')
		}
	})
})

////task 1
// const text = document.querySelector('#text')
// text.onmouseover = function () {
// 	this.style.color = 'red'
// }
// text.onmouseout = function () {
// 	this.style.color = 'black'
// }

////task 2
// text.onclick = function () {
// 	if (this.style.fontSize === '30px') {
// 		this.style.fontSize = '20px'
// 	} else {
// 		this.style.fontSize = '30px'
// 	}
// }

////task 3
// let img = document.getElementById('currentImage')
// img.onclick = function changeImage() {
// 	if (img.src.includes('images/3.jpg')) {
// 		img.src = 'images/6.jpg'
// 	} else {
// 		img.src = 'images/3.jpg'
// 	}
// }

////task 4
// let myText = document.getElementById('myText')
// let myImg = document.getElementById('myImg')

// function textToImageAndBack() {
// 	if (myText.style.display === 'block') {
// 		myText.style.display = 'none'
// 		myImg.style.display = 'block'
// 	} else {
// 		myText.style.display = 'block'
// 		myImg.style.display = 'none'
// 	}
// }
// document.querySelector('#photo-text').onclick = textToImageAndBack

////task 5
// let imgFrom5 = document.getElementById('imgFrom5')
// imgFrom5.onmouseover = function () {
// 	this.style.width = '500px'
// 	this.style.height = '500px'
// }
// imgFrom5.onmouseout = function () {
// 	this.style.width = '250px'
// 	this.style.height = '250px'
// }

////task 6
// let textBorder = document.getElementById('coloredBorder')
// textBorder.ondblclick = function () {
//   let color = getRandomColor()
// 	if (textBorder.style.border.includes('none')) {
//     textBorder.style.border = '5px dotted ' + color
// 	} else {
//     textBorder.style.border = 'none'
// 	}
// }
