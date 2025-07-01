<div align="center">
  <h2>
    🌐︎ Webserv (42 project)
  </h2>
  <p>
    <b><i>Basic HTTP server based on NGINX written in C++ with a sample webpage and CGI Scripts.</i></b>
  </p>
</div>

## Brief

Our server manages connections using a synchronous I/O multiplexing system. Sockets are kept alive for each incomming connection
and a timeout system ensures that they are closed if no activity occurs in the time given via config. Our config is rather flexible, allowing for precise location premissions and settings. CGI can be executed and configured by setting a CGI script directory. Timeout time and max body size can also be set for each server. Additionally, any amount of servers can be set up in a single config.

## Features

- _Socket Management:_
  We use `poll()` to manage incoming connections efficiently. Servers can handle any ammout of simultaneous requests without crashing.
- _HTTP Methods:_
  Currently the implemented methods are `GET`, `POST` and `DELETE`. They must be set explicitly for each location in the server root through the config file. `GET` sends back requested data to the client. `POST` uploads data from the client to the server's upload store (also set in the config). `DELETE` removes requested data from the server etirely. Obviously, these methods have safety measures.
- _Config Directives:_
  There are many configuration directives that can be used to specify various features of your servers. There are always defaults as well, to fall back on any missing directives. A detailed table of these directives is shown in the following section.
- _CGI Script Execution:_
  Our servers can handle CGI script execution which can be specified via the `cgi-path` directive. This will set the path that the server will use to identify CGI-specific requests.
- _Example Pages and Config:_
  The project comes with a default configuration file and an example website with multiple HTML pages and CSS styling. We made an upload page where you can easily test POST requests and a CGI testing page where input can be sent to a sample program that will process and respond through a script as HTML.
- _Verbose Logging:_
  We also made a rebust logging and debugging system that reports useful information back to the user during runtime, such as request information and errors.

## Usage

The program can be simply launched with ```./webserv``` and it will use the `default.conf` file if available. If you wish to write your own custom config, you can run it with ```./webserv [config file path]```.
The process will then run indefinately; displaying logs until it is closed by sending an interrupt signal with `CTRL+c`.

## Configuration

There can be any amount of server scopes and location scopes inside said servers. No directive is absolutely mandatory, as there are defaults for everything that will take effect. For example, I can not define the index and it will always default to `index.html`. If it does not exist, a 404 response will be sent.
Each scope is opened and closed with `{ }` and each directive is terminated with `;`.

Here is every available directive and scope in our configuration system:
<table border="1" cellpadding="6" cellspacing="0">
  <thead>
    <tr>
      <th>Keyword</th>
      <th>Type</th>
      <th>Fields</th>
      <th>Description</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>server</td>
      <td>scope</td>
      <td>N/A</td>
      <td>Defines a server to run upon execution.</td>
    </tr>
    <tr>
      <td>location</td>
      <td>scope</td>
      <td>path (relative to server root)</td>
      <td>Defines a location in the server. Must be inside a server scope.</td>
    </tr>
    <tr>
      <td>listen</td>
      <td>directive</td>
      <td>port number</td>
      <td>Sets the port that this server will listen to.</td>
    </tr>
    <tr>
      <td>host</td>
      <td>directive</td>
      <td>ip address</td>
      <td>Sets the IP the server will be hosted on.</td>
    </tr>
    <tr>
      <td>root</td>
      <td>directive</td>
      <td>path</td>
      <td>Sets the root directory of the server.</td>
    </tr>
    <tr>
      <td>index</td>
      <td>directive</td>
      <td>path/filename</td>
      <td>Sets the default response file for a simple request to the server.</td>
    </tr>
    <tr>
      <td>timeout</td>
      <td>directive</td>
      <td>time (seconds)</td>
      <td>Sets the maximum time the socket connection will stay open awaiting a request before timing out.</td>
    </tr>
    <tr>
      <td>cgi_path</td>
      <td>directive</td>
      <td>path</td>
      <td>Sets the path that will be treated as the CGI execution directory.</td>
    </tr>
    <tr>
      <td>max_body</td>
      <td>directive</td>
      <td>size (bytes)</td>
      <td>Sets the maximum body size that can be handled from a <code>POST</code> request for a specific location.</td>
    </tr>
    <tr>
      <td>methods</td>
      <td>directive</td>
      <td>method names (space separated)</td>
      <td>Sets the allowed methods for a specific location.</td>
    </tr>
  </tbody>
</table>

Example config:
```
server {
	listen 8080;
	host 127.0.0.1;
	root www/;
	index index.html;
	timeout 10;

	location / {
		methods GET;
	}

	location uploads/ {
		methods GET POST DELETE;
		max_body 5242880;
	}
}
```
