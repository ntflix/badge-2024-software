import asyncio
from app import SASPPUApp
import sasppu
import math
import time
import display

from events.input import BUTTON_TYPES, ButtonDownEvent
from system.eventbus import eventbus
from system.scheduler.events import RequestStopAppEvent

BLACK = 0
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
        self.ms = sasppu.MainState()
        self.ms.bind()
        self.cs = sasppu.CMathState()
        self.cs.bind()
        self.init_bg0_map()
        sasppu.draw_text_background(90, 90, WHITE, 150, "Hello, World!", True)

    def init_bg0_map(self):
        for x in range(sasppu.MAP_WIDTH):
            for y in range(sasppu.MAP_HEIGHT):
                index = ((y % 32) * 256 * 8) + ((x % 32) * 8)
                sasppu.sasppuinternal_bg0_map(x, y, index)

    def _cleanup(self):
        eventbus.remove(ButtonDownEvent, self._handle_buttondown, self)
        self.exit = True
        self.minimise()

    async def run(self, render_update):
        while not self.exit:
            await render_update()
            #await asyncio.sleep(1)

    def draw(self):
        cur_time = time.ticks_ms()
        self.ms.mainscreen_colour = BLACK
        self.ms.bgcol_windows = sasppu.WINDOW_A
        self.ms.flags = sasppu.MainState.BG0_ENABLE
        #self.ms.flags = sasppu.MainState.CMATH_ENABLE
        #ms.window_1_left = int((math.sin(cur_time / 1300.0) + 1) * 64)
        #ms.window_1_right = int((math.cos(cur_time / 1500.0) + 3) * 64)
        #self.cs.flags = sasppu.CMathState.FADE_ENABLE
        #self.cs.fade = int((math.sin(cur_time / 1000.0) + 1) * 127)
        print("fps:", display.get_fps())

    def _handle_buttondown(self, event: ButtonDownEvent):
        if BUTTON_TYPES["CANCEL"] in event.button:
            self._cleanup()

    def minimise(self):
        # Close this app each time
        eventbus.emit(RequestStopAppEvent(self))
            