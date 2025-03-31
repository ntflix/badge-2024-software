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

ASSET_PATH = "./apps/sasppu_test_cart/"

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
        self.bg0 = sasppu.Background()
        self.bg0.bind(0)
        self.init_bg0_map()
        self.init_sprites()
        self.ms.mainscreen_colour = BLACK
        #self.ms.bgcol_windows = sasppu.WINDOW_A
        self.ms.flags = sasppu.MainState.BG0_ENABLE | sasppu.MainState.SPR0_ENABLE
        sasppu.draw_text_background(90, 90, WHITE, 150, "Hello, World!", True)
        with open(ASSET_PATH + "bg.bin", "rb") as f:
            sasppu.blit_background(0, 0, 256, 256, f.read())
        with open(ASSET_PATH + "spr.bin", "rb") as f:
            sasppu.blit_sprite(0, 0, 32*8, 32, f.read())

    def init_bg0_map(self):
        for i in range(sasppu.MAP_WIDTH * sasppu.MAP_HEIGHT):
            if ((i % 64) >= 32):
                if (((i / 64) % 64) >= 32):
                    sasppu.bg0[i] = int((31 - (i % 32)) * 8 + ((31 - (int(i / 64) % 32)) * 256 * 8)) | 0b11
                else:
                    sasppu.bg0[i] = int((31 - (i % 32)) * 8 + ((int(i / 64) % 32) * 256 * 8)) | 0b01
            else:
                if (((i / 64) % 64) >= 32):
                    sasppu.bg0[i] = int((i % 32) * 8 + ((31 - (int(i / 64) % 32)) * 256 * 8)) | 0b10
                else:
                    sasppu.bg0[i] = int((i % 32) * 8 + ((int(i / 64) % 32) * 256 * 8)) | 0b00

    def init_sprites(self):
        self.sprites = []
        for i in range(32):
            spr = sasppu.oam[i]
            spr.width = 32
            spr.height = 32
            spr.graphics_x = (i % 8) * 32
            spr.graphics_y = 0
            spr.windows = sasppu.WINDOW_ALL
            spr.flags = spr.ENABLED #| spr.DOUBLE | spr.FLIP_Y | spr.FLIP_X
            self.sprites.append(spr)

    def move_sprites(self, i):
        for x, spr in enumerate(self.sprites):
            spr.x = int((math.sin(float(i) / (10.0 + float(x))) * ((240 - 16) / 2.0)) + 120.0)
            spr.y = int((math.cos(float(i) / (13.0 + float(x))) * ((240 - 16) / 2.0)) + 120.0)

    def _cleanup(self):
        eventbus.remove(ButtonDownEvent, self._handle_buttondown, self)
        self.exit = True
        self.minimise()

    async def run(self, render_update):
        count = 0
        while not self.exit:

            self.move_sprites(count)
            self.bg0.x = count
            self.bg0.y = int(count / 2)

            count += 1
            await render_update()
            #await asyncio.sleep(1)

    def draw(self):
        cur_time = time.ticks_ms()

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
            