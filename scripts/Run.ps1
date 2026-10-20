#requires -Version 7.0
param(
    [ValidateSet('Debug','Release')][string]$Configuration = 'Debug',
    [ValidateSet('environment_probe','mat_demo','vision_viewer')][string]$Program = 'environment_probe'
)
. "$PSScriptRoot/Common.ps1"
$preset = 'local-' + $Configuration.ToLowerInvariant()
$executable = "$script:RepoRoot/build/$preset/$Program.exe"
if (-not (Test-Path -LiteralPath $executable)) { throw '尚未构建该程序，先运行 Build.ps1。' }
$opencvDll = @(Get-ChildItem -LiteralPath "$script:RepoRoot/build/deps/install/$Configuration" -Recurse -Filter 'opencv_core*.dll' -File)
if ($opencvDll.Count -ne 1) { throw 'OpenCV 运行库不唯一或缺失。' }
$env:PATH = "$($script:LocalTools.qtRoot)/bin;$($opencvDll[0].DirectoryName);" + $env:PATH
& $executable
if ($LASTEXITCODE -ne 0) { throw "程序退出失败: $LASTEXITCODE；检查可执行文件旁 logs。" }
