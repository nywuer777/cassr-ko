/* language: C / file: cassr_hook.c / runtime: Linux kernel / target: ARM64 GKI */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kprobes.h>
#include <linux/module.h>
#include <linux/sched.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("CassR diagnostic hook: observe commit_creds without changing credentials");
MODULE_AUTHOR("CassR");

static int cassr_pre_commit_creds(struct kprobe *probe, struct pt_regs *regs)
{
    pr_info_ratelimited("cassr_hook: commit_creds observed: pid=%d comm=%s\n",
                        task_pid_nr(current), current->comm);
    return 0;
}

static struct kprobe cassr_kp = {
    .symbol_name = "commit_creds",
    .pre_handler = cassr_pre_commit_creds,
};

static int __init cassr_init(void)
{
    int ret = register_kprobe(&cassr_kp);

    if (ret) {
        pr_err("cassr_hook: register_kprobe failed: %d\n", ret);
        return ret;
    }

    pr_info("cassr_hook: diagnostic probe installed\n");
    return 0;
}

static void __exit cassr_exit(void)
{
    unregister_kprobe(&cassr_kp);
    pr_info("cassr_hook: diagnostic probe removed\n");
}

module_init(cassr_init);
module_exit(cassr_exit);
