#include <fs.h>
#include <xinu.h>

#define SIZE 1200

void fs_testbitmask(void);
shellcmd xsh_fstest(int nargs, char *args[])
{
    int rval; 
    int mask;
    int fd, i, j;
    char *buf1, *buf2;
    
    if (nargs == 2 && strncmp(args[1], "--help", 7) == 0)
    {
        kprintf("Usage: %s\n\n", args[0]);
        kprintf("Description:\n");
        kprintf("\tFilesystem Test\n");
        kprintf("Options:\n");
        kprintf("\t--help\tdisplay this help and exit\n");
        return OK;
    }

    /* Check for correct number of arguments */
    if (nargs > 1)
    {
        fprintf(stderr, "%s: too many arguments\n", args[0]);
        fprintf(stderr, "Try '%s --help' for more information\n",
                args[0]);
        return SYSERR;
    }
    if (nargs < 1)
    {
        fprintf(stderr, "%s: too few arguments\n", args[0]);
        fprintf(stderr, "Try '%s --help' for more information\n",
                args[0]);
        return SYSERR;
    }
#ifdef FS

    bs_mkdev(0, MDEV_BLOCK_SIZE, MDEV_NUM_BLOCKS); /* device "0" and default blocksize (=0) and count */
    fs_mkfs(0,DEFAULT_NUM_INODES); /* bsdev 0*/
    fs_testbitmask();
    //fs_print_fsd();
    buf1 = getmem(SIZE*sizeof(char));
    buf2 = getmem(SIZE*sizeof(char));
    
    // Create test file
    fd = fs_create("Test_File", O_CREAT);
    // Fill buffer with random stuff
    for(i=0; i<SIZE; i++)
    {
        j = i%(127-33);
        j = j+33;
        buf1[i] = (char) j;
    }
    
    rval = fs_write(fd,buf1,SIZE);
    if(rval == 0 || rval != SIZE )
    {
        //kprintf("\nCannot Write\n");
        goto clean_up;
    }
     mask = disable();
    // Now my file offset is pointing at EOF file, i need to bring it back to start of file
    // Assuming here implementation of fs_seek is like "original_offset = original_offset + input_offset_from_fs_seek"
    fs_seek(fd,-rval);
    rval = fs_read(fd, buf2, rval);
    restore(mask);
     
    if(rval == 0)
    {
        kprintf("\nRead failed\n");
        goto clean_up;
    }
        
    kprintf("\nFile contents:\n%s\n",buf2);
    rval = fs_close(fd);

clean_up:
    freemem(buf1,SIZE);
    freemem(buf2,SIZE);
    
#else
    kprintf("\nNo filesystem support\n");
#endif
    return 0;

}


void fs_testbitmask(void) {

    fs_setmaskbit(31); fs_setmaskbit(95); fs_setmaskbit(159);fs_setmaskbit(223);
    fs_setmaskbit(287); fs_setmaskbit(351); fs_setmaskbit(415);fs_setmaskbit(479);
    fs_setmaskbit(90); fs_setmaskbit(154);fs_setmaskbit(218); fs_setmaskbit(282);
    fs_setmaskbit(346); fs_setmaskbit(347); fs_setmaskbit(348); fs_setmaskbit(349);
    fs_setmaskbit(350); fs_setmaskbit(100); fs_setmaskbit(164);fs_setmaskbit(228);
    fs_setmaskbit(292); fs_setmaskbit(356); fs_setmaskbit(355); fs_setmaskbit(354);
    fs_setmaskbit(353); fs_setmaskbit(352);
    
    fs_printfreemask();

    fs_clearmaskbit(31); fs_clearmaskbit(95);fs_clearmaskbit(159);fs_clearmaskbit(223);
    fs_clearmaskbit(287); fs_clearmaskbit(351); fs_clearmaskbit(415);fs_clearmaskbit(479);
    fs_clearmaskbit(90); fs_clearmaskbit(154);fs_clearmaskbit(218); fs_clearmaskbit(282);
    fs_clearmaskbit(346); fs_clearmaskbit(347); fs_clearmaskbit(348); fs_clearmaskbit(349);
    fs_clearmaskbit(350); fs_clearmaskbit(100); fs_clearmaskbit(164);fs_clearmaskbit(228);
    fs_clearmaskbit(292); fs_clearmaskbit(356); fs_clearmaskbit(355); fs_clearmaskbit(354);
    fs_clearmaskbit(353); fs_clearmaskbit(352);

    fs_printfreemask();

}