#requires -Version 7.0
# Updated: 2026-10-20 16:16:08 +08:00 wbliu
param([ValidateSet('Debug','Release')][string]$Configuration = 'Debug', [switch]$SkipTests)
. "$PSScriptRoot/Common.ps1"
Import-CompilerEnvironment
Set-Location -LiteralPath $script:RepoRoot
$preset = 'local-' + $Configuration.ToLowerInvariant()
Invoke-LoggedCommand $script:LocalTools.cmake @('--preset',$preset) "$script:RepoRoot/build/verification/$preset-configure.log"
Invoke-LoggedCommand $script:LocalTools.cmake @('--build','--preset',$preset) "$script:RepoRoot/build/verification/$preset-build.log"
if (-not $SkipTests) {
    $ctest = Join-Path (Split-Path $script:LocalTools.cmake) 'ctest.exe'
    Invoke-LoggedCommand $ctest @('--preset',$preset) "$script:RepoRoot/build/verification/$preset-tests.log"
    & "$script:RepoRoot/tests/scripts/TestLocalPresets.ps1" -LocalPresetPath "$script:RepoRoot/CMakeUserPresets.json"
}
