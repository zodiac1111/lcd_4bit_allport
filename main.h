#ifndef _MAIN_H_
#define _MAIN_H_

//EN
#define EN_PORT  PORTC
#define EN_DDR DDRC
#define EN_PIN PC0
//RW
#define RW_PORT PORTC
#define RW_DDR DDRC
#define RW_PIN PC1
//RS
#define RS_PORT  PORTC
#define RS_DDR DDRC
#define RS_PIN PC2
//定义4根数据总线位置(4个定义一组)
//DB4
#define DB4_DAT PORTD 	//数据线所在端口 -D 端口
#define DB4_IN PIND	//数据输入端口
#define DB4_PORT DDRD	//数据线方向控制
#define DB4 PD4 	//数据总线号	-4 端口号
//DB5
#define DB5_DAT PORTD
#define DB5_IN PIND
#define DB5_PORT DDRD
#define DB5 PD5
//DB6
#define DB6_DAT PORTD
#define DB6_IN PIND
#define DB6_PORT DDRD
#define DB6 PD6
//DB7
#define DB7_DAT PORTD
#define DB7_IN PIND
#define DB7_PORT DDRD
#define DB7 PD7

/**
 * 设置所有数据总线为输出(方向位置1)
 */
#define SET_DB_OUT() {			\
		DB4_PORT |= _BV(DB4);	\
		DB5_PORT |= _BV(DB5);	\
		DB6_PORT |= _BV(DB6);	\
		DB7_PORT |= _BV(DB7);	\
	}
/**
 * 设置所有数据总线位输入(方向位清0)
 */
#define SET_DB_IN() {			\
		DB4_PORT &= ~_BV(DB4);	\
		DB5_PORT &= ~_BV(DB5);	\
		DB6_PORT &= ~_BV(DB6);	\
		DB7_PORT &= ~_BV(DB7);	\
	}

/**
 * 设置控制端口为输出
 * @param offset
 */
#define CRTL_OUT() { 		\
	EN_DDR |= _BV(EN_PIN) ;	\
	RW_DDR |= _BV(RW_PIN) ;	\
	RS_DDR |= _BV(RS_PIN) ;	\
		}
#define En_H EN_PORT |= _BV(EN_PIN) //sbi(PORTA,2)
//En 接 Pa2
#define En_L EN_PORT &= ~_BV(EN_PIN) //cbi(PORTA,2)

#define RW_R RW_PORT |= _BV(RW_PIN) //sbi(PORTA,0)
//RW 接 Pa0
#define RW_W RW_PORT &= ~_BV(RW_PIN)
//cbi(PORTA,0)

#define RS_H RS_PORT |= _BV(RS_PIN) //sbi(PORTA,3)
//RS 接 Pa3
#define RS_L RS_PORT &= ~_BV(RS_PIN) //cbi(PORTA,3)
#define RS_ADDR RS_L //地址(读时)
#define RS_INST RS_L //指令(读时)
#define RS_DAT RS_H //数据(读/写)
#define DelaytE _delay_us(2)

void set_pos(uint8_t offset);
void write_str(char * str);
uint8_t get_pos(void);
uint8_t get_pos2(void);
uint8_t get_data(void);
void set_cgram_data(uint8_t data);
void set_cgram_addr(uint8_t addr);
void set_data(uint8_t data);
void set_ac(uint8_t addr);
void set_command(uint8_t data, uint8_t busyFlag);
void wait_until_ready(void);
void en_toggle(void);
void init_chardb(void);
void init_lcd(void);
#endif
