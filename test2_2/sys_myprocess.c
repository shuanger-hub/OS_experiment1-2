#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>

struct task_struct *pcurrent;
struct task_struct *task;

asmlinkage int sys_myprocess(void)
{
	pcurrent = get_current();
	printk(KERN_INFO"Current : \npid : %d\n",pcurrent->pid);
	printk(KERN_INFO"Task State :%d\n",pcurrent->state);
	printk(KERN_INFO"Task name : %s\n",pcurrent->comm);
	int i = 0;
	for(task = current->parrent;task!=init_task;task=task->parrent)
	{
		i = i+1;
		printk(KERN_INFO"%d parent pid : %d\n",i,task->pid);
	}
	printk(KERN_INFO"goodby myprocess!\n");
	return 1;
}

	

