#requires -Version 7.0
# Updated: 2026-11-02 11:12:46 +08:00 wbliu
$ErrorActionPreference = 'Stop'
$root = Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$temp = Join-Path ([IO.Path]::GetTempPath()) ('cvlab-preflight-' + [guid]::NewGuid())
try {
    New-Item -ItemType Directory -Path "$temp/scripts", "$temp/build/deps/install/Debug", "$temp/build/deps/vcpkg_installed/cvlab-x64-windows-static-md/share/spdlog" -Force | Out-Null
    Copy-Item "$root/scripts/Preflight.ps1" "$temp/scripts/Preflight.ps1"
    Copy-Item "$root/CMakeUserPresets.json" "$temp/CMakeUserPresets.json"
    Copy-Item "$root/build/deps/install/Debug/cvlab-build.json" "$temp/build/deps/install/Debug/cvlab-build.json"
    Copy-Item "$root/build/deps/install/Debug/OpenCVConfig.cmake" "$temp/build/deps/install/Debug/OpenCVConfig.cmake"
    Copy-Item "$root/build/deps/vcpkg_installed/cvlab-x64-windows-static-md/share/spdlog/spdlogConfigVersion.cmake" "$temp/build/deps/vcpkg_installed/cvlab-x64-windows-static-md/share/spdlog/"
    & pwsh -NoProfile -File "$temp/scripts/Preflight.ps1" -Configuration Debug
    if ($LASTEXITCODE -ne 0) { throw '成功路径失败' }
    $preset = Get-Content "$temp/CMakeUserPresets.json" -Raw | ConvertFrom-Json
    $preset.vendor.cvlab.ninja = $preset.vendor.cvlab.cmake
    $preset | ConvertTo-Json -Depth 30 | Set-Content "$temp/CMakeUserPresets.json"
    & pwsh -NoProfile -File "$temp/scripts/Preflight.ps1" -Configuration Debug 2>&1 | Out-Null
    if ($LASTEXITCODE -eq 0) { throw '错误版本未被拒绝' }
    $preset.vendor.cvlab.ninja = Join-Path $temp 'missing.exe'
    $preset | ConvertTo-Json -Depth 30 | Set-Content "$temp/CMakeUserPresets.json"
    & pwsh -NoProfile -File "$temp/scripts/Preflight.ps1" -Configuration Debug 2>&1 | Out-Null
    if ($LASTEXITCODE -eq 0) { throw '缺失路径未被拒绝' }
    Write-Output '预检隔离测试通过。'
} finally {
    if (Test-Path -LiteralPath $temp) {
        $resolved = (Resolve-Path -LiteralPath $temp).Path
        $base = (Resolve-Path -LiteralPath ([IO.Path]::GetTempPath())).Path.TrimEnd([IO.Path]::DirectorySeparatorChar)
        if (-not $resolved.StartsWith($base + [IO.Path]::DirectorySeparatorChar, [StringComparison]::OrdinalIgnoreCase) -or
            -not (Split-Path -Leaf $resolved).StartsWith('cvlab-preflight-', [StringComparison]::Ordinal)) {
            throw '临时目录不在预期范围，拒绝递归删除。'
        }
        Remove-Item -LiteralPath $resolved -Recurse -Force
    }
}
