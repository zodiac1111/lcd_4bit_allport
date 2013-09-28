/**
 * main.c
 * Created: 1/14/2012 6:16:49 PM
 *  Author: Radu Motisan , radu.motisan@gmail.com ,
 * (C) 2012 , All rights reserved.
 * @todo done 4路传输  使用4数据总线时仅连接 4 5 6 7高四根,数据分两次传输
 * @todo 使用任意4跟数据线//检测1换同端口其他PIN 通过,待检测其他
 * @todo done 自定义字符
 */
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "main.h"
/**
 * 主函数,显示一些东西
 * @return
 */
int main(void)
{
	uint8_t ch;
	//0. 初始化LCD
	init_lcd();
	//1. 最简单的字符输出测试 (写数据)
	set_pos(0x00);
	write_str("hello world");
	_delay_ms(100);
	//2. 读取数据测试 (读数据)
	_delay_ms(100);
	set_pos(0x06);     //设置读取的位置,第1行6列,'w'的位置
	_delay_ms(1000);
	ch = get_data();
	set_pos(0x0D);     //设置光标位置
	set_data(ch);
	_delay_ms(1000);
	//3. 自定义字符测试 (写内存+读内存)
	init_chardb();     //初始化自定义的字符集
	set_pos(0x40);     //设置在第2行还是显示
	int i;
	for ( i =0; i<0b111+1; i++) {
		set_data(i);
		//_delay_ms(50);
	}
	//4. 读取光标位置 (读地址/位置) I处即光标所在地址
	_delay_ms(1000);
	char str[5];
	set_pos(0x4a);
	ch=get_pos();
	sprintf(str,"I=%02XH",ch);
	write_str(str);
	return 0;
}
/**
 * 初始化LCD.
 * 参考:<http://www.repairfaq.org/filipg/LINK/F_Tech_LCD4.html#TECHLCD_001>
 * http://lcd-linux.sourceforge.net/pdfdocs/hd44780.pdf
 */
void init_lcd(void)
{

	//3个控制端口为输出
	CRTL_OUT()
	//4条数据总线:输出
	SET_DB_OUT()
	//多次设置,确保初始化
	//设置功能: [0] [0]  [1] [1]DL-8位  X X X X
	uint8_t fun_8bit_2line = 0b00110000;//低四位无关
	//设置功能: [0] [0]  [1] [0]DL-4位 [1]N-双行 [0]F-5*7字形 X X
	uint8_t fun_4bit_2line = 0b00101000;
	_delay_ms(15);     //2
	set_command(fun_8bit_2line, 0);     //3设为 8 位接口模式,busy=0 不检测忙信号
	_delay_ms(4.1);     //4
	set_command(fun_8bit_2line, 0);	//5
	_delay_us(100);     //6
	set_command(fun_8bit_2line, 0);	//7
	_delay_ms(4.1);     //8
	//DL:数据接口宽度标志:DL=1,8 位数据总线 DB7~DB0;DL=0,4 位数据总线 DB7~DB4,DB3~DB0
	//不用,使用此方式传送数据,需分两次进行;
	//N:显示行数标志;
	//F:显示字符点阵字体标志;
	//功能设置
	set_command(fun_4bit_2line, 0);	//9
	_delay_us(40);     //10
	set_command(fun_4bit_2line, 1);	//11
	_delay_us(40);     //12
	//开关显示
	set_command(0b00001000, 1);     //13.显示关闭(0) 关光标(0) 关光标闪烁(0)
	_delay_us(40);     //14
	//开关显示
	set_command(0b00001111, 1);     //15.显示开启(1) 显示光标(1) 光标闪烁(1)
	_delay_us(40);     //16
	//设置输入方式
	set_command(0b00000110, 1);     //17.写入新数据后光标右移(1) 文字不动(0)
	_delay_us(40);     //18
	//19 初始化完成
	//清屏 可选的
	set_command(0b00000001, 1);     //显示清屏(1)
	_delay_us(40);     //18

}
///产生一个使能脉冲
void en_toggle(void)
{
	En_H;     //拉高使能位
	DelaytE;     //保持高电平一定时间
	En_L;     //拉低使能位,产生一个下降沿
	DelaytE;     //保持低电平一定时间
}

/**
 * 等待直到不繁忙. 不断检测 LCD 忙标志位,知道其等于0 表示空闲状态,可以接受指令
 */
