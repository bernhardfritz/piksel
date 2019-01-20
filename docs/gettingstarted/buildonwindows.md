## How to build on Windows

At the moment only web builds are supported on Windows using the **Windows Subsystem for Linux** (WSL). In theory native builds on Windows should also be possible, but that would require some adaptions to piksel's [CMakeLists.txt](https://github.com/bernhardfritz/piksel/blob/master/CMakeLists.txt). If you get a native build to run on Windows, <a href="javascript:void(0);" onclick="document.querySelector('.gitter-open-chat-button').click();">please let us know</a>!

#### How to set up WSL

Please refer to the official [Windows Subsystem for Linux Installation Guide for Windows 10](https://docs.microsoft.com/en-us/windows/wsl/install-win10) by Microsoft.

#### Tips when working with WSL

If you are completely new to Linux, please make yourself familiar with the following [cheat sheet](http://cheatsheetworld.com/programming/unix-linux-cheat-sheet/).

When you open WSL, per default your current working directory will be the home directory, a.k.a. `~`. You can **print** the current **working directory** by entering `pwd`. To access files from your `C:\` drive, you need to **change** your current working **directory** to `/mnt/c` using `cd`

```bash
cd /mnt/c
```

This allows you to write your code in Windows, using the code editors you are accustomed to and only use WSL to build.

#### Build for the web on Windows

With WSL set up, you can continue the [Build for the web](gettingstarted/buildproject.md#build-for-the-web-img-srcgettingstarted_mediawindows_logo_2012-blacksvgdata-origin_mediawindows_logo_2012-blacksvg-altwindows) guide.
