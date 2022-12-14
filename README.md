# 42-Ping

[![CodeFactor](https://www.codefactor.io/repository/github/kibotrel/42-ping/badge)](https://www.codefactor.io/repository/github/kibotrel/42-ping)
![GitHub](https://img.shields.io/github/license/kibotrel/42-Ping?color=blue)
![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/kibotrel/42-Ping?label=size)
![Lines of code](https://img.shields.io/tokei/lines/github/kibotrel/42-Ping?label=code%20lines)

This project's goal is to understand how raw sockets and [ICMP Protocol](https://en.wikipedia.org/wiki/Internet_Control_Message_Protocol) works by implementing a subset of the [`ping`](https://linux.die.net/man/8/ping) utility command.

## 📦 Install

Since creating raw socket need administrator permissions, there's a script that copies the project within a [docker container](https://www.docker.com/resources/what-container/) and bind the host default network interface to it.

> ℹ️ If you need to install docker, follow [the official documentation](https://docs.docker.com/get-docker/) to get started.


If you decide to go the `docker` way, then all you have to do is:

```shell
  $> git clone https://github.com/kibotrel/42-Ping.git ; cd 42-Ping
  $> ./run.sh
```

> ℹ️ Once everything's done, you'll be inside the container's shell with the project already compiled + access to [`iputils-ping`](https://packages.debian.org/sid/iputils-ping) for reference and [`valgrind`](https://valgrind.org/) if needed for debug purposes.

If you can get administrator permissions on your machine:

```shell
  $> git clone https://github.com/kibotrel/42-Ping.git ; cd 42-Ping
  $> make
```
> ℹ️ Then to use the program either connect as `root` user or use [`sudo`](https://linux.die.net/man/8/sudo) to be able to create raw sockets.

### Usage

This tries to replicate the original `ping` as best as it can within the scope of all the supported features. So basic usage would be the same:

```shell
  $> ./ft_ping <destination>
```

> ℹ️ Use `./ft_ping -h` to get more information on how to use the program and look into the different supported features.

## 📚 Resources

This is a collection of useful links that were helpful during this project development.
### Wikipedia

- [ICMP Protocol](https://en.wikipedia.org/wiki/Internet_Control_Message_Protocol)
- [Network byte order](https://en.wikipedia.org/wiki/Endianness#Networking)
- [Signals in C](https://en.wikipedia.org/wiki/C_signal_handling)

### RFC

- [791: Internet Protocol](https://www.rfc-editor.org/rfc/rfc791)
- [792: Internet Control Message Protocol](https://www.rfc-editor.org/rfc/rfc792)
- [1017: Internet Checksum algorithm](https://www.rfc-editor.org/rfc/rfc1071)

### Misc

- [Write modern C](https://matt.sh/howto-c)
- [`ping` man page](https://linux.die.net/man/8/ping)