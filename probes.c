#include <linux/kprobes.h>
#include <linux/kthread.h>

extern int check_name(char *name);


static int dirty_handler(struct ctl_table *table, int write, void __user *buffer, size_t *lenp, loff_t *ppos){
	printk("dratio : %lu - %lld\n", *lenp, *ppos);
	jprobe_return();
}

static void process_handler(struct task_struct *tsk){
	// kernel/exit.c
	if(tsk->flags & PF_EXITING){
		printk("dratio : %s stop (pid %d)\n", tsk->comm, tsk->pid);
		jprobe_return();
	}

	// fs/exec.c
	if(check_name(tsk->comm) && !(tsk->flags & PF_EXITING)){
			printk("dratio : new process %s (pid %d)\n", tsk->comm, tsk->pid);
	}

	jprobe_return();
}

static struct jprobe dirty_probe = {
	.entry = dirty_handler,
	.kp.symbol_name = "dirty_background_ratio_handler",
};

static struct jprobe process_probe = {
	.entry = process_handler,
	.kp.symbol_name = "acct_update_integrals",
};


void dratio_probes_init(void){
	int ret;

	if((ret = register_jprobe(&dirty_probe))){
		printk("dratio bug: dirty_background_ratio_handler missing, %d\n", ret);
	}

	if((ret = register_jprobe(&process_probe))){
		printk("dratio bug: acct_update_integrals missing, %d\n", ret);
	}
}

void dratio_probes_cleanup(void){
	unregister_jprobe(&dirty_probe);
	unregister_jprobe(&process_probe);
}