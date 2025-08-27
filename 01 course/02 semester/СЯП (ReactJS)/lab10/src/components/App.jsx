import React from 'react'
import { BrowserRouter as Router, Route, Routes, Link } from 'react-router-dom'

import Homepage from './Homepage/Homepage'
import Catalog from './Catalog/Catalog'
import SortTable from './SortTable/SortTable'
import Search from './Search/Search'
import OrderForm from './OrderForm/OrderForm'
import products from './products'
import './App.css'

function App() {
	return (
		<Router basename='/Catalog'>
			<div className='content'>
				<nav className='navbar'>
					<ul className='nav-menu'>
						<li className='nav-item'>
							<Link className='nav-link' to='/'>
								Домашняя страница
							</Link>
						</li>
						<li className='nav-item'>
							<Link className='nav-link' to='/catalog'>
								Каталог
							</Link>
						</li>
						<li className='nav-item'>
							<Link className='nav-link' to='/table'>
								Таблица
							</Link>
						</li>
						<li className='nav-item'>
							<Link className='nav-link' to='/search'>
								Поиск
							</Link>
						</li>
						<li className='nav-item'>
							<Link className='nav-link' to='/order'>
								Корзина
							</Link>
						</li>
					</ul>
				</nav>
				<Routes>
					<Route path='/' element={<Homepage />} />
					<Route path='/catalog' element={<Catalog products={products} />} />
					<Route path='/table' element={<SortTable products={products} />} />
					<Route path='/search' element={<Search products={products} />} />
					<Route path='/order' element={<OrderForm />} />
				</Routes>
			</div>
		</Router>
	)
}

export default App
