#requires -Version 7.0
# Updated: 2026-11-02 11:12:46 +08:00 wbliu
param([ValidateSet('Debug','Release')][string]$Configuration = 'Release', [switch]$Run)
$savedPath = $env:PATH
$savedPlatform = $env:QT_QPA_PLATFORM
try {
    . "$PSScriptRoot/Common.ps1"
    Import-CompilerEnvironment
    $build = "$script:RepoRoot/build/workbench-reference-$($Configuration.ToLowerInvariant())"
    Invoke-LoggedCommand $script:LocalTools.cmake @('-S',"$script:RepoRoot/solutions/0028-capstone-workbench/reference",'-B',$build,'-G','Ninja',"-DCMAKE_BUILD_TYPE=$Configuration","-DCMAKE_MAKE_PROGRAM=$($script:LocalTools.ninja)","-DCMAKE_PREFIX_PATH=$($script:LocalTools.qtRoot)","-DOpenCV_DIR=$script:RepoRoot/build/deps/install/$Configuration") "$script:RepoRoot/build/verification/workbench-$Configuration-configure.log"
    Invoke-LoggedCommand $script:LocalTools.cmake @('--build',$build,'--parallel','4') "$script:RepoRoot/build/verification/workbench-$Configuration-build.log"
    $env:PATH = "$($script:LocalTools.qtRoot)/bin;$script:RepoRoot/build/deps/install/$Configuration/x64/vc17/bin;$env:PATH"
    $env:QT_QPA_PLATFORM = 'offscreen'
    $check = Start-Process -FilePath "$build/workbench_reference.exe" -ArgumentList '--self-test' -WindowStyle Hidden -PassThru -RedirectStandardOutput "$script:RepoRoot/build/verification/workbench-$Configuration-tests.log" -RedirectStandardError "$script:RepoRoot/build/verification/workbench-$Configuration-tests-errors.log"
    if (-not $check.WaitForExit(30000)) { Stop-Process -Id $check.Id -Force; throw '参考窗口自检超过30秒，已停止本次测试进程。' }
    if ($check.ExitCode -ne 0) { throw "参考窗口自检失败 exit=$($check.ExitCode)" }
    if ($Run) {
        Remove-Item Env:QT_QPA_PLATFORM -ErrorAction SilentlyContinue
        & "$build/workbench_reference.exe"
        if ($LASTEXITCODE -ne 0) { throw '参考窗口运行失败。' }
    }
} finally {
    $env:PATH = $savedPath
    if ($null -eq $savedPlatform) { Remove-Item Env:QT_QPA_PLATFORM -ErrorAction SilentlyContinue }
    else { $env:QT_QPA_PLATFORM = $savedPlatform }
}
