#include<stdio.h>
#include<stdint.h>
#include<fcntl.h>
#include<unistd.h>

#define page_map_file "/proc/self/pagemap"
#define PFN_MASK (((uint64_t)1 << 55) - 1)
#define PFN_PRESENT_FLAG ((uint64_t)1 << 63)

int vir_to_phy(unsigned long vir, unsigned long *phy)
{
    int fd;
    int page_size = getpagesize();
    unsigned long vir_page_idx = vir / page_size;
    unsigned long pfn_item_offset = vir_page_idx * sizeof(uint64_t);
    uint64_t pfn_item;

    printf("pagesize is %d\n", page_size);

    fd = open(page_map_file, O_RDONLY);
    if (fd < 0)
    {
        printf("open %s failed", page_map_file);
        return -1;
    }

    if ((off_t)-1 == lseek(fd, pfn_item_offset, SEEK_SET))
    {
        printf("lseek %s failed", page_map_file);
        close(fd);
        return -1; 
    }

    if (sizeof(uint64_t) != read(fd, &pfn_item, sizeof(uint64_t)))
    {
        printf("read %s failed", page_map_file);
        close(fd);
        return -1;
    }

    *phy = (pfn_item & PFN_MASK) * page_size + vir % page_size;
    return 0;
}


int main()
{
    static int i = 8;
    unsigned long i_phy_addr, i_vir_addr;    

    i_vir_addr = (unsigned long)&i;

    if (0 == vir_to_phy(i_vir_addr, &i_phy_addr))
    {
        printf("vir addr is 0x%x, phy addr is 0x%x\n", i_vir_addr, i_phy_addr);
    }

    while(1)
        sleep(2);    

    return 0;
    
}
