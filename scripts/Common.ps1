#requires -Version 7.0
$ErrorActionPreference = 'Stop'
$script:RepoRoot = Split-Path $PSScriptRoot -Parent
$localPreset = Join-Path $script:RepoRoot 'CMakeUserPresets.json'
if (-not (Test-Path -LiteralPath $localPreset)) { throw '先运行 scripts/InitializeLocal.ps1，见 README。' }
$script:LocalTools = (Get-Content -LiteralPath $localPreset -Raw | ConvertFrom-Json).vendor.cvlab

function Import-CompilerEnvironment {
    $devShell = Join-Path $script:LocalTools.vsRoot 'Common7/Tools/VsDevCmd.bat'
    if (-not (Test-Path -LiteralPath $devShell)) { throw '未找到 Visual Studio 开发环境。' }
    $environmentLines = & $env:ComSpec /d /c "call `"$devShell`" -no_logo -arch=x64 -host_arch=x64 -vcvars_ver=14.44 >nul && set"
    if ($LASTEXITCODE -ne 0) { throw '导入 MSVC 环境失败。' }
    foreach ($entry in $environmentLines) {
        if ($entry -match '^([^=]+)=(.*)$') { [Environment]::SetEnvironmentVariable($Matches[1], $Matches[2], 'Process') }
    }
    if ($env:VCToolsVersion.TrimEnd([char]'\') -ne '14.44.35207') { throw "MSVC 工具集不匹配: $env:VCToolsVersion" }
    $env:PATH = (Split-Path $script:LocalTools.ninja) + ';' + $env:PATH
    $cmakeVersion = & $script:LocalTools.cmake --version
    if ($LASTEXITCODE -ne 0 -or $cmakeVersion[0] -ne 'cmake version 3.30.5') { throw '需要 CMake 3.30.5。' }
    $ninjaVersion = & $script:LocalTools.ninja --version
    if ($LASTEXITCODE -ne 0 -or $ninjaVersion -ne '1.12.1') { throw '需要 Ninja 1.12.1。' }
    $qtVersion = & (Join-Path $script:LocalTools.qtRoot 'bin/qmake.exe') -query QT_VERSION
    if ($LASTEXITCODE -ne 0 -or $qtVersion -ne '6.8.3') { throw '需要 Qt 6.8.3。' }
}

function Invoke-LoggedCommand {
    param([string]$Program, [string[]]$Arguments, [string]$LogPath)
    New-Item -ItemType Directory -Force -Path (Split-Path $LogPath) | Out-Null
    & $Program @Arguments *> $LogPath
    $commandExit = $LASTEXITCODE
    Get-Content -LiteralPath $LogPath -Tail 12
    if ($commandExit -ne 0) { throw "命令失败 (exit=$commandExit)，详情: $LogPath" }
}
