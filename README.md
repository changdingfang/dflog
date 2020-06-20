# dflog
c++ logging library


## 平台
- linux

## 使用
> 拷贝src到项目路径
> 添加该路径到cmake子目录项


## 使用示例
```cpp
#include <dflog/dflog.h>

void exampleLOG()
{
	/* 日志, 使用类似于print输出 */
	LOG(INFO, "* * * * * * * * * LOG example * * * * * * * * * *");
	LOG(TRACE, "i am trace log ({}), ({:.2})", 123, 2.3);
	LOG(DEBUG, "i am debug  log ({}), ({})", 123, 3.3123);
	LOG(INFO, "i am debug  log ({:05}), ({:.3})", 123, 3.3123);
	LOG(INFO, "i am info log ({})", static_cast<long int>(123));
	LOG(WARN, "i am warn log [{:#5}]", 100);
	LOG(ERROR, "i am error log {}", "hello wrold");
	LOG(CRITICAL, "i am critical log ");
	LOG(INFO, "* * * * * * * * * * * * * * * * * * * * * * * * *");
}

void exampleLOGF()
{
	/* 日志, 使用类似于print输出 */
	LOGF(INFO, "* * * * * * * * LOGF example * * * * * * * * * * *");
	LOGF(TRACE, "i am trace log (%d), (%.2f)", 123, 2.3);
	LOGF(DEBUG, "i am debug  log (%d), (%f)", 123, 3.3123);
	LOGF(INFO, "i am info log (%ld)", static_cast<long int>(123));
	LOGF(WARN, "i am warn log %d", 100);
	LOGF(ERROR, "i am error log %s", "hello wrold");
	LOGF(CRITICAL, "i am critical log ");
	LOGF(INFO, "* * * * * * * * * * * * * * * * * * * * * * * * *");
}

void init()
{
	/* 
	 * 初始化日志
	 * 1. 可以添加不同的定向log输出
	 * 2. 可以设置异步或同步模式
	 * */
	dflog::InitLog("./example.log", dflog::loggerOption::FILELOG | dflog::loggerOption::CONSOLE,dflog::Method::SYNC);
	dflog::SetLevel(TRACE); /* default DEBUG */
	/* 不同的log输出, 可以控制不同的日志等级 */
	// dflog::SetLevel(DEBUG, dflog::loggerOption::FILELOG | dflog::loggerOption::CONSOLE); /* 默认所有日志等级改动 */;
	
	// dflog::SetFlushLevel(WARN) /* 默认每次刷新(TRACE) */;
}


int main(void)
{
	init();
	exampleLOG();
	exampleLOGF();

	return 0;
}

```

## 问题反馈
email: 245292011@qq.com

## 参考
- [spdlog](https://github.com/gabime/spdlog)


