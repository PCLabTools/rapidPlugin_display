/**
 * @file rapidPlugin_displayCanvas.h
 * @author Larry Colvin (PCLabTools@github)
 * @brief 
 * @version 0.1
 * @date 2023-10-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef SCREEN_WIDTH
#define SCREEN_WIDTH 128
#endif

#ifndef SCREEN_HEIGHT
#define SCREEN_HEIGHT 64
#endif

#ifndef DISPLAY_BLACK
#define DISPLAY_BLACK 0
#endif

#ifndef DISPLAY_WHITE
#define DISPLAY_WHITE 1
#endif

GFXcanvas1 canvas(SCREEN_WIDTH, SCREEN_HEIGHT);

/**
 * @brief Class used to provide drawing functions for the canvas that
 * extend the already comprehensive Adafruit GFXcanvas functions
 * 
 */
class displayCanvas
{
  public:
    static void progressBar(int percentage, int16_t width, int16_t height, int16_t x_pos, int16_t y_pos, bool scale = true);
    static void analogClock(time_t time, int16_t size, int16_t x_pos, int16_t y_pos, bool scale = true);
    static void tank(float min, float max, float value, int16_t width, int16_t height, int16_t x_pos, int16_t y_pos, bool scale = true);
    static void gauge(float min, float max, float value, int16_t radius, int16_t x_pos, int16_t y_pos, bool scale = true);
    static void gauge(int min, int max, int value, int16_t radius, int16_t x_pos, int16_t y_pos, bool scale = true);
    static void lineGraph(float* array, size_t length, int16_t width, int16_t height, int16_t x_pos, int16_t y_pos, bool scale = true);
    static void lineGraph(int* array, size_t length, int16_t width, int16_t height, int16_t x_pos, int16_t y_pos, bool scale = true);
    static void barGraph(float* array, size_t length, int16_t width, int16_t height, int16_t x_pos, int16_t y_pos, bool scale = true);
    static void barGraph(int* array, size_t length, int16_t width, int16_t height, int16_t x_pos, int16_t y_pos, bool scale = true);
    template<typename... Args> static void textBox(const char* text, int16_t x_pos, int16_t y_pos, int16_t thickness = 1, int16_t padding = 1, Args... args);
    static void symbol(uint16_t symbol_type, uint16_t x_pos, uint16_t y_pos);
  
  private:
    //
};

/**
 * @brief Draws a progress bar (horizontal) which represents a percentage value
 * 
 * @param percentage current value
 * @param width width of progress bar in pixels
 * @param height height of progress bar in pixels
 * @param x_pos top left position x coordinate
 * @param y_pos top left position y coordinate
 * @param scale include scale (default: true)
 */
void displayCanvas::progressBar(int percentage, int16_t width, int16_t height, int16_t x_pos, int16_t y_pos, bool scale)
{
  const int x1 = x_pos;
  const int x2 = x_pos + width - 2;
  const int y1 = y_pos;
  const int y2 = y_pos + height - 2;
  //draw borders
  canvas.drawLine(x1 + 1, y1, x2 - 1, y1, DISPLAY_WHITE);
  canvas.drawLine(x1 + 1, y2, x2 - 1, y2, DISPLAY_WHITE);
  canvas.drawLine(x1, y1 + 1, x1, y2 - 1, DISPLAY_WHITE);
  canvas.drawLine(x2, y1 + 1, x2, y2 - 1, DISPLAY_WHITE);
  // calc width and draw progress
  const int innerWidth = (round(x2 -x1 - 2) * percentage / 100);
  canvas.fillRect(x1 + 2, y1 + 2, innerWidth, (y2 - y1 - 3), DISPLAY_WHITE);
  if (scale)
  {
    int textWidth = percentage < 10 ? 6 : 9;
    int numChars = percentage < 10 ? 2 : 3;
    canvas.setCursor(((canvas.width() / 2) - textWidth), (y2 - ((y2 - y1) / 2) - 4));
    char progressIndicator[3];
    sprintf(progressIndicator, "%d%%", percentage);
    for (int i = 0 ; i <= numChars ; i++)
    {
      if (canvas.getCursorX() < x1 + innerWidth)
      {
        canvas.setTextColor(DISPLAY_BLACK);
      }
      else
      {
        canvas.setTextColor(DISPLAY_WHITE);
      }
      canvas.print(progressIndicator[i]);
    }
    canvas.setTextColor(DISPLAY_WHITE);
  }
}

/**
 * @brief Draws an analog clock on the display with hour, minute and second hands
 * 
 * @param time current time (epoch)
 * @param size radius of the clock in pixels
 * @param x_pos center position x coordinate
 * @param y_pos center position y coordinate
 * @param scale include scale (default: true)
 */
