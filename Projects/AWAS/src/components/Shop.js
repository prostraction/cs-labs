import { Button } from 'bootstrap';
import ShopList from './ShopList'
import React, {Component} from "react";
import { Link } from "react-router-dom";


export default class Shop extends ShopList {
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
        })
    }

    render() {
        const {shop, isLoaded} = this.state;
        const id = window.location.href.split('/')[4];
        if (!isLoaded) {
            return <p></p>;
        } else {
            return (
                <div>
                    <h2 className="h2 text-center"><b>Магазин {shop.name}</b></h2>

                    <br></br>

                    <div>
                        <h3 className="h3 text-center">Действия</h3>
                        <p></p>
                        <div className="btn-toolbar center justify-content-center">
                                <Link to={`/shops/`+ id +`/orders/buy`}><button type="button" className="btn btn-outline-primary mx-2">Закупки</button></Link>
                                <Link to={`/shops/`+ id +`/orders/sell`}><button type="button" className="btn btn-outline-primary mx-2">Продажи</button></Link>
                                <button type="button" className="btn btn-outline-primary mx-2">Склад</button>
                                <button type="button" className="btn btn-outline-primary mx-2">Средства</button>
                        </div>
                    </div>

                </div>
            )
        }
    }
}