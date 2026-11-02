#requires -Version 7.0
# Updated: 2026-11-02 11:12:46 +08:00 wbliu
param([ValidateSet('Foundation','Full')][string]$Profile = 'Foundation', [ValidateSet('Debug','Release','Both')][string]$Configuration = 'Both')
$ErrorActionPreference = 'Stop'
$root = Split-Path $PSScriptRoot -Parent
try {
    $preset = Join-Path $root 'CMakeUserPresets.json'
    if (-not (Test-Path -LiteralPath $preset -PathType Leaf)) { throw '缺少 CMakeUserPresets.json，请先运行 InitializeLocal.ps1。' }
    $tools = (Get-Content -LiteralPath $preset -Raw | ConvertFrom-Json).vendor.cvlab
    if (-not $tools) { throw '本地预设缺少 vendor.cvlab 工具路径。' }
    $compiler = Join-Path $tools.vsRoot 'VC/Tools/MSVC/14.44.35207/bin/Hostx64/x64/cl.exe'
    foreach ($path in @($tools.cmake, $tools.ninja, (Join-Path $tools.qtRoot 'bin/qmake.exe'), (Join-Path $tools.qtRoot 'bin/windeployqt.exe'), $compiler, (Join-Path $tools.vsRoot 'Common7/Tools/VsDevCmd.bat'))) {
        if (-not $path -or -not (Test-Path -LiteralPath $path -PathType Leaf)) { throw "缺少工具文件: $path" }
    }
    $checks = @(@($tools.cmake, '--version', '^cmake version 3\.30\.5$'), @($tools.ninja, '--version', '^1\.12\.1$'), @((Join-Path $tools.qtRoot 'bin/qmake.exe'), '-query', '^6\.8\.3$'))
    foreach ($check in $checks) {
        $output = @(if ($check[1] -eq '-query') { & $check[0] -query QT_VERSION } else { & $check[0] $check[1] })
        if ($LASTEXITCODE -ne 0 -or $output[0] -notmatch $check[2]) { throw "工具版本不匹配: $($check[0])，实际: $output" }
    }
    $version = (Get-Item -LiteralPath $compiler).VersionInfo.FileVersion
    if ($version -notmatch '^19\.44\.') { throw "MSVC 编译器版本不匹配: $version" }
    $configs = if ($Configuration -eq 'Both') { @('Debug','Release') } else { @($Configuration) }
    foreach ($config in $configs) {
        $prefix = if ($Profile -eq 'Full') { 'full-install' } else { 'install' }
        $install = Join-Path $root "build/deps/$prefix/$config"
        $manifest = Join-Path $install 'cvlab-build.json'
        if (-not (Test-Path -LiteralPath $manifest -PathType Leaf) -or -not (Test-Path -LiteralPath (Join-Path $install 'OpenCVConfig.cmake') -PathType Leaf)) { throw "缺少 $Profile $config OpenCV 安装，请运行 PrepareDependencies.ps1。" }
        $info = Get-Content -LiteralPath $manifest -Raw | ConvertFrom-Json
        if ($info.version -ne '5.0.0' -or $info.commit -ne '40738fb16ceddb5fb3fea747585f7ce6abb0605b' -or $info.configuration -ne $config -or ($info.profile -ne $Profile -and -not ($Profile -eq 'Foundation' -and -not $info.profile)) -or $info.compiler.TrimEnd([char]92) -ne '14.44.35207') { throw "OpenCV 安装记录不匹配: $manifest" }
        foreach ($module in @('core','imgproc','imgcodecs')) { if ($module -notin $info.modules) { throw "安装缺少模块: $module" } }
        if ($Profile -eq 'Full') { foreach ($module in @('features','calib','stereo','objdetect','video','videoio','dnn')) { if ($module -notin $info.modules) { throw "Full 安装缺少模块: $module" } } }
    }
    $spdlog = Join-Path $root 'build/deps/vcpkg_installed/cvlab-x64-windows-static-md/share/spdlog/spdlogConfigVersion.cmake'
    if (-not (Test-Path -LiteralPath $spdlog -PathType Leaf) -or (Get-Content -LiteralPath $spdlog -Raw) -notmatch '1\.17\.0') { throw '缺少 spdlog 1.17.0，请运行 PrepareLoggerDependencies.ps1。' }
    Write-Output "预检通过: $Profile / $Configuration；未修改环境。"
} catch { Write-Error "预检失败：$_"; exit 1 }
