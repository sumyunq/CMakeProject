#requires -Version 7.0
# Updated: 2026-10-23 16:44:36 +08:00 wbliu
param([ValidateSet('Debug','Release')][string]$Configuration = 'Debug')
$savedPath = $env:PATH
try {
    . "$PSScriptRoot/Common.ps1"
    Import-CompilerEnvironment
    $preset = 'local-' + $Configuration.ToLowerInvariant()
    Invoke-LoggedCommand $script:LocalTools.cmake @('--preset',$preset) "$script:RepoRoot/build/verification/answers-$preset-configure.log"
    Invoke-LoggedCommand $script:LocalTools.cmake @('--build','--preset',$preset,'--target','reference_answer_tests') "$script:RepoRoot/build/verification/answers-$preset-build.log"
    $dll = @(Get-ChildItem -LiteralPath "$script:RepoRoot/build/deps/install/$Configuration" -Recurse -Filter 'opencv_core*.dll' -File)
    if ($dll.Count -ne 1) { throw 'OpenCV DLL缺失或不唯一。' }
    $env:PATH = "$($dll[0].DirectoryName);$savedPath"
    Invoke-LoggedCommand "$script:RepoRoot/build/$preset/reference_answer_tests.exe" @() "$script:RepoRoot/build/verification/answers-$preset-tests.log"
} finally { $env:PATH = $savedPath }
