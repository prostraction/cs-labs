import React, {Component, Fragment} from "react";
import {Link} from 'react-router-dom'

export default class ShopList extends Component {
    constructor(props) {
        super(props);
        this.state = {

        }
    }

    updateID (value) {
        this.setState({id: value});
    }

    componentDidMount() {
        fetch('http://localhost:4000/shops')
        .then((response) => response.json())
        .then((json) => {
            this.setState({
                shops: json.shops,
                isLoaded: true,
            });
        })
    }

    render() {
        const {shops, isLoaded} = this.state;
        if (!isLoaded) {
            return <p>Loading...</p>;
        } else {
            return (
                <Fragment>
                    <h2 className="h2 text-center"><b>Список магазинов</b></h2>
                    <div>
                        <ul>
                        {shops.map((c) => 
                            <li key={c.id} className="h3">
                                <Link to={`/shops/${c.id}`} onClick = {() => this.updateID(c.id)}>{c.name}</Link>
                            </li>
                        )}
                        </ul>
                    </div>
                </Fragment>
            )
        }
    }
}