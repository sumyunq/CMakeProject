#requires -Version 7.0
# Updated: 2026-10-23 16:44:36 +08:00 wbliu
param([ValidateSet('Debug','Release')][string]$Configuration = 'Release')
$savedPath = $env:PATH
try {
    . "$PSScriptRoot/Common.ps1"
    Import-CompilerEnvironment
    $build = "$script:RepoRoot/build/native-$($Configuration.ToLowerInvariant())"
    $opencv = "$script:RepoRoot/build/deps/full-install/$Configuration"
    if (-not (Test-Path -LiteralPath "$opencv/OpenCVConfig.cmake")) { throw "先运行 ./scripts/PrepareDependencies.ps1 -Profile Full -Configuration $Configuration" }
    Invoke-LoggedCommand $script:LocalTools.cmake @('-S',"$script:RepoRoot/examples/native",'-B',$build,'-G','Ninja',"-DCMAKE_BUILD_TYPE=$Configuration", "-DCMAKE_MAKE_PROGRAM=$($script:LocalTools.ninja)","-DOpenCV_DIR=$opencv", "-DCMAKE_PREFIX_PATH=$($script:LocalTools.qtRoot)") "$script:RepoRoot/build/verification/native-configure.log"
    & python "$script:RepoRoot/scripts/GenerateTinyOnnx.py" "$build/relu.onnx"
    if ($LASTEXITCODE -ne 0) { throw '无法生成本地模型。' }
    Invoke-LoggedCommand $script:LocalTools.cmake @('--build',$build,'--parallel','4') "$script:RepoRoot/build/verification/native-build.log"
    $ctest = Join-Path (Split-Path $script:LocalTools.cmake) 'ctest.exe'
    Invoke-LoggedCommand $ctest @('--test-dir',$build,'--output-on-failure') "$script:RepoRoot/build/verification/native-tests.log"
} finally { $env:PATH = $savedPath }
