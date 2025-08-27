import React from 'react'
import ReactDOM from 'react-dom/client'
import Productcatalog from './components/Productcatalog'

const products = [
	{ name: 'Product 1', price: 10, quantity: 5 },
	{ name: 'Product 2', price: 20, quantity: 2 },
	{ name: 'Product 3', price: 15, quantity: 0 },
	{ name: 'Product 4', price: 25, quantity: 1 },
	{ name: 'Product 5', price: 55, quantity: 6 },
	{ name: 'Product 6', price: 25, quantity: 3 },
	{ name: 'Product 7', price: 50, quantity: 2 },
	{ name: 'Product 8', price: 45, quantity: 10 },
	{ name: 'Product 9', price: 23, quantity: 0 },
]

const root = ReactDOM.createRoot(document.getElementById('root'))
root.render(<Productcatalog products={products} />)
