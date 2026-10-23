#requires -Version 7.0
# Updated: 2026-10-23 16:44:36 +08:00 wbliu
param(
    [Parameter(Mandatory)][ValidateRange(3,30)][int]$Lesson,
    [ValidateSet('Debug','Release')][string]$Configuration = 'Debug',
    [string]$OutputDirectory
)
$savedPath = $env:PATH
try {
    . "$PSScriptRoot/Common.ps1"
    $target = if ($Lesson -le 13) { 'foundation_labs' } elseif ($Lesson -le 23) { 'advanced_labs' } else { 'engineering_labs' }
    $preset = 'local-' + $Configuration.ToLowerInvariant()
    $executable = "$script:RepoRoot/build/$preset/$target.exe"
    if (-not (Test-Path -LiteralPath $executable -PathType Leaf)) { throw "先运行 ./scripts/Build.ps1 -Configuration $Configuration" }
    $dll = @(Get-ChildItem -LiteralPath "$script:RepoRoot/build/deps/install/$Configuration" -Recurse -Filter 'opencv_core*.dll' -File)
    if ($dll.Count -ne 1) { throw 'OpenCV DLL 缺失或不唯一。' }
    if (-not $OutputDirectory) { $OutputDirectory = "$script:RepoRoot/build/lab-output/$preset/" + $Lesson.ToString('00') }
    New-Item -ItemType Directory -Force -Path $OutputDirectory -ErrorAction Stop | Out-Null
    $env:PATH = "$($script:LocalTools.qtRoot)/bin;$($dll[0].DirectoryName);$savedPath"
    & $executable $Lesson.ToString('00') (Resolve-Path -LiteralPath $OutputDirectory).Path
    if ($LASTEXITCODE -ne 0) { throw "实验 $Lesson 失败，exit=$LASTEXITCODE；检查输出及可执行文件旁日志。" }
    Write-Output "实验输出：$OutputDirectory"
} finally {
    $env:PATH = $savedPath
}
