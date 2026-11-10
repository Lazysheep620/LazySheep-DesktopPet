# LazySheep-DesktopPet

一个用 C++17、Qt 6 Widgets 和 CMake 编写的 Windows 像素桌宠。小羊的窗口透明、无边框且始终置顶，内置五组动画。

## 在 Qt Creator 中运行

1. 安装 Qt 6 Desktop、Qt Creator、CMake 3.21 或更高版本，以及与 Qt 套件匹配的编译器。使用 Qt Online Installer 时，可以选择 **Desktop MinGW 64-bit**，并安装配套的 MinGW 和 Ninja。
2. 在 Qt Creator 中选择 **文件 → 打开文件或项目**，打开本目录的 `CMakeLists.txt`。
3. 选择与所装 Qt 对应的 **Desktop Qt 6** 套件，点击 **Configure Project**。
4. 点击绿色运行按钮。小羊第一次启动时出现在主屏幕右下角。

## 用命令行构建

下面以 Qt 6.11.2 的 MinGW 套件为例。在 PowerShell 里进入本项目目录，把路径改成实际的 Qt 安装路径。

```powershell
$qt = 'C:\Qt\6.11.2\mingw_64'
$tools = 'C:\Qt\Tools'
$env:Path = "$qt\bin;$tools\mingw1310_64\bin;$tools\CMake_64\bin;$tools\Ninja;$env:Path"

cmake -S . -B build -G Ninja "-DCMAKE_PREFIX_PATH=$qt" -DCMAKE_BUILD_TYPE=Release
cmake --build build
.\build\LazySheep-DesktopPet.exe
```

从同一个 PowerShell 窗口启动可执行文件，以便找到 Qt DLL。Qt Creator 会自动为所选套件设置运行环境。

## 操作

| 操作 | 效果 |
| --- | --- |
| 左键轻点小羊 | 播放开心动画，睡着时则叫醒它 |
| 按住左键拖动 | 移动小羊，拖动期间播放行走动画 |
| 右键点击小羊 | 查看数值，喂食、玩耍、睡觉、叫醒或退出 |
| 喂食 | 饱食度 +15，播放进食动画 |
| 玩耍 | 至少需要 15 点精力，心情 +15、精力 −15 |
| 睡觉 | 精力 +50，持续播放睡觉动画，直到被叫醒 |

初始饱食度为 60，心情为 70，精力为 50。饱食度越高表示越饱。所有数值限制在 0–100。动作完成后回到待机动画，睡觉时可通过左键或菜单叫醒，也可以直接喂食或玩耍。

程序通过 `QSettings` 自动保存数值、睡眠状态和窗口位置。重启会恢复这些信息，临时的进食、开心和行走动画会从待机开始。显示器配置变化后，恢复的位置会调整到可见的屏幕范围内。

## 项目结构

- `src/Sheep.*` 管理数值、动作和存档。
- `src/PetWindow.*` 管理窗口、拖动、菜单和动画。
- `assets/sprites/` 包含 20 张带透明背景的 96 × 80 像素 PNG。CMake 将它们打包到可执行文件中。
