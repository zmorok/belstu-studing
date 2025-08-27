import React from 'react'

class Productcatalog extends React.Component {
	constructor(props) {
		super(props)
		this.state = {
			products: props.products,
			sortedBy: null,
			sortDirection: 'asc',
		}
	}

	sortBy = columnIndex => {
		const { products, sortedBy, sortDirection } = this.state
		let newSortDirection = 'asc'
		if (sortedBy === columnIndex) {
			newSortDirection = sortDirection === 'asc' ? 'desc' : 'asc'
		}
		const sortedProducts = [...products].sort((a, b) => {
			const aValue = a[Object.keys(a)[columnIndex]]
			const bValue = b[Object.keys(b)[columnIndex]]

			if (aValue < bValue) {
				return newSortDirection === 'asc' ? -1 : 1
			} else if (aValue > bValue) {
				return newSortDirection === 'asc' ? 1 : -1
			} else {
				return 0
			}
		})

		this.setState({
			products: sortedProducts,
			sortedBy: columnIndex,
			sortDirection: newSortDirection,
		})
	}

	getTotalPrice() {
		return this.state.products.reduce(
			(total, product) => total + product.price,
			0
		)
	}

	getTotalQuantity() {
		return this.state.products.reduce(
			(total, product) => total + product.quantity,
			0
		)
	}

	render() {
		return (
			<div>
				<table border='1'>
					<thead>
						<tr>
							<th onClick={() => this.sortBy(0)}>Row #</th>
							<th onClick={() => this.sortBy(0)}>Name</th>
							<th onClick={() => this.sortBy(1)}>Price</th>
							<th onClick={() => this.sortBy(2)}>Quantity</th>
						</tr>
					</thead>
					<tbody>
						{this.state.products.map((product, index) => (
							<tr key={index}>
								<td>{index + 1}</td>
								<td>{product.name}</td>
								<td>{product.price}</td>
								<td
									className={
										product.quantity < 3
											? 'low-stock'
											: product.quantity === 0
											? 'out-of-stock'
											: ''
									}
									style={{
										color:
											product.quantity === 0
												? 'red'
												: product.quantity < 3
												? 'yellow'
												: 'black',
									}}
								>
									{product.quantity}
								</td>
							</tr>
						))}
					</tbody>
					<tfoot>
						<tr>
							<td></td>
							<td>Total</td>
							<td>{this.getTotalPrice()}</td>
							<td>{this.getTotalQuantity()}</td>
						</tr>
					</tfoot>
				</table>
			</div>
		)
	}
}

export default Productcatalog
