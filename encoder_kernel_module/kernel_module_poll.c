#include <linux/init.h> 
#include <linux/module.h> 
#include <linux/kernel.h> 
#include <linux/gpio.h>                       // for the GPIO functions 
#include <linux/interrupt.h>                  // for the IRQ code
#include <linux/uaccess.h>          // for copy_to_user
#define DEVICE_MAJOR 0
#define DEVICE_NAME "encoderCounter"
MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("Friðrik - Ólafur - Sindri"); 
MODULE_DESCRIPTION("An encoder driver to count pulses"); 
MODULE_VERSION("0.1");

static unsigned int gpioEncoder = 19;          // GPIO13 
static unsigned int irqNumber;                // share IRQ num within file 
static unsigned int numberPulses = 0;        // store number of presses
static unsigned int measuredOutput = 17 ; // GPIO17

static int majorNumber;
static ssize_t mydev_write(struct file * file, const char *buf, size_t count, loff_t *ppos);
//also read
static ssize_t mydev_read(struct file * file, char *buf, size_t count, loff_t *ppos);

static struct file_operations fops = {
    .read = mydev_read,
    .write = mydev_write,
};

// prototype for the custom IRQ handler function, function below 
static irq_handler_t  encoder_counter_irq_handler(unsigned int irq, 
                                            void *dev_id, struct pt_regs *regs);

static int __init encoder_counter_init(void) 
{
    int result = 0;
    majorNumber = register_chrdev(DEVICE_MAJOR, DEVICE_NAME, &fops);
    printk("Major number is: %d\n", majorNumber);
    printk(KERN_INFO "ENCODER PULSE COUNTER INIT\n");

    if (!gpio_is_valid(gpioEncoder)) 
    {
        printk(KERN_INFO "GPIO_TEST: invalid Encoder GPIO\n");
        return -ENODEV;
    } 
    gpio_request(gpioEncoder, "sysfs");       // set up gpioEncoder   
    gpio_direction_input(gpioEncoder);        // set up as input   
    gpio_export(gpioEncoder, false);          // appears in /sys/class/gpio

    //for feedback, on gpio 17
    if (!gpio_is_valid(measuredOutput)) 
    {
        printk(KERN_INFO "GPIO_TEST: invalid Measured Output GPIO\n");
        return -ENODEV;
    }
    gpio_request(measuredOutput, "sysfs");       // set up gpioEncoder
    gpio_direction_output(measuredOutput, 0);        // set up as output
    gpio_export(measuredOutput, false);          // appears in /sys/class/gpio

    printk(KERN_INFO "GPIO_TEST: button value is currently: %d\n", 
           gpio_get_value(gpioEncoder));

    irqNumber = gpio_to_irq(gpioEncoder);     // map GPIO to IRQ number
    printk(KERN_INFO "GPIO_TEST: button mapped to IRQ: %d\n", irqNumber);

    // This next call requests an interrupt line   
    result = request_irq(irqNumber,          // interrupt number requested            
        (irq_handler_t) encoder_counter_irq_handler,   // handler function            
        IRQF_TRIGGER_RISING,                     // on rising edge (press, not release)            
        "encoder_counter_handler",                     // used in /proc/interrupts
        NULL);                                   // *dev_id for shared interrupt lines
    printk(KERN_INFO "GPIO_TEST: IRQ request result is: %d\n", result);
    return result;
}

static void __exit encoder_counter_exit(void) 
{   
    printk(KERN_INFO "GPIO_TEST: button value is currently: %d\n", 
           gpio_get_value(gpioEncoder));

    printk(KERN_INFO "GPIO_TEST: pressed %d times\n", numberPulses);
    free_irq(irqNumber, NULL);               // free the IRQ number, no *dev_id   
    gpio_unexport(gpioEncoder);               // unexport the Button GPIO   
    gpio_free(gpioEncoder);                   // free the Button GPIO
    unregister_chrdev(majorNumber, DEVICE_NAME);
    printk(KERN_INFO "GPIO_TEST: Goodbye from the LKM!\n"); 
}

static irq_handler_t encoder_counter_irq_handler(unsigned int irq, 
                                           void *dev_id, struct pt_regs *regs) 
{   
    int current_state = gpio_get_value(gpioEncoder);
    gpio_set_value(measuredOutput, current_state);
    if (current_state)
        numberPulses++;          // global counter
    return (irq_handler_t) IRQ_HANDLED;      // announce IRQ handled 
}
//read returns the current number of pulses
static ssize_t mydev_read(struct file * file, char *buf, size_t count, loff_t *ppos)
{
    int res = 0;
    if (*ppos != 0)
        return 0;
    
    // convert to string
    char numberPulsesString[11]; // Account for potential maximum size of int + null terminator
    int len = snprintf(numberPulsesString, sizeof(numberPulsesString), "%d", numberPulses);
    
    if(len > count)  // this checks if the user buffer can hold our data
        return -EFAULT;

    res = copy_to_user(buf, numberPulsesString, len);
    if (res != 0)
        return -EFAULT;

    *ppos = len;  // update the position in the open file
    return len;
}


//write does nothing for now
static ssize_t mydev_write(struct file * file, const char *buf, size_t count, loff_t *ppos)
{
    return sizeof(buf);
}

module_init(encoder_counter_init);
module_exit(encoder_counter_exit);