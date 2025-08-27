import React from 'react'
import { BrowserRouter as Router, Route, Routes, Link } from 'react-router-dom'
import Catalog from './Catalog'
import SortTable from './SortTable'
import Homepage from './Homepage'
import products from './products'
import './styles.css'

function App() {
	return (
		<Router basename='/Table-and-Catalog'>
			<div className='content'>
				<nav className='navbar'>
					<ul className='nav-menu'>
						<li className='nav-item'>
							<Link className='nav-link' to='/'>
								Домашняя страница
							</Link>
						</li>
						<li className='nav-item'>
							<Link className='nav-link' to='/sort'>
								Таблица
							</Link>
						</li>
						<li className='nav-item'>
							<Link className='nav-link' to='/catalog'>
								Каталог
							</Link>
						</li>
					</ul>
				</nav>
				<Routes>
					<Route path='/' element={<Homepage />} />
					<Route path='/sort' element={<SortTable products={products} />} />
					<Route path='/catalog' element={<Catalog products={products} />} />
				</Routes>
			</div>
		</Router>
	)
}

export default App
