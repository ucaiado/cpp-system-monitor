Building a System Monitor
================


This project is part of the [C++ Nanodegree](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213)
 program from Udacity. ....

<p align="center"><img src="monitor.png" alt="Example" width="50%" style="middle"></p>


### Install
To set up your environment to run the code in this repository, start by cloning
 this project using the `--recurse-submodules` flag. Afterward, install Docker
 in your machine and the appropriate Xserver. Then, start both and configure
 Xserver as explained [here](https://medium.com/@mreichelt/how-to-show-x11-windows-within-docker-on-mac-50759f4b65cb).
 You may need to restart your computer. Finally, run the following commands. You
 should see a blank map.
```shell
$ make docker-build
$ make ...
```


### Run
In a terminal or command window, navigate to the top-level project directory
 `cpp-system-monitor/` (that contains this README) and run the following
 commands. This will run the project tests and then run the final code, opening
 a ...

```shell
$ make tests
$ make ...
```


### License
The contents of this repository are covered under the [MIT License](LICENSE).

