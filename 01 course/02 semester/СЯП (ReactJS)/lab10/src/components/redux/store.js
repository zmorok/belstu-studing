// store.js
import { createStore } from 'redux'
import rootReducer from './reducers' // Импорт корневого редуктора

const store = createStore(rootReducer)

export default store
