Building a System Monitor
================


This project is part of the [C++ Nanodegree](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213)
 program from Udacity. I wrote a Linux system monitor with similar functionality
 to the widely used htop application using C++ OOP.

<p align="center"><img src="monitor.png" alt="Example" width="65%" style="middle"></p>


### Install
To set up your environment to run the code in this repository, start by cloning
 this project using the `--recurse-submodules` flag. Afterward, install Docker
 on your machine. Finally, run the following commands. This will start a
 initial monitor without data to test the environment.
```shell
$ make docker-build
$ make basic-monitor
```


### Run
In a terminal or command window, navigate to the top-level project directory
 `cpp-system-monitor/` (that contains this README) and run the following
 command, which will start a monitor using the code implemented in this
 repository.

```shell
# make compile-monitor
$ make system-monitor
```


### License
The contents of this repository are covered under the [MIT License](LICENSE).

