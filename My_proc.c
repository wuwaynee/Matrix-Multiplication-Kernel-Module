#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/sched.h>
#include <asm/current.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/version.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
#define HAVE_PROC_OPS
#endif

#define PROCFS_MAX_SIZE 8096 
#define procfs_name "thread_info"

static struct proc_dir_entry *Our_Proc_File;

/* The buffer used to store character for this module */ 
static char procfs_buffer[PROCFS_MAX_SIZE]; 
static char stringforuserglobal[PROCFS_MAX_SIZE]; 

/* The size of the buffer */ 
static unsigned long procfs_buffer_size = 0; 

/* This function is called with the /proc file is read */
static ssize_t procfile_read(struct file *filePointer,
                      char __user *buffer,
                      size_t buffer_length,
                      loff_t *offset)
{
    char stringforuser[PROCFS_MAX_SIZE]; 
    snprintf(stringforuser, PROCFS_MAX_SIZE, "PID:%d\n", current->pid);
    strcat(stringforuser,stringforuserglobal);
    ssize_t len = strlen(stringforuser);
    ssize_t ret = len;    
    // use copy_to_user
    if (*offset >= len || copy_to_user(buffer, stringforuser, len)) {
        pr_info("copy_to_user fail_copy_to_usered\n");
        ret = 0;
    } else {
        pr_info("procfile read %s\n", filePointer->f_path.dentry->d_name.name);
        *offset += len;
    }    
    strcpy(stringforuserglobal,"") ;
    return ret;
}

/* This function is called with the /proc file is written. */ 
static ssize_t procfile_write(struct file *file, 
                              const char __user *buff, 
                              size_t len, loff_t *off) 
{ 
    procfs_buffer_size = len; 
    char kernel_buffer[PROCFS_MAX_SIZE]; 
    
    if (procfs_buffer_size > PROCFS_MAX_SIZE) 
        procfs_buffer_size = PROCFS_MAX_SIZE; 
    if (copy_from_user(procfs_buffer, buff, procfs_buffer_size)) 
        return -EFAULT; 
    procfs_buffer[procfs_buffer_size & (PROCFS_MAX_SIZE - 1)] = '\0'; 
    *off += procfs_buffer_size; 
    pr_info("procfile write %s\n", procfs_buffer); 

    long long unsigned uutime=((current->utime)/100)/1000;
    
    snprintf(kernel_buffer, PROCFS_MAX_SIZE, "\tThreadID:%s Time:%llu(ms) context switch times:%lu\n", procfs_buffer, uutime, current->nvcsw+current->nivcsw);
    strcat(stringforuserglobal,kernel_buffer);
 
    return procfs_buffer_size; 
} 

#ifdef HAVE_PROC_OPS
static const struct proc_ops proc_file_fops = {
    .proc_read = procfile_read,
    .proc_write = procfile_write, 
};
#else
static const struct file_operations proc_file_fops = {
    .read = procfile_read,
    .write = procfile_write, 
};

#endif

static int __init procfs1_init(void)
{
    Our_Proc_File = proc_create(procfs_name, 0644, NULL, &proc_file_fops);    
    if (Our_Proc_File == NULL) {
        proc_remove(Our_Proc_File);
        pr_alert("Error:Could not initialize /proc/%s\n", procfs_name);
        return -ENOMEM;
    }    
    pr_info("/proc/%s created\n", procfs_name);
    return 0;
}

static void __exit procfs1_exit(void)
{
    proc_remove(Our_Proc_File);
    pr_info("/proc/%s removed\n", procfs_name);
    return;
}

module_init(procfs1_init);
module_exit(procfs1_exit);

MODULE_LICENSE("GPL");