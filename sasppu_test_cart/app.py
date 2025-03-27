from app import SASPPUApp
import sasppu
import math
import time
import display

from events.input import BUTTON_TYPES, ButtonDownEvent
from system.eventbus import eventbus
from system.scheduler.events import RequestStopAppEvent

WHITE = 0b0111111111111111
RED = 0b0111110000000000
GREEN = 0b0000001111100000
BLUE = 0b0000000000011111
GREY = 0b0100001000010000

class SASPPUTest(SASPPUApp):
    def __init__(self):
        super().__init__()
        self.request_fast_updates = True
        eventbus.on(ButtonDownEvent, self._handle_buttondown, self)
        self.exit = False

    def _cleanup(self):
        eventbus.remove(ButtonDownEvent, self._handle_buttondown, self)
        self.exit = True
        self.minimise()

    async def run(self, render_update):
        while not self.exit:
            await render_update()

    def draw(self):
        cur_time = time.ticks_ms()
        gl = sasppu.Global()
        ms = gl.main_state
        ms.mainscreen_colour = WHITE
        ms.bgcol_windows = sasppu.Windows.A
        ms.flags = sasppu.MainState.CMATH_ENABLE
        #ms.window_1_left = int((math.sin(cur_time / 1300.0) + 1) * 64)
        #ms.window_1_right = int((math.cos(cur_time / 1500.0) + 3) * 64)
        gl.main_state = ms
        cs = gl.cmath_state
        cs.flags = sasppu.CMathState.FADE_ENABLE
        cs.fade = int((math.sin(cur_time / 1000.0) + 1) * 127)
        gl.cmath_state = cs
        print("fps:", display.get_fps())

    def _handle_buttondown(self, event: ButtonDownEvent):
        if BUTTON_TYPES["CANCEL"] in event.button:
            self._cleanup()

    def minimise(self):
        # Close this app each time
        eventbus.emit(RequestStopAppEvent(self))
            