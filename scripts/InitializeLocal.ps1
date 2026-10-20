#requires -Version 7.0
# Updated: 2026-10-20 16:16:08 +08:00 wbliu
param(
    [Parameter(Mandatory)][string]$QtRoot,
    [Parameter(Mandatory)][string]$VisualStudioRoot,
    [Parameter(Mandatory)][string]$ToolsRoot
)
$ErrorActionPreference = 'Stop'
$repoRoot = Split-Path $PSScriptRoot -Parent
$qtPath = (Resolve-Path -LiteralPath $QtRoot).Path
$vsPath = (Resolve-Path -LiteralPath $VisualStudioRoot).Path
$toolsPath = (Resolve-Path -LiteralPath $ToolsRoot).Path
foreach ($required in @("$qtPath/bin/qmake.exe", "$qtPath/bin/windeployqt.exe", "$toolsPath/CMake_64/bin/cmake.exe", "$toolsPath/Ninja/ninja.exe", "$vsPath/VC/Tools/MSVC/14.44.35207/bin/Hostx64/x64/cl.exe")) {
    if (-not (Test-Path -LiteralPath $required -PathType Leaf)) { throw "缺少工具: $required" }
}
$cmakePath = "$toolsPath/CMake_64/bin/cmake.exe"
$ninjaPath = "$toolsPath/Ninja/ninja.exe"
$compilerPath = ("$vsPath/VC/Tools/MSVC/14.44.35207/bin/Hostx64/x64/cl.exe").Replace('\', '/')
$configurePresets = foreach ($configuration in @('Debug', 'Release')) {
    @{
        name = 'local-' + $configuration.ToLowerInvariant()
        displayName = "Qt 6.8.3 / OpenCV 5.0.0 / x64 $configuration"
        inherits = $configuration.ToLowerInvariant()
        # IDEs otherwise select their bundled CMake, independently of PATH.
        cmakeExecutable = $cmakePath
        environment = @{
            CVLAB_QT_ROOT = $qtPath
            PATH = "$qtPath/bin;$repoRoot/build/deps/install/$configuration/x64/vc17/bin;" + '$penv{PATH}'
        }
        cacheVariables = @{
            CMAKE_MAKE_PROGRAM = $ninjaPath
            CMAKE_CXX_COMPILER = $compilerPath
        }
    }
}
$presets = @{
    version = 6
    vendor = @{ cvlab = @{ qtRoot = $qtPath; vsRoot = $vsPath; cmake = $cmakePath; ninja = $ninjaPath } }
    configurePresets = @($configurePresets)
    buildPresets = @(
        @{ name = 'local-debug'; inherits = 'base-build'; configurePreset = 'local-debug' },
        @{ name = 'local-release'; inherits = 'base-build'; configurePreset = 'local-release' }
    )
    testPresets = @(
        @{ name = 'local-debug'; inherits = 'base-test'; configurePreset = 'local-debug' },
        @{ name = 'local-release'; inherits = 'base-test'; configurePreset = 'local-release' }
    )
}
$presets | ConvertTo-Json -Depth 10 | Set-Content -LiteralPath "$repoRoot/CMakeUserPresets.json" -Encoding utf8
Write-Output '本机预设已生成（不进入 Git），未修改全局环境。'
