import React, {Component} from "react";
import { BrowserRouter as Rounter, Routes, Route, Link, useParams } from "react-router-dom";

import Shop from './components/Shop'
import ShopList from './components/ShopList'
import Admin from './components/Admin'
import TrackNumber from './components/TrackNumber'
import ProductList from './components/Product/ProductList'
import ShopOrderBuy from './components/ShopOrderBuy'
import ShopOrderSell from './components/ShopOrderSell'

import './App.css';

export default class App extends Component {
    render() {
        return (
            <Rounter>
            <div className="container">
                <div className="row">
                    <h1 className="mt-1">
                        Test
                    </h1>
                    <hr className="mb-3"></hr>
                </div>

                <div className="row">
                    <div className="col-md-2">
                        <nav>
                            <ul className="list-group">
                                <li className="list-group-item list-group-item-action">
                                    <Link to="/shops">Магазины</Link>
                                </li>
                                <li className="list-group-item list-group-item-action">
                                    <Link to="/products">Продукция</Link>
                                </li>
                            </ul>
                        </nav>
                    </div>

                    <div className="col-md-10">
                        <Routes>
                            <Route path='/shops' element={<ShopList/>}/>
                            <Route path='/shops/:id' element={<Shop/>}/>
                            <Route path='/admin' element={<Admin/>}/>
                            <Route path='/track-number/:id' element={<Number/>}/>
                            <Route path='/track-number' element={<TrackNumber/>}/>
                            <Route path='/products' element={<ProductList/>}/>
                            <Route path='/shops/:id/orders/buy' element={<ShopOrderBuy/>}/>
                            <Route path='/shops/:id/orders/sell' element={<ShopOrderSell/>}/>
                        </Routes>
                    </div>
                </div>
            </div>
            </Rounter>
            
        )
    }
}

function Number() {
    let {id} = useParams();
    return <h2>Fix me: Number id = {id} </h2>
}