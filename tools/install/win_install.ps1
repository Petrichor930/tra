# 检查是否以管理员权限运行
if (-NOT ([Security.Principal.WindowsPrincipal][Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole] "Administrator")) {
    Write-Host "请以管理员身份运行此脚本"
    exit
}

# 安装 Chocolatey
if (-not (Get-Command choco -ErrorAction SilentlyContinue)) {
    Write-Host "正在安装Chocolatey..."
    Set-ExecutionPolicy Bypass -Scope Process -Force
    [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072
    iex ((New-Object System.Net.WebClient).DownloadString('https://chocolatey.org/install.ps1'))
    refreshenv
}

# 安装 CMake
if (Get-Command cmake -ErrorAction SilentlyContinue) {
    $version = (cmake --version | Select-String -Pattern "cmake version").Line
    Write-Host "CMake 已安装: $version" -ForegroundColor Green
}else {
# 使用 Chocolatey 安装 CMake 并添加到系统 PATH
    Write-Host "正在通过 Chocolatey 安装 CMake..." -ForegroundColor Yellow
    choco install cmake --installargs 'ADD_CMAKE_TO_PATH=System' -y
}

# 安装 ninja
if (Get-Command ninja -ErrorAction SilentlyContinue) {
    $version = (ninja --version | Select-String -Pattern "ninja version").Line
    Write-Host "ninja 已安装: $version" -ForegroundColor Green
}else {
# 使用 Chocolatey 安装 CMake 并添加到系统 PATH
    Write-Host "正在通过 Chocolatey 安装 ninja..." -ForegroundColor Yellow
    choco install ninja --installargs 'ADD_CMAKE_TO_PATH=System' -y
}

# 安装 openocd
if (Get-Command openocd -ErrorAction SilentlyContinue) {
    $version = (openocd --version | Select-String -Pattern "Open On-Chip Debugger").Line
    Write-Host "openocd 已安装: $version" -ForegroundColor Green
} else {
    # 使用 Chocolatey 安装 openocd 并添加到系统 PATH
    Write-Host "正在通过 Chocolatey 安装 openocd..." -ForegroundColor Yellow
    choco install openocd -y
}

# 安装 arm-eabi-none-gcc
if (Get-Command arm-none-eabi-gcc -ErrorAction SilentlyContinue) {
    $version = (arm-none-eabi-gcc --version | Select-String -Pattern "arm-none-eabi-gcc").Line
    Write-Host "arm-none-eabi-gcc 已安装: $version" -ForegroundColor Green
} else {
    # 使用 Chocolatey 安装 arm-none-eabi-gcc 并添加到系统 PATH
    Write-Host "正在通过 Chocolatey 安装 arm-none-eabi-gcc..." -ForegroundColor Yellow
    choco install gcc-arm-embedded -y
}

# 刷新环境变量
Write-Host "正在刷新环境变量..." -ForegroundColor Yellow
RefreshEnv.cmd

# 验证安装
Write-Host "验证安装..."
cmake -v
ninja -v
openocd -v
arm-none-eabi-gcc -v


Write-Host "安装完成！" -ForegroundColor Green
