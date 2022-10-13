#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
//#include <err.h>
#include <linux/kernel.h>
#define size 512
MODULE_LICENSE("GPL");

static char *source;
static char *target;
module_param(source,charp,0644);
module_param(target,charp,0644);

static int module2_init(void)
{
	int cnt;
	char buffer[size];
	struct file *source_filp;
	struct file *target_filp;
	loff_t s_pos;
	loff_t t_pos;
	mm_segment_t fs;

	printk(KERN_ALERT"This is the test about file\n");

	source_filp = filp_open(source,O_RDONLY,0);
	if(IS_ERR(source_filp))
	{
		printk(KERN_ALERT"open %s error",source);
		return -1;
	}

	target_filp = filp_open(target,O_CREAT|O_WRONLY|O_TRUNC,S_IRUSR|S_IWUSR);//文件打开操作，中间的参数是选择打开方式
	if(IS_ERR(target_filp))
	{
		printk(KERN_ALERT"open %s error",target);
		return -1;
	}
	//设置我的文件定位
	s_pos=source_filp->f_pos;
	t_pos=target_filp->f_pos;
	
	//force_uaccess_begin();

	//fs=get_fs();//获得当前用户的fs，等会要返回回来
	//set_fs(KERNEL_DS);//切换权限，在当前内核空间中使用内存
	while((cnt = vfs_read(target_filp,buffer,size,&t_pos)) > 0)
	{
		t_pos = t_pos + cnt;
	}
	while((cnt = vfs_read(source_filp,buffer,size,&s_pos)) > 0)
	{
		vfs_write(target_filp,buffer,cnt,&t_pos);
		s_pos+=cnt;
		t_pos+=cnt;
	}
	
	//force_uaccess_begin();
	//set_fs(fs);//设置回用户空间
	//关闭文件
	filp_close(source_filp,NULL);
	filp_close(target_filp,NULL);

	return 0;
}

static void module2_exit(void)
{
	printk(KERN_ALERT"goodbye module!\n");
}

module_init(module2_init);
module_exit(module2_exit);

