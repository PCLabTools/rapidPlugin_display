/**
 * @file rapidPlugin_display.h
 * @author Larry Colvin (PCLabTools@github)
 * @brief 
 * @version 0.1
 * @date 2023-10-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef rapidPlugin_display_h
#define rapidPlugin_display_h

#ifndef rapidPlugin_display_stack_size
#define rapidPlugin_display_stack_size 512
#endif

#include "rapidRTOS.h"

#include <Adafruit_GFX.h>

#include "display.h"

/**
 * @brief rapidPlugin for abstracting and simplifying running a display
 * that uses the AdafruitGFX library as the rendering agent to allow for
 * multiple display types to be used
 * 
 */
class rapidPlugin_display : public rapidPlugin
{
  public:
    rapidPlugin_display(const char* identity);
    BaseType_t run(time_t splash = 2000);
    BaseType_t runCore(BaseType_t core, time_t splash = 1000);
  
  private:
    static void main_loop(void*);
    time_t _splash = 0;
};

/**
 * @brief Construct a new rapidPlugin display::rapidPlugin display object
 * 
 * @param identity string literal containing the task name
 */
rapidPlugin_display::rapidPlugin_display(const char* identity)
{
  _pID = identity;
}

/**
 * @brief Runs the task
 * 
 * @param splash time in milliseconds to show splash screen
 * @return BaseType_t 1 = task run successful | 0 = task failed to start
 */
BaseType_t rapidPlugin_display::run(time_t splash)
{
  _splash = splash;
  return rapidPlugin::run(&main_loop, rapidPlugin_display_stack_size);
}

/**
 * @brief Runs the task on the specified core
 * 
 * @param core core ID
 * @param splash time in milliseconds to show splash screen
 * @return BaseType_t 1 = task run successful | 0 = task failed to start
 */
BaseType_t rapidPlugin_display::runCore(BaseType_t core, time_t splash)
{
  _splash = splash;
  return rapidPlugin::runCore(core, &main_loop, rapidPlugin_display_stack_size);
}

#ifndef rapidPlugin_display_override_main_loop
/**
 * @brief Main processing loop responsible for rendering the canvas object
 * onto the target display object. The task delay directly affects the screen
 * refresh rate
 * 
 * @param pModule 
 */
void rapidPlugin_display::main_loop(void* pModule)
{
  rapidPlugin_display* plugin = (rapidPlugin_display*)pModule;

  #if defined(BOARD_ESP32)
  Wire.setPins(OLED_WIRE_SDA, OLED_WIRE_SCL);
  #else
  Wire.setSCL(OLED_WIRE_SCL);
  Wire.setSDA(OLED_WIRE_SDA);
  #endif
  if (!displayObject.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    for (;;)
    {
      rapidRTOS.printDebug(0, rapidDebug::ERROR, "Cannot initialise SSD1306\n", NULL);
      delay(1000);
    }
  }
  displayObject.clearDisplay();
  if (plugin->_splash)
  {
    splashScreen();
    displayObject.drawBitmap(0, 0,  canvas.getBuffer(), SCREEN_WIDTH, SCREEN_HEIGHT, DISPLAY_WHITE, DISPLAY_BLACK);
    displayObject.display();
    delay(plugin->_splash);
  }
  displayObject.clearDisplay();
  for ( ;; )
  {
    if (canvasController()) 
    {
      displayObject.drawBitmap(0, 0, canvas.getBuffer(), SCREEN_WIDTH, SCREEN_HEIGHT, DISPLAY_WHITE, DISPLAY_BLACK);
      displayObject.display(); 
    }
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}
#endif // rapidPlugin_display_override_main_loop

#endif // rapidPlugin_display_h