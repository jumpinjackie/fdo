echo %1
..\..\..\..\thirdparty\util\stampver\StampVer.exe -v"..\..\..\SubDoc\Providers.version" -f"0.8.12.0%PROVIDERS_VERSION%" %1\KingOracleProvider.dll
..\..\..\..\thirdparty\util\stampver\StampVer.exe -v"..\..\..\SubDoc\Providers.version" -f"0.8.12.0%PROVIDERS_VERSION%" %1\KingOracleOverrides.dll
..\..\..\..\thirdparty\util\stampver\StampVer.exe -v"..\..\..\SubDoc\Providers.version" -f"0.8.12.0%PROVIDERS_VERSION%" %1\KingOracleMessage.dll