void displayCanvas::analogClock(time_t time, int16_t size, int16_t x_pos, int16_t y_pos, bool scale)
{
  float rad2deg = 57.29577951;
  time_t rawTime = time;
  struct tm* timeInfo = localtime(&rawTime);
  int radius = size;
  canvas.drawCircle(x_pos, y_pos, 2, DISPLAY_WHITE);
  // draw clock
  for (int i = 0 ; i <360; i = i + 30)
  {
    float angle = i;
    angle = angle / rad2deg;
    int x1 = (x_pos + (sin(angle) * radius));
    int y1 = (y_pos - (cos(angle) * radius));
    int x2 = (x_pos + (sin(angle) * (radius - 5)));
    int y2 = (y_pos - (cos(angle) * (radius - 5)));
    canvas.drawLine(x1, y1, x2, y2, DISPLAY_WHITE);
  }
  // draw second hand
  float angle = timeInfo->tm_sec * 6;
  angle = angle / rad2deg;
  int x2 = (x_pos + (sin(angle) * radius));
  int y2 = (y_pos - (cos(angle) * radius));
  canvas.drawLine(x_pos, y_pos, x2, y2, DISPLAY_WHITE);
  // draw minute hand
  angle = timeInfo->tm_min * 6;
  angle = angle / rad2deg;
  x2 = (y_pos + (sin(angle) * (radius - 3)));
  y2 = (y_pos - (cos(angle) * (radius - 3)));
  canvas.drawLine(x_pos, y_pos, x2, y2, DISPLAY_WHITE);
  // draw hour hand
  angle = timeInfo->tm_hour * 30 + int((timeInfo->tm_min / 12) * 6);
  angle = angle / rad2deg;
  x2 = (x_pos + (sin(angle) * (radius - 11)));
  y2 = (y_pos - (cos(angle) * (radius - 11)));
  canvas.drawLine(x_pos, y_pos, x2, y2, DISPLAY_WHITE);
}

/**
 * @brief Draws a vertical tank slider which can be used to represent
 * positions, progress, gauges, etc
 * 
 * @param min minimum value
 * @param max maximum value
 * @param value current value
 * @param width width of tank in pixels
 * @param height height of tank in pixels
 * @param x_pos top left position x coordinate
 * @param y_pos top left position y coordinate
 * @param scale include scale (default: true)
 */
void displayCanvas::tank(float min, float max, float value, int16_t width, int16_t height, int16_t x_pos, int16_t y_pos, bool scale)
{
  const int x1 = x_pos;
  const int x2 = x_pos + width - 2;
  const int y1 = y_pos;
  const int y2 = y_pos + height - 2;
  // draw borders
  canvas.drawLine(x1 + 1, y1, x2 - 1, y1, DISPLAY_WHITE);
  canvas.drawLine(x1 + 1, y2, x2 - 1, y2, DISPLAY_WHITE);
  canvas.drawLine(x1, y1 + 1, x1, y2 - 1, DISPLAY_WHITE);
  canvas.drawLine(x2, y1 + 1, x2, y2 - 1, DISPLAY_WHITE);
  // calculate height and draw tank filler
  const int fill_height = (round(y2 - y1 - 2) * value / (max - min));
  const int offset = height - fill_height - 5;
  canvas.fillRect(x1 + 2, y1 + 2 + offset, (x2 -x1 - 3), fill_height, 1);
  // draw scale indicator if enabled
  if (scale)
  {
    canvas.setCursor(x_pos + width + 1, y_pos);
    canvas.printf("%.3g", max);
    int text_offset = y1 + 2 + offset - 4;
    if (text_offset - y_pos < 8)
    {
      text_offset = y_pos + 8;
    }
    else if (y_pos + height - text_offset < 16)
    {
      text_offset = y_pos + height - 16;
    }
    canvas.setCursor(x_pos + width + 1, text_offset);
    canvas.printf("%.3g", value);
    canvas.setCursor(x_pos + width + 1, y_pos + height - 8);
    canvas.printf("%.3g", min);
  }
}

/**
 * @brief Draws a gauge on the display which can be used to represent
 * various numerical output (e.g. voltage reading or speed)
 * 
 * @param min minimum value
 * @param max maximum value
 * @param value current value
 * @param radius radius of the gauge
 * @param x_pos center position x coordinate
 * @param y_pos center position y coordinate
 * @param scale include scale (default: true)
 */
