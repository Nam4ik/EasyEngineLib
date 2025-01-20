#include <pybind11/pybind11.h>
#include "engine.h"

namespace py = pybind11;

PYBIND11_MODULE(engine, m) {
    m.def("initEngine", &initEngine, "Initialize the engine");
    m.def("cleanupEngine", &cleanupEngine, "Cleanup the engine");
    m.def("renderRectangle", &renderRectangle, "Render a rectangle");
    m.def("renderImage", &renderImage, "Render an image");
    m.def("renderGif", &renderGIF, "Render a GIF");
//   m.def("startRecordingThread", &startRenderingThread, "Startring thread for async work with engine")
//   m.def("stopRecordingThread", &stopRenderingThread, "Stopping rec thread")
//   m.def("startRenderingThread", &startRecordingThread, "Starting new SDL renderer")
//   m.def("stopRenderingThread", &stopRenderingThread, "Stop SDL renderer"))
}
