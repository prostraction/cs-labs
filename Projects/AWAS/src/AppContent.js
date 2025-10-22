import React, {Component} from "react";

export default class AppContent extends Component {
    constructor(props) {
        super(props);
        this.handlePostChange = this.handlePostChange.bind(this);
    }

    handlePostChange(posts) {
        this.props.handlePostChange(posts);
    }


    fetchList = () => {
        fetch('https://jsonplaceholder.typicode.com/posts')
        .then((response) => response.json())
        .then(json => {
            console.log(json);
            this.handlePostChange(json);
            //let posts = document.getElementById("post-list");
            //const posts = this.listRef.current;

            //json.forEach(function(obj) {
            //    let li = document.createElement("li");
            //    li.appendChild(document.createTextNode(obj.id + " " + obj.title));
            //    posts.appendChild(li)
            //})
            
        })
    }

    clickedItem = (x) => {
        console.log("clicked", x)
    }

    // <ul id="post-list" ref={this.listRef}></ul>
    render() {
        return (
            <div>
            <div>
            <p>
                Content!<br/> <button onClick = {this.fetchList} className="btn btn-primary" href='#'>Button!</button>
            </p>
            </div>

            <p>{this.props.posts.length}</p>

            <div>
                <ul id="post-list">
                    {this.props.posts.map((c) => (
                    <li key = {c.id}> 
                        <a href="#!" onClick={() => this.clickedItem(c.id)}>
                            {c.title}
                        </a>
                    </li>))}
                </ul>
            </div>
            </div>
        );
    }
}