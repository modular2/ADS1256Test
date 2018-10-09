#include "mbed.h"
#define RDATA 0x01
#define RDATAC 0x03
#define SDATAC 0x0F
#define SELFCAL 0xF0
#define SELFOCAL 0xF1
#define SELFGCAL 0xF2
#define SYSOCAL 0xF3
#define SYSGCAL 0xF4
#define SYNC 0xFC
#define ATANDBY 0xFD
#define RESET  0xFE
#define WAKEUP 0xFF
class ADS1256 {
	public:
		ADS1256(PinName mosi,PinName miso,PinName clk,PinName cs,PinName drdy,PinName rst,PinName sync); 
	uint32_t ReadData();
	uint32_t ReadDataContinuous(uint32_t * buffer,int samples);
	void WriteControl(uint8_t ctrl);
	void write_reg(uint8_t command,uint8_t data);
	uint8_t read_reg(uint8_t command);
	void SetPGAgain(uint8_t gain);
	void SlectChannel(uint8_t ainp,uint8_t ainn);
	bool  SensorDetected();
	private: 
	  uint8_t ReadDRDY();
	  void init();
   SPI _spi;
   DigitalIn _drdy;	
	 DigitalOut _cs;
   DigitalOut _rst;
   DigitalOut _sync;	
};

