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

static unsigned int gpioEncoder_A = 19;          // GPIO13 
static unsigned int gpioEncoder_B = 26;          // GPIO26
static unsigned int irqNumberA;                // share IRQ num within file 
static unsigned int irqNumberB;                // share IRQ num within file
static unsigned int numberPulses = 0;        // store number of presses
static unsigned int measuredOutput = 17 ; // GPIO17
bool encA_last = 0;
bool encB_last = 0;

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

static irq_handler_t  encoder_counter_irq_handler_B(unsigned int irq, 
                                            void *dev_id, struct pt_regs *regs);

static int __init encoder_counter_init(void) 
{
    int result = 0;
    majorNumber = register_chrdev(DEVICE_MAJOR, DEVICE_NAME, &fops);
    printk("Major number is: %d\n", majorNumber);
    printk(KERN_INFO "ENCODER PULSE COUNTER INIT\n");

    if (!gpio_is_valid(gpioEncoder_A) || !gpio_is_valid(gpioEncoder_B)) 
    {
        printk(KERN_INFO "GPIO_TEST: invalid Encoder GPIO\n");
        return -ENODEV;
    } 
    gpio_request(gpioEncoder_A, "sysfs");       // set up gpioEncoder_A   
    gpio_direction_input(gpioEncoder_A);        // set up as input   
    gpio_export(gpioEncoder_A, false);          // appears in /sys/class/gpio
    gpio_request(gpioEncoder_B, "sysfs");       // set up gpioEncoder_B
    gpio_direction_input(gpioEncoder_B);        // set up as input
    gpio_export(gpioEncoder_B, false);          // appears in /sys/class/gpio


    //for feedback, on gpio 17
    if (!gpio_is_valid(measuredOutput)) 
    {
        printk(KERN_INFO "GPIO_TEST: invalid Measured Output GPIO\n");
        return -ENODEV;
    }
    gpio_request(measuredOutput, "sysfs");       // set up measured output as output
    gpio_direction_output(measuredOutput, 0);        // set up as output
    gpio_export(measuredOutput, false);          // appears in /sys/class/gpio

    printk(KERN_INFO "GPIO_TEST: button value is currently: %d\n", 
           gpio_get_value(gpioEncoder_A));

    irqNumberA = gpio_to_irq(gpioEncoder_A);     // map GPIO to IRQ number
    printk(KERN_INFO "GPIO_TEST: button mapped to IRQ: %d\n", irqNumberA);
    irqNumberB = gpio_to_irq(gpioEncoder_B);     // map GPIO to IRQ number
    printk(KERN_INFO "GPIO_TEST: button mapped to IRQ: %d\n", irqNumberB);

    // This next call requests an interrupt line   
    result = request_irq(irqNumberA,          // interrupt number requested            
        (irq_handler_t) encoder_counter_irq_handler,   // handler function            
        IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING,                    // on rising             
        "encoder_counter_handler",                     // used in /proc/interrupts
        NULL);                                   // *dev_id for shared interrupt lines
    printk(KERN_INFO "GPIO_TEST: IRQ request result is: %d\n", result);
    result = request_irq(irqNumberB,          // interrupt number requested            
        (irq_handler_t) encoder_counter_irq_handler_B,   // handler function            
        IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING,                     // on rising            
        "encoder_counter_handler_B",                     // used in /proc/interrupts
        NULL);                                   // *dev_id for shared interrupt lines
    printk(KERN_INFO "GPIO_TEST: IRQ request result is: %d\n", result);
    return result;
}

static void __exit encoder_counter_exit(void) 
{   
    printk(KERN_INFO "GPIO_TEST: button value is currently: %d\n", 
           gpio_get_value(gpioEncoder_A));

    printk(KERN_INFO "GPIO_TEST: pressed %d times\n", numberPulses);
    free_irq(irqNumberA, NULL);               // free the IRQ number, no *dev_id 
    free_irq(irqNumberB, NULL);               // free the IRQ number, no *dev_id  
    gpio_unexport(gpioEncoder_A);               // unexport the Button GPIO 
    gpio_unexport(gpioEncoder_B);               // unexport the Button GPIO
    gpio_free(gpioEncoder_A);                   // free the Button GPIO
    gpio_free(gpioEncoder_B);                   // free the Button GPIO
    unregister_chrdev(majorNumber, DEVICE_NAME);
    printk(KERN_INFO "GPIO_TEST: Goodbye from the LKM!\n"); 
}

static irq_handler_t encoder_counter_irq_handler_B(unsigned int irq, 
                                           void *dev_id, struct pt_regs *regs) 
{   
    bool encA_current = gpio_get_value(gpioEncoder_A);
    bool encB_current = gpio_get_value(gpioEncoder_B);
    
    if (encB_last != encB_current)
    {
        if (encB_current != encA_current)
            numberPulses--;
        else
            numberPulses++;

        encB_last = encB_current;
    }
    
    return (irq_handler_t) IRQ_HANDLED; 
}



static irq_handler_t encoder_counter_irq_handler(unsigned int irq, 
                                           void *dev_id, struct pt_regs *regs) 
{   
    bool encA_current = gpio_get_value(gpioEncoder_A);
    bool encB_current = gpio_get_value(gpioEncoder_B);
    
    gpio_set_value(measuredOutput, encA_current);

    if (encA_last != encA_current)
    {
        if (encA_current == encB_current)
            numberPulses--;
        else
            numberPulses++;

        encA_last = encA_current;
    }
    
    return (irq_handler_t) IRQ_HANDLED; 
}


//read returns the current number of pulses
static ssize_t mydev_read(struct file * file, char *buf, size_t count, loff_t *ppos)
{
    char numberPulsesString[11]; 
    int len = snprintf(numberPulsesString, sizeof(numberPulsesString), "%d", numberPulses);
    int res = 0;
    if (*ppos != 0)
        return 0;
    
    // convert to string
    
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