void wait_until_ready(void)
{
	//LCDDDR = 0x00;     //数据总线设为输入,用以读取 LCD 忙标志
	//set datebus in
	//LCDDDR = 0x0F;
//	DB4_PORT &= ~_BV(DB4);
//	DB5_PORT &= ~_BV(DB5);
//	DB6_PORT &= ~_BV(DB6);
//	DB7_PORT &= ~_BV(DB7);
	SET_DB_IN()
	;
	//命令选择
	RW_R;     //读状态
	RS_ADDR;     //地址(状态)所读为状态位
	//命令选择完毕
	DelaytE;
	En_H;     //触发
	DelaytE;     // 这是忙标志被映射到数据DB7 位
	loop_until_bit_is_clear(DB7_IN, DB7);
	En_L;
}
/**
 * 写命令
 * @param[in] Command 命令
 * @param[in] busyFlag 是否检查繁忙
 */
void set_command(uint8_t data, uint8_t busyFlag)
{
	uint8_t busy;
	busy = busyFlag;
	//busy=1 表示系统希望检测忙信号
	if (busy==1)
		//等待指令执行完毕
		wait_until_ready();
	//完毕,可以写了
	SET_DB_OUT()
	;
	RW_W;     //置为写状态 =写 0
	RS_L;     //写入的是命令字 =指令
	En_H;
	(data&0b00010000) ? (DB4_DAT |= _BV(DB4)) : (DB4_DAT &= ~_BV(DB4));     //第4位
	(data&0b00100000) ? (DB5_DAT |= _BV(DB5)) : (DB5_DAT &= ~_BV(DB5));     //第5位
	(data&0b01000000) ? (DB6_DAT |= _BV(DB6)) : (DB6_DAT &= ~_BV(DB6));     //第6位
	(data&0b10000000) ? (DB7_DAT |= _BV(DB7)) : (DB7_DAT &= ~_BV(DB7));     //第7位
	_delay_us(25);
	en_toggle();     //产生使能脉冲,使之在下降沿开始执行指令
	(data&0b00000001) ? (DB4_DAT |= _BV(DB4)) : (DB4_DAT &= ~_BV(DB4));     //第0位
	(data&0b00000010) ? (DB5_DAT |= _BV(DB5)) : (DB5_DAT &= ~_BV(DB5));     //第1位
	(data&0b00000100) ? (DB6_DAT |= _BV(DB6)) : (DB6_DAT &= ~_BV(DB6));     //第2位
	(data&0b00001000) ? (DB7_DAT |= _BV(DB7)) : (DB7_DAT &= ~_BV(DB7));     //第3位
	_delay_us(25);
	en_toggle();     //产生使能脉冲,使之在下降沿开始执行指令
}

/**
 * 写数据
 * @param data
 */
void set_data(uint8_t data)
{
	wait_until_ready();     //等待指令执行完毕
	//LCDDDR = 0xff;     //数据总线位输出
	SET_DB_OUT()
	;
	RW_W;     //置为写状态
	RS_H;     //写入的是数据
	En_H;
	(data&0b00010000) ? (DB4_DAT |= _BV(DB4)) : (DB4_DAT &= ~_BV(DB4));     //第4位
	(data&0b00100000) ? (DB5_DAT |= _BV(DB5)) : (DB5_DAT &= ~_BV(DB5));     //第5位
	(data&0b01000000) ? (DB6_DAT |= _BV(DB6)) : (DB6_DAT &= ~_BV(DB6));     //第6位
	(data&0b10000000) ? (DB7_DAT |= _BV(DB7)) : (DB7_DAT &= ~_BV(DB7));     //第7位
	//_delay_us(25);
	en_toggle();     //产生使能脉冲,使之在下降沿开始执行指令
	(data&0b00000001) ? (DB4_DAT |= _BV(DB4)) : (DB4_DAT &= ~_BV(DB4));     //第0位
	(data&0b00000010) ? (DB5_DAT |= _BV(DB5)) : (DB5_DAT &= ~_BV(DB5));     //第1位
	(data&0b00000100) ? (DB6_DAT |= _BV(DB6)) : (DB6_DAT &= ~_BV(DB6));     //第2位
	(data&0b00001000) ? (DB7_DAT |= _BV(DB7)) : (DB7_DAT &= ~_BV(DB7));     //第3位
	//_delay_us(25);
	en_toggle();     //产生使能脉冲,使之在下降沿开始执行指令
}

/**
 * 读数据
 * 读当前AC地址的字符的值,
 * @note 4位模式先传高4位,再传低4位
 * @return
 */
