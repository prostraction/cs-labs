import React from 'react';
import {createRoot} from 'react-dom/client'

import App from './App'

/*
class App extends Component {
  // every react component must have a render() function
  render() {
    return (
      <div className="app">
         <App/>
      </div>
    );
  }
}
*/
//ReactDOM.render(<App />, document.getElementById('root'));
const root = createRoot(document.getElementById('root'));
root.render(<App />);