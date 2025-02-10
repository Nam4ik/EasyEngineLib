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
Их не так много и они интуитивно понятны.  <br>
- `PocketSphinx_model_path` - Путь к PocketSphinx модели если вы используете `C` распознаватель голоса <br>  
- `VoiceRecognizer_Type` - Выбор распознавателя голоса, имеет только два правильных варианта `Python` и `C` <br>
- `NaturalLanguage_Type` - Выбор натурального языка, поддерживает только en-us и ru-ru

<h2>EngineWorkMode</h2>

Настройки основной работы движка. Переменные конфигуратора - `EngineDebug_Mode`, `MaxOptimisation_Mode`, `DebugShell_Type`, `ConsoleEmulator_Type`, `MultithreadWorkEngine_Mode` и `BuildVer_Type`.
- `EngineDebug_Mode` - Логическая переменная, включает дебаг режим для.
- `MaxOptimisation_Mode` - Включает режим оптимизации движка чтобы запустилось на любом ведре
- `DebugShell_Type` - Окружение в котором будет выводится логи, например `/usr/bin/zsh` и тп. Как правило выбирает bash автоматически.
- `ConsoleEmulator_Type` - Эмулятор консоли в котором хотите видеть логи, например `kitty`, `powershell`, `gnometerm` и др.
- `MultithreadWorkEngine_Mode` - Движок может работать однопоточно, но используя многопотчную оптимизацию он будет гораздо быстрее и практичнее. Следователно это логическая переменная.
- `BuildVer_Type` - Вариант поставки движка, `Python`, `GUI` или `Native`.

<h2>ScreenProcessing</h2>

- `XDG_SESSION_Type` - `Win`, `x11` или `wayland` (Эксперементальная поддержка)
- `IgnoringWindows_List` - названия приложений/приложения которое следует игнорировать обработчику экрана __(Не смотря на то что кроме математических вычислений для находки схожих кадров он не используется, чтоб параноики не переживали)__. Никаких предупреждений, ошибок в случае неправильного написания не будет.
- `IgnoreMyScreen_Mode` - абсолютное игнорирование экрана, если в проекте будет ссылка на ваш экран то обработчик ее проигнорирует. Логическая переменная.
- `TresholdFrameProcessing_Time` - если прошлая переменная `false` то задержка сверки кадров в милисекнуду.
- `InactivityTimeTreshold` - Время при котором вы считаетесь не активным в секундах, нужно для некоторых функций движка обрабатывающих экран __(Например для реализации некоторых функций связанных с отсутвием активности, вы можете его отключить как я упоминал раннее)__
<h2>Logging</h2>
- `LogLevel_Mode` - имеет значения `zero`, `basic`, `debug`
