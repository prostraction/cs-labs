import { Button } from 'bootstrap';
import Shop from './Shop'
import ShopList from './ShopList'
import React, {Component, Fragment} from "react";
import { Link } from "react-router-dom";


export default class ShopOrderSell extends ShopList {
    constructor(props) {
        super(props);
        this.handlePostChange = this.handlePostChange.bind(this);
        
        this.state = {

        }
    }

    handlePostChange(posts) {
        this.props.handlePostChange(posts);
    }


    componentDidMount() {
        const id = window.location.href.split('/')[4];
        fetch('http://localhost:4000/shops/' + id)
        .then((response) => response.json())
        .then((json) => {
            this.setState({
                shop: json.shop,
                isLoaded: true,
            });
            console.log(json);
        });

        fetch('http://localhost:4000/shops/' + id + '/orders/sell')
        .then((response) => response.json())
        .then((json) => {
            this.setState({
                orders: json.orders,
                isLoaded: true,
            });
            console.log('http://localhost:4000/shops/' + id + '/orders/sell')
            console.log(json);
        });
    }

    render() {
        const {shop, isLoaded} = this.state;
        const id = window.location.href.split('/')[4];
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

                    <h3 class="h3 text-center">Продажи</h3>



                    <br></br>

                    <h3 class="h3 text-center">Заказы</h3>
                        <table class="table">
                            <thead class="table-dark">
                                <tr>
                                    <th scope="col">Открыто на покупку</th>
                                    <th scope="col">Открыто на продажу</th>
                                    <th scope="col">Закрыто на покупку</th>
                                    <th scope="col">Закрыто на продажу</th>
                                </tr>
                            </thead>
                            <tbody>
                                <tr>
                                    <th scope="col">100</th>
                                    <th scope="col">12</th>
                                    <th scope="col">666</th>
                                    <th scope="col">666</th>
                                </tr>
                            </tbody>
                        </table>


                </div>
            )
        }
    }
}