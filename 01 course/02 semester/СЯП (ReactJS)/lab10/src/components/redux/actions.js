// actions.js
export const NEXT_PAGE = 'NEXT_PAGE'
export const PREV_PAGE = 'PREV_PAGE'
export const UPDATE_CART_ITEM = 'UPDATE_CART_ITEM'
export const ADD_CART_ITEM = 'ADD_CART_ITEM'
export const REMOVE_CART_ITEM = 'REMOVE_CART_ITEM'
export const SET_DELIVERY_METHOD = 'SET_DELIVERY_METHOD'
export const SET_PAYMENT_METHOD = 'SET_PAYMENT_METHOD'
export const SET_DELIVERY_ADDRESS = 'SET_DELIVERY_ADDRESS'
export const CONFIRM_ORDER = 'CONFIRM_ORDER'
export const SAVE_ORDER_DETAILS = 'SAVE_ORDER_DETAILS'
export const UPDATE_TOTAL_CART_PRICE = 'UPDATE_TOTAL_CART_PRICE'

export const nextPage = () => ({
	type: NEXT_PAGE,
})

export const prevPage = () => ({
	type: PREV_PAGE,
})

export const updateCartItem = (id, data) => ({
	type: UPDATE_CART_ITEM,
	payload: { id, data },
})

export const addCartItem = data => ({
	type: ADD_CART_ITEM,
	payload: data,
})

export const removeCartItem = id => ({
	type: REMOVE_CART_ITEM,
	payload: { id },
})

export const setDeliveryMethod = method => ({
	type: SET_DELIVERY_METHOD,
	payload: { method },
})

export const setPaymentMethod = method => ({
	type: SET_PAYMENT_METHOD,
	payload: { method },
})

export const setDeliveryAddress = address => ({
	type: SET_DELIVERY_ADDRESS,
	payload: { address },
})

export const confirmOrder = order => ({
	type: CONFIRM_ORDER,
	payload: order,
})

export const updateTotalCartPrice = totalPrice => ({
	type: UPDATE_TOTAL_CART_PRICE,
	payload: totalPrice,
})
