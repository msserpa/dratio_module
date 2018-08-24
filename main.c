#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Matheus Serpa");
MODULE_DESCRIPTION("monitoring dirty background ratio for I/O");

inline int check_name(char *name) {
	int len = strlen(name);

	/* Only programs whose name ends with ".x" are accepted */
	if (name[len-2] == '.' && name[len-1] == 'x')
		return 1;

	return 0;
}

extern void dratio_probes_init(void);
extern void dratio_probes_cleanup(void);

int init_module(void) {
	printk(KERN_INFO "dratio: starting...\n");

	dratio_probes_init();

	printk(KERN_INFO "lamp: started!\n");

	return 0;
}

void cleanup_module(void){
	printk(KERN_INFO "dratio: exiting...\n");

	dratio_probes_cleanup();

	printk(KERN_INFO "dratio: exit!\n");
}