#requires -Version 7.0
# Updated: 2026-10-20 16:55:25 +08:00 wbliu
$savedEnvironment = @{}
foreach ($name in @('PATH','VCPKG_VISUAL_STUDIO_PATH','VCPKG_DISABLE_METRICS','VCPKG_FORCE_SYSTEM_BINARIES')) {
    $savedEnvironment[$name] = @{ Exists = (Test-Path -LiteralPath ("Env:" + $name)); Value = [Environment]::GetEnvironmentVariable($name, 'Process') }
}
try {
    . "$PSScriptRoot/Common.ps1"
    Import-CompilerEnvironment
    Set-Location -LiteralPath $script:RepoRoot
    $installedVcpkg = Join-Path $script:LocalTools.vsRoot 'VC/vcpkg/vcpkg.exe'
    if (-not (Test-Path -LiteralPath $installedVcpkg -PathType Leaf)) { throw '当前 Visual Studio 缺少 vcpkg.exe，请安装其 vcpkg 组件。' }
    $registryRoot = Join-Path $script:RepoRoot '3rdparty/vcpkg'
    $baseline = (Get-Content -LiteralPath "$script:RepoRoot/vcpkg.json" -Raw | ConvertFrom-Json).'builtin-baseline'
    if (-not (Test-Path -LiteralPath "$registryRoot/.git")) {
        New-Item -ItemType Directory -Force -Path $registryRoot | Out-Null
        Invoke-LoggedCommand 'git' @('init',$registryRoot) "$script:RepoRoot/build/verification/vcpkg-init.log"
        Invoke-LoggedCommand 'git' @('-C',$registryRoot,'remote','add','origin','https://github.com/microsoft/vcpkg.git') "$script:RepoRoot/build/verification/vcpkg-origin.log"
    }
    $currentCommit = & git -C $registryRoot rev-parse --verify HEAD 2>$null
    if ($LASTEXITCODE -ne 0) {
        Invoke-LoggedCommand 'git' @('-C',$registryRoot,'-c','http.lowSpeedLimit=1024','-c','http.lowSpeedTime=60','fetch','--depth','1','--filter=blob:none','origin',$baseline) "$script:RepoRoot/build/verification/vcpkg-fetch.log"
        Invoke-LoggedCommand 'git' @('-C',$registryRoot,'sparse-checkout','init','--cone') "$script:RepoRoot/build/verification/vcpkg-sparse.log"
        Invoke-LoggedCommand 'git' @('-C',$registryRoot,'sparse-checkout','set','ports/spdlog','ports/vcpkg-cmake','ports/vcpkg-cmake-config','scripts','triplets','versions') "$script:RepoRoot/build/verification/vcpkg-sparse.log"
        Invoke-LoggedCommand 'git' @('-C',$registryRoot,'-c','http.lowSpeedLimit=1024','-c','http.lowSpeedTime=60','checkout','--detach','FETCH_HEAD') "$script:RepoRoot/build/verification/vcpkg-checkout.log"
        $currentCommit = & git -C $registryRoot rev-parse HEAD
    }
    if ($currentCommit -ne $baseline) { throw '本地 vcpkg 注册表提交不符合清单 baseline。' }
    $vcpkg = Join-Path $registryRoot 'vcpkg.exe'
    Copy-Item -LiteralPath $installedVcpkg -Destination $vcpkg -Force -ErrorAction Stop
    if ((Get-FileHash -LiteralPath $installedVcpkg).Hash -ne (Get-FileHash -LiteralPath $vcpkg).Hash) { throw 'vcpkg 工具复制校验失败。' }
    $env:VCPKG_VISUAL_STUDIO_PATH = $script:LocalTools.vsRoot
    $env:VCPKG_DISABLE_METRICS = '1'
    # Acquire only the archive helper, then reuse the already locked CMake/Ninja.
    if (Test-Path -LiteralPath Env:VCPKG_FORCE_SYSTEM_BINARIES) {
        Remove-Item -LiteralPath Env:VCPKG_FORCE_SYSTEM_BINARIES -ErrorAction Stop
    }
    Invoke-LoggedCommand $vcpkg @('fetch','7zip',"--vcpkg-root=$registryRoot", "--downloads-root=$script:RepoRoot/build/downloads/vcpkg") "$script:RepoRoot/build/verification/vcpkg-7zip.log"
    $archiveTool = (Get-Content -LiteralPath "$script:RepoRoot/build/verification/vcpkg-7zip.log" -Tail 1).Trim().Trim('"')
    if (-not (Test-Path -LiteralPath $archiveTool -PathType Leaf) -or [IO.Path]::GetFileName($archiveTool) -ne '7z.exe') { throw '未找到 vcpkg 校验后的解压工具。' }
    $env:PATH = (Split-Path $archiveTool -Parent) + ';' + $env:PATH
    $packageCmake = Join-Path $script:LocalTools.vsRoot 'Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/cmake.exe'
    if (Test-Path -LiteralPath $packageCmake -PathType Leaf) {
        $capabilities = & $packageCmake --help-command string
        if ($LASTEXITCODE -ne 0) { throw '无法检查 vcpkg 构建辅助 CMake 的能力。' }
        if ($capabilities -match 'STRING_ENCODE') {
            # This CMake is used only inside vcpkg, never by the project presets.
            $env:PATH = (Split-Path $packageCmake -Parent) + ';' + $env:PATH
            $env:VCPKG_FORCE_SYSTEM_BINARIES = '1'
        }
    }
    $arguments = @('install',"--vcpkg-root=$registryRoot",'--triplet','cvlab-x64-windows-static-md',"--overlay-triplets=$script:RepoRoot/cmake/triplets", "--x-manifest-root=$script:RepoRoot", "--x-install-root=$script:RepoRoot/build/deps/vcpkg_installed", "--x-buildtrees-root=$script:RepoRoot/build/deps/vcpkg_buildtrees", "--x-packages-root=$script:RepoRoot/build/deps/vcpkg_packages", "--downloads-root=$script:RepoRoot/build/downloads/vcpkg")
    Invoke-LoggedCommand $vcpkg $arguments "$script:RepoRoot/build/verification/spdlog-install.log"
    Write-Output 'spdlog 1.17.0 已通过固定 vcpkg 清单准备完成。'

} finally {
    foreach ($name in $savedEnvironment.Keys) {
        $variable = "Env:" + $name
        if ($savedEnvironment[$name].Exists) {
            Set-Item -LiteralPath $variable -Value $savedEnvironment[$name].Value -ErrorAction Stop
        } elseif (Test-Path -LiteralPath $variable) {
            Remove-Item -LiteralPath $variable -ErrorAction Stop
        }
    }
}
