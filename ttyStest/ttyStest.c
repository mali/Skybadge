/*
 - ttyStest.c
 - by Laurent Navet 'Mali' <laurent dot navet (at) gmail dot com>

 - Largement inspiré de l'article de Pierre Ficheux paru
 dans GNU/Linux Magazine n°89 (Pilotes de périphériques en mode caractere )

This software is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; version 2 dated June, 1991.

This software is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this software; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.

 */

/*
 * Includes
 */
#include <linux/kernel.h>	/* printk() */
#include <linux/module.h>	/* modules */
#include <linux/init.h>		/* module_{init,exit}() */
#include <linux/fs.h>           /* file_operations */
#include <linux/wait.h>         /* wait queues */
#include <linux/sched.h>        /* wait queues */
#include <asm/uaccess.h>	/* copy_{from,to}_user() */
#include <linux/miscdevice.h>   /* misc driver interface */

MODULE_DESCRIPTION("ttyStest");
MODULE_AUTHOR("Laurent Navet");
MODULE_LICENSE("GPL");

/*
 * Global variables
 */

#define BUF_SIZE 256

static struct miscdevice mymisc; /* Misc device handler */
static char buffer[BUF_SIZE]; /* copy_from/to_user buffer */
static size_t num = 0; /* Number of available bytes in the buffer */

static DECLARE_WAIT_QUEUE_HEAD(read_wait_queue);

/*
 * File operations
 */
static ssize_t ttyStest_read(struct file *file, char *buf, size_t count, loff_t *ppos)
{
  size_t real;
 
  if(!num)
    printk(KERN_DEBUG "ttyStest: process %i (%s) va dormir!\n", current->pid, current->comm);

  if(wait_event_interruptible(read_wait_queue, num != 0) != 0)
  {
    printk(KERN_DEBUG "ttyStest: debout %i (%s)!\n", current->pid, current->comm);
    return -ERESTARTSYS;
  }
  else
  {
   printk(KERN_DEBUG "ttyStest: process %i (%s) se reveille!\n", current->pid, current->comm);  
  }

  real = min(num, count);

  if (real)
    if (copy_to_user(buf, buffer, real))
      return -EFAULT;

  num = 0; /* Destructive read (no more data after a read) */
//  *buffer = '\0';	
  printk(KERN_DEBUG "ttyStest: process %i (%s) a lu %d/%d caracteres %s\n", current->pid, current->comm, (int)real, (int)count, buffer);
  memset(buffer, 0, sizeof(buffer)); /* no more data in the buffer */
  return real;
}

static ssize_t ttyStest_write(struct file *file, const char *buf, size_t count, loff_t *ppos)
{
  size_t real;

  real = min((size_t)BUF_SIZE, count);

  if (real)
    if (copy_from_user(buffer, buf, real))
      return -EFAULT;

  num = real; /* Destructive write (overwrite previous data if any) */

  printk(KERN_DEBUG "ttyStest: process %i (%s) a ecrit %d/%d caracteres %s\n", current->pid, current->comm, (int)real, (int)count, buffer);

  /* wake up blocked processes in wait queue */
  wake_up_interruptible(&read_wait_queue);
  return real;
}

static int ttyStest_open(struct inode *inode, struct file *file)
{
  printk(KERN_INFO "ttyStest: process %i (%s) open()\n", current->pid, current->comm);

  return 0;
}

static int ttyStest_release(struct inode *inode, struct file *file)
{
  printk(KERN_INFO "ttyStest: process %i (%s) release()\n", current->pid, current->comm);

  return 0;
}

static int ttyStest_ioctl(struct inode *inode, struct file *file, unsigned int ioctl_num, unsigned long arg)
{
  printk(KERN_INFO "ttyStest: process %i (%s) => ioctl(%d)\n", current->pid, current->comm, ioctl_num);

  return 0;
}

static struct file_operations ttyStest_fops = {
  .owner   =	THIS_MODULE,
  .read    =	ttyStest_read,
  .write   =	ttyStest_write,
  .open    =	ttyStest_open,
  .release =	ttyStest_release,
  .ioctl   =	ttyStest_ioctl,
};

/*
 * Init and Exit
 */
static int __init ttyStest_init(void)
{
  int ret;

  mymisc.minor = MISC_DYNAMIC_MINOR;
  mymisc.name = "ttyStest";
  mymisc.fops = &ttyStest_fops;

  ret = misc_register(&mymisc);

  if (ret < 0) {
    printk(KERN_WARNING "ttyStest: unable to get a dynamic minor\n");
    return ret;
  }

  return 0;
}

static void __exit ttyStest_exit(void)
{
  misc_deregister(&mymisc);
  printk(KERN_INFO "ttyStest: successfully unloaded\n");
}

/*
 * Module entry points
 */
module_init(ttyStest_init);
module_exit(ttyStest_exit);
