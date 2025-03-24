from . import _engine

# Экспорт функций из  _engine
initEngine = _engine.initEngine
cleanupEngine = _engine.cleanupEngine
renderRectangle = _engine.renderRectangle
renderImage = _engine.renderImage
renderGIF = _engine.renderGIF
startRenderingThread = _engine.startRenderingThread
stopRenderingThread = _engine.stopRenderingThread
startRecordingThread = _engine.startRecordingThread
stopRecordingThread = _engine.stopRecordingThread
setPrompt = _engine.setPrompt
