#include <xinu.h>
#include <fs.h>
#include <string.h>
static struct fsystem fsd;
int dev0_numblocks;
int dev0_blocksize;
char *dev0_blocks;

extern int dev0;

char block_cache[512];

#define SB_BLK 0
#define BM_BLK 1
#define RT_BLK 2

#define NUM_FD 16
struct filetable oft[NUM_FD];
int next_open_fd = 0;

#define INODES_PER_BLOCK (fsd.blocksz / sizeof(struct inode))
#define NUM_INODE_BLOCKS (( (fsd.ninodes % INODES_PER_BLOCK) == 0) ? fsd.ninodes / INODES_PER_BLOCK : (fsd.ninodes / INODES_PER_BLOCK) + 1)
#define FIRST_INODE_BLOCK 2

int inode_id=1;

int fileblock_to_diskblock(int dev, int fd, int fileblock);

int fs_open(char *filename, int flags) 
{   
    if(flags>=0 && flags<=2){
        struct directory dir=fsd.root_dir;
        int i,floc=-1;
        for(i=0;i<dir.numentries;i++){
            if(!strcmp(filename,dir.entry[i].name)){
                floc=i;
	  		}
   		}
        if(floc==-1){
            kprintf("\nFile not found\n");
			return SYSERR;
        }
        //Filetable entry
        int fd=next_open_fd++;
        struct filetable ft;
        ft.state=FSTATE_OPEN;
        ft.fileptr=0;
        ft.de=&(dir.entry[floc]);
        strcpy((ft.de)->name, filename);
        struct inode in;
        get_inode_by_num(0,ft.de->inode_num,&in);
        memcpy(&(ft.in),&in,sizeof(struct inode));
        memcpy(oft+fd,&ft,sizeof(struct filetable));
        return fd;
    }
    else { 
    	return SYSERR;
    }	
}

int fs_close(int fd)
{
    if ((fd > next_open_fd) || (fd < 0)){
        kprintf("\nCannot close file. Error\n");
        return SYSERR;
    }
    oft[fd].state = FSTATE_CLOSED;
    return OK;
}

int fs_create(char *filename, int mode)
{
    if(mode==O_CREAT){
        struct directory dir=fsd.root_dir;
        //check if file name is valid. i.e., no file with same name exists
        int i;
        for(i=0;i<dir.numentries;i++){
            if(!strcmp(filename,dir.entry[i].name)){
                kprintf("\nFile already exists.\n");
                return SYSERR;
            }
        }
        //Filetable entry
        int fd=next_open_fd++;
        struct filetable ft;
        ft.state=FSTATE_OPEN;
        ft.fileptr=0;
        ft.de=&(dir.entry[dir.numentries++]);       
        strcpy((ft.de)->name, filename); 
        memcpy(&(fsd.root_dir),&dir,sizeof(struct directory));
        struct inode in;
        in.id=inode_id++;
        in.type=INODE_TYPE_FILE; 
        put_inode_by_num(0,in.id,&in);
        int bl = in.id / INODES_PER_BLOCK;
        bl += FIRST_INODE_BLOCK;
        fs_setmaskbit(bl);
        memcpy(&(ft.in),&in,sizeof(struct inode));
        memcpy(&(ft.de->inode_num),&(in.id),sizeof(int));
        memcpy(&(fsd.root_dir.entry[dir.numentries-1].inode_num),&(in.id),sizeof(int));
        memcpy(oft+fd,&ft,sizeof(struct filetable));
        fsd.inodes_used++;
        return fd;
    }
    else {
    	return SYSERR;
    }
}

int fs_seek(int fd, int offset)
{  
    struct filetable ft=oft[fd];
    offset=ft.fileptr+offset;
    if(offset<0){
    	offset=0;
        kprintf("\nInvalid offset\n");
    }
    memcpy(&((oft+fd)->fileptr),&offset,sizeof(int));
    return fd;
}

