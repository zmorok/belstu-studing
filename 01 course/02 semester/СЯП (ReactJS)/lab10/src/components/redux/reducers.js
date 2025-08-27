// reducers.js
import { combineReducers } from 'redux'
import {
	NEXT_PAGE,
	PREV_PAGE,
	ADD_CART_ITEM,
	UPDATE_CART_ITEM,
	REMOVE_CART_ITEM,
	SET_DELIVERY_METHOD,
	SET_PAYMENT_METHOD,
	SET_DELIVERY_ADDRESS,
	CONFIRM_ORDER,
	UPDATE_TOTAL_CART_PRICE,
} from './actions'

const currentPageReducer = (state = 1, action) => {
	switch (action.type) {
		case NEXT_PAGE:
			return state + 1
		case PREV_PAGE:
			return state - 1
		default:
			return state
	}
}

const cartReducer = (state = [], action) => {
	switch (action.type) {
		case UPDATE_CART_ITEM: {
			return state.map(item =>
				item.id === action.payload.id
					? { ...item, ...action.payload.data }
					: item
			)
		}
		case ADD_CART_ITEM: {
			return [...state, action.payload]
		}
		case REMOVE_CART_ITEM: {
			return state.filter(item => item.id !== action.payload.id)
		}
		case CONFIRM_ORDER:
			return []
		default: {
			return state
		}
	}
}

const deliveryMethodReducer = (state = 'courier', action) => {
	switch (action.type) {
		case SET_DELIVERY_METHOD:
			return action.payload.method
		default:
			return state
	}
}

const paymentMethodReducer = (state = 'cash', action) => {
	switch (action.type) {
		case SET_PAYMENT_METHOD:
			return action.payload.method
		default:
			return state
	}
}

const deliveryAddressReducer = (state = '', action) => {
	switch (action.type) {
		case SET_DELIVERY_ADDRESS:
			return action.payload.address
		default:
			return state
	}
}

const ordersReducer = (state = [], action) => {
	switch (action.type) {
		case CONFIRM_ORDER:
			return [...state, action.payload]
		default:
			return state
	}
}

const totalCartPriceReducer = (state = 0, action) => {
	switch (action.type) {
		case UPDATE_TOTAL_CART_PRICE:
			return action.payload
		default:
			return state
	}
}

const rootReducer = combineReducers({
	currentPage: currentPageReducer,
	cart: cartReducer,
	deliveryMethod: deliveryMethodReducer,
	paymentMethod: paymentMethodReducer,
	deliveryAddress: deliveryAddressReducer,
	orders: ordersReducer,
	totalCartPrice: totalCartPriceReducer,
})

export default rootReducer