void displayCanvas::gauge(float min, float max, float value, int16_t radius, int16_t x_pos, int16_t y_pos, bool scale)
{
  canvas.drawCircleHelper(x_pos, y_pos, radius, 1, DISPLAY_WHITE);
  canvas.drawCircleHelper(x_pos, y_pos, radius, 2, DISPLAY_WHITE);
  canvas.drawPixel(x_pos, y_pos - radius, DISPLAY_WHITE);
  canvas.drawCircle(x_pos, y_pos - 1, 4, DISPLAY_WHITE);
  canvas.drawLine(x_pos - radius, y_pos, x_pos + radius, y_pos, DISPLAY_WHITE);
  // calculate angle of gauge line
  float angle = (3.1415 * (value / max)) - 1.5787;
  int x2 = x_pos + (sin(angle) * radius) + 1;
  int y2 = (y_pos - 1 - (cos(angle) * radius)) + 1;
  canvas.drawLine(x_pos, y_pos, x2, y2, DISPLAY_WHITE);
  // draw scale indicator
  if (scale)
  {
    canvas.setCursor(x_pos - radius + 4, y_pos - 8);
    canvas.printf("%.3g", min);
    canvas.setCursor(x_pos + radius - 20, y_pos - 8);
    canvas.printf("%.3g", max);
    canvas.setCursor(x_pos - 4, y_pos - (radius / 2) - 8);
    canvas.printf("%.3g", value);
  }
}

/**
 * @brief Draws a gauge on the display which can be used to represent
 * various numerical output (e.g. voltage reading or speed)
 * 
 * @param min minimum value
 * @param max maximum value
 * @param value current value
 * @param radius radius of the gauge
 * @param x_pos center position x coordinate
 * @param y_pos center position y coordinate
 * @param scale include scale (default: true)
 */
void displayCanvas::gauge(int min, int max, int value, int16_t radius, int16_t x_pos, int16_t y_pos, bool scale)
{
  displayCanvas::gauge((float) min, (float) max, (float) value, radius, x_pos, y_pos, scale);
}

/**
 * @brief Draws a line graph on the display that can be used
 * to represent an array of values over time. The graph is auto
 * scaled to adjust minimum and maximum values to best use the space
 * on the graph
 * 
 * @param array array of values
 * @param length length of the array
 * @param width width of graph in pixels
 * @param height height of graph in pixels
 * @param x_pos top left position x coordinate
 * @param y_pos top left position y coordinate
 * @param scale include scale (default: true)
 */
void displayCanvas::lineGraph(float* array, size_t length, int16_t width, int16_t height, int16_t x_pos, int16_t y_pos, bool scale)
{
  int16_t x1 = x_pos;
  int16_t y1 = y_pos;
  const int16_t scale_offset = 24;
  float min_value = array[0];
  float max_value = array[0];
  float delta_x = width / (length - 1);
  if (scale)
  {
    x1 = x1 + scale_offset;
    delta_x = (width - scale_offset) / (length - 1);
  }
  for( int i = 0 ; i < length ; i++)
  {
    if (array[i] < min_value) { min_value = array[i]; }
    if (array[i] > max_value) { max_value = array[i]; }
  }
  canvas.drawLine(x1, y_pos, x1, y_pos + height - 1, DISPLAY_WHITE);
  canvas.drawLine(x1, y_pos + height - 1, x1 + width, y_pos + height - 1, DISPLAY_WHITE);
  for (int i = 0 ; i < length - 1 ; i++)
  {
    canvas.drawLine(x1 + (i * delta_x), y_pos + height - ((array[i]/(max_value - min_value)) * height), x1 + (i * delta_x) + delta_x, y_pos + height - ((array[i + 1] / (max_value - min_value)) * height), DISPLAY_WHITE);
  }
  if (scale)
  {
    canvas.setCursor(x1 - scale_offset, y_pos);
    canvas.printf("%.3g", 4, max_value);
    canvas.setCursor(x1 - scale_offset, y_pos + height - 8);
    canvas.printf("%.3g", 4, min_value);
  }
}

/**
 * @brief Draws a line graph on the display that can be used
 * to represent an array of values over time. The graph is auto
 * scaled to adjust minimum and maximum values to best use the space
 * on the graph
 * 
 * @param array array of values
 * @param length length of the array
 * @param width width of graph in pixels
 * @param height height of graph in pixels
 * @param x_pos top left position x coordinate
 * @param y_pos top left position y coordinate
 * @param scale include scale (default: true)
 */
void displayCanvas::lineGraph(int* array, size_t length, int16_t width, int16_t height, int16_t x_pos, int16_t y_pos, bool scale)
{
  float float_array[length];
  for (int i = 0 ; i < length ; i++)
  {
    float_array[i] = (float)array[i];
  }
  displayCanvas::lineGraph(float_array, length, width, height, x_pos, y_pos, scale);
}