int fs_read(int fd, void *buf, int nbytes)
{
    int temp_nbytes=nbytes;
    struct filetable ft=oft[fd];
    if(ft.state==FSTATE_CLOSED){
        return 0;
    }
    struct inode in = ft.in;
    int inode= (ft.fileptr / fsd.blocksz);
    int inodeoffset=(ft.fileptr % fsd.blocksz);
    if (inode<INODEBLOCKS){
        int blk=ft.in.blocks[inode];
        while(nbytes>0){
            if(nbytes<(fsd.blocksz-inodeoffset)){
                bs_bread(0,blk,inodeoffset,buf,nbytes);
                ft.fileptr+=nbytes;
                buf+=nbytes;
                nbytes=0;
                return temp_nbytes;
            }
            else{
                if(inode==INODEBLOCKS-1){
                    kprintf("\nInvalid read request\n");
                    return temp_nbytes-nbytes;
                }
                bs_bread(0,blk,inodeoffset,buf,fsd.blocksz-inodeoffset);
                buf+=(fsd.blocksz-inodeoffset);
                nbytes-=(fsd.blocksz-inodeoffset);
                ft.fileptr+=(fsd.blocksz-inodeoffset);
                memcpy(oft+fd,&ft,sizeof(struct filetable)); 
                blk=in.blocks[++inode];
                inodeoffset=0;
            }
        }
    }
    return temp_nbytes-nbytes;
}

int fs_write(int fd, void *buf, int nbytes)
{
    int temp_nbytes=nbytes;
    struct filetable ft=oft[fd];
    if(ft.state==FSTATE_CLOSED){
        return 0;
    }
    struct inode in = ft.in;
    int inode= (ft.fileptr / fsd.blocksz);
    int inodeoffset=(ft.fileptr % fsd.blocksz);
    if (inode<INODEBLOCKS){
        int blk;
        while(nbytes>0){
            if(in.blocks[inode]==0){
                blk=get_free_block();
                memcpy(in.blocks+inode,&blk,sizeof(int));
                memcpy(&((oft+fd)->in),&(in),sizeof(struct inode));
                ft.in=in;  
                put_inode_by_num(0,in.id,&in);
                fs_setmaskbit(blk);
            }
            else if(in.blocks[inode]>0){
                blk=in.blocks[inode]; 
            }
            //Check if all data we want to write can be put in same block
            if(nbytes<(fsd.blocksz-inodeoffset)){
                bs_bwrite(0,blk,inodeoffset,buf,nbytes);
                ft.fileptr+=nbytes;
                nbytes=0;
                memcpy(oft+fd,&ft,sizeof(struct filetable));
                return temp_nbytes;
            }
            else{
                if(inode==INODEBLOCKS-1){
                    kprintf("\nInvalid write request\n");
                    return temp_nbytes-nbytes;
                }
                bs_bwrite(0,blk,inodeoffset,buf,fsd.blocksz-inodeoffset);
                buf+=(fsd.blocksz-inodeoffset);
                nbytes-=(fsd.blocksz-inodeoffset);
                ft.fileptr+=(fsd.blocksz-inodeoffset);
                //write to oft:
                memcpy(oft+fd,&ft,sizeof(struct filetable));
                inode++;
                inodeoffset=0;
            }
        }
    }
    return temp_nbytes-nbytes;
}

int fs_mkfs(int dev, int num_inodes) {
  int i;
  if (dev == 0) {
    fsd.nblocks = dev0_numblocks;
    fsd.blocksz = dev0_blocksize;
  }
  else {
    printf("Unsupported device\n");
    return SYSERR;
  }

  if (num_inodes < 1) {
    fsd.ninodes = DEFAULT_NUM_INODES;
  }
  else {
    fsd.ninodes = num_inodes;
  }
  i = fsd.nblocks;
  while ( (i % 8) != 0) {i++;}
  fsd.freemaskbytes = i / 8;
  if ((fsd.freemask = getmem(fsd.freemaskbytes)) == (void *)SYSERR) {
    printf("fs_mkfs getmem failed.\n");
    return SYSERR;
  }
  for(i=0;i<fsd.freemaskbytes;i++) {
    fsd.freemask[i] = '\0';
  }

  fsd.inodes_used = 0;
  /* write the fsystem block to SB_BLK, mark block used */
  fs_setmaskbit(SB_BLK);
  bs_bwrite(dev0, SB_BLK, 0, &fsd, sizeof(struct fsystem));
  /* write the free block bitmask in BM_BLK, mark block used */
  fs_setmaskbit(BM_BLK);
  bs_bwrite(dev0, BM_BLK, 0, fsd.freemask, fsd.freemaskbytes);
  return 1;
}

