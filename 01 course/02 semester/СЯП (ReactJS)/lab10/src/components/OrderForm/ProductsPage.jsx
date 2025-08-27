// ProductsPage.jsx
import React from 'react'
import { connect } from 'react-redux'
import {
	updateCartItem,
	removeCartItem,
	updateTotalCartPrice,
} from '../redux/actions'
import { useState, useEffect } from 'react'
import products from '../products'
import './ProductsPage.css'

const ProductsPage = ({ cart, updateCartItem, removeCartItem }) => {
	const [quantityValues, setQuantityValues] = useState({}) // состояние для счётчиков товаров
	const [totalCartPrice, setTotalCartPrice] = useState(0) // состояние для цены корзины

	// вычисление суммы для каждого товара и общей суммы корзины
	useEffect(() => {
		let cartTotalPrice = 0
		cart.map(item => {
			const product = products.find(prod => prod.id === item.id)
			const quantity = item.selected ? quantityValues[item.id] || 1 : 0
			const totalPrice = quantity * product.price
			cartTotalPrice += totalPrice
			return { ...item, totalPrice }
		})
		setTotalCartPrice(cartTotalPrice)
		updateTotalCartPrice(cartTotalPrice)
	}, [cart, quantityValues, updateTotalCartPrice])

	// обработчик счётчиков количества товаров
	const handleQuantityChange = (id, quantity) => {
		const itemProd = products.find(prod => prod.id === id)
		if (quantity >= 1 && quantity <= itemProd.quantity) {
			setQuantityValues(prevState => ({
				...prevState,
				[id]: quantity,
			}))
			updateCartItem(id, { quantity, selected: true })
		}
	}

	const handleCheckboxChange = (id, selected) => {
		updateCartItem(id, { selected })
	}

	return (
		<div className='order-first'>
			<h1>Список товаров в корзине</h1>
			<div className='order-products-page'>
				{cart.map(item => (
					<div className='ordered-product' key={item.id}>
						<input
							type='checkbox'
							checked={item.selected}
							onChange={() => handleCheckboxChange(item.id, !item.selected)}
						/>
						<span>{item.name}</span>
						<div>
							<img src={item.image} alt={item.name} />
						</div>
						<input
							type='number'
							value={quantityValues[item.id] || 1}
							onChange={e =>
								handleQuantityChange(item.id, parseInt(e.target.value))
							}
						/>
						<span>
							Доступное кол-во{' '}
							{products.find(prod => prod.id === item.id).quantity}
						</span>
						<span>Сумма: {item.totalPrice}</span>
						<button onClick={() => removeCartItem(item.id)}>Удалить</button>
					</div>
				))}
			</div>
			<div className='total-cart-price'>
				Общая сумма корзины: {totalCartPrice}
			</div>
		</div>
	)
}

const mapStateToProps = state => ({
	cart: state.cart,
	totalCartPrice: state.totalCartPrice,
})

const mapDispatchToProps = dispatch => ({
	updateCartItem: (id, data) => dispatch(updateCartItem(id, data)),
	removeCartItem: id => dispatch(removeCartItem(id)),
	updateTotalCartPrice: price => dispatch(updateTotalCartPrice(price)),
})

export default connect(mapStateToProps, mapDispatchToProps)(ProductsPage)
