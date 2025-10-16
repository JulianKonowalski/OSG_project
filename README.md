# Klonowanie projektu
No to jest całkiem ez
```
git clone --recurse-submodules https://github.com/JulianKonowalski/OSG_project.git
```

# Wymagania
Wymagania są proste:
* git
* cmake
* cmake generator, np Ninja
* jakiś kompilator, ja używam clanga

Jeśli jesteś na windowsie, to polecam:
```
# instalacja scoopa
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser
Invoke-RestMethod -Uri https://get.scoop.sh | Invoke-Expression

scoop install git       # scm
scoop install cmake     # build system
scoop install ninja     # make generator lub inny
scoop install clangd    # cpp language server, opcjonalnie
scoop install gcc       # kompilator lub inny
```

# budowanie
```
cmake -G Ninja -B build -S .
cmake --build build
```

Jeśli nie korzystacie z Ninjy to zastąpcie to jakimkolwiek innym generatorem, ale nie obiecuję, że
zadziała.

# odpalanie
```
./build/osg_application
```