uint8_t get_data(void)
{
	uint8_t data = 0;
	wait_until_ready();     //等待指令执行完毕
	//LCDDDR = 0x00;     //数据总线位输入
	SET_DB_IN()
	;
	RW_R;     //读 Hi
	RS_DAT;     //数据 Hi
	En_L;
	DelaytE;
	En_H;     //触发
	DelaytE;
	//data = PIND;
	//先传高四位
	(DB4_IN &_BV(DB4)) ? (data |= _BV(4)) : (data &= ~_BV(4));
	(DB5_IN &_BV(DB5)) ? (data |= _BV(5)) : (data &= ~_BV(5));
	(DB6_IN &_BV(DB6)) ? (data |= _BV(6)) : (data &= ~_BV(6));
	(DB7_IN &_BV(DB7)) ? (data |= _BV(7)) : (data &= ~_BV(7));
	En_L;
	DelaytE;
	En_H;     //触发
	DelaytE;     // 这是忙标志被映射到数据DB7 位
	(DB4_IN &_BV(DB4)) ? (data |= _BV(0)) : (data &= ~_BV(0));
	(DB5_IN &_BV(DB5)) ? (data |= _BV(1)) : (data &= ~_BV(1));
	(DB6_IN &_BV(DB6)) ? (data |= _BV(2)) : (data &= ~_BV(2));
	(DB7_IN &_BV(DB7)) ? (data |= _BV(3)) : (data &= ~_BV(3));
	//En_L;
	return data;
}
/**
 * 读AC寄存器的值 done 即当前光标位置
 * 4位模式下先传低4位地址,再传高4位地址
 * @return
 */
uint8_t get_pos(void)
{
	uint8_t data = 0;
	wait_until_ready();     //等待指令执行完毕
	//LCDDDR = 0x00;     //数据总线位输入
	SET_DB_IN()
	;
	RW_R;     //读 HI
	RS_ADDR;  //地址 0
	DelaytE;
	En_H;     //触发
	DelaytE;
	//先传位置低4位
	(DB4_IN &_BV(DB4)) ? (data |= _BV(0)) : (data &= ~_BV(0));
	(DB5_IN &_BV(DB5)) ? (data |= _BV(1)) : (data &= ~_BV(1));
	(DB6_IN &_BV(DB6)) ? (data |= _BV(2)) : (data &= ~_BV(2));
	(DB7_IN &_BV(DB7)) ? (data |= _BV(3)) : (data &= ~_BV(3));
	En_L;
	DelaytE;
	En_H;     //触发
	DelaytE;
	//在传高4位
	(DB4_IN &_BV(DB4)) ? (data |= _BV(4)) : (data &= ~_BV(4));
	(DB5_IN &_BV(DB5)) ? (data |= _BV(5)) : (data &= ~_BV(5));
	(DB6_IN &_BV(DB6)) ? (data |= _BV(6)) : (data &= ~_BV(6));
	(DB7_IN &_BV(DB7)) ? (data |= _BV(7)) : (data &= ~_BV(7));
	return data;
}
/**
 * 这个暂时不知道是读的什么东西,好像是字符的点阵的值,一行8个点那个 @note
 * @return
 */
uint8_t get_pos2(void)
{
	uint8_t data = 0;
	wait_until_ready();     //等待指令执行完毕
	//LCDDDR = 0x00;     //数据总线位输入
	SET_DB_IN()
	;
	RW_R;     //读 HI
	RS_DAT;  //地址 0
	DelaytE;
	En_H;     //触发
	DelaytE;
	//先传位置低4位
	(DB4_IN &_BV(DB4)) ? (data |= _BV(0)) : (data &= ~_BV(0));
	(DB5_IN &_BV(DB5)) ? (data |= _BV(1)) : (data &= ~_BV(1));
	(DB6_IN &_BV(DB6)) ? (data |= _BV(2)) : (data &= ~_BV(2));
	(DB7_IN &_BV(DB7)) ? (data |= _BV(3)) : (data &= ~_BV(3));
	En_L;
	DelaytE;
	En_H;     //触发
	DelaytE;
	//在传高4位
	(DB4_IN &_BV(DB4)) ? (data |= _BV(4)) : (data &= ~_BV(4));
	(DB5_IN &_BV(DB5)) ? (data |= _BV(5)) : (data &= ~_BV(5));
	(DB6_IN &_BV(DB6)) ? (data |= _BV(6)) : (data &= ~_BV(6));
	(DB7_IN &_BV(DB7)) ? (data |= _BV(7)) : (data &= ~_BV(7));
	return data;
}
/**
 * 写字符串 就是把自己循环一下而已
 * @param str
 */