int fileblock_to_diskblock(int dev, int fd, int fileblock) {
  int diskblock;
  if (fileblock >= INODEBLOCKS - 2) {
    printf("No indirect block support\n");
    return SYSERR;
  }
  diskblock = oft[fd].in.blocks[fileblock]; //get the logical block address
  return diskblock;
}
/* read in an inode and fill in the pointer */
int get_inode_by_num(int dev, int inode_number, struct inode *in) {
  int bl, inn;
  int inode_off;

  if (dev != 0) {
    printf("Unsupported device\n");
    return SYSERR;
  }
  if (inode_number > fsd.ninodes) {
    printf("get_inode_by_num: inode %d out of range\n", inode_number);
    return SYSERR;
  }

  bl = inode_number / INODES_PER_BLOCK;
  inn = inode_number % INODES_PER_BLOCK;
  bl += FIRST_INODE_BLOCK;
  inode_off = inn * sizeof(struct inode);
  bs_bread(dev0, bl, 0, &block_cache[0], fsd.blocksz);
  memcpy(in, &block_cache[inode_off], sizeof(struct inode));
  return OK;
}

int put_inode_by_num(int dev, int inode_number, struct inode *in) {
  int bl, inn;
  if (dev != 0) {
    printf("Unsupported device\n");
    return SYSERR;
  }
  if (inode_number > fsd.ninodes) {
    printf("put_inode_by_num: inode %d out of range\n", inode_number);
    return SYSERR;
  }
  bl = inode_number / INODES_PER_BLOCK;
  inn = inode_number % INODES_PER_BLOCK;
  bl += FIRST_INODE_BLOCK;
  bs_bread(dev0, bl, 0, block_cache, fsd.blocksz);
  memcpy(&block_cache[(inn*sizeof(struct inode))], in, sizeof(struct inode));
  bs_bwrite(dev0, bl, 0, block_cache, fsd.blocksz);
  return OK;
}
/* specify the block number to be set in the mask */
int fs_setmaskbit(int b) {
  int mbyte, mbit;
  mbyte = b / 8;
  mbit = b % 8;
  fsd.freemask[mbyte] |= (0x80 >> mbit);
  return OK;
}
/* specify the block number to be read in the mask */
int fs_getmaskbit(int b) {
  int mbyte, mbit;
  mbyte = b / 8;
  mbit = b % 8;
  return( ( (fsd.freemask[mbyte] << mbit) & 0x80 ) >> 7);
  return OK;
}
/* specify the block number to be unset in the mask */
int fs_clearmaskbit(int b) {
  int mbyte, mbit, invb;
  mbyte = b / 8;
  mbit = b % 8;

  invb = ~(0x80 >> mbit);
  invb &= 0xFF;

  fsd.freemask[mbyte] &= invb;
  return OK;
}
/*  This is maybe a little overcomplicated since the lowest-numbered
 *  block is indicated in the high-order bit.  Shift the byte by j
 *  positions to make the match in bit7 (the 8th bit) and then shift
 *  that value 7 times to the low-order bit to print.  Yes, it could be
 *  the other way...  */
void fs_printfreemask(void) {
  int i,j;
  for (i=0; i < fsd.freemaskbytes; i++) {
    for (j=0; j < 8; j++) {
      printf("%d", ((fsd.freemask[i] << j) & 0x80) >> 7);
    }
    if ( (i % 8) == 7) {
      printf("\n");
    }
  }
  printf("\n");
}

void fs_print_fsd(void) {
  kprintf("fsd.ninodes: %d\n", fsd.ninodes);
  kprintf("sizeof(struct inode): %d\n", sizeof(struct inode));
  kprintf("INODES_PER_BLOCK: %d\n", INODES_PER_BLOCK);
  kprintf("NUM_INODE_BLOCKS: %d\n", NUM_INODE_BLOCKS);
}

int get_free_block(){
    int i;
    for(i=0;i<fsd.nblocks;i++){
        if(fs_getmaskbit(i)==0){
            return i;
        }
    }
    kprintf("\nNo free blocks available\n");
    return -1;
}

