# LALWE
This is the official repository for the source-code of LALWE - Learn Assembly Languages With Ease

## Installation for end users
If you want to get LALWE installed on your computer, you can download the installer from the latest [release!](https://github.com/Multimodcrafter/LALWE/releases/latest) Just open the file and follow the instructions. It's as simple as that ;-)
(NOTE: Your antivirus might not like my installer-file since it has not been signed by a trustworthy key. However, I promise that the installer is totally virus, malware and adware free. If you don't believe me, you can of course download the source and compile it yourself!)

## Installation for developers
If you are a developer and want to contribute to the project or just want to compile the software on your own, follow these steps:

1. Fork this repository.
2. Clone your fork to your local machine (Or just clone this repository if you don't plan to make a contribution).
3. Download and install Visual Studio 2015 Community from [here!](https://www.visualstudio.com/post-download-vs?sku=community&clcid=0x407&downloadrename=true)
4. Download Qt 5.7 Open Source from [here!](https://www.qt.io/) (Choose the **ONLINE** installer!!!)
5. Install Qt 5.7 MSVC 2013 and Qt-Creator with the downloaded installer.
6. Download and compile the STXXL according to [these](http://stxxl.sourceforge.net/tags/master/install_windows.html) instructions.
7. From Qt-Creator, create a new Qt-Quick project and import the cloned files or try to import the Git clone directly via the import function.
8. Add the STXXL as an **external and statically** linked library.
9. Make your changes and compile the project in **release mode** to use it or to check if your changes are working.
10. Make as many commits to your local repo as you want and need. Then, push your changes to your own fork of the repo.
11. Issue a pull request to this repo and wait for your changes to get accepted.

Beware that you should have some experience with Qt and Qt-Creator before contributing to this project. Setting up your local dev-environment can be a nuisance so be prepared to experiment (sometimes a **lot**) until it's working.
