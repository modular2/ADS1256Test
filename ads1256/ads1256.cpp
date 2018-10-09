#include "mbed.h"
#include "ads1256.h"
ADS1256::ADS1256(PinName mosi,PinName miso,PinName clk,PinName cs,PinName drdy,PinName rst,PinName sync)
:_spi(mosi,miso,clk),
_cs(cs),
_drdy(drdy),
_rst(rst),
_sync(sync){
	  _spi.format(8,1);
    _spi.frequency(4000000);
	init();
}
uint32_t ADS1256::ReadData()
{
	uint32_t value1,value2,value3;
	  WriteControl(RDATA);
	  wait_us(20);
	while(_drdy);
	value1=_spi.write(0xff);
	value2= _spi.write(0xff);
	value3= _spi.write(0xff);
	return (value1<<16) | (value2<<8) | value3; 
}
void ADS1256::write_reg(uint8_t command,uint8_t data)
{
 
	while(_drdy);
	_spi.write(command | 0x50);
	_spi.write(0x00);
	_spi.write(data);
 
}
uint8_t ADS1256::read_reg(uint8_t command)
{
	 uint8_t v;
	while(_drdy);
	_spi.write(command | 0x10);
	_spi.write(0x00);
	v=_spi.write(0xff);
	return v;
}
uint32_t ADS1256::ReadDataContinuous(uint32_t * buffer,int samples)
{  int i;
   uint32_t value;
	while(_drdy);
	WriteControl(RDATAC);
	for (i=0;i<samples;i++)
	  { 
	    while(_drdy);
	  	value=_spi.write(0xff);
	    value= (value<<8)+_spi.write(0xff);
	    value= (value<<8)+_spi.write(0xff);
	    buffer[i]=value;
	  }
	}
void ADS1256::WriteControl(uint8_t ctrl)
{
		while(_drdy);
	  _spi.write(ctrl);
}

void ADS1256::init()
{ _cs=1;
	_sync=1;
	_rst=0;
	 wait_ms(1);
	_rst=1;
	_cs=0;
	write_reg(0x00,0x04);//status
	wait_us(1);
	write_reg(0x01,0x01);//MUX
	wait_us(1);
	write_reg(0x02,0x20);//ADCON PAG=1
	wait_us(1);
	write_reg(0x03,0x03);// A/D Data Rate
	wait_us(1);
	write_reg(0x04,0x00);// GPIO Control Register
	wait_us(1);
	while(_drdy);
	WriteControl(SELFCAL);
	wait_us(5);
	WriteControl(SYNC);
  wait_ms(20);
	WriteControl(WAKEUP);
	wait_us(5);
}
void ADS1256::SetPGAgain(uint8_t gain)
{
	write_reg(0x02,gain);
}
void ADS1256::SlectChannel(uint8_t ainp,uint8_t ainn)
{
	 write_reg(0x01,(ainp<<4) | ainn );
     write_reg(0x03,0x72);
	}
bool  ADS1256::SensorDetected()	
{
	//not implement
	return true;
	}