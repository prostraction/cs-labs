import React, {Component, Fragment} from "react";
import {Link} from 'react-router-dom'

export default class TrackNumber extends Component {

    state = {numbers: []};

    componentDidMount() {
        this.setState({
            numbers: [
                {id: 1, title: "0000000001"},
                {id: 2, title: "0000000002"},
                {id: 3, title: "0000000003"},
            ]
        })
    }

    render() {
        return (
            <Fragment>
                <h2>Track Number</h2>
                <ul>
                    {this.state.numbers.map((n) => 
                        <li key={n.id}>
                            <Link to={`/track-number/${n.id}`}>{n.title}</Link>
                        </li>
                    )}
                </ul>
            </Fragment>
        )
    }
}