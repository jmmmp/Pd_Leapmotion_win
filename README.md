
## leapmotion object
A pure data external object for Leapmotion v.2

- Commissioned by Joao Pais
- Developed by Dr. Chikashi Miyama
- Thanks to: Dan Wilcox and Jakab Valtar

## dependencies
- [Leap SDK](https://developer.leapmotion.com/documentation/v2/cpp/index.html)
- [flext](https://github.com/grrrr/flext) by Thomas Grill
- pthread
- pure-data



### clone the repo

clone the repo and submodules
```
> git clone --recursive git://github.com/foo/bar.git
```

open command line by Start -> Run -> cmd and find folder that contains **vcvars32.bat** and run. This is necessary to run msvc( micro soft visual C++ compiler).

vcvars32.bat can be found C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin

in vcvars32.bat
```
cd C:\flext
build pd msvc
```
this generates error but it's normal.
open Open C:\flext\buildsys\config-win-pd-msvc.txt
and edit the corresponding lines

```
ROOTPATH="C:\*to\your\cloned\repo\"
PDPATH=${ROOTPATH}\pure-data"
PTHREADSVERSION=2
PTHREADSINC="${ROOTPATH}\pthread\include"
PTHREADSLIB="${ROOTPATH}\pthread\lib\x86"
```

## Testing environment
The build was tested with
- Windows 10
- Visual Studio 2015
- Pd 0.48 Vanilla 32bit
- Leapmotion SDK v2.3.1
