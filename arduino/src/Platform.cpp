#include "GarStickS3.h"
#include "gar_board_config.h"
#include <Arduino.h>
#include <M5Unified.h>
#include <algorithm>
#include <utility>
#include <lvgl.h>

namespace gar::sticks3 {
namespace {
Hooks callbacks;
Hardware facts;
lv_disp_buf_t drawBuffer;
lv_color_t pixels[gar_board::display::width*16];
uint32_t clockAt=0,batteryAt=0;
bool usbConnected=false;

void flush(lv_disp_drv_t* driver,const lv_area_t* area,lv_color_t* colors) {
  M5.Display.pushImage(area->x1,area->y1,area->x2-area->x1+1,area->y2-area->y1+1,
                       reinterpret_cast<lgfx::rgb565_t*>(colors));
  lv_disp_flush_ready(driver);
}
void button(m5::Button_Class& input,char key) {
  if(input.wasPressed())callbacks.button(key,1);
  if(input.wasReleased())callbacks.button(key,0);
}
void readUsb() {
  char bytes[256];
  for(int i=0;i<4 && Serial.available();++i) {
    size_t count=0;
    while(count<sizeof(bytes) && Serial.available())bytes[count++]=static_cast<char>(Serial.read());
    callbacks.receive(Link::usb,bytes,count);
  }
}

}
void begin(const Options& options,Hooks hooks) {
  callbacks=std::move(hooks);
  Serial.begin(gar_board::usb::baud);
  auto config=M5.config();config.internal_mic=false;config.internal_spk=false;
  M5.begin(config); // M5Unified detects StickS3; never force a Plus2 fallback.
  M5.Display.setRotation(options.rotation);M5.Display.setBrightness(options.brightness);M5.Display.fillScreen(TFT_BLACK);
  facts.battery=std::max(0,static_cast<int>(M5.Power.getBatteryLevel()));
  facts.width=M5.Display.width();facts.height=M5.Display.height();facts.board=static_cast<int>(M5.getBoard());
  lv_init();lv_disp_buf_init(&drawBuffer,pixels,nullptr,gar_board::display::width*16);
  lv_disp_drv_t driver;lv_disp_drv_init(&driver);driver.hor_res=facts.width;driver.ver_res=facts.height;
  driver.buffer=&drawBuffer;driver.flush_cb=flush;lv_disp_drv_register(&driver);
  clockAt=millis();
}
void poll() {
  const auto now=millis();lv_tick_inc(now-clockAt);clockAt=now;
  M5.update();
  const bool connected=static_cast<bool>(Serial);
  if(connected!=usbConnected){usbConnected=connected;callbacks.reset(Link::usb,false);}
  readUsb();
  button(M5.BtnA,'A');button(M5.BtnB,'B');
  if(now-batteryAt>=10000) {
    batteryAt=now;facts.battery=std::max(0,static_cast<int>(M5.Power.getBatteryLevel()));callbacks.hardware(facts);
  }
}
Hardware hardware() { return facts; }
void send(Link link,const std::string& bytes) {
  if(link==Link::usb)Serial.write(reinterpret_cast<const uint8_t*>(bytes.data()),bytes.size());

}
}
