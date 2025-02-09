<h1>Config.json - настройки конфигурации</h1> <br>

>[!WARNING]
>Конфиг пока что не парсится, в ближайших версиях допилю



**Конфиг по умолчанию**
```json
"Config":{
  "VoiceRecognizer":{
    "PocketSphinx_model_path": "/usr/share/pocketsphinx/model/en-us/",
    "VoiceRecognizer_Type": "Python",
    "NaturalLanguage_Type": "en-us"
  }
  "EngineWorkMode":{
    "EngineDebug_Mode": false,
    "MaxOptimisation_Mode": false,
    "DebugShell_Type": "bash",
    "ConsoleEmulator_Type": "gnometerm",
    "MultithreadWorkEngine_Mode": true,
    "BuildVer_Type": "Python"   
 }
  "ScreenProcessing":{
    "XDG_SESSION_Type":"x11",
    "IgnoringWindows_List": "NO",
    "IgnoreMyScreen_Mode": false
    "TresholdFrameProcessing_Time": 1000.0,
    "InactivityTimeTreshold": 600,
    "CheckIntervalTime": 60
  }
  "Logging":{
    "LogLevel_Mode": "zero",
    "PrivateLogs_Mode": true,
    "LogToFile_Mode": false,
    "LogToConsole_Mode": false
  }

}
```

**Очевидно что**
- `VoiceRecognizer` - конфиг распознавателя голоса
- `EngineWorkMode` - конфиг работы движка
- `ScreenProcessing` - конфиг обработчика экрана
- `Logging` - конфиг логгера

<h2>VoiceRecognizer</h2>

Настройки распознавателя голоса имеют переменные конфигуратора `PocketSphinx_model_path`, `VoiceRecognizer_Type`, `NaturalLanguage_Type`.
Их не так много и они интуитивно понятны.
`PocketSphinx_model_path` - Путь к PocketSphinx модели если вы используете `C` распознаватель голоса 
`VoiceRecognizer_Type` - Выбор распознавателя голоса, имеет только два правильных варианта `Python` и `C`
