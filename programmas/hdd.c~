#include <system.h>
void loadHDDSector(char* location,long LBA,long count);
ata_device dev;

typedef struct fat_BS
{
	unsigned char 		bootjmp[3];
	unsigned char 		oem_name[8];
	unsigned short 	        bytes_per_sector;
	unsigned char		sectors_per_cluster;
	unsigned short		reserved_sector_count;
	unsigned char		table_count;
	unsigned short		root_entry_count;
	unsigned short		total_sectors_16;
	unsigned char		media_type;
	unsigned short		table_size_16;
	unsigned short		sectors_per_track;
	unsigned short		head_side_count;
	unsigned int 		hidden_sector_count;
	unsigned int 		total_sectors_32;
 
	//extended fat12 and fat16 stuff
	unsigned char		bios_drive_num;
	unsigned char		reserved1;
	unsigned char		boot_signature;
	unsigned int		volume_id;
	unsigned char		volume_label[11];
	unsigned char		fat_type_label[8];
 
}__attribute__((packed)) fat_BS_t;

typedef struct fatfile{
	unsigned char filename[11];
	unsigned char attr;
	unsigned char winnt;
	unsigned char creat;
	unsigned short tcreat;
	unsigned short tdate;
	unsigned short tacces;
	unsigned short clusterhigh;
	unsigned short lastmodt;
	unsigned short lastmodd;
	unsigned short clusterlow;
	unsigned long filesize;
}__attribute__((packed)) FATFile;


void main(ata_device *dxv){
	dev.io_base = dxv->io_base;
	dev.slave   = dxv->slave;
	printf("Hello small world at IOBase %x!\n",dev.io_base);
	void* mx = malloc(512);
	loadHDDSector(mx,0,1);
	unsigned long X = ((unsigned long*)(mx+8+0x01BE))[0];
	printf("Partition1: %x \n",X);
	loadHDDSector(mx,X,1);
	fat_BS_t *fat_boot = mx;
	unsigned short fat_size = fat_boot->table_size_16;
	unsigned short root_dir_sectors = ((fat_boot->root_entry_count * 32) + (fat_boot->bytes_per_sector - 1)) / fat_boot->bytes_per_sector;
	unsigned short first_data_sector = fat_boot->reserved_sector_count + (fat_boot->table_count * fat_size) + root_dir_sectors;
	unsigned short first_root_dir_sector = first_data_sector - root_dir_sectors;
	printf("FATSIZE: %x \n",fat_size);
	printf("ROOT DIR SECTORS: %x \n",root_dir_sectors);
	printf("FIRST DATA SECTOR: %x \n",first_data_sector);
	printf("FIRST ROOT DIR SECTOR: %x \n",first_root_dir_sector);
	printf("VOLUMELABEL: %s",fat_boot->volume_label);
	printf("FATDIR: %x \n",sizeof(FATFile));
	loadHDDSector(mx,X+first_root_dir_sector,1);
	FATFile *fatfiles[10] = mx;
	for(int i = 0 ; i < 10 ; i++){
		printf("FILE %s \n",fatfiles[i].filename);
	}
}


void loadHDDSector(char* location,long LBA,long count){
	outportb(dev.io_base+6, 0xE0 | (dev.slave << 4) | ((LBA >> 24) & 0x0F));
	outportb(dev.io_base+1, 0x00);
	outportb(dev.io_base+2, (unsigned char) count);
	outportb(dev.io_base+3, (unsigned char) LBA);
	outportb(dev.io_base+4, (unsigned char)(LBA >> 8));
	outportb(dev.io_base+5, (unsigned char)(LBA >> 16));
	outportb(dev.io_base+7, 0x20);
	int status;
	while((status = inportb(dev.io_base+7)) & 0x80 ){}
	insw(dev.io_base+0, location,count*512);
}
