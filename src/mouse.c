/* vim: tabstop=4 shiftwidth=4 noexpandtab
 * Mouse driver
 */
#include <system.h>

unsigned char mouse_cycle = 0;
signed char  mouse_byte[3];

#define PACKETS_IN_PIPE 1024
#define DISCARD_POINT 32

#define MOUSE_IRQ 12

#define MOUSE_PORT   0x60
#define MOUSE_STATUS 0x64
#define MOUSE_ABIT   0x02
#define MOUSE_BBIT   0x01
#define MOUSE_WRITE  0xD4
#define MOUSE_F_BIT  0x20
#define MOUSE_V_BIT  0x08


void mouse_wait(unsigned char a_type) {
	unsigned long timeout = 100000;
	if (!a_type) {
		while (--timeout) {
			if ((inportb(MOUSE_STATUS) & MOUSE_BBIT) == 1) {
				return;
			}
		}
		printf("mouse timeout");
		return;
	} else {
		while (--timeout) {
			if (!((inportb(MOUSE_STATUS) & MOUSE_ABIT))) {
				return;
			}
		}
		printf("mouse timeout");
		return;
	}
}

void mouse_write(unsigned char write) {
	mouse_wait(1);
	outportb(MOUSE_STATUS, MOUSE_WRITE);
	mouse_wait(1);
	outportb(MOUSE_PORT, write);
}

unsigned char mouse_read() {
	mouse_wait(0);
	char t = inportb(MOUSE_PORT);
	return t;
}


void mouse_handler() {
	unsigned char status = inportb(MOUSE_STATUS);
	while (status & MOUSE_BBIT) {
		unsigned char mouse_in = inportb(MOUSE_PORT);
		if (status & MOUSE_F_BIT) {
			switch (mouse_cycle) {
				case 0:
					mouse_byte[0] = mouse_in;
					if (!(mouse_in & MOUSE_V_BIT)) return;
					++mouse_cycle;
					break;
				case 1:
					mouse_byte[1] = mouse_in;
					++mouse_cycle;
					break;
				case 2:
					mouse_byte[2] = mouse_in;
					/* We now have a full mouse packet ready to use */
					if (mouse_byte[0] & 0x80 || mouse_byte[0] & 0x40) {
						/* x/y overflow? bad packet! */
						break;
					}
					mouse_device_packet_t packet;
					packet.magic = MOUSE_MAGIC;
					packet.x_difference = mouse_byte[1];
					packet.y_difference = mouse_byte[2];
					packet.buttons = 0;
					if (mouse_byte[0] & 0x01) {
						packet.buttons |= LEFT_CLICK;
					}
					if (mouse_byte[0] & 0x02) {
						packet.buttons |= RIGHT_CLICK;
					}
					if (mouse_byte[0] & 0x04) {
						packet.buttons |= MIDDLE_CLICK;
					}
					mouse_cycle = 0;

//					mouse_device_packet_t bitbucket;
//					while (pipe_size(mouse_pipe) > (DISCARD_POINT * sizeof(packet))) {
//						read_fs(mouse_pipe, 0, sizeof(packet), (unsigned char *)&bitbucket);
//					}
//					write_fs(mouse_pipe, 0, sizeof(packet), (unsigned char *)&packet);
					break;
			}
		}
		status = inportb(MOUSE_STATUS);
	}
}

extern void irq_mouse();

void mouse_install() {
	printf("Initializing PS/2 mouse interface");
	unsigned char status;
//	mouse_pipe = make_pipe(sizeof(mouse_device_packet_t) * PACKETS_IN_PIPE);
	mouse_wait(1);
	outportb(MOUSE_STATUS, 0xA8);
	mouse_wait(1);
	outportb(MOUSE_STATUS, 0x20);
	mouse_wait(0);
	status = inportb(0x60) | 2;
	mouse_wait(1);
	outportb(MOUSE_STATUS, 0x60);
	mouse_wait(1);
	outportb(MOUSE_PORT, status);
	mouse_write(0xF6);
	mouse_read();
	mouse_write(0xF4);
	mouse_read();
	setInterrupt(MOUSE_IRQ, irq_mouse);
}
