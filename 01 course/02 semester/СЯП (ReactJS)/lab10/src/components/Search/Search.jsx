import React, { useState } from 'react'
import { useDispatch, useSelector } from 'react-redux'
import { addCartItem } from '../redux/actions'
import './Search.css'

const Search = ({ products }) => {
	const [searchProduct, setSearchTerm] = useState('')
	const [searchMode, setSearchMode] = useState('partial')
	const dispatch = useDispatch()

	// обработчик изменения текстового поля для ввода поискового запроса
	const handleSearchChange = e => {
		setSearchTerm(e.target.value)
	}

	// обработчик изменения выбора режима поиска
	const handleModeChange = e => {
		setSearchMode(e.target.value)
	}

	// Функция фильтрации списка товаров в соответствии с введенным запросом и выбранным режимом поиска
	const filterProducts = () => {
		if (searchProduct.trim() === '') {
			return products
		}
		return products.filter(product => {
			const name = product.name.toLowerCase()
			const term = searchProduct.toLowerCase()

			if (searchMode === 'exact') {
				return name === term
			} else {
				return name.includes(term)
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
		<div className='search-content'>
			<h1>Поиск товаров</h1>
			<div className='search-container'>
				<input
					type='text'
					className='search-input'
					value={searchProduct}
					onChange={handleSearchChange}
					placeholder='Введите поисковый запрос...'
				/>
				<select
					className='search-select'
					value={searchMode}
					onChange={handleModeChange}
				>
					<option value='partial'>Частичное совпадение</option>
					<option value='exact'>Точное совпадение</option>
				</select>
				<table className='search-table'>
					<thead>
						<tr>
							<th>Наименование</th>
							<th>Стоимость</th>
							<th>Количество</th>
							<th>Скидка</th>
						</tr>
					</thead>
					<tbody>
						{filterProducts().map((product, index) => (
							<tr key={index}>
								<td>{product.name}</td>
								<td>{product.price}</td>
								<td>{product.quantity}</td>
								<td>{product.discount}%</td>
								<td>
									<button
										className='addToCart'
										onClick={() => addToCart(product)}
									>
										{cart.some(item => item.id === product.id)
											? 'Добавлено'
											: 'Добавить в корзину'}
									</button>
								</td>
							</tr>
						))}
					</tbody>
				</table>
			</div>
		</div>
	)
}

export default Search