void write_str(char * str)
{
	while (*str!='\0') {
		set_data(*str++);
	}
}
/**
 * 设置字符显示的位置 Set position
 * @param offset 输入位置 第7位为1,其他为地址
 * 1行: 0x00 0x01 ... 0x0f
 * 2行: 0x40 0x41 ... 0x4f
 */
void set_pos(uint8_t offset)
{
	set_command(offset|0b10000000, 1);
}

/**
 * 光标复位到1行1列处.
 */
void return_home(void)
{
	set_command(0b00000010, 1);     //显示开启(1) 显示光标(1) 光标闪烁(1)
}

/**
 *  set CGRAM address 设置CGRAM地址.
 *  用于自定义字符,低6位为地址位,
 *  7 6 5 4 3 2 1 0 bit
 * +-+-+-+-+-+-+-+-+
 * |0|1| CGRAM addr|
 * +-+-+-----------+
 * 其中0-6为有分为:
 * * 0-2位:表示5*8阵列的第几行(共8行),
 * * 3-6位:表示第几个字符,最多定义8个自定义字符.
 * @param data
 */
void set_cgram_addr(uint8_t addr)
{
	set_command(0b01000000|(addr&0b00111111), 1);
}
/**
 * 写字模的一行,仅低5位有效
 * @param data
 */
void set_cgram_data(uint8_t data)
{
	set_data(0b00011111&data);
}
/**
 * 初始化自定义字符表 最多8个 地址从0b00,000,000 到 0b00,111,000
 * http://www.quinapalus.com/hd44780udg.html
 */
void init_chardb(void)
{
	set_cgram_addr(0b00000000);     //000,000
	set_cgram_data(0b00000000);
	set_cgram_data(0b00001010);
	set_cgram_data(0b0000000);
	set_cgram_data(0b00000100);
	set_cgram_data(0b00010001);
	set_cgram_data(0b00001110);
	set_cgram_data(0b00000000);
	set_cgram_data(0b00000000);	//最下面一行光标行,最好不要使用,效果不好
	set_cgram_addr(0b00001000);   //001,000
	set_cgram_data(0b00000000);
	set_cgram_data(0b00011101);
	set_cgram_data(0b00000101);
	set_cgram_data(0b00011111);
	set_cgram_data(0b00010100);
	set_cgram_data(0b00010111);
	set_cgram_data(0b00000000);
	set_cgram_data(0b00000000);
	set_cgram_addr(0b00010000);     //010,000
	set_cgram_data(0b00000100);
	set_cgram_data(0b00000100);
	set_cgram_data(0b00011111);
	set_cgram_data(0b00000100);
	set_cgram_data(0b00001110);
	set_cgram_data(0b00001110);
	set_cgram_data(0b00000100);
	set_cgram_data(0b00000000);
	set_cgram_addr(0b00011000);     //011,000
	set_cgram_data(0b00000000);
	set_cgram_data(0b00010001);
	set_cgram_data(0b00001010);
	set_cgram_data(0b00000000);
	set_cgram_data(0b00011111);
	set_cgram_data(0b00010001);
	set_cgram_data(0b00001110);
	set_cgram_data(0b00000000);
	set_cgram_addr(0b00100000);     //100,000
	set_cgram_data(0b00000100);
	set_cgram_data(0b00001110);
	set_cgram_data(0b00010101);
	set_cgram_data(0b00000100);
	set_cgram_data(0b00000100);
	set_cgram_data(0b00000100);
	set_cgram_data(0b00000100);
	set_cgram_data(0b00000000);
	set_cgram_addr(0b00101000);     //101,000
	set_cgram_data(0b00000100);
	set_cgram_data(0b00000100);
	set_cgram_data(0b00000100);
	set_cgram_data(0b00000100);
	set_cgram_data(0b00010101);
	set_cgram_data(0b00001110);
	set_cgram_data(0b00000100);
	set_cgram_data(0b00000000);
	set_cgram_addr(0b00110000);     //110,000
	set_cgram_data(0b00000100);
	set_cgram_data(0b00011111);
	set_cgram_data(0b00000100);
	set_cgram_data(0b00011111);
	set_cgram_data(0b00010001);
	set_cgram_data(0b00010001);
	set_cgram_data(0b00011111);
	set_cgram_data(0b00000000);
	set_cgram_addr(0b00111000);     //111,000
	set_cgram_data(0b00000000);
	set_cgram_data(0b00001010);
	set_cgram_data(0b00010101);
	set_cgram_data(0b00001010);
	set_cgram_data(0b00000100);
	set_cgram_data(0b00000000);
	set_cgram_data(0b00000000);
	set_cgram_data(0b00000000);
}
