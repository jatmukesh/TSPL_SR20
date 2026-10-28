$ErrorActionPreference = 'Stop'
$tsplRoot = (Resolve-Path (Join-Path $PSScriptRoot '../..')).Path
$tsplBuild = Join-Path $PSScriptRoot 'build'
New-Item -ItemType Directory -Force -Path $tsplBuild | Out-Null
Push-Location $tsplRoot
try {
    $tsplFlags = @('-std=c99','-Wall','-Wextra','-Werror','-pedantic','-DTSPL_PROTOCOL=1','-DESC_POS_PROTOCOL=0','-ICode')
    $tsplSources = @('tests/tspl/test_tspl.c', 'Code/TsplParser.c')
    & gcc -std=c99 -Wall -Wextra -Werror -pedantic -DTSPL_PROTOCOL=1 -DESC_POS_PROTOCOL=0 -ICode @tsplSources -o "$tsplBuild/test_tspl.exe"
    if ($LASTEXITCODE -ne 0) { throw 'TSPL host compilation failed' }
    & "$tsplBuild/test_tspl.exe"
    if ($LASTEXITCODE -ne 0) { throw 'TSPL host tests failed' }
    & gcc -std=c99 -Wall -Wextra -Werror -pedantic -ICode tests/tspl/test_job.c Code/TsplJob.c -o "$tsplBuild/test_job.exe"
    if ($LASTEXITCODE -ne 0) { throw 'TSPL job compilation failed' }
    & "$tsplBuild/test_job.exe"
    if ($LASTEXITCODE -ne 0) { throw 'TSPL job tests failed' }
    & gcc -std=c99 -Wall -Wextra -Werror -pedantic -ICode tests/tspl/test_raster.c Code/TsplRaster.c Code/TsplJob.c -o "$tsplBuild/test_raster.exe"
    if ($LASTEXITCODE -ne 0) { throw 'TSPL raster compilation failed' }
    & "$tsplBuild/test_raster.exe"
    if ($LASTEXITCODE -ne 0) { throw 'TSPL raster tests failed' }
    & gcc -std=c99 -Wall -Wextra -Werror -pedantic -ICode tests/tspl/test_firmware.c Code/TsplFirmware.c Code/TsplParser.c Code/TsplJob.c -o "$tsplBuild/test_firmware.exe"
    if ($LASTEXITCODE -ne 0) { throw 'TSPL firmware integration compilation failed' }
    & "$tsplBuild/test_firmware.exe"
    if ($LASTEXITCODE -ne 0) { throw 'TSPL firmware integration tests failed' }
    & gcc @tsplFlags -DPRINTER_58MM -DENCODE_ENABLE -Itests/tspl/stubs -IPDL tests/tspl/test_native.c Code/TsplNative.c Code/TsplReceive.c Code/TsplRaster.c Code/TsplJob.c -o "$tsplBuild/test_native.exe"
    if ($LASTEXITCODE -ne 0) { throw 'TSPL native-port compilation failed' }
    & "$tsplBuild/test_native.exe"
    if ($LASTEXITCODE -ne 0) { throw 'TSPL native-port tests failed' }
    & gcc @tsplFlags tests/tspl/test_limits.c Code/TsplParser.c -o "$tsplBuild/test_limits.exe"
    if ($LASTEXITCODE -ne 0) { throw 'TSPL limit-test compilation failed' }
    & "$tsplBuild/test_limits.exe"
    if ($LASTEXITCODE -ne 0) { throw 'TSPL limit tests failed' }
    foreach ($tsplPair in @(@(1,0),@(0,1),@(0,0),@(1,1),@(2,0),@(0,-1))) {
        $tsplConfigArgs = @('-std=c99','-Wall','-Wextra','-Werror','-pedantic','-ICode',"-DTSPL_PROTOCOL=$($tsplPair[0])","-DESC_POS_PROTOCOL=$($tsplPair[1])",'tests/tspl/test_config.c','-o',"$tsplBuild/test_config.exe")
        $ErrorActionPreference = 'Continue'
        $tsplOutput = & gcc @tsplConfigArgs 2>&1
        $tsplConfigExit = $LASTEXITCODE
        $ErrorActionPreference = 'Stop'
        $tsplExpectedSuccess = (($tsplPair[0] -eq 1 -and $tsplPair[1] -eq 0) -or ($tsplPair[0] -eq 0 -and $tsplPair[1] -eq 1))
        if (($tsplConfigExit -eq 0) -ne $tsplExpectedSuccess) { throw "Unexpected protocol configuration result: $tsplPair $tsplOutput" }
    }
    Write-Output 'Protocol selection: 2 valid and 4 invalid configurations verified'
} finally { Pop-Location }
