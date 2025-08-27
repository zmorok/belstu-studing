import React, { useState } from 'react'
import { useDispatch, useSelector } from 'react-redux'
import { addCartItem } from '../redux/actions'
import './Catalog.css'

const Catalog = ({ products }) => {
	const [sortBy, setSortBy] = useState(null)
	const [sortOrder, setSortOrder] = useState('asc')
	const isNew = product => (product.isNew ? 'Новинка!!' : '')
	const dispatch = useDispatch()

	// функция высчитывания цены
	const renderPrice = product => {
		if (product.discount > 0) {
			const discountedPrice =
				product.price - (product.price * product.discount) / 100
			return (
				<>
					<div className='old-price'>Старая цена: {product.price}</div>
					<div className='discount'>
						Стоимость:
						{discountedPrice} <br></br>(скидка {product.discount}%)
					</div>
				</>
			)
		}
		return <div className='price'>Стоимость: {product.price}</div>
	}

	// функция изменения способа сортировки
	const sortProducts = criteria => {
		if (sortBy === criteria) {
			setSortOrder(sortOrder === 'asc' ? 'desc' : 'asc')
		} else {
			setSortBy(criteria)
			setSortOrder('asc')
		}
	}

	// сортировка массива продуктов по sortBy и sortOrder
	const sortedProducts = () => {
		return products.slice().sort((a, b) => {
			if (sortOrder === 'asc') {
				return a[sortBy] > b[sortBy] ? 1 : -1
			} else {
				return a[sortBy] < b[sortBy] ? 1 : -1
			}
		})
	}

	// добавление товара в корзину
	const cart = useSelector(state => state.cart)
	const addToCart = product => {
		const isInCart = cart.some(item => item.id === product.id)
		if (isInCart) {
			return
		}
		dispatch(addCartItem(product))
	}

	return (
		<div className='catalog-content'>
			<h1>Каталог товаров</h1>
			<div className='sort-buttons'>
				<button onClick={() => sortProducts('name')}>
					Сортировать по наименованию
				</button>
				<button onClick={() => sortProducts('price')}>
					Сортировать по стоимости
				</button>
				<button onClick={() => sortProducts('quantity')}>
					Сортировать по количеству
				</button>
				<button onClick={() => sortProducts('discount')}>
					Сортировать по скидке
				</button>
				<button onClick={() => sortProducts('isNew')}>
					Сортировать по новинке
				</button>
				<button onClick={() => sortProducts('weight')}>
					Сортировать по массе
				</button>
			</div>
			<div className='catalog'>
				{sortedProducts().map((product, index) => (
					<div
						style={{ background: product.isNew ? '#a8ce8f' : '' }}
						key={index}
						className='product'
					>
						<div className='new'>{isNew(product)}</div>
						<div className='name'>{product.name}</div>
						<div>{renderPrice(product)}</div>
						<div>Количество: {product.quantity}</div>
						<div>Масса: {product.weight} кг</div>
						<div>
							<img src={product.image} alt={product.name} />
						</div>
						<div className='description'>{product.description}</div>
						<button className='addToCart' onClick={() => addToCart(product)}>
							{cart.some(item => item.id === product.id)
								? 'Добавлено'
								: 'Добавить в корзину'}
						</button>
					</div>
				))}
			</div>
		</div>
	)
}

export default Catalog
