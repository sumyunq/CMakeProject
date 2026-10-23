#requires -Version 7.0
# Updated: 2026-10-23 16:44:36 +08:00 wbliu
param([switch]$Run)
$savedPath = $env:PATH
$savedPlatform = $env:QT_QPA_PLATFORM
try {
    . "$PSScriptRoot/Common.ps1"
    Import-CompilerEnvironment
    $build = "$script:RepoRoot/build/workbench-reference"
    Invoke-LoggedCommand $script:LocalTools.cmake @('-S',"$script:RepoRoot/solutions/0028-capstone-workbench/reference",'-B',$build,'-G','Ninja','-DCMAKE_BUILD_TYPE=Release',"-DCMAKE_MAKE_PROGRAM=$($script:LocalTools.ninja)","-DCMAKE_PREFIX_PATH=$($script:LocalTools.qtRoot)","-DOpenCV_DIR=$script:RepoRoot/build/deps/install/Release") "$script:RepoRoot/build/verification/workbench-configure.log"
    Invoke-LoggedCommand $script:LocalTools.cmake @('--build',$build,'--parallel','4') "$script:RepoRoot/build/verification/workbench-build.log"
    $env:PATH = "$($script:LocalTools.qtRoot)/bin;$script:RepoRoot/build/deps/install/Release/x64/vc17/bin;$env:PATH"
    $env:QT_QPA_PLATFORM = 'offscreen'
    $check = Start-Process -FilePath "$build/workbench_reference.exe" -ArgumentList '--self-test' -WindowStyle Hidden -PassThru -Wait -RedirectStandardOutput "$script:RepoRoot/build/verification/workbench-tests.log" -RedirectStandardError "$script:RepoRoot/build/verification/workbench-tests-errors.log"
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
