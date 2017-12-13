# Plugin SkyWave Beta Test

Plugin for Monita Service get data from SkyWave Server

## **Get Started NOW**

### **Prerequisites**
```bash
## QT 5.6++
$ sudo apt install        \
    libqt5network5        \
    libqt5core5a          \
    qt5-qmake             \
    qtbase5-dev           \
    qtdeclarative5-dev    \
    qt5-qmake             \
    qt5-default           \
    qttools5-dev-tools
```

### **Setup**

```bash
$ git clone https://github.com/zenaki/Plugins-SkyWave.git
$ cd Plugins-SkyWave
```

Using QtCreator, open `SkyWave.pro` and enjoy!
or
Using QMake:
```bash
$ qmake CONFIG+=debug SkyWave.pro
$ make
$ sudo mkdir /usr/local/bin/plugins/
$ sudo cp SkyWave /usr/local/bin/plugins/ && sudo chown root:root /usr/local/bin/plugins/SkyWave
```
