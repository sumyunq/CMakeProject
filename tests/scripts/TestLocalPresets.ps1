#requires -Version 7.0
# Updated: 2026-10-20 16:16:08 +08:00 wbliu
param([Parameter(Mandatory)][string]$LocalPresetPath)
$ErrorActionPreference = 'Stop'
$projectRoot = Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$tools = (Get-Content -LiteralPath $LocalPresetPath -Raw | ConvertFrom-Json).vendor.cvlab
$temporaryRoot = Join-Path ([System.IO.Path]::GetTempPath()) ('cvlab-preset-' + [Guid]::NewGuid().ToString('N'))
New-Item -ItemType Directory -Path "$temporaryRoot/scripts" -Force | Out-Null
try {
    Copy-Item -LiteralPath "$projectRoot/scripts/InitializeLocal.ps1" -Destination "$temporaryRoot/scripts/InitializeLocal.ps1"
    Copy-Item -LiteralPath "$projectRoot/CMakePresets.json" -Destination "$temporaryRoot/CMakePresets.json"
    $toolsRoot = Split-Path (Split-Path (Split-Path $tools.cmake -Parent) -Parent) -Parent
    & "$temporaryRoot/scripts/InitializeLocal.ps1" -QtRoot $tools.qtRoot -VisualStudioRoot $tools.vsRoot -ToolsRoot $toolsRoot
    $generated = Get-Content -LiteralPath "$temporaryRoot/CMakeUserPresets.json" -Raw | ConvertFrom-Json
    $shared = Get-Content -LiteralPath "$temporaryRoot/CMakePresets.json" -Raw | ConvertFrom-Json
    $base = $shared.configurePresets | Where-Object name -eq 'base'
    foreach ($preset in $generated.configurePresets) {
        if ($preset.cmakeExecutable -ne $tools.cmake) { throw "$($preset.name): IDE must use the locked CMake executable" }
        if ($base.architecture.value -ne 'x64' -or $base.architecture.strategy -ne 'external') { throw 'IDE must receive x64 architecture metadata' }
        if ($base.toolset.value -ne 'v143,host=x64,version=14.44.35207' -or $base.toolset.strategy -ne 'external') { throw 'IDE must receive the locked v143 toolset metadata' }
        if ($preset.cacheVariables.CMAKE_CXX_COMPILER -notmatch '14[.]44[.]35207[/\\]bin[/\\]Hostx64[/\\]x64[/\\]cl[.]exe$') { throw 'IDE compiler must be pinned' }
        $configuration = if ($preset.name -eq 'local-debug') { 'Debug' } else { 'Release' }
        if (-not $preset.environment.PATH.Contains("/build/deps/install/$configuration/x64/vc17/bin;")) { throw 'Runtime path must match Debug/Release' }
        if (-not $preset.environment.PATH.Contains('/bin;') -or -not $preset.environment.PATH.EndsWith('$penv{PATH}')) { throw 'Runtime path must preserve the parent environment' }
    }
    & $tools.cmake --list-presets=all -S $temporaryRoot
    if ($LASTEXITCODE -ne 0) { throw 'Generated presets are not consumable by CMake' }
    Write-Output 'PASS: generated IDE presets select the locked tools and runtime paths'
} finally {
    $resolvedTemporary = (Resolve-Path -LiteralPath $temporaryRoot).Path
    $temporaryBase = [System.IO.Path]::GetFullPath([System.IO.Path]::GetTempPath()).TrimEnd([char]'\') + '\'
    if (-not $resolvedTemporary.StartsWith($temporaryBase, [StringComparison]::OrdinalIgnoreCase) -or (Split-Path $resolvedTemporary -Leaf) -notlike 'cvlab-preset-*') { throw 'Temporary cleanup path rejected' }
    Remove-Item -LiteralPath $resolvedTemporary -Recurse -Force -ErrorAction Stop
}
