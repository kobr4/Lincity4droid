LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

SDL_PATH := ../SDL
SDL_TTF_PATH := ../SDL_ttf
SDL_MIXER_PATH := ../SDL_mixer
SDL_IMAGE_PATH := ../SDL_image
SDL_GFX_PATH := ../SDL_gfx
PHYSFS_PATH := ../physfs
LIBXML_PATH := ../xml2
LIBICONV_PATH := ../libiconv-1.14

LOCAL_CFLAGS := -DDEBUG

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include \
                    $(LOCAL_PATH)/$(SDL_TTF_PATH) \
                    $(LOCAL_PATH)/$(SDL_MIXER_PATH) \
                    $(LOCAL_PATH)/$(SDL_IMAGE_PATH) \
                    $(LOCAL_PATH)/$(SDL_GFX_PATH) \
                    $(LOCAL_PATH)/$(PHYSFS_PATH) \
                    $(LOCAL_PATH)/$(LIBXML_PATH)/include \
                    src/lincity/ \
                    src/include/

# Add your application source files here...
#LOCAL_SRC_FILES := $(SDL_PATH)/src/main/android/SDL_android_main.cpp \
#        $(wildcard lincity-ng/*.c) \
#	$(wildcard lincity-ng/*.cpp) \

LOCAL_SRC_FILES := $(SDL_PATH)/src/main/android/SDL_android_main.cpp \
lincity-ng/main.cpp \
lincity-ng/binreloc.c \
lincity-ng/ButtonInterface.cpp lincity-ng/ButtonPanel.cpp lincity-ng/CheckButton.cpp \
lincity-ng/Config.cpp \
lincity-ng/Dialog.cpp \
lincity-ng/EconomyGraph.cpp \
lincity-ng/ErrorInterface.cpp \
lincity-ng/Game.cpp \
lincity-ng/GameView.cpp \
lincity-ng/HelpWindow.cpp \
lincity-ng/MainLincity.cpp \
lincity-ng/MainMenu.cpp \
lincity-ng/MapEdit.cpp \
lincity-ng/MiniMap.cpp \
lincity-ng/Mps.cpp \
lincity-ng/MpsInterface.cpp \
lincity-ng/PBar.cpp \
lincity-ng/PbarInterface.cpp \
lincity-ng/readdir.cpp \
lincity-ng/ReadPngInterface.cpp \
lincity-ng/ScreenInterface.cpp \
lincity-ng/Sound.cpp \
lincity-ng/TimerInterface.cpp \
lincity-ng/Util.cpp \
gui/PainterSDL/PainterSDL.cpp \
gui/PainterSDL/TextureManagerSDL.cpp \
gui/PainterSDL/TextureSDL.cpp \
gui/XmlReader.cpp \
gui/Button.cpp \
gui/Child.cpp \
gui/Color.cpp \
gui/Component.cpp \
gui/ComponentFactory.cpp \
gui/ComponentLoader.cpp \
gui/Desktop.cpp \
gui/Document.cpp \
gui/DocumentImage.cpp \
gui/Event.cpp \
gui/FilledRectangle.cpp \
gui/Filter.cpp \
gui/FontManager.cpp \
gui/Gradient.cpp \
gui/Image.cpp \
gui/Panel.cpp \
gui/Paragraph.cpp \
gui/Rect2D.cpp \
gui/ScrollBar.cpp \
gui/ScrollView.cpp \
gui/Style.cpp \
gui/SwitchComponent.cpp \
gui/TableLayout.cpp \
gui/TextureManager.cpp \
gui/TooltipManager.cpp \
gui/Window.cpp \
tinygettext/findlocale.cpp \
tinygettext/TinyGetText.cpp \
PhysfsStream/PhysfsSDL.cpp \
PhysfsStream/PhysfsStream.cpp \
lincity/engglobs.cpp \
lincity/engine.cpp \
lincity/init_game.cpp \
lincity/lc_locale.cpp  \
lincity/lclib.cpp \
lincity/lintypes.cpp \
lincity/loadsave.cpp \
lincity/old_ldsvguts.cpp \
lincity/power.cpp \
lincity/simulate.cpp \
lincity/stats.cpp \
lincity/transport.cpp \
lincity/fileutil.cpp \
lincity/modules/blacksmith.cpp \
lincity/modules/coal_power.cpp \
lincity/modules/coalmine.cpp \
lincity/modules/commune.cpp \
lincity/modules/cricket.cpp \
lincity/modules/fire.cpp \
lincity/modules/firestation.cpp \
lincity/modules/health_centre.cpp \
lincity/modules/heavy_industry.cpp \
lincity/modules/light_industry.cpp \
lincity/modules/market.cpp \
lincity/modules/mill.cpp \
lincity/modules/monument.cpp \
lincity/modules/oremine.cpp \
lincity/modules/organic_farm.cpp \
lincity/modules/parkland.cpp \
lincity/modules/port.cpp \
lincity/modules/pottery.cpp \
lincity/modules/power_line.cpp \
lincity/modules/rail.cpp \
lincity/modules/recycle.cpp \
lincity/modules/residence.cpp \
lincity/modules/road.cpp \
lincity/modules/rocket_pad.cpp \
lincity/modules/school.cpp \
lincity/modules/shanty.cpp \
lincity/modules/solar_power.cpp \
lincity/modules/substation.cpp \
lincity/modules/tip.cpp \
lincity/modules/track.cpp \
lincity/modules/university.cpp \
lincity/modules/water.cpp \
lincity/modules/waterwell.cpp \
lincity/modules/windmill.cpp \
gui/PainterGL/PainterGL.cpp \
gui/PainterGL/TextureManagerGL.cpp \
gui/PainterGL/TextureGL.cpp \
#lincity-ng/Debug.cpp \
#	$(LOCAL_PATH)/lincity-ng/binreloc.c \

LOCAL_SHARED_LIBRARIES := SDL SDL_ttf SDL_image SDL_mixer SDL_gfx physfs xml2

LOCAL_LDLIBS := -lGLESv1_CM -llog -lz

include $(BUILD_SHARED_LIBRARY)