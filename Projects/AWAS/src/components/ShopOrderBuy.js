import Shop from './Shop'
import ShopList from './ShopList'
import React, {Fragment} from "react";
import { Link } from "react-router-dom";


export default class ShopOrderBuy extends ShopList {
    componentDidMount() {
        const id = window.location.href.split('/')[4];
        fetch('http://localhost:4000/shops/' + id + '/orders/buy')
        .then((response) => response.json())
        .then((json) => {
            this.setState({
                orders: json.orders,
                isLoaded: true,
            });
        });
    }

    render() {
        const {orders, isLoaded} = this.state;
        if (!isLoaded) {
            return <p></p>;
        } else {
            return (
                <div>
                    <div>
                    <Fragment>
                        <Shop/> 
                    </Fragment>
                    </div>
                    <br></br>

                    <h3 className="h3 text-center">Закупки</h3>
                        <div className="btn-toolbar center justify-content-center">
                            <Link to={window.location.href + '#test'}>
                                <button type="button" className="btn btn-outline-secondary mx-2">Добавить заказ</button>
                            </Link>
                        </div>

                    <br></br>

                    <h3 className="h3 text-center">Просмотр заказов</h3>
                        <br></br>
                        <table className="table table-hover table-striped table-sm">
                            <thead className = "table-dark">
                                <tr>
                                    <th scope="col" key = "head_1">№</th>
                                    <th scope="col" key = "head_2">Время</th>
                                    <th scope="col" key = "head_3">Поставщик</th>
                                    <th scope="col" key = "head_4">Кол-во позиций</th>
                                    <th scope="col" key = "head_5">Общая цена</th>
                                    <th scope="col" key = "head_6">Статус</th>
                                    <th scope="col" key = "head_7">Удалить</th>
                                </tr>
                            </thead>
                            {orders.map((o) => (
                                <tbody className = "table">
                                    <tr onclick={console.log("clicked !!!!")}>
                                        <th scope="col" key = {o.id} onclick={console.log("clicked !!!!")}> {o.id}</th>
                                        <th scope="col" key = {o.time}> {o.time}</th>
                                        <th scope="col" key = {o.distrib}> {o.distrib.String}</th>
                                        <th scope="col" key = {o.count}> {o.count.Int64}</th>
                                        <th scope="col" key = {o.price}> {o.price.Float64}</th>
                                        <th scope="col" key = {o.status}> {o.status.String}</th>
                                        <th scope="col" key = {o.id + "_clicked"}> <Link to={window.location.href + '#test'}><button type="button" className="btn btn-danger btn-sm" style={{transform : 'scale(0.75)'}}>X</button></Link></th>
                                    </tr>
                                </tbody>
                            ))}
                        </table>
                </div>
            )
        }
    }
}