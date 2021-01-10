

输出的文件都在 项目目录下的 `build/bin/` 下

- testCompile.sh   
    测试运行脚本, 直接运行该脚本, 可以完成从编译到测试整个流程     
- testDflog.cpp    
    dflog基本功能测试, 输出样式结果打印到dflog.log文件   
- testFMT.cpp   
    测试FMT性能, 输出结果可以在 result_testFMT 中查看   
- testLogLoop.cpp   
    日志文件循环覆盖测试, 出错会有异常打印   
- testNormalSink.cpp   
    日志分割测试, 输出结果在result_testNormalSink中查看   
- testPerformanceTest.cpp   
    性能测试, 测试结果在result_testPerformanceTest中查看   
- testTerminalColor.cpp   
    终端颜色测试,输出结果在result_testTerminalColor文件, 使用cat命令查看文件内容, 可看到颜色   
- touch.sh   
    用以辅助生成日志文件, 用于日志循环测试使用   
