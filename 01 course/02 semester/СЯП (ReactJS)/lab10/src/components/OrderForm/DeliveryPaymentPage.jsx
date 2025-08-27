// DeliveryPaymentPage.js

import React from 'react'
import { connect } from 'react-redux'
import {
	setDeliveryMethod,
	setPaymentMethod,
	setDeliveryAddress,
	confirmOrder,
} from '../redux/actions'
import ConfirmedOrders from './ConfirmedOrders'
import './DeliveryPaymentPage.css'

const DeliveryPaymentPage = ({
	deliveryMethod,
	paymentMethod,
	deliveryAddress,
	setDeliveryMethod,
	setPaymentMethod,
	setDeliveryAddress,
	confirmOrder,
	cart,
	orders,
	totalCartPrice,
}) => {
	// обработчик подтверждения заказа
	const handleConfirmOrder = () => {
		const order = {
			deliveryMethod,
			paymentMethod,
			deliveryAddress,
			totalPrice: cart.reduce((total, item) => total + item.totalPrice, 0),
		}

		if (!deliveryAddress && deliveryMethod !== 'pickup') {
			alert('Введите адресс доставки.')
			return
		}

		if (
			cart.length === 0 ||
			!cart.find(item => (item.selected ? true : false))
		) {
			alert('Ваша корзина пуста или товары не выбраны.')
			return
		}

		const orderWithCart = { ...order, cart }
		confirmOrder(orderWithCart)

		setDeliveryMethod('courier')
		setPaymentMethod('cash')
		setDeliveryAddress('')
	}

	return (
		<div className='order-second'>
			<div className='delivery-method'>
				<h2>Выберите способ доставки</h2>
				<input
					type='radio'
					value='courier'
					checked={deliveryMethod === 'courier'}
					onChange={() => setDeliveryMethod('courier')}
				/>{' '}
				<label>Курьером</label>
				<br />
				<input
					type='radio'
					value='mail'
					checked={deliveryMethod === 'mail'}
					onChange={() => setDeliveryMethod('mail')}
				/>{' '}
				<label>Почтой</label>
				<br />
				<input
					type='radio'
					value='pickup'
					checked={deliveryMethod === 'pickup'}
					onChange={() => setDeliveryMethod('pickup')}
				/>{' '}
				<label>Самовывоз</label>
				<br />
				{deliveryMethod !== 'pickup' && (
					<div>
						<h2>Адрес доставки</h2>
						<input
							type='text'
							value={deliveryAddress}
							onChange={e => setDeliveryAddress(e.target.value)}
						/>
					</div>
				)}
			</div>
			<div className='payment-method'>
				<h2>Выберите способ оплаты</h2>
				<input
					type='radio'
					value='cash'
					checked={paymentMethod === 'cash'}
					onChange={() => setPaymentMethod('cash')}
				/>{' '}
				<label>Наличными</label>
				<br />
				<input
					type='radio'
					value='card'
					checked={paymentMethod === 'card'}
					onChange={() => setPaymentMethod('card')}
				/>{' '}
				<label>Банковской картой</label>
				<br />
				<input
					type='radio'
					value='bankTransfer'
					checked={paymentMethod === 'bankTransfer'}
					onChange={() => setPaymentMethod('bankTransfer')}
				/>{' '}
				<label>Банковским переводом</label>
				<br />
			</div>
			<button className='submit-btn' onClick={handleConfirmOrder}>
				Подтвердить заказ
			</button>
			<ConfirmedOrders orders={orders} totalCartPrice={totalCartPrice} />
		</div>
	)
}

const mapStateToProps = state => ({
	deliveryMethod: state.deliveryMethod,
	paymentMethod: state.paymentMethod,
	deliveryAddress: state.deliveryAddress,
	cart: state.cart,
	orders: state.orders,
	totalCartPrice: state.totalCartPrice,
})

const mapDispatchToProps = dispatch => ({
	setDeliveryMethod: method => dispatch(setDeliveryMethod(method)),
	setPaymentMethod: method => dispatch(setPaymentMethod(method)),
	setDeliveryAddress: address => dispatch(setDeliveryAddress(address)),
	confirmOrder: order => dispatch(confirmOrder(order)),
})

export default connect(mapStateToProps, mapDispatchToProps)(DeliveryPaymentPage)
