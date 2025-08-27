document.addEventListener('click', e => {
	const favImg = e.target.closest('.product-card_fav')
	if (!favImg) return

	favImg.classList.add('pop')
	favImg.addEventListener('animationend', () => {
		favImg.classList.remove('pop')
		favImg.removeEventListener('animationend')
	})

	const liked = favImg.dataset.liked === 'true'
	favImg.src = liked
		? 'assets/icons/heart/no.svg'
		: 'assets/icons/heart/yes.svg'
	favImg.dataset.liked = (!liked).toString()
})

document.addEventListener('click', e => {
	const scrollToTopBtn = e.target.closest('.scroll-top')
	if (!scrollToTopBtn) return
	window.scrollTo({
		top: 0,
		behavior: 'smooth',
	})
})

document.addEventListener('click', e => {
	const decBtn = e.target.closest('[data-qty-dec]')
	if (decBtn) {
		const container = decBtn.closest('[data-quantity]')
		const valSpan = container.querySelector('.qty-val')
		let val = parseInt(valSpan.textContent, 10) || 1
		if (val > 1) valSpan.textContent = val - 1
		return
	}

	const incBtn = e.target.closest('[data-qty-inc]')
	if (incBtn) {
		const container = incBtn.closest('[data-quantity]')
		const valSpan = container.querySelector('.qty-val')
		let val = parseInt(valSpan.textContent, 10) || 0
		valSpan.textContent = val + 1
		return
	}
})
