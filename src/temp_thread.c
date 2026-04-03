#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

// 启用日志记录
LOG_MODULE_REGISTER(TEMP_TASK, LOG_LEVEL_INF);

/* 
* 获取设备树中定义的传感器设备句柄
*/
static const struct device *const dev = DEVICE_DT_GET(DT_NODELABEL(p3t1755));

void temp_thread_entry(void *p1, void *p2, void *p3)
{
    k_msleep(100);
    
    // ... 在这里初始化您的 LED 设备 ...
    LOG_INF("TEMP Thread started");

    /* 检查设备是否准备就绪 */
    if (!device_is_ready(dev)) {
        LOG_ERR("传感器设备 %s 未就绪，请检查硬件连接和 Overlay 配置！", dev->name);
        return;
    }

	LOG_INF("成功找到 P3T1755 温度传感器，开始读取数据...");

    struct sensor_value temp_val;

    while (1) {
        /* 触发一次传感器数据采集 */
        int rc = sensor_sample_fetch(dev);
        if (rc < 0) {
            LOG_ERR("数据采集失败 (错误代码: %d)", rc);
        } else {
            /* 获取环境温度通道的值 */
            rc = sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, &temp_val);
            if (rc < 0) {
                LOG_ERR("获取通道数据失败 (错误代码: %d)", rc);
            } else {
                /* 打印温度结果（Zephyr 将浮点数拆分为整数 val1 和小数 val2） */
                // LOG_INF("当前温度: %d.%02d °C", temp_val.val1, temp_val.val2);
                double temp_double = sensor_value_to_double(&temp_val);
                /* 使用 %.2f 表示保留两位小数 */
                LOG_INF("当前温度: %.2f °C", temp_double);
            }
        }

        /* 每隔 2 秒读取一次 */
        k_msleep(2000);
    }
}

// 线程栈和定义
#define TEMP_STACK_SIZE 1024
#define TEMP_PRIORITY 13

K_THREAD_DEFINE(temp_tid, TEMP_STACK_SIZE, 
                temp_thread_entry, NULL, NULL, NULL,
                TEMP_PRIORITY, 0, 0);