/**
 * @brief Draws a bar graph on the display to help
 * present quantities
 * 
 * @param array array of values
 * @param length length of array
 * @param width width of graph in pixels
 * @param height height of graph in pixels
 * @param x_pos top left position x coordinate
 * @param y_pos top left position y coordinate
 * @param scale include scale (default: true)
 */
void displayCanvas::barGraph(float* array, size_t length, int16_t width, int16_t height, int16_t x_pos, int16_t y_pos, bool scale)
{
  int16_t x1 = x_pos;
  int16_t y1 = y_pos;
  const int16_t scale_offset = 24;
  float min_value = array[0];
  float max_value = array[0];
  float delta_x = width / (length - 1);
  if (scale)
  {
    x1 = x1 + scale_offset;
    delta_x = (width - scale_offset) / (length - 1);
  }
  for( int i = 0 ; i < length ; i++)
  {
    if (array[i] < min_value) { min_value = array[i]; }
    if (array[i] > max_value) { max_value = array[i]; }
  }
  canvas.drawLine(x1, y_pos, x1, y_pos + height - 1, DISPLAY_WHITE);
  canvas.drawLine(x1, y_pos + height - 1, x1 + width, y_pos + height - 1, DISPLAY_WHITE);
  for (int i = 0 ; i < length - 1 ; i++)
  {
    canvas.fillRect(x1 + (i * delta_x), y_pos + height - ((array[i]/(max_value - min_value)) * height), delta_x, (array[i + 1] / (max_value - min_value)) * height, DISPLAY_WHITE);
  }
  if (scale)
  {
    canvas.setCursor(x1 - scale_offset, y_pos);
    canvas.printf("%.3g", 4, max_value);
    canvas.setCursor(x1 - scale_offset, y_pos + height - 8);
    canvas.printf("%.3g", 4, min_value);
  }
}

/**
 * @brief Draws a bar graph on the display to help
 * present quantities
 * 
 * @param array array of values
 * @param length length of array
 * @param width width of graph in pixels
 * @param height height of graph in pixels
 * @param x_pos top left position x coordinate
 * @param y_pos top left position y coordinate
 * @param scale include scale (default: true)
 */
void displayCanvas::barGraph(int* array, size_t length, int16_t width, int16_t height, int16_t x_pos, int16_t y_pos, bool scale)
{
  float float_array[length];
  for (int i = 0 ; i < length ; i++)
  {
    float_array[i] = (float)array[i];
  }
  displayCanvas::barGraph(float_array, length, width, height, x_pos, y_pos, scale);
}

/**
 * @brief Draws a text box on the screen and adjust size
 * based on the text that is entered into the text box.
 * The text can be modified using standard escape characters
 * to allow for flexible use of the text box
 * 
 * @tparam Args any number of arguments
 * @param text string literal to display in the text box
 * @param x_pos top left position x coordinate
 * @param y_pos top left position y coordinate
 * @param thickness thickness of border in pixels (default: 1)
 * @param padding thickness of padding in pixels (default: 1)
 * @param args argument parameters to override escape characters in the string
 */
template<typename... Args>
void displayCanvas::textBox(const char* text, int16_t x_pos, int16_t y_pos, int16_t thickness, int16_t padding, Args... args)
{
  char formattedText[256];
  sprintf(formattedText, text, args...);
  const int x1 = x_pos;
  const int y1 = y_pos;
  int num_chars = 0;
  int prev_lf_index = 0;
  int largest_line = 0;
  int current_line = 0;
  canvas.setCursor(x1 + padding + thickness, y1 + padding + thickness);
  for (int i = 0 ; i < strlen(formattedText) ; i++ )
  {
    canvas.print(formattedText[i]);
    if (formattedText[i] == '\n' || i == strlen(formattedText) - 1)
    {
      num_chars = i - prev_lf_index;
      if (num_chars > largest_line) { largest_line = num_chars; }
      prev_lf_index = i + 1;
      current_line++;
      canvas.setCursor(x1 + padding + thickness, y1 + padding + (current_line * 8) + thickness);
    }
  }
  int text_length = largest_line * 6;
  const int x2 = x_pos + text_length + (padding * 2) + thickness - 1;
  const int y2 = y_pos + (8 * current_line) + (padding * 2) + thickness - 1;
  canvas.fillRect(x1, y1, x2 - x1 + thickness, thickness, DISPLAY_WHITE);
  canvas.fillRect(x1, y2, x2 - x1 + thickness, thickness, DISPLAY_WHITE);
  canvas.fillRect(x1, y1, thickness, y2 - y1, DISPLAY_WHITE);
  canvas.fillRect(x2, y1, thickness, y2 - y1, DISPLAY_WHITE);
}