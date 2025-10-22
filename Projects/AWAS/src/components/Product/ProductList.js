import React, {Component, Fragment} from "react";
import { Link } from "react-router-dom";

export default class ProductList extends Component {

    render() {
        return (
            <div>
                <h2 className="h2 text-center"><b>Продукция</b></h2>
                
                <br></br>

                <div>
                    <h3 className="h3 text-center">Действия</h3>
                    <p></p>
                    <div className="btn-toolbar center justify-content-center">
                            <button type="button" className="btn btn-outline-primary mx-2">1</button>
                            <button type="button" className="btn btn-outline-primary mx-2">2</button>
                            <button type="button" className="btn btn-outline-primary mx-2">3</button>
                            <button type="button" className="btn btn-outline-primary mx-2">4</button>
                    </div>
                </div>

            </div>
        )
    }
}