# Webserv

This project is about writing your own HTTP server.

Being able to understand how the HTTP protocol works is a fundamental skill for web developers. This project will help you understand what happens when you type an address into your browser and press enter.

<img width="1000" alt="webserv-example" src="https://github.com/magnitopic/webserv/assets/21156058/38603f9a-7fd1-4635-b4aa-1b6f14dcfc5b">

# Team work 💪

This project was done alongside `Javier Sarabia`(JaviBurn).
Check him and his work out at these links:

-   [LinkedIn](https://www.linkedin.com/in/javier-sarabia-224580195)
-   [GitHub](https://github.com/javiburn)
-   [42Intra](https://profile.intra.42.fr/users/jsarabia)

# Program

## Initial installation and compilation

```bash
git clone https://github.com/magnitopic/webserv.git
cd webserv
make
```

## Execution

The program take's a configuration file as argument. If none is provided the default configuration file is `./webserv.conf`.

```bash
./webserv [config_file]
```

## Configuration file

The configuration file is a simple text file that contains the server's configuration. It's composed of a series of blocks, each one representing a server. Each block is composed of a series of directives.

Example configuration files can be found in the root of the repository.

# Stress test

You can test the servers performance by running the `siege` command. This command will send a series of requests to test the server's performance.

```bash
siege -b http://localhost:8080
```

# Open ports

The server will open a socket for every port specified in the configuration file.
You can check it has opened the ports by running the example script `open_ports.sh`:

> **_NOTE:_** This script checks ports `8080` and `8082`. You can add your own ports to the script.

```bash
./open_ports.sh
```

# Pages directory

The pages directory of the repository contains example pages that can be accessed by the server. You can add your own pages to this directory and access them through the server